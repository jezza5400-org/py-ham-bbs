"""Construct the configured Direwolf KISS transport."""

from runtime_config import Settings, load_settings

from .direwolf import DirewolfRadio


def create_radio(settings: Settings | None = None) -> DirewolfRadio:
	settings = settings or load_settings()
	return DirewolfRadio(settings.kiss_host, settings.kiss_port)
