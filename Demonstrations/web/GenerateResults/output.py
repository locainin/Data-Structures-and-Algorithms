"""Output writers and terminal reporting

This module handles file writes and human-readable summary output
"""

from __future__ import annotations

import json
from pathlib import Path


def atomic_write_json(output_path: Path, payload: dict) -> None:
    """Write JSON atomically to avoid partial reads by the web app

    Strategy:
    1) write to temporary sibling file
    2) rename into final target
    """
    output_path.parent.mkdir(parents=True, exist_ok=True)

    temp_path = output_path.with_suffix(output_path.suffix + ".tmp")
    temp_path.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    temp_path.replace(output_path)


def print_generation_summary(output_path: Path, results: dict, stats: dict[str, int]) -> None:
    """Print compact summary for terminal usage"""
    summary = results["summary"]

    print("Generated results:")
    print(f"  file: {output_path}")
    print(f"  concepts: {summary['successful_concepts']}/{summary['total_concepts']} successful")
    print(f"  cache_hits: {summary['cache_hits']}")
    print(f"  benchmark_runs: {stats['benchmark_runs']}")
    print(f"  measured_speedups: {summary['measured_speedups']}")
    if summary["average_speedup"] is not None:
        print(f"  average_speedup: {summary['average_speedup']}x")
    print(f"  elapsed_ms: {stats['elapsed_ms']}")
