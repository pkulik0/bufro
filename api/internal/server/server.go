package server

import (
	"encoding/json"
	"fmt"
	"net/http"

	"github.com/rs/zerolog/log"

	"github.com/pkulik0/bufro/api/internal/version"
)

type Server struct {
}

func NewServer() *Server {
	return &Server{}
}

func (s *Server) handlerRoot(w http.ResponseWriter, r *http.Request) {
	data, err := json.Marshal(version.Information())
	if err != nil {
		log.Error().Err(err).Msg("failed to marshal version information")
		http.Error(w, "internal server error", http.StatusInternalServerError)
		return
	}
	w.Write(data)
}

func (s *Server) Start(port int) error {
	mux := http.NewServeMux()
	mux.HandleFunc("GET /", s.handlerRoot)

	addr := fmt.Sprintf(":%d", port)
	log.Info().Str("addr", addr).Msg("starting server")
	return http.ListenAndServe(addr, mux)
}
