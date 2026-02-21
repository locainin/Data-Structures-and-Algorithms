#!/usr/bin/env bash
set -euo pipefail

# Runs all benchmark binaries, writes fresh timing data, then serves the dashboard
# Usage:
#   ./run_web_demo.sh           # uses default port 8080
#   ./run_web_demo.sh 9000      # uses custom port 9000 (example value)
# Environment:
#   HOST=127.0.0.1              # bind host
#   NO_BROWSER=1                # do not auto-open browser

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WEB_DIR="$ROOT_DIR/web"
PORT="${1:-8080}"
HOST="${HOST:-127.0.0.1}"
URL="http://${HOST}:${PORT}"
mkdir -p "$WEB_DIR/data"

printf '\nRunning benchmarks for fresh measurements...\n'
# Run generator as a module from web/ so package imports stay clean
(cd "$WEB_DIR" && python3 -m GenerateResults.main --refresh) >/tmp/ds_web_generate.log 2>&1
printf 'Benchmark generation complete\n'

python3 "$WEB_DIR/serve.py" --host "$HOST" --port "$PORT" --directory "$WEB_DIR" >/tmp/ds_web_server.log 2>&1 &
SERVER_PID=$!

cleanup() {
  kill "$SERVER_PID" >/dev/null 2>&1 || true
}
trap cleanup EXIT INT TERM

if [[ "${NO_BROWSER:-0}" != "1" ]]; then
  if command -v xdg-open >/dev/null 2>&1; then
    xdg-open "$URL" >/dev/null 2>&1 || true
  elif command -v open >/dev/null 2>&1; then
    open "$URL" >/dev/null 2>&1 || true
  fi
fi

printf '\nDashboard ready: %s\n' "$URL"
printf 'HTTP server log: /tmp/ds_web_server.log\n'
printf 'Benchmark refresh log: /tmp/ds_web_generate.log\n'
printf 'Press Ctrl+C to stop the server\n\n'

wait "$SERVER_PID"
