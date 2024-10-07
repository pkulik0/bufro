package model

import (
	petname "github.com/dustinkirkland/golang-petname"
	"github.com/pkulik0/bufro/api/internal/pb"
)

type BufType string

func (t BufType) String() string {
	return string(t)
}

func (t BufType) MIME() string {
	switch t {
	case TypeImage:
		return "image/webp"
	case TypeVideo:
		return "video/mp4"
	default:
		return "unknown"
	}
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
	UserID string
}

// NewBuf creates a new Buf instance.
func NewBuf(userID string, type_ BufType, data []byte) *Buf {
	id := petname.Generate(2, "-")
	return &Buf{
		ID:     id,
		Type:   type_,
		Data:   data,
		UserID: userID,
	}
}

func (b *Buf) ToProto() *pb.Buf {
	return &pb.Buf{
		Id:     b.ID,
		Type:   InternalToPbType(b.Type),
		Data:   b.Data,
		UserId: b.UserID,
	}
}
