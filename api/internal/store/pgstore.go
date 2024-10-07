package store

import (
	"context"
	"database/sql"
	"embed"
	"fmt"

	"github.com/golang-migrate/migrate/v4"
	"github.com/golang-migrate/migrate/v4/database/postgres"
	"github.com/golang-migrate/migrate/v4/source/iofs"
	_ "github.com/jackc/pgx/v5/stdlib"

	"github.com/pkulik0/bufro/api/internal/config"
	"github.com/pkulik0/bufro/api/internal/model"
)

var _ Store = &pgStore{}

type pgStore struct {
	db *sql.DB
}

//go:embed migrations/*.sql
var migrationsFS embed.FS

func NewPgStore(ctx context.Context) (*pgStore, error) {
	url := fmt.Sprintf("postgres://%s:%s@%s:%s/%s",
		config.PostgresUser,
		config.PostgresPassword,
		config.PostgresHost,
		config.PostgresPort,
		config.PostgresDB,
	)

	db, err := sql.Open("pgx", url)
	if err != nil {
		return nil, err
	}

	f, err := iofs.New(migrationsFS, "migrations")
	if err != nil {
		return nil, err
	}

	psql, err := postgres.WithInstance(db, &postgres.Config{})
	if err != nil {
		return nil, err
	}

	m, err := migrate.NewWithInstance("iofs", f, "postgres", psql)
	if err != nil {
		return nil, err
	}
	err = m.Up()
	if err != nil && err != migrate.ErrNoChange {
		return nil, err
	}

	return &pgStore{
		db: db,
	}, nil
}

func (s *pgStore) Close() error {
	return s.db.Close()
}

func (s *pgStore) GetBuf(ctx context.Context, bufId string) (*model.Buf, error) {
	var buf model.Buf
	err := s.db.QueryRowContext(ctx, "SELECT id, type, user_id, data FROM buf WHERE id = $1", bufId).Scan(&buf.ID, &buf.Type, &buf.UserID, &buf.Data)
	if err != nil {
		return nil, err
	}
	return &buf, nil
}

func (s *pgStore) GetUserBufs(ctx context.Context, userId string, limit, offset int) ([]model.Buf, error) {
	rows, err := s.db.QueryContext(ctx, "SELECT id, type, user_id, data FROM buf WHERE user_id = $1 LIMIT $2 OFFSET $3", userId, limit, offset)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	var bufs []model.Buf
	for rows.Next() {
		var buf model.Buf
		err = rows.Scan(&buf.ID, &buf.Type, &buf.UserID, &buf.Data)
		if err != nil {
			return nil, err
		}
		bufs = append(bufs, buf)
	}

	return bufs, nil
}

func (s *pgStore) CreateBuf(ctx context.Context, buf *model.Buf) error {
	_, err := s.db.ExecContext(ctx, "INSERT INTO buf (id, type, user_id, data) VALUES ($1, $2, $3, $4)", buf.ID, buf.Type, buf.UserID, buf.Data)
	return err
}
