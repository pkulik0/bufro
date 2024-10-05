package model

import "github.com/google/uuid"

type BufType string

func (t BufType) String() string {
	return string(t)
}

const (
	TypeUnknown BufType = "unknown"
	TypeImage   BufType = "image"
	TypeVideo   BufType = "video"
)

// Buf represents an image or video by a user.
type Buf struct {
	ID     string
	Type   BufType
	Data   []byte
	UserID uint64
}

func NewBuf(userID uint64, type_ BufType, data []byte) *Buf {
	id := uuid.New().String()[:8]
	return &Buf{
		ID:     id,
		Type:   type_,
		Data:   data,
		UserID: userID,
	}
}
