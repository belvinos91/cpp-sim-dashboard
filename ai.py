"""Local AI tutor for the dashboard, using a model served by Ollama on this computer.

Nothing leaves the machine: the model runs locally at OLLAMA_URL.
Every C++ program the model writes is compiled and run before you see the verdict,
so a wrong answer shows up as a compiler error or a different output, never as a silent mistake.

Settings (environment variables):
    OLLAMA_URL    default http://127.0.0.1:11434
    OLLAMA_MODEL  default qwen2.5-coder:7b
"""
import json
import os
import re
import subprocess
import sys
import tempfile
import urllib.error
import urllib.request
from pathlib import Path

OLLAMA_URL = os.environ.get("OLLAMA_URL", "http://127.0.0.1:11434")
MODEL = os.environ.get("OLLAMA_MODEL", "qwen2.5-coder:7b")
LLM_TIMEOUT_S = 600          # a 7B model on CPU can take a while for long answers
PY_TIMEOUT_S = 10
MAX_FIX_ATTEMPTS = 3

SYSTEM_TUTOR = """You are a patient C++ tutor for an applied mathematician who knows Python well \
and is learning C++ for scientific simulation (ODEs, SDEs, stochastic models, data files).
Rules:
- Answer in plain English, short paragraphs.
- When you give code, give ONE complete C++17 program in a ```cpp block, with #include lines and main(),
  using only the standard library, so it compiles with: g++ -std=c++17 -Wall -Wextra.
- If the program produces data for a plot, write it to a file named data.csv
  (first row = column names, first column = x).
- Explain the key lines and, when useful, show the Python equivalent.
- Never invent library functions. If unsure, say so."""

SYSTEM_CONVERT = """You translate Python programs into C++17. Rules:
- Output ONE complete C++17 program in a single ```cpp block, nothing else.
- Use only the standard library; it must compile with g++ -std=c++17 -Wall -Wextra without warnings.
- Keep the same behaviour and the same printed output, in the same order and format where possible.
  Python prints 2.0 for floats with no decimals; C++ prints 2, which is acceptable.
- Replace numpy arrays with std::vector<double> and numpy random with <random> (std::mt19937).
- If the Python writes a CSV file, write the same CSV file in C++.
- Add short comments that explain each C++ construct to a Python programmer."""


class AIUnavailable(Exception):
    pass


def status():
    """Is Ollama running, and is the model downloaded?"""
    try:
        with urllib.request.urlopen(OLLAMA_URL + "/api/tags", timeout=3) as r:
            names = [m["name"] for m in json.load(r).get("models", [])]
    except (urllib.error.URLError, OSError, ValueError):
        return {"ok": False, "model": MODEL, "reason": "not_running",
                "message": f"Ollama is not running at {OLLAMA_URL}."}
    if MODEL not in names and f"{MODEL}:latest" not in names:
        return {"ok": False, "model": MODEL, "reason": "no_model", "available": names,
                "message": f"Ollama is running, but the model {MODEL} is not downloaded."}
    return {"ok": True, "model": MODEL}


def llm_chat(messages):
    """Send a conversation to Ollama and return the reply text."""
    body = json.dumps({"model": MODEL, "messages": messages, "stream": False,
                       "options": {"temperature": 0.2}}).encode()
    req = urllib.request.Request(OLLAMA_URL + "/api/chat", data=body,
                                 headers={"Content-Type": "application/json"})
    try:
        with urllib.request.urlopen(req, timeout=LLM_TIMEOUT_S) as r:
            return json.load(r)["message"]["content"]
    except (urllib.error.URLError, OSError, ValueError, KeyError) as e:
        raise AIUnavailable(f"Could not get an answer from Ollama ({e}).") from e


CODE_BLOCK = re.compile(r"```([\w+#-]*)[ \t]*\n(.*?)```", re.S)


def code_blocks(text):
    """All fenced code blocks as (language, code)."""
    return [(lang.lower(), code) for lang, code in CODE_BLOCK.findall(text)]


