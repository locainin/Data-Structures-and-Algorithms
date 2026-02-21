#!/usr/bin/env python3
"""Local static server with basic hardening for demo content

Security posture for local use:
- Serves only files under the provided web directory
- Adds defensive response headers
- Disables directory listing responses
"""

from __future__ import annotations

import argparse
import mimetypes
from os import PathLike
from functools import partial
from http import HTTPStatus
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import IO

# Ensure modern JS module/content types are recognized on all platforms
mimetypes.add_type("application/javascript", ".js")


class HardenedStaticHandler(SimpleHTTPRequestHandler):
    """Simple static handler with small local hardening defaults"""

    def end_headers(self) -> None:
        self.send_header("X-Content-Type-Options", "nosniff")
        self.send_header("Referrer-Policy", "no-referrer")
        self.send_header("X-Frame-Options", "DENY")
        self.send_header(
            "Content-Security-Policy",
            "default-src 'self'; style-src 'self' https://fonts.googleapis.com; "
            "font-src 'self' https://fonts.gstatic.com; script-src 'self'; connect-src 'self';",
        )
        super().end_headers()

    def list_directory(self, path: str | PathLike[str]) -> IO[bytes] | None:
        # Block directory indexing to reduce accidental disclosure of file lists
        self.send_error(HTTPStatus.FORBIDDEN, "Directory listing is disabled")
        return None


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Serve static dashboard files with hardening")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8080)
    parser.add_argument("--directory", type=Path, required=True)
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    web_dir = args.directory.resolve()

    if not web_dir.exists() or not web_dir.is_dir():
        raise SystemExit(f"Directory not found: {web_dir}")

    handler = partial(HardenedStaticHandler, directory=str(web_dir))
    server = ThreadingHTTPServer((args.host, args.port), handler)

    print(f"Serving {web_dir} on http://{args.host}:{args.port}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
