package version

import (
	"github.com/rs/zerolog/log"
)

var (
	// Version is the commit hash or tag
	Version string = ""
	// BuildTime is the time of the build
	BuildTime string = ""
)

type info struct {
	Message   string `json:"message"`
	Version   string `json:"version"`
	BuildTime string `json:"build_time"`
}

func Information() info {
	return info{
		Version:   Version,
		BuildTime: BuildTime,
	}
}

func mustBeSet(vars ...string) {
	for _, s := range vars {
		if s == "" {
			log.Fatal().Msg("compile time variable not set")
		}
	}
}

func init() {
	mustBeSet(Version, BuildTime)
}
