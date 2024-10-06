package store

import (
	"context"
	"fmt"

	"github.com/jackc/pgx/v5"
	"github.com/pkulik0/bufro/api/internal/config"
	"github.com/pkulik0/bufro/api/internal/model"
)

var _ Store = &pgStore{}

type pgStore struct {
	conn *pgx.Conn
}

func NewPgStore(ctx context.Context) (*pgStore, error) {
	url := fmt.Sprintf("postgres://%s:%s@%s:%s/%s",
		config.PostgresUser,
		config.PostgresPassword,
		config.PostgresHost,
		config.PostgresPort,
		config.PostgresDB,
	)

	conn, err := pgx.Connect(ctx, url)
	if err != nil {
		return nil, err
	}

	_, err = conn.Exec(ctx, "CREATE TABLE IF NOT EXISTS buf (id TEXT PRIMARY KEY, type VARCHAR(8), user_id TEXT, data BYTEA);")
	if err != nil {
		return nil, err
	}

	return &pgStore{
		conn: conn,
	}, nil
}

func (s *pgStore) Close(ctx context.Context) error {
	return s.conn.Close(ctx)
}

func (s *pgStore) GetBuf(ctx context.Context, bufId string) (*model.Buf, error) {
	var buf model.Buf
	err := s.conn.QueryRow(ctx, "SELECT id, type, user_id, data FROM buf WHERE id = $1", bufId).Scan(&buf.ID, &buf.Type, &buf.UserID, &buf.Data)
	if err != nil {
		return nil, err
	}
	return &buf, nil
}

func (s *pgStore) CreateBuf(ctx context.Context, buf *model.Buf) error {
	_, err := s.conn.Exec(ctx, "INSERT INTO buf (id, type, user_id, data) VALUES ($1, $2, $3, $4)", buf.ID, buf.Type, buf.UserID, buf.Data)
	return err
}
