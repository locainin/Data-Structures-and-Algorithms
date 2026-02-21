"""Cache helpers for concept signature matching

This module decides when old benchmark data can be safely reused
"""

from __future__ import annotations

import hashlib
import json
from pathlib import Path

from .settings import CACHE_SCHEMA_VERSION


def load_previous_by_slug(output_path: Path) -> dict[str, dict]:
    """Load existing results file and index concepts by slug

    Returns empty map when the file is missing, invalid, or schema-mismatched
    """
    if not output_path.exists():
        return {}

    try:
        payload = json.loads(output_path.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return {}

    if payload.get("cache_schema_version") != CACHE_SCHEMA_VERSION:
        return {}

    previous: dict[str, dict] = {}
    for concept in payload.get("concepts", []):
        slug = concept.get("slug")
        if isinstance(slug, str):
            previous[slug] = concept

    return previous


def compute_signature(
    concept: dict,
    case_payload: dict,
    unoptimized_source_text: str,
    optimized_source_text: str,
) -> str:
    """Build deterministic hash for all inputs that affect concept output

    If this signature is unchanged, runtime results can be reused safely
    """
    payload = {
        "cache_schema_version": CACHE_SCHEMA_VERSION,
        "concept": concept,
        "case": case_payload,
        "source": {
            "unoptimized": unoptimized_source_text,
            "optimized": optimized_source_text,
        },
    }

    # Use compact and sorted JSON to produce stable hashes across runs
    normalized = json.dumps(payload, sort_keys=True, separators=(",", ":"))
    return hashlib.sha256(normalized.encode("utf-8")).hexdigest()