def extract_cpp(text):
    """The first C++ block (or the first block of any language, or the whole text)."""
    blocks = code_blocks(text)
    for lang, code in blocks:
        if lang in ("cpp", "c++", "cc", "cxx"):
            return code
    return blocks[0][1] if blocks else text


def is_full_program(code):
    return re.search(r"\bint\s+main\s*\(", code) is not None


def run_python(code):
    """Run Python code in a temporary folder with a time limit (same rules as the C++ runner)."""
    with tempfile.TemporaryDirectory(prefix="cppdash_py_") as work:
        Path(work, "main.py").write_text(code, encoding="utf-8")
        try:
            r = subprocess.run([sys.executable, "main.py"], cwd=work, capture_output=True,
                               text=True, timeout=PY_TIMEOUT_S)
        except subprocess.TimeoutExpired:
            return {"ok": False, "stdout": "", "stderr": f"Stopped after {PY_TIMEOUT_S} s."}
        return {"ok": r.returncode == 0, "stdout": r.stdout[-20000:], "stderr": r.stderr[-20000:]}


NUMBER = re.compile(r"^[-+]?(\d+\.?\d*|\.\d+)([eE][-+]?\d+)?$")


def outputs_match(a, b, rel_tol=1e-6):
    """Compare two printed outputs word by word; numbers may differ in format (2.0 vs 2)
    or in the last digits (up to rel_tol)."""
    ta, tb = a.split(), b.split()
    if len(ta) != len(tb):
        return False
    for x, y in zip(ta, tb):
        x, y = x.strip(",;:()[]"), y.strip(",;:()[]")
        if x == y:
            continue
        if NUMBER.match(x) and NUMBER.match(y):
            fx, fy = float(x), float(y)
            if abs(fx - fy) <= rel_tol * max(1.0, abs(fx), abs(fy)):
                continue
        return False
    return True


def convert(python_code, compile_and_run, llm=None):
    """Python -> C++: ask the model, compile and run the result, and if it fails to compile
    send the compiler errors back and try again (up to MAX_FIX_ATTEMPTS)."""
    llm = llm or llm_chat
    messages = [{"role": "system", "content": SYSTEM_CONVERT},
                {"role": "user", "content": "Translate this Python program to C++:\n```python\n"
                                            + python_code + "\n```"}]
    attempts = []
    for _ in range(MAX_FIX_ATTEMPTS):
        reply = llm(messages)
        cpp = extract_cpp(reply)
        result = compile_and_run(cpp)
        attempts.append({"cpp": cpp, "result": result})
        if result.get("stage") != "compile":
            break
        messages += [{"role": "assistant", "content": reply},
                     {"role": "user", "content": "It does not compile. g++ says:\n```\n"
                      + result.get("compiler", "")[:4000]
                      + "\n```\nFix it and give the full corrected program in one ```cpp block."}]
    py = run_python(python_code)
    final = attempts[-1]["result"]
    match = None
    if py["ok"] and final.get("ok"):
        match = outputs_match(py["stdout"], final.get("stdout", ""))
    return {"cpp": attempts[-1]["cpp"], "attempts": attempts, "python": py, "outputs_match": match}


def ask(history, compile_and_run, editor_code=None, llm=None):
    """Tutor chat. Every complete C++ program in the answer is compiled and run, so the
    page can show whether it actually works."""
    llm = llm or llm_chat
    messages = [{"role": "system", "content": SYSTEM_TUTOR}] + [
        {"role": m["role"], "content": m["content"]} for m in history if m.get("role") in ("user", "assistant")]
    if editor_code and messages[-1]["role"] == "user":
        messages[-1] = {"role": "user", "content": messages[-1]["content"]
                        + "\n\nHere is my current code:\n```cpp\n" + editor_code + "\n```"}
    reply = llm(messages)
    checks = []
    for lang, code in code_blocks(reply):
        if lang in ("cpp", "c++", "cc", "cxx", "") and is_full_program(code):
            r = compile_and_run(code)
            checks.append({"ok": bool(r.get("ok")), "stage": r.get("stage"),
                           "compiler": r.get("compiler", ""), "stdout": r.get("stdout", "")})
        else:
            checks.append(None)
    return {"answer": reply, "checks": checks}
