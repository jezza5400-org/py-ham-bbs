#!/usr/bin/env bash
set -euo pipefail

if [[ "$(id -u)" -ne 0 ]]; then echo "Run as root: sudo $0" >&2; exit 1; fi
app_root="${HAM_BBS_APP_ROOT:-/opt/ham-bbs}"
source_dir="${HAM_BBS_SOURCE_DIR:-$app_root}"
target_ref="${HAM_BBS_VERSION:-${1:-}}"

if [[ -z "$target_ref" ]]; then echo "Usage: HAM_BBS_SOURCE_DIR=/path/to/checkout sudo $0 <tag-or-commit>" >&2; exit 1; fi
if [[ ! -d "$source_dir/.git" ]]; then echo "Source checkout not found: $source_dir" >&2; exit 1; fi
git -C "$source_dir" fetch --tags --prune
git -C "$source_dir" checkout --detach "$target_ref"

HAM_BBS_SOURCE_DIR="$source_dir" "$(dirname "${BASH_SOURCE[0]}")/install-pi.sh"

echo "Updated py-ham-bbs to $(git -C "$app_root" rev-parse --short HEAD). Configuration and state were preserved. No rollback is implemented."
