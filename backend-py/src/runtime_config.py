"""Runtime configuration loaded from environment variables."""

import os
from dataclasses import dataclass
from pathlib import Path

from lib.database import resolve_db_path

PROJECT_ROOT = Path(__file__).resolve().parents[3]


def _path_env(name: str, default: Path) -> Path:
	raw_path = os.getenv(name)
	if raw_path is None:
		return default
	path = Path(raw_path).expanduser()
	return path if path.is_absolute() else PROJECT_ROOT / path


def _int_env(name: str, default: int) -> int:
	try:
		value = int(os.getenv(name, str(default)))
	except ValueError:
		return default
	return value if 1 <= value <= 65535 else default


@dataclass(frozen=True, slots=True)
class Settings:
	bind_host: str
	protocol_port: int
	health_port: int
	database_path: Path
	server_source: str
	kiss_host: str
	kiss_port: int
	direwolf_config_path: Path
	hamlib_host: str
	hamlib_port: int


def load_settings() -> Settings:
	return Settings(
		bind_host=os.getenv("PY_HAM_BBS_BIND_HOST", "0.0.0.0"),  # noqa: S104
		protocol_port=_int_env("PY_HAM_BBS_PORT", 8765),
		health_port=_int_env("PY_HAM_BBS_HEALTH_PORT", 8080),
		database_path=resolve_db_path(),
		server_source=os.getenv("PY_HAM_BBS_SERVER_SOURCE", "SERVER-0"),
		kiss_host=os.getenv("PY_HAM_BBS_DIREWOLF_HOST", os.getenv("DIREWOLF_HOST", "127.0.0.1")),
		kiss_port=_int_env("PY_HAM_BBS_DIREWOLF_PORT", _int_env("DIREWOLF_PORT", 8001)),
		direwolf_config_path=_path_env("PY_HAM_BBS_DIREWOLF_CONFIG", PROJECT_ROOT / "config" / "direwolf" / "direwolf.conf"),
		hamlib_host=os.getenv("PY_HAM_BBS_HAMLIB_HOST", "127.0.0.1"),
		hamlib_port=_int_env("PY_HAM_BBS_HAMLIB_PORT", 4532),
	)
