PKG = github.com/pkulik0/bufro/api
VERSION = $(shell git describe --tags --always --dirty)
BUILD_TIME = $(shell date -u '+%Y-%m-%d_%H:%M:%S')
FLAGS = -ldflags "-X $(PKG)/internal/version.Version=$(VERSION) -X $(PKG)/internal/version.BuildTime=$(BUILD_TIME)"

run:
	@go run $(FLAGS) ./cmd

build:
	@mkdir -p ./build
	@go build $(FLAGS) -o ./build/api ./cmd