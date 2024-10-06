package auth

import (
	"context"

	"github.com/Nerzal/gocloak/v13"

	"github.com/pkulik0/bufro/api/internal/config"
)

var _ Auth = &keycloakAuth{}

type keycloakAuth struct {
	client *gocloak.GoCloak
}

func NewKeycloakAuth(ctx context.Context) (*keycloakAuth, error) {
	return &keycloakAuth{
		client: gocloak.NewClient(config.KeycloakURL),
	}, nil
}

func (a *keycloakAuth) ValidateAccessToken(ctx context.Context, accessToken string) (string, error) {
	claims, _, err := a.client.DecodeAccessToken(ctx, accessToken, config.KeycloakRealm)
	if err != nil {
		return "", err
	}
	if !claims.Valid {
		return "", ErrInvalidToken
	}

	subject, err := claims.Claims.GetSubject()
	if err != nil {
		return "", err
	}

	return subject, nil
}
