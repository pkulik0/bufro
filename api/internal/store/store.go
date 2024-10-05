package store

import (
	"context"

	"github.com/pkulik0/bufro/api/internal/model"
)

// Store is an interface for storing and retrieving data.
//
//go:generate mockgen -destination=../mock/store.go -package=mock . Store
type Store interface {
	// GetBuf returns the buf with the given ID.
	GetBuf(ctx context.Context, bufId string) (*model.Buf, error)
	// CreateBuf creates a new buf and returns its ID.
	CreateBuf(ctx context.Context, buf *model.Buf) error
}
