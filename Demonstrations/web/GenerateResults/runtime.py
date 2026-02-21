"""Compile/run helpers for benchmark concept programs

This module owns execution-time behavior only
"""

from __future__ import annotations

import subprocess  # nosec B404
import tempfile
from pathlib import Path
from time import perf_counter_ns

from .parse import parse_elapsed_ms, parse_metrics
from .settings import GPP_PATH


def run_binary(source: Path, binary: Path, timeout_seconds: int) -> tuple[str, int]:
    """Compile one source file and execute it

    Returns:
    - stdout text from the program
    - measured wall-clock runtime in microseconds
    """
    if GPP_PATH is None:
        raise FileNotFoundError("g++ not found in PATH")

    # Build with consistent optimization flags for all concept programs
    subprocess.run(
        [GPP_PATH, "-std=c++20", "-O2", str(source), "-o", str(binary)],  # nosec B603
        check=True,
        capture_output=True,
        text=True,
    )

    # Measure actual runtime with high-resolution clock
    start_ns = perf_counter_ns()
    completed = subprocess.run(
        [str(binary)],  # nosec B603
        check=True,
        capture_output=True,
        text=True,
        timeout=timeout_seconds,
    )
    elapsed_us = max(1, int((perf_counter_ns() - start_ns) / 1_000))

    return completed.stdout.strip(), elapsed_us


def evaluate_concept_runtime(
    slug: str,
    unoptimized_source: Path,
    optimized_source: Path,
    timeout_seconds: int,
) -> dict:
    """Run unoptimized and optimized binaries for one concept

    The returned dictionary is merged into the concept payload by the builder
    """
    # Use an isolated temp build folder per concept to avoid collisions
    with tempfile.TemporaryDirectory(prefix=f"ds_demo_{slug}_") as temp_dir_name:
        temp_dir = Path(temp_dir_name)
        unoptimized_binary = temp_dir / f"{slug}_unoptimized"
        optimized_binary = temp_dir / f"{slug}_optimized"

        unoptimized_output, unoptimized_elapsed_us = run_binary(unoptimized_source, unoptimized_binary, timeout_seconds)
        optimized_output, optimized_elapsed_us = run_binary(optimized_source, optimized_binary, timeout_seconds)

    # Keep both the program-reported ms and measured high-resolution runtime
    program_unoptimized_ms = parse_elapsed_ms(unoptimized_output)
    program_optimized_ms = parse_elapsed_ms(optimized_output)
    measured_unoptimized_ms = round(unoptimized_elapsed_us / 1000.0, 3)
    measured_optimized_ms = round(optimized_elapsed_us / 1000.0, 3)

    speedup: float | None = None
    if optimized_elapsed_us > 0:
        speedup = round(unoptimized_elapsed_us / optimized_elapsed_us, 2)

    return {
        "unoptimized": {
            "elapsed_ms": measured_unoptimized_ms,
            "elapsed_display": f"{measured_unoptimized_ms:.3f}ms",
            "elapsed_us": unoptimized_elapsed_us,
            "program_elapsed_ms": program_unoptimized_ms,
            "metrics": parse_metrics(unoptimized_output),
            "raw_output": unoptimized_output,
        },
        "optimized": {
            "elapsed_ms": measured_optimized_ms,
            "elapsed_display": f"{measured_optimized_ms:.3f}ms",
            "elapsed_us": optimized_elapsed_us,
            "program_elapsed_ms": program_optimized_ms,
            "metrics": parse_metrics(optimized_output),
            "raw_output": optimized_output,
        },
        "speedup": speedup,
    }
