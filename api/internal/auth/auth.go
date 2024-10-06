package auth

import (
	"context"
	"errors"
)

var (
	ErrInvalidToken = errors.New("invalid token")
)

// Auth is an interface for authenticating users.
//
//go:generate mockgen -destination=../mock/auth.go -package=mock . Auth
type Auth interface {
	// ValidateAccessToken validates the access token.
	ValidateAccessToken(ctx context.Context, accessToken string) (string, error)
}
