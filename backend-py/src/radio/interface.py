"""Stable interface between message handling and radio transports."""

from typing import Protocol


class RadioInterface(Protocol):
	"""Minimal transport contract required by the protocol server."""

	@property
	def host(self) -> str:
		raise NotImplementedError

	@property
	def port(self) -> int:
		raise NotImplementedError

	def send_kiss_frame(self, kiss_frame: bytes) -> None:
		raise NotImplementedError

	def close(self) -> None:
		raise NotImplementedError
