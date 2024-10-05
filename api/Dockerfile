FROM golang:alpine AS builder
RUN apk add --no-cache make
WORKDIR /app
COPY go.mod go.sum ./
RUN go mod download
COPY . .
RUN make build

FROM alpine:latest AS runner
RUN apk add --no-cache ca-certificates
WORKDIR /app
COPY --from=builder /app/build/api .
CMD ["/app/api"]
