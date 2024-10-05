package server

import (
	"net/http"
	"time"

	"github.com/rs/zerolog/log"
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
