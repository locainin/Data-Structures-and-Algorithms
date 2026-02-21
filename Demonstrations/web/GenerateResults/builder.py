"""Core payload builder

This module composes concept metadata, source snapshots, cache decisions,
and runtime benchmark results into the final JSON structure
"""

from __future__ import annotations

from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
from time import perf_counter

from .cache import compute_signature
from .concepts import CONCEPTS
from .parse import parse_case_file
from .runtime import evaluate_concept_runtime
from .settings import CACHE_SCHEMA_VERSION


def read_source(path: Path) -> str:
    """Read source text for code-comparison panels in the UI"""
    return path.read_text(encoding="utf-8")


def concept_base_payload(
    concept: dict,
    case_payload: dict,
    signature: str,
    unoptimized_source_text: str,
    optimized_source_text: str,
) -> dict:
    """Build concept fields that do not require executing binaries"""
    return {
        "slug": concept["slug"],
        "signature": signature,
        "title": concept["title"],
        "focus": concept["focus"],
        "case": case_payload,
        "education": {
            "definition": concept["definition"],
            "used_for": concept["used_for"],
            "why_unoptimized": concept["why_unoptimized"],
            "small_change": concept["small_change"],
            "before_complexity": concept["before_complexity"],
            "after_complexity": concept["after_complexity"],
        },
        "unoptimized_structure": concept["unoptimized_structure"],
        "optimized_structure": concept["optimized_structure"],
        "source": {
            "unoptimized": unoptimized_source_text,
            "optimized": optimized_source_text,
        },
    }


def build_results(
    root_dir: Path,
    previous_by_slug: dict[str, dict],
    force_refresh: bool,
    max_workers: int,
    timeout_seconds: int,
    generated_at_iso: str,
) -> tuple[dict, dict[str, int]]:
    """Build final dashboard payload and generation stats"""
    start_time = perf_counter()

    concept_payloads: list[dict] = []
    pending_jobs: dict = {}
    cache_hits = 0

    # Run concept benchmarks in parallel for faster full refreshes
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        for concept in CONCEPTS:
            slug = concept["slug"]
            concept_dir = root_dir / slug
            case_file = concept_dir / "01_real_world_case" / "case.txt"
            unoptimized_source = concept_dir / "02_unoptimized" / "main.cpp"
            optimized_source = concept_dir / "03_optimized_with_data_structure" / "main.cpp"

            # These inputs control cache invalidation
            case_payload = parse_case_file(case_file)
            unoptimized_source_text = read_source(unoptimized_source)
            optimized_source_text = read_source(optimized_source)
            signature = compute_signature(concept, case_payload, unoptimized_source_text, optimized_source_text)

            payload = concept_base_payload(
                concept=concept,
                case_payload=case_payload,
                signature=signature,
                unoptimized_source_text=unoptimized_source_text,
                optimized_source_text=optimized_source_text,
            )

            previous = previous_by_slug.get(slug)
            # Reuse only when signature and required runtime fields are present
            if (
                not force_refresh
                and previous
                and previous.get("signature") == signature
                and "error" not in previous
                and "unoptimized" in previous
                and "optimized" in previous
            ):
                payload["unoptimized"] = previous["unoptimized"]
                payload["optimized"] = previous["optimized"]
                payload["speedup"] = previous.get("speedup")
                payload["cache_hit"] = True
                cache_hits += 1
            else:
                # Queue runtime evaluation for this concept
                future = executor.submit(
                    evaluate_concept_runtime,
                    slug,
                    unoptimized_source,
                    optimized_source,
                    timeout_seconds,
                )
                pending_jobs[future] = payload

            concept_payloads.append(payload)

        # Merge async runtime outcomes into matching payload entries
        for future in as_completed(pending_jobs):
            payload = pending_jobs[future]
            try:
                runtime_payload = future.result()
                payload.update(runtime_payload)
                payload["cache_hit"] = False
            except Exception as exc:  # keep per-concept errors isolated
                payload["error"] = str(exc)
                payload["cache_hit"] = False

    available = [concept for concept in concept_payloads if "error" not in concept]
    finite_speedups = [concept["speedup"] for concept in available if concept.get("speedup") is not None]

    results = {
        "cache_schema_version": CACHE_SCHEMA_VERSION,
        "generated_at": generated_at_iso,
        "summary": {
            "total_concepts": len(concept_payloads),
            "successful_concepts": len(available),
            "cache_hits": cache_hits,
            "measured_speedups": len(finite_speedups),
            "average_speedup": round(sum(finite_speedups) / len(finite_speedups), 2) if finite_speedups else None,
            "max_speedup": max(finite_speedups) if finite_speedups else None,
        },
        "concepts": concept_payloads,
    }

    stats = {
        "cache_hits": cache_hits,
        "benchmark_runs": len(concept_payloads) - cache_hits,
        "elapsed_ms": int((perf_counter() - start_time) * 1000),
    }

    return results, stats
