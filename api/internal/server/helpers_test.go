package server

import (
	"io"
	"net/http/httptest"

	"google.golang.org/protobuf/proto"
)

func pbFromResp[T proto.Message](w *httptest.ResponseRecorder, dest T) error {
	data, err := io.ReadAll(w.Body)
	if err != nil {
		return err
	}
	return proto.Unmarshal(data, dest)
}
