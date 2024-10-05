package config

import (
	"os"
	"strings"

	"github.com/rs/zerolog/log"
)

var (
	// PostgresHost is the hostname of the Postgres database
	PostgresHost string
	// PostgresPort is the port of the Postgres database
	PostgresPort string
	// PostgresUser is the username to connect to the Postgres database
	PostgresUser string
	// PostgresPassword is the password to connect to the Postgres database
	PostgresPassword string
	// PostgresDB is the name of the Postgres database
	PostgresDB string
)

const (
	prefix string = "BUFRO_"
)

func getEnv(key, fallback string, secret bool) string {
	key = prefix + strings.ToUpper(key)
	value, ok := os.LookupEnv(key)
	if !ok {
		if fallback == "" {
			log.Fatal().Str("key", key).Msg("environment variable not set")
		}
		value = fallback
	}

	printable := value
	if secret {
		printable = "********"
	}

	log.Debug().Str("key", key).Str("value", printable).Msg("environment variable")
	return value
}

// Parse reads the environment variables and sets the configuration
func Parse() {
	PostgresHost = getEnv("POSTGRES_HOST", "postgres", false)
	PostgresPort = getEnv("POSTGRES_PORT", "5432", false)
	PostgresUser = getEnv("POSTGRES_USER", "bufro", false)
	PostgresPassword = getEnv("POSTGRES_PASSWORD", "postgres", true)
	PostgresDB = getEnv("POSTGRES_DB", "bufro", false)
}
