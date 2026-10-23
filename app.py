"""C++ Simulation Dashboard: write C++ in the browser, compile it with your own
g++, see the output, and plot any data.csv the program writes.

Runs only on your own computer (127.0.0.1). Start it with:  python app.py
"""
import csv
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

from flask import Flask, jsonify, request, send_from_directory

HERE = Path(__file__).resolve().parent
EXAMPLES = HERE / "examples"
EXE_NAME = "prog.exe" if sys.platform.startswith("win") else "prog"
COMPILE_TIMEOUT_S = 30
RUN_TIMEOUT_S = 10
MAX_OUTPUT_CHARS = 20_000
MAX_POINTS = 50_000          # rows of data.csv sent to the plot

app = Flask(__name__, static_folder=str(HERE / "static"))


def compiler():
    """Return the g++ command, or None if g++ is not installed."""
    return shutil.which("g++")


def clip(text):
    if len(text) > MAX_OUTPUT_CHARS:
        return text[:MAX_OUTPUT_CHARS] + "\n... (output cut)"
    return text


def read_csv(path):
    """Read data.csv: first row = column names, other rows = numbers."""
    with open(path, newline="") as f:
        rows = list(csv.reader(f))
    if len(rows) < 2:
        return None
    header = [h.strip() for h in rows[0]]
    data = []
    for r in rows[1:]:
        try:
            data.append([float(v) for v in r])
        except ValueError:
            continue            # skip lines that are not numbers
    if len(data) > MAX_POINTS:  # keep the plot fast: take evenly spaced rows
        step = len(data) // MAX_POINTS + 1
        data = data[::step]
    return {"header": header, "rows": data}


def compile_and_run(code, args=""):
    gpp = compiler()
    if gpp is None:
        return {"stage": "setup", "ok": False,
                "message": "g++ was not found. Install it and make sure 'g++ --version' works in a terminal."}
    work = Path(tempfile.mkdtemp(prefix="cppdash_"))
    try:
        (work / "main.cpp").write_text(code, encoding="utf-8")
        t0 = time.perf_counter()
        try:
            comp = subprocess.run(
                [gpp, "-std=c++17", "-O2", "-Wall", "-Wextra", "main.cpp", "-o", EXE_NAME],
                cwd=work, capture_output=True, text=True, timeout=COMPILE_TIMEOUT_S)
        except subprocess.TimeoutExpired:
            return {"stage": "compile", "ok": False, "message": "Compilation took too long."}
        compile_s = time.perf_counter() - t0
        if comp.returncode != 0:
            return {"stage": "compile", "ok": False, "compiler": clip(comp.stderr),
                    "compile_s": compile_s}

        t0 = time.perf_counter()
        try:
            run = subprocess.run([str(work / EXE_NAME), *args.split()], cwd=work,
                                 capture_output=True, text=True, timeout=RUN_TIMEOUT_S)
        except subprocess.TimeoutExpired:
            return {"stage": "run", "ok": False, "compiler": clip(comp.stderr),
                    "message": f"The program ran longer than {RUN_TIMEOUT_S} s and was stopped."}
        run_s = time.perf_counter() - t0

        data_file = work / "data.csv"
        return {"stage": "run", "ok": run.returncode == 0, "exit_code": run.returncode,
                "compiler": clip(comp.stderr),            # warnings, if any
                "stdout": clip(run.stdout), "stderr": clip(run.stderr),
                "compile_s": compile_s, "run_s": run_s,
                "data": read_csv(data_file) if data_file.exists() else None}
    finally:
        shutil.rmtree(work, ignore_errors=True)


@app.get("/")
def index():
    return send_from_directory(app.static_folder, "index.html")


@app.get("/api/examples")
def list_examples():
    return jsonify(sorted(p.stem for p in EXAMPLES.glob("*.cpp")))


@app.get("/api/examples/<name>")
def get_example(name):
    path = EXAMPLES / f"{name}.cpp"
    if not path.is_file() or path.parent != EXAMPLES:
        return jsonify({"error": "not found"}), 404
    return jsonify({"name": name, "code": path.read_text(encoding="utf-8")})


@app.post("/api/run")
def api_run():
    body = request.get_json(force=True, silent=True) or {}
    return jsonify(compile_and_run(body.get("code", ""), body.get("args", "")))


@app.get("/api/info")
def info():
    gpp = compiler()
    version = ""
    if gpp:
        version = subprocess.run([gpp, "--version"], capture_output=True, text=True).stdout.splitlines()[0]
    return jsonify({"compiler": version or "g++ not found"})


if __name__ == "__main__":
    print("Open http://127.0.0.1:5000 in your browser (Ctrl+C to stop)")
    app.run(host="127.0.0.1", port=5000, debug=False)
