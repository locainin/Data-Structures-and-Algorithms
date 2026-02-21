"""Shared configuration for benchmark generation

Only global constants and environment-derived values live here
"""

from __future__ import annotations

import shutil

# Increment when payload structure changes so stale cached JSON is ignored
CACHE_SCHEMA_VERSION = "4"

# Hard timeout for each demo binary execution
DEFAULT_TIMEOUT_SECONDS = 120

# Locate compiler once during startup to avoid repeated path lookups
GPP_PATH = shutil.which("g++")
