package version

import (
	_ "embed"
	"strings"

	"github.com/rs/zerolog/log"
)

var (
	// Version is the commit hash or tag
	//go:embed version.txt
	Version string
	// BuildTime is the time of the build
	BuildTime string
)

type info struct {
	Version   string `json:"version"`
	BuildTime string `json:"build_time"`
}

func Information() info {
	return info{
		Version:   Version,
		BuildTime: BuildTime,
	}
}

func checkAndTrim(vars ...*string) {
	for _, s := range vars {
		*s = strings.Trim(*s, " \t\n")
		if *s == "" {
			log.Fatal().Msg("compile time variable not set")
		}
	}
}

func EnsureSet() {
	checkAndTrim(&Version, &BuildTime)
}
