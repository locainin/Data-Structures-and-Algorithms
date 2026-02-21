#!/usr/bin/env bash
set -euo pipefail

# Script purpose: run and explain one full before/after demonstration

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
UNOPT_SRC="$ROOT_DIR/02_unoptimized/main.cpp"
OPT_SRC="$ROOT_DIR/03_optimized_with_data_structure/main.cpp"
BUILD_DIR="$(mktemp -d)"
trap 'rm -rf "$BUILD_DIR"' EXIT

CONCEPT="Linked Lists"
UNOPT_DS="Dynamic array middle inserts/erases"
OPT_DS="Doubly-linked list via std::list"
UNOPT_HOW="Middle updates shift many elements on each operation"
OPT_HOW="Node relinking supports local insert/remove around current position"
UNOPT_COMPLEXITY="Frequent shifting dominates edit loops"
OPT_COMPLEXITY="Localized pointer updates reduce update cost"

extract_elapsed_ms() {
  local output_file="$1"
  awk -F': ' '/elapsed_ms:/ {value=$2} END {if (value != "") print value}' "$output_file"
}

printf 'Concept: %s\n' "$CONCEPT"
printf 'Case:\n'; cat "$ROOT_DIR/01_real_world_case/case.txt"
printf '\nWhat is happening\n'
printf '  unoptimized: %s\n' "$UNOPT_DS"
printf '  how: %s\n' "$UNOPT_HOW"
printf '  behavior: %s\n' "$UNOPT_COMPLEXITY"
printf '  optimized: %s\n' "$OPT_DS"
printf '  how: %s\n' "$OPT_HOW"
printf '  behavior: %s\n' "$OPT_COMPLEXITY"

printf '\nCompiling...\n'
g++ -std=c++17 -O2 "$UNOPT_SRC" -o "$BUILD_DIR/unoptimized_demo"
g++ -std=c++17 -O2 "$OPT_SRC" -o "$BUILD_DIR/optimized_demo"

UNOPT_OUT="$BUILD_DIR/unoptimized_output.txt"
OPT_OUT="$BUILD_DIR/optimized_output.txt"

printf '\nRunning unoptimized version...\n'
"$BUILD_DIR/unoptimized_demo" | tee "$UNOPT_OUT"
printf '\nRunning optimized version...\n'
"$BUILD_DIR/optimized_demo" | tee "$OPT_OUT"

UNOPT_MS="$(extract_elapsed_ms "$UNOPT_OUT")"
OPT_MS="$(extract_elapsed_ms "$OPT_OUT")"

printf '\nSummary\n'
printf '  unoptimized_elapsed_ms: %s\n' "${UNOPT_MS:-n/a}"
printf '  optimized_elapsed_ms: %s\n' "${OPT_MS:-n/a}"
if [[ -n "${UNOPT_MS:-}" && -n "${OPT_MS:-}" ]]; then
  if [[ "$OPT_MS" -eq 0 ]]; then
    printf '  result: optimized run is below 1ms timer granularity\n'
  else
    SPEEDUP="$(awk -v unopt="$UNOPT_MS" -v opt="$OPT_MS" 'BEGIN {printf "%.2f", unopt / opt}')"
    printf '  result: optimized is %sx faster in this run\n' "$SPEEDUP"
  fi
fi
