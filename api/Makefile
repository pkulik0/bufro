PKG = github.com/pkulik0/bufro/api
VERSION = $(shell git describe --tags --always --dirty)
BUILD_TIME = $(shell date -u +"%Y-%m-%dT%H:%M:%SZ")
FLAGS = -ldflags "-X $(PKG)/internal/version.BuildTime=$(BUILD_TIME)"

run: 
	@go run $(FLAGS) ./cmd

version:
	@echo $(VERSION) > ./internal/version/version.txt

build: 
	@mkdir -p ./build
	@go build $(FLAGS) -o ./build/api ./cmd

test:
	@go test -v ./...

deps:
	@go install go.uber.org/mock/mockgen@latest

generate: deps
	@go generate ./...

OCI_TAG = ghcr.io/pkulik0/bufro/api:latest
oci: version
	@docker build --platform linux/amd64 -t $(OCI_TAG) .
	@docker push $(OCI_TAG)
