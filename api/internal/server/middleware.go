package server

import (
	"net/http"
	"strings"
	"time"

	"github.com/rs/zerolog/log"

	"github.com/pkulik0/bufro/api/internal/auth"
)

func logMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		startTime := time.Now()
		rW := &respWriter{ResponseWriter: w}

		defer func() {
			if r.URL.Path == "/favicon.ico" {
				return
			}
			log.Info().Str("path", r.URL.Path).Str("method", r.Method).Str("addr", r.RemoteAddr).Int("status", rW.Status()).Dur("duration", time.Since(startTime)).Msg("request")
		}()
		next.ServeHTTP(rW, r)
	})
}

const (
	uidHeader = "Bufro-UID"
)

func authMiddleware(a auth.Auth, next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		accessToken := r.Header.Get("Authorization")
		if accessToken == "" {
			http.Error(w, "unauthorized", http.StatusUnauthorized)
			return
		}
		accessToken = strings.TrimPrefix(accessToken, "Bearer ")

		uid, err := a.ValidateAccessToken(r.Context(), accessToken)
		if err != nil {
			http.Error(w, "unauthorized", http.StatusUnauthorized)
			return
		}
		r.Header.Set(uidHeader, uid)

		next.ServeHTTP(w, r)
	})
}

func getUID(r *http.Request) string {
	return r.Header.Get(uidHeader)
}
