package bufro

import (
	"context"
	"errors"

	"github.com/rs/zerolog/log"

	"github.com/pkulik0/bufro/api/internal/model"
	"github.com/pkulik0/bufro/api/internal/store"
)

// Bufro is the interface for the service that manages bufs.
//
//go:generate mockgen -destination=../mock/bufro.go -package=mock . Bufro
type Bufro interface {
	// GetBuf returns the buffer with the given ID and user ID.
	GetBuf(ctx context.Context, bufId string) (*model.Buf, error)

	// GetUserBufs returns the buffers of the user with the given ID.
	GetUserBufs(ctx context.Context, userId string, limit, offset int) ([]model.Buf, error)

	// CreateBuf creates a new buffer with the given ID and user ID.
	CreateBuf(ctx context.Context, buf *model.Buf) error
}

var _ Bufro = &bufro{}

type bufro struct {
	s store.Store
}

// NewBufro creates a new instance of the Bufro service.
func NewBufro(s store.Store) *bufro {
	return &bufro{
		s: s,
	}
}

var (
	ErrInvalidID      = errors.New("invalid ID")
	ErrInvalidBufData = errors.New("invalid buf data")
	ErrInvalidBufType = errors.New("invalid buf type")
)

func (b *bufro) GetBuf(ctx context.Context, bufId string) (*model.Buf, error) {
	if len(bufId) != 8 {
		return nil, ErrInvalidID
	}

	return b.s.GetBuf(ctx, bufId)
}

func (b *bufro) GetUserBufs(ctx context.Context, userId string, limit, offset int) ([]model.Buf, error) {
	if len(userId) != 8 {
		return nil, ErrInvalidID
	}

	if limit < 0 || limit > 100 || offset < 0 {
		return nil, errors.New("invalid limit or offset")
	}
	if limit == 0 {
		limit = 25
	}

	return b.s.GetUserBufs(ctx, userId, limit, offset)
}

func (b *bufro) CreateBuf(ctx context.Context, buf *model.Buf) error {
	if len(buf.ID) != 8 {
		return ErrInvalidID
	}
	if buf.Data == nil {
		return ErrInvalidBufData
	}
	if buf.Type != model.TypeImage && buf.Type != model.TypeVideo {
		return ErrInvalidBufType
	}
	// TODO: check user
	// TODO: validate data (MIME, size)

	err := b.s.CreateBuf(ctx, buf)
	if err != nil {
		return err
	}

	log.Info().Str("id", buf.ID).Str("type", buf.Type.String()).Msg("created buf")
	return nil
}
