#!/usr/bin/env bash
set -euo pipefail

if [[ "$(id -u)" -ne 0 ]]; then
	echo "Run as root: sudo $0" >&2
	exit 1
fi

source_dir="${HAM_BBS_SOURCE_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}"
app_root="${HAM_BBS_APP_ROOT:-/opt/ham-bbs}"
config_root="${HAM_BBS_CONFIG_ROOT:-/etc/ham-bbs}"
state_root="${HAM_BBS_STATE_ROOT:-/var/lib/ham-bbs}"
app_user="${HAM_BBS_USER:-ham-bbs}"
app_group="${HAM_BBS_GROUP:-ham-bbs}"
uv_version="${UV_VERSION:-0.8.22}"

if [[ ! -f /etc/os-release ]]; then
	echo "Unsupported system: /etc/os-release is missing" >&2
	exit 1
fi
# shellcheck disable=SC1091
source /etc/os-release
if [[ "${ID:-}" != "debian" && "${ID_LIKE:-}" != *debian* ]]; then
	echo "This installer supports Debian-based Raspberry Pi OS only." >&2
	exit 1
fi
case "$(dpkg --print-architecture)" in
	arm64|amd64) ;;
	*) echo "Unsupported architecture: $(dpkg --print-architecture)" >&2; exit 1 ;;
esac

apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y ca-certificates curl git rsync build-essential python3 python3-venv nodejs npm direwolf hamlib-utils

if ! getent group "$app_group" >/dev/null; then groupadd --system "$app_group"; fi
if ! id "$app_user" >/dev/null 2>&1; then useradd --system --gid "$app_group" --home-dir "$app_root" --shell /usr/sbin/nologin "$app_user"; fi
usermod -aG audio,dialout "$app_user" || true

install -d -o "$app_user" -g "$app_group" "$app_root" "$state_root" "$state_root/frontend"
install -d -o root -g "$app_group" -m 0750 "$config_root"
if [[ "$(realpath "$source_dir")" != "$(realpath "$app_root" 2>/dev/null || true)" ]]; then
	rsync -a --delete --exclude .git --exclude .venv --exclude node_modules --exclude dist --exclude '*.db' "$source_dir/" "$app_root/"
fi

if ! command -v uv >/dev/null 2>&1; then
	curl --proto '=https' --tlsv1.2 -LsSf "https://astral.sh/uv/${uv_version}/install.sh" | sh
	install -m 0755 /root/.local/bin/uv /usr/local/bin/uv
fi
runuser -u "$app_user" -- env HOME="$app_root" uv sync --frozen --project "$app_root/backend-py"

npm install --global "pnpm@11.0.0"
(cd "$app_root/frontend-ts" && pnpm install --frozen-lockfile && pnpm build)
rsync -a --delete "$app_root/frontend-ts/dist/" "$state_root/frontend/"

if [[ ! -f "$config_root/ham-bbs.env" ]]; then
	cat > "$config_root/ham-bbs.env" <<EOF
PY_HAM_BBS_BIND_HOST=127.0.0.1
PY_HAM_BBS_PORT=8765
PY_HAM_BBS_HEALTH_PORT=8080
PY_HAM_BBS_DB_PATH=$state_root/py_ham_bbs_protocol.db
PY_HAM_BBS_DIREWOLF_HOST=127.0.0.1
PY_HAM_BBS_DIREWOLF_PORT=8001
PY_HAM_BBS_DIREWOLF_CONFIG=$config_root/direwolf.conf
PY_HAM_BBS_HAMLIB_HOST=127.0.0.1
PY_HAM_BBS_HAMLIB_PORT=4532
RADIO_MODEL=1
RADIO_DEVICE=/dev/serial/by-id/radio
EOF
	chown root:"$app_group" "$config_root/ham-bbs.env"
	chmod 0640 "$config_root/ham-bbs.env"
fi
if [[ ! -f "$config_root/direwolf.conf" ]]; then
	install -m 0640 -o root -g "$app_group" "$app_root/config/direwolf/direwolf.conf" "$config_root/direwolf.conf"
fi

install -d -o root -g root -m 0755 /etc/systemd/system
install -m 0644 "$app_root/deploy/pi/systemd/"*.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable ham-bbs.service direwolf.service rigctld.service
systemctl restart ham-bbs.service
systemctl restart rigctld.service direwolf.service

if curl -fsS --max-time 5 http://127.0.0.1:8080/ >/dev/null; then
	echo "Backend health check passed."
else
	echo "Backend health check failed; inspect: journalctl -u ham-bbs.service -e" >&2
fi
echo "Installed version: $(git -C "$app_root" rev-parse --short HEAD 2>/dev/null || echo unknown)"
echo "Application: $app_root"
echo "Configuration: $config_root"
echo "State: $state_root"
