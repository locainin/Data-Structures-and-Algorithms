"""Concept metadata used to build educational dashboard cards

This file intentionally contains static data only
"""

from __future__ import annotations

# Every concept object powers one dashboard card and one benchmark pair
# Required keys are consumed by builder.py and app.js
CONCEPTS = [
    # Graph traversal story: repeated reachability checks
    {
        "slug": "graphs",
        "title": "Graphs",
        "focus": "Fast repeated reachability checks",
        "definition": "A graph models entities as nodes and relationships as edges",
        "used_for": [
            "routing and navigation",
            "social network connections",
            "dependency analysis",
        ],
        "unoptimized_structure": "Edge list scanned repeatedly",
        "optimized_structure": "Adjacency list + BFS",
        "why_unoptimized": "Each traversal step loops over all edges to find neighbors, repeating expensive work",
        "small_change": "Precompute neighbors per node once, then BFS only over connected neighbors",
        "before_complexity": "Neighbor discovery repeatedly scans the entire edge list",
        "after_complexity": "Traversal cost tracks visited nodes and adjacent edges",
    },
    # Hashing story: key lookup under request load
    {
        "slug": "hash-tables",
        "title": "Hash Tables",
        "focus": "Low-latency key lookup under load",
        "definition": "A hash table maps keys to values through a hash function for fast direct access",
        "used_for": [
            "auth token/session lookup",
            "caches",
            "deduplication",
        ],
        "unoptimized_structure": "Linear scan over token list",
        "optimized_structure": "std::unordered_set hash table",
        "why_unoptimized": "Every token validation compares against many entries before finding a match or failing",
        "small_change": "Replace sequential comparison loop with hashed membership lookup",
        "before_complexity": "Lookup grows with token count",
        "after_complexity": "Average lookup stays near constant time",
    },
    # Linked-list story: local edits around a moving cursor
    {
        "slug": "linked-lists",
        "title": "Linked Lists",
        "focus": "Frequent local insertions/removals",
        "definition": "A linked list stores elements as nodes connected by pointers",
        "used_for": [
            "playlist/editor cursor operations",
            "LRU-like local reordering",
            "fast local splice operations",
        ],
        "unoptimized_structure": "Vector middle insert/erase shifts",
        "optimized_structure": "Doubly linked list",
        "why_unoptimized": "Middle insert and erase force element shifting on almost every operation",
        "small_change": "Store items as linked nodes so local insert/remove updates pointers instead of shifting arrays",
        "before_complexity": "Middle edits trigger repeated move costs",
        "after_complexity": "Local edits avoid large-scale memory movement",
    },
    # Search story: linear scan versus logarithmic search
    {
        "slug": "searching-and-analysis",
        "title": "Searching and Analysis",
        "focus": "Repeated ID lookup at scale",
        "definition": "Searching chooses data-access patterns to locate values efficiently",
        "used_for": [
            "customer/account lookups",
            "record filtering",
            "membership checks",
        ],
        "unoptimized_structure": "Linear scan search",
        "optimized_structure": "Binary search on sorted vector",
        "why_unoptimized": "Each query walks through IDs one-by-one until match or end",
        "small_change": "Keep data ordered and use binary search to halve search space each step",
        "before_complexity": "Lookup time grows linearly with input size",
        "after_complexity": "Lookup time grows logarithmically with input size",
    },
    # Sorting story: O(n^2) baseline versus optimized library sort
    {
        "slug": "sorting-algorithms",
        "title": "Sorting Algorithms",
        "focus": "Efficient ordering of large datasets",
        "definition": "Sorting arranges items so later processing and lookup become predictable",
        "used_for": [
            "transaction/report preparation",
            "time-series ordering",
            "ranked outputs",
        ],
        "unoptimized_structure": "Bubble sort",
        "optimized_structure": "std::sort",
        "why_unoptimized": "Adjacent swap passes repeat too many comparisons for bigger arrays",
        "small_change": "Use the optimized standard-library sort strategy instead of manual bubble passes",
        "before_complexity": "Quadratic comparison growth",
        "after_complexity": "Near n log n behavior for typical data",
    },
    # Queue/stack story: proper FIFO/LIFO containers versus costly shifts
    {
        "slug": "stacks-and-queues",
        "title": "Stacks and Queues",
        "focus": "Stable FIFO/LIFO processing",
        "definition": "Queues process first-in-first-out while stacks process last-in-first-out",
        "used_for": [
            "ticket pipelines",
            "task scheduling",
            "undo history",
        ],
        "unoptimized_structure": "Vector front erase queue",
        "optimized_structure": "std::deque queue + stack",
        "why_unoptimized": "Erasing index 0 shifts all remaining items every cycle",
        "small_change": "Use deque pop_front for queue behavior and a stack for recent-action rollback",
        "before_complexity": "Front removals repeatedly shift memory",
        "after_complexity": "Queue/stack operations stay stable per action",
    },
    # Tree story: ordered key navigation and range-style lookup
    {
        "slug": "trees",
        "title": "Trees",
        "focus": "Exact and range queries",
        "definition": "A tree keeps hierarchical ordered structure for efficient navigation",
        "used_for": [
            "catalog indexing",
            "range queries",
            "ordered key traversal",
        ],
        "unoptimized_structure": "Flat vector scan",
        "optimized_structure": "std::map ordered tree",
        "why_unoptimized": "Every lookup walks a flat list even when keys are naturally ordered",
        "small_change": "Store records in ordered tree nodes and query via tree navigation",
        "before_complexity": "Exact lookups scan linearly",
        "after_complexity": "Exact lookup and range start are logarithmic",
    },
]
