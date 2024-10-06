package main

import (
	"context"

	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"

	"github.com/pkulik0/bufro/api/internal/auth"
	"github.com/pkulik0/bufro/api/internal/bufro"
	"github.com/pkulik0/bufro/api/internal/config"
	"github.com/pkulik0/bufro/api/internal/server"
	"github.com/pkulik0/bufro/api/internal/store"
	"github.com/pkulik0/bufro/api/internal/version"
)

func init() {
	zerolog.TimeFieldFormat = zerolog.TimeFormatUnix
	zerolog.SetGlobalLevel(zerolog.DebugLevel)
}

func main() {
	config.Parse()
	version.EnsureSet()

	st, err := store.NewPgStore(context.Background())
	if err != nil {
		log.Fatal().Err(err).Msg("store init failed")
	}

	a, err := auth.NewKeycloakAuth(context.Background())
	if err != nil {
		log.Fatal().Err(err).Msg("auth init failed")
	}

	b := bufro.NewBufro(st)

	s := server.NewServer(b, a)
	if err := s.Start(8080); err != nil {
		log.Fatal().Err(err).Msg("server failed")
	}
}
