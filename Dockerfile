FROM debian:bookworm-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake libpq-dev ca-certificates && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF && cmake --build build -j2

FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends libpq5 ca-certificates && rm -rf /var/lib/apt/lists/* && useradd --system --uid 10001 audit
WORKDIR /app
COPY --from=build /src/build/audit-log-service ./audit-log-service
COPY db/migration ./db/migration
USER audit
ENV AUDIT_LOG_HOST=0.0.0.0
EXPOSE 8080
ENTRYPOINT ["./audit-log-service"]
