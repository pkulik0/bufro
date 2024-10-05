package server

import (
	"io"
	"net/http"

	"github.com/rs/zerolog/log"
	"google.golang.org/protobuf/proto"

	"github.com/pkulik0/bufro/api/internal/model"
	"github.com/pkulik0/bufro/api/internal/pb"
)

func pbToInternalType(t pb.BufType) model.BufType {
	switch t {
	case pb.BufType_BUF_TYPE_IMAGE:
		return model.TypeImage
	case pb.BufType_BUF_TYPE_VIDEO:
		return model.TypeVideo
	default:
		return model.TypeUnknown
	}
}

func internalToPbType(t model.BufType) pb.BufType {
	switch t {
	case model.TypeImage:
		return pb.BufType_BUF_TYPE_IMAGE
	case model.TypeVideo:
		return pb.BufType_BUF_TYPE_VIDEO
	default:
		return pb.BufType_BUF_TYPE_UNKNOWN
	}
}

func readReq[T proto.Message](req *http.Request, dest T) error {
	data, err := io.ReadAll(req.Body)
	if err != nil {
		return err
	}
	return proto.Unmarshal(data, dest)
}

func respond[T proto.Message](w http.ResponseWriter, t T) {
	data, err := proto.Marshal(t)
	if err != nil {
		nilOrErr(w, err, "failed to marshal response", http.StatusInternalServerError)
		return
	}
	writeOrLog(w, data)
}

func writeOrLog(w http.ResponseWriter, data []byte) {
	if _, err := w.Write(data); err != nil {
		log.Error().Err(err).Msg("failed to write response")
	}
}

func nilOrErr(w http.ResponseWriter, err error, msg string, status int) {
	if err == nil {
		return
	}

	publicMsg := ""
	switch status {
	case http.StatusBadRequest:
		publicMsg = "bad request"
	case http.StatusInternalServerError:
		publicMsg = "internal server error"
	default:
		publicMsg = "unknown error"
	}

	log.Error().Err(err).Msg(msg)
	http.Error(w, publicMsg, status)
}
