import asyncio
from datetime import UTC, datetime
from zoneinfo import ZoneInfo

from websockets.asyncio.server import ServerConnection, serve
from websockets.exceptions import ConnectionClosed


async def handler(websocket: ServerConnection) -> None:
	"""Handles a single client connection."""

	try:
		while True:
			now = datetime.now(UTC).isoformat()
			now_local = datetime.now(ZoneInfo("Australia/Melbourne")).isoformat()
			print(f"U-NOW: {now}\nL-NOW: {now_local}")
			await websocket.send(now_local)
			await asyncio.sleep(1)
	except ConnectionClosed:
		print("Client disconnected.")


async def main() -> None:
	async with serve(handler, "0.0.0.0", 8765) as server:  # noqa: S104
		print("Time server started on ws://0.0.0.0:8765")
		await server.serve_forever()


if __name__ == "__main__":
	asyncio.run(main())
