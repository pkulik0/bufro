package server

import (
	"encoding/json"
	"fmt"
	"net/http"

	"github.com/rs/zerolog/log"

	"github.com/pkulik0/bufro/api/internal/auth"
	"github.com/pkulik0/bufro/api/internal/bufro"
	"github.com/pkulik0/bufro/api/internal/model"
	"github.com/pkulik0/bufro/api/internal/pb"
	"github.com/pkulik0/bufro/api/internal/version"
)

type server struct {
	b bufro.Bufro
	a auth.Auth
}

// NewServer creates a new API server
func NewServer(b bufro.Bufro, a auth.Auth) *server {
	return &server{
		b: b,
		a: a,
	}
}

func (s *server) handlerRoot(w http.ResponseWriter, r *http.Request) {
	data, err := json.Marshal(version.Information())
	if err != nil {
		nilOrErr(w, err, "failed to marshal response", http.StatusInternalServerError)
		return
	}
	writeOrLog(w, data)
}

func (s *server) handlerCreate(w http.ResponseWriter, r *http.Request) {
	var req pb.CreateBufRequest
	err := readReq(r, &req)
	if err != nil {
		nilOrErr(w, err, "failed to read request", http.StatusBadRequest)
		return
	}

	uid := getUID(r)
	buf := model.NewBuf(uid, pbToInternalType(req.Type), req.Data)
	err = s.b.CreateBuf(r.Context(), buf)
	if err != nil {
		nilOrErr(w, err, "failed to create buf", http.StatusInternalServerError)
		return
	}

	respond(w, &pb.CreateBufResponse{Id: buf.ID})
}

func (s *server) handlerGet(w http.ResponseWriter, r *http.Request) {
	id := r.PathValue("id")
	buf, err := s.b.GetBuf(r.Context(), id)
	if err != nil {
		nilOrErr(w, err, "failed to get buf", http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "image/png")
	writeOrLog(w, buf.Data)
}

// Start starts serving the API on the given port
func (s *server) Start(port int) error {
	mux := http.NewServeMux()

	mux.HandleFunc("/{$}", s.handlerRoot)
	mux.Handle("POST /b", authMiddleware(s.a, http.HandlerFunc(s.handlerCreate)))
	mux.HandleFunc("GET /b/{id}", s.handlerGet)

	addr := fmt.Sprintf(":%d", port)
	log.Info().Str("addr", addr).Msg("starting server")
	return http.ListenAndServe(addr, logMiddleware(mux))
}
