"""Stable interface between message handling and radio transports."""

from typing import Protocol


class RadioInterface(Protocol):
	"""Minimal transport contract required by the protocol server."""

	@property
	def host(self) -> str: ...

	@property
	def port(self) -> int: ...

	def send_kiss_frame(self, kiss_frame: bytes) -> None: ...

	def close(self) -> None: ...
