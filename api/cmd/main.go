package main

import (
	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"

	"github.com/pkulik0/bufro/api/internal/server"
)

func init() {
	zerolog.TimeFieldFormat = zerolog.TimeFormatUnix
	zerolog.SetGlobalLevel(zerolog.DebugLevel)
}

func main() {
	s := server.NewServer()
	if err := s.Start(8080); err != nil {
		log.Fatal().Err(err).Msg("server failed")
	}
}
