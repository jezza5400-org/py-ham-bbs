FROM astral/uv:python3.14-trixie-slim AS builder

WORKDIR /app
ENV UV_COMPILE_BYTECODE=1
ENV UV_NO_DEV=1

COPY backend-py/pyproject.toml backend-py/uv.lock ./backend-py/
RUN cd backend-py && uv sync --frozen --no-install-project

COPY backend-py/ ./backend-py/
RUN cd backend-py && uv sync --frozen

FROM astral/uv:python3.14-trixie-slim AS runner
WORKDIR /app
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1
ENV PATH="/app/backend-py/.venv/bin:$PATH"

COPY --from=builder /app/backend-py/.venv /app/backend-py/.venv
COPY backend-py/ /app/backend-py/

ARG APP_GID=1000
RUN mkdir -p /app/data && chgrp ${APP_GID} /app/data && chmod g+w /app/data

EXPOSE 8765 8080
HEALTHCHECK --interval=10s --timeout=5s --start-period=5s --retries=3 \
	CMD python3 -c "import urllib.request; urllib.request.urlopen('http://127.0.0.1:8080/', timeout=2)"

ENTRYPOINT ["python3", "backend-py/src/server.py"]