"""Direwolf KISS transport adapter."""

from lib.direwolf import DirewolfKISSClient


class DirewolfRadio:
	"""Expose the existing Direwolf client through the radio interface."""

	def __init__(self, host: str, port: int) -> None:
		self._client = DirewolfKISSClient(host=host, port=port)

	@property
	def host(self) -> str:
		return self._client.host

	@property
	def port(self) -> int:
		return self._client.port

	def send_kiss_frame(self, kiss_frame: bytes) -> None:
		self._client.send_kiss_frame(kiss_frame)

	def close(self) -> None:
		self._client.close()
