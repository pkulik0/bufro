default: clean go cxx

GO_PB_PATH = ../api/internal/pb
CXX_PB_PATH = ../client/pb
TS_PB_PATH = ../web/src/lib/pb

deps:
	@go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

go:
	@mkdir -p $(GO_PB_PATH)
	@protoc --go_out=$(GO_PB_PATH) --go_opt=paths=source_relative bufro.proto

cxx:
	@mkdir -p $(CXX_PB_PATH)
	@protoc --cpp_out=$(CXX_PB_PATH) bufro.proto

clean:
	@rm -rf $(GO_PB_PATH) $(CXX_PB_PATH)
