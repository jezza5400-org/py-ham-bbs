#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BACKEND_DIR="$ROOT_DIR/backend-py"
FRONTEND_DIR="$ROOT_DIR/frontend-ts"

export PY_HAM_BBS_DB_PATH="${PY_HAM_BBS_DB_PATH:-$ROOT_DIR/.runtime/py_ham_bbs_protocol.db}"
mkdir -p "$(dirname "$PY_HAM_BBS_DB_PATH")"

cleanup() {
	trap - INT TERM EXIT
	kill 0 2>/dev/null || true
}
trap cleanup INT TERM EXIT

echo "Backend: ws://127.0.0.1:${PY_HAM_BBS_PORT:-8765} (Direwolf KISS required)"
echo "Frontend: http://127.0.0.1:5173"

(cd "$BACKEND_DIR" && exec uv run python src/server.py) &
(cd "$FRONTEND_DIR" && exec pnpm dev --host 127.0.0.1) &
wait
