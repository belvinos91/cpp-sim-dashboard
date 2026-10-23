"""Tests: run with  python -m pytest  (needs g++ installed)."""
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from app import EXAMPLES, app, compile_and_run  # noqa: E402


@pytest.fixture
def client():
    return app.test_client()


def test_hello_runs():
    res = compile_and_run('#include <iostream>\nint main(){ std::cout << 42; }')
    assert res["ok"] and res["stdout"].strip() == "42"


def test_arguments_are_passed():
    code = '#include <iostream>\nint main(int c, char** v){ std::cout << c - 1; }'
    assert compile_and_run(code, "a b c")["stdout"] == "3"


def test_compile_error_is_reported():
    res = compile_and_run("int main( { }")
    assert res["stage"] == "compile" and not res["ok"] and "error" in res["compiler"]


def test_runtime_error_exit_code():
    res = compile_and_run("int main(){ return 3; }")
    assert res["stage"] == "run" and not res["ok"] and res["exit_code"] == 3


def test_infinite_loop_is_stopped():
    res = compile_and_run("int main(){ volatile int x = 0; while (true) { ++x; } }")
    assert not res["ok"] and "stopped" in res["message"]


def test_csv_is_read():
    code = ('#include <fstream>\nint main(){ std::ofstream f("data.csv");'
            ' f << "t,y\\n0,1\\n1,2\\n"; }')
    data = compile_and_run(code)["data"]
    assert data["header"] == ["t", "y"] and data["rows"] == [[0.0, 1.0], [1.0, 2.0]]


@pytest.mark.parametrize("path", sorted(EXAMPLES.glob("*.cpp")), ids=lambda p: p.stem)
def test_every_example_compiles_and_runs(path):
    res = compile_and_run(path.read_text())
    assert res["ok"], res


def test_api_endpoints(client):
    names = client.get("/api/examples").get_json()
    assert "03_sir_rk4" in names
    assert client.get("/api/examples/../app").status_code == 404
    res = client.post("/api/run", json={"code": "int main(){}"}).get_json()
    assert res["ok"]
