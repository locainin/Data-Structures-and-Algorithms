"""Parsing helpers for case files and benchmark output text

This module converts plain-text program output into structured fields
"""

from __future__ import annotations

import re
from pathlib import Path


def parse_case_file(path: Path) -> dict[str, str]:
    """Read case metadata from a concept case.txt file

    Expected input format:
    - Real-world case: ...
    - Problem: ...
    - Pain point: ...
    """
    details: dict[str, str] = {}

    # Read all lines once so parse behavior is deterministic and simple
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or ":" not in line:
            continue

        # Convert keys into predictable snake_case names
        key, value = line.split(":", 1)
        normalized_key = key.strip().lower().replace("-", "_").replace(" ", "_")
        details[normalized_key] = value.strip()

    return {
        "real_world_case": details.get("real_world_case", ""),
        "problem": details.get("problem", ""),
        "pain_point": details.get("pain_point", ""),
    }


def parse_elapsed_ms(output: str) -> int | None:
    """Extract integer elapsed_ms from one demo output block"""
    match = re.search(r"elapsed_ms:\s*(\d+)", output)
    if not match:
        return None
    return int(match.group(1))


def parse_metrics(output: str) -> dict[str, str]:
    """Collect non-timing key/value fields from program output

    Every line in form `name: value` is parsed except elapsed_ms
    """
    metrics: dict[str, str] = {}

    # Preserve all custom data points emitted by each concept program
    for line in output.splitlines():
        if ":" not in line:
            continue

        key, value = line.split(":", 1)
        normalized_key = key.strip().lower().replace(" ", "_")
        if normalized_key == "elapsed_ms":
            continue
        metrics[normalized_key] = value.strip()

    return metrics
