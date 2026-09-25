FROM node:26-alpine AS builder

WORKDIR /app
RUN npm install -g pnpm
ENV PNPM_HOME="/pnpm"
ENV PATH="$PNPM_HOME:$PATH"

COPY frontend-ts/pnpm-lock.yaml frontend-ts/pnpm-workspace.yaml frontend-ts/package.json frontend-ts/tsconfig.json frontend-ts/vite.config.ts ./
RUN pnpm install --frozen-lockfile

COPY frontend-ts/src ./src
COPY frontend-ts/css ./css
COPY frontend-ts/images ./images
COPY frontend-ts/index.html ./
RUN pnpm build

FROM caddy:2.11-alpine
WORKDIR /srv
RUN apk add --no-cache curl
COPY frontend-ts/Caddyfile ./Caddyfile
COPY --from=builder /app/dist ./dist

EXPOSE 8080
HEALTHCHECK --interval=10s --timeout=5s --start-period=5s --retries=3 \
	CMD curl -fsI --max-time 3 http://localhost:8080/ || exit 1
ENTRYPOINT ["caddy", "run", "--config", "Caddyfile", "--adapter", "caddyfile"]