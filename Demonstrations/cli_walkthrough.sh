#!/usr/bin/env bash
set -euo pipefail

# Runs every CLI concept walkthrough in a consistent presentation order
# This script is intentionally sequential so each concept's explanation and
# benchmark output remains grouped in terminal output

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

printf 'Data Structures CLI Walkthrough\n'
printf 'Each concept explains baseline behavior, data structure choice, and runtime impact\n'

for concept in \
    graphs \
    hash-tables \
    linked-lists \
    searching-and-analysis \
    sorting-algorithms \
    stacks-and-queues \
    trees; do
  printf '\n========================================\n'
  printf 'Running: %s\n' "$concept"
  printf '========================================\n'
  "$ROOT_DIR/$concept/04_cli_walkthrough/run_demo.sh"
done
