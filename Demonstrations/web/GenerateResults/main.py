#!/usr/bin/env python3
"""CLI entrypoint for benchmark JSON generation

This file wires together parser, builder, cache loader, and output writer
"""

from __future__ import annotations

import argparse
import os
from datetime import datetime, timezone
from pathlib import Path

from .builder import build_results
from .cache import load_previous_by_slug
from .output import atomic_write_json, print_generation_summary
from .settings import DEFAULT_TIMEOUT_SECONDS


def parse_args() -> argparse.Namespace:
    """Build command-line interface for generation options"""
    parser = argparse.ArgumentParser(description="Generate data structure demo benchmark JSON")
    parser.add_argument("--refresh", action="store_true", help="Ignore cache and benchmark every concept")
    parser.add_argument(
        "--max-workers",
        type=int,
        default=min(6, max(1, os.cpu_count() or 1)),
        help="Maximum parallel concept benchmark workers",
    )
    parser.add_argument(
        "--timeout-seconds",
        type=int,
        default=DEFAULT_TIMEOUT_SECONDS,
        help="Per-program execution timeout",
    )
    return parser.parse_args()


def main() -> None:
    """Run full generation flow and write results.json"""
    args = parse_args()

    # Resolve file paths relative to this package directory
    package_dir = Path(__file__).resolve().parent
    web_dir = package_dir.parent
    root_dir = web_dir.parent
    output_path = web_dir / "data" / "results.json"

    # Load previous payload to enable safe cache reuse
    previous_by_slug = load_previous_by_slug(output_path)

    # Use UTC timestamp for deterministic machine-readable metadata
    generated_at_iso = datetime.now(timezone.utc).isoformat()

    results, stats = build_results(
        root_dir=root_dir,
        previous_by_slug=previous_by_slug,
        force_refresh=args.refresh,
        max_workers=max(1, args.max_workers),
        timeout_seconds=max(1, args.timeout_seconds),
        generated_at_iso=generated_at_iso,
    )

    atomic_write_json(output_path, results)
    print_generation_summary(output_path, results, stats)


if __name__ == "__main__":
    main()
