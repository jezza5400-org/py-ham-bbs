# py-ham-bbs

py-ham-bbs is a lightweight packet-radio bulletin board with a Python WebSocket backend, a TypeScript browser client, and optional Direwolf KISS/Hamlib hardware integration.

## Architecture

- `backend-py/` contains protocol validation, persistence, and the WebSocket server.
- `backend-py/src/radio/` defines the hardware boundary; the application uses Direwolf KISS and does not provide a simulated radio backend.
- `frontend-ts/` contains the browser client.
- `config/` contains versioned service configuration, including Direwolf.
- `docker/` contains the backend, frontend, and Direwolf/Hamlib container builds.
- `deploy/pi/` and `scripts/` contain native Raspberry Pi OS deployment only.

## Direct development

Requirements: Python 3.14+, `uv`, Node.js, and pnpm 11.

Run the backend tests:

```sh
cd backend-py
uv sync
uv run pytest
```

Run both applications with the configured Direwolf/Hamlib services:

```sh
./scripts/dev.sh
```

The backend listens on `ws://127.0.0.1:8765` and its health endpoint is `http://127.0.0.1:8080/`. Vite serves the browser client at `http://127.0.0.1:5173` and proxies `/ws` to the backend. `scripts/dev.sh` stores local runtime data in ignored `.runtime/`.

## Configuration

Configuration is injected with environment variables. Important settings are:

| Variable | Default | Purpose |
| --- | --- | --- |
| `PY_HAM_BBS_DB_PATH` | `.runtime/py_ham_bbs_protocol.db` | SQLite database path |
| `PY_HAM_BBS_PORT` | `8765` | WebSocket port |
| `PY_HAM_BBS_HEALTH_PORT` | `8080` | HTTP health port |
| `PY_HAM_BBS_DIREWOLF_HOST` | `127.0.0.1` | KISS TCP host |
| `PY_HAM_BBS_DIREWOLF_PORT` | `8001` | KISS TCP port |
| `PY_HAM_BBS_DIREWOLF_CONFIG` | `config/direwolf/direwolf.conf` | Direwolf config path |
| `PY_HAM_BBS_HAMLIB_HOST` | `127.0.0.1` | Hamlib companion host |
| `PY_HAM_BBS_HAMLIB_PORT` | `4532` | Hamlib companion port |

The Compose development stack maps `.runtime/py_ham_bbs_protocol.db` to `/app/data/py_ham_bbs_protocol.db`, so development data survives container replacement without living beside application code. The populated database was migrated from `backend-py/py_ham_bbs_protocol.db`. Pi installs use the separate mutable path `/var/lib/ham-bbs/`; Pi updates never replace that database.

## Docker Compose

Compose remains the integration path and builds all three services from the shared source tree:

```sh
docker compose config
docker compose up --build
```

Compose expects `RADIO_DEVICE`, `RADIO_MODEL`, audio/serial group IDs, and a hardware-appropriate `config/direwolf/direwolf.conf` in `.env`. The application connects to the Compose Direwolf KISS service; Hamlib `4.7.2` and Direwolf `1.8.1` are explicit build inputs. For a no-radio daemon check only, set `RADIO_MODEL=1`, `RADIO_DEVICE=/dev/null`, `ADEVICE null`, and `PTT none` explicitly; these are radio-daemon settings, not an application backend.

The browser is available at `http://localhost:8080`; the backend WebSocket remains internal to the frontend proxy.

## Raspberry Pi OS Lite

The initial Pi deployment is native provisioning on Raspberry Pi OS Lite 64-bit. Use a tagged checkout or release source:

```sh
sudo HAM_BBS_SOURCE_DIR="$PWD" ./scripts/install-pi.sh
sudo HAM_BBS_SOURCE_DIR="$PWD" ./scripts/update-pi.sh <tag-or-commit>
```

The installer creates `ham-bbs`, installs pinned pnpm and uv bootstrap versions, builds the frontend, installs `ham-bbs.service`, `direwolf.service`, and `rigctld.service`, and performs a health check. It preserves existing `/etc/ham-bbs` configuration and `/var/lib/ham-bbs` state. See [deploy/pi/README.md](deploy/pi/README.md) for paths and hardware setup.

Configure stable `/dev/serial/by-id` and ALSA identifiers in `/etc/ham-bbs/ham-bbs.env` and `/etc/ham-bbs/direwolf.conf`. Logs are managed by journald:

```sh
journalctl -u ham-bbs.service -f
```

The updater has no rollback implementation yet; keep a backup of `/etc/ham-bbs` and `/var/lib/ham-bbs` as part of the host backup policy.

## Tests and builds

```sh
cd backend-py && uv run pytest
cd frontend-ts && pnpm install --frozen-lockfile && pnpm build
docker compose config
```
