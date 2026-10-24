"""Tests for the local AI features, using a stand-in for the model (no Ollama needed)."""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import ai  # noqa: E402
from app import app, compile_and_run  # noqa: E402

GOOD = "```cpp\n#include <iostream>\nint main() { std::cout << \"x = \" << 2.0 << \"\\n\"; }\n```"
BAD = "```cpp\nint main() { return 0 }\n```"   # missing semicolon


def test_extract_cpp_prefers_cpp_block():
    text = "Intro\n```python\nprint(1)\n```\nThen\n```cpp\nint main(){}\n```"
    assert ai.extract_cpp(text).strip() == "int main(){}"


def test_outputs_match_tolerates_number_format():
    assert ai.outputs_match("x = 2.0\n", "x = 2\n")
    assert ai.outputs_match("mean 0.3333333333", "mean 0.333333")
    assert not ai.outputs_match("x = 2.0", "x = 3")
    assert not ai.outputs_match("a b", "a")


def test_convert_fixes_compile_error_and_compares_outputs():
    replies = iter([BAD, GOOD])
    seen = []

    def fake_llm(messages):
        seen.append(messages[-1]["content"])
        return next(replies)

    res = ai.convert('print("x =", 2.0)', compile_and_run, llm=fake_llm)
    assert len(res["attempts"]) == 2
    assert res["attempts"][0]["result"]["stage"] == "compile"
    assert "does not compile" in seen[1]           # compiler errors were sent back
    assert res["attempts"][1]["result"]["ok"]
    assert res["outputs_match"] is True


def test_convert_gives_up_after_max_attempts():
    res = ai.convert("print(1)", compile_and_run, llm=lambda m: BAD)
    assert len(res["attempts"]) == ai.MAX_FIX_ATTEMPTS
    assert res["attempts"][-1]["result"]["stage"] == "compile"


def test_ask_compiles_every_full_program():
    answer = "Explanation.\n" + GOOD + "\nA fragment:\n```cpp\nx += 1;\n```\n" + BAD
    res = ai.ask([{"role": "user", "content": "q"}], compile_and_run, llm=lambda m: answer)
    ok, fragment, bad = res["checks"]
    assert ok["ok"] and fragment is None and bad["stage"] == "compile"


def test_ask_includes_editor_code():
    seen = {}

    def fake_llm(messages):
        seen["last"] = messages[-1]["content"]
        return "ok"

    ai.ask([{"role": "user", "content": "explain"}], compile_and_run, editor_code="int main(){}", llm=fake_llm)
    assert "int main(){}" in seen["last"]


def test_status_when_ollama_is_not_running(monkeypatch):
    monkeypatch.setattr(ai, "OLLAMA_URL", "http://127.0.0.1:9")   # nothing listens on port 9
    st = ai.status()
    assert st["ok"] is False and st["reason"] == "not_running"


def test_endpoints_report_unavailable_ai(monkeypatch):
    monkeypatch.setattr(ai, "OLLAMA_URL", "http://127.0.0.1:9")
    client = app.test_client()
    assert client.get("/api/ai/status").get_json()["ok"] is False
    r = client.post("/api/ai/convert", json={"python": "print(1)"})
    assert r.status_code == 503 and "error" in r.get_json()
    r = client.post("/api/ai/ask", json={"history": [{"role": "user", "content": "hi"}]})
    assert r.status_code == 503


def test_python_runner_time_limit(monkeypatch):
    monkeypatch.setattr(ai, "PY_TIMEOUT_S", 1)
    res = ai.run_python("while True: pass")
    assert not res["ok"] and "Stopped" in res["stderr"]
