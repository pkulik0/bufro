package model

import "github.com/pkulik0/bufro/api/internal/pb"

// PbToInternalType converts protobuf type to internal type.
func PbToInternalType(t pb.BufType) BufType {
	switch t {
	case pb.BufType_BUF_TYPE_IMAGE:
		return TypeImage
	case pb.BufType_BUF_TYPE_VIDEO:
		return TypeVideo
	default:
		return TypeUnknown
	}
}

// InternalToPbType converts internal type to protobuf type.
func InternalToPbType(t BufType) pb.BufType {
	switch t {
	case TypeImage:
		return pb.BufType_BUF_TYPE_IMAGE
	case TypeVideo:
		return pb.BufType_BUF_TYPE_VIDEO
	default:
		return pb.BufType_BUF_TYPE_UNKNOWN
	}
}
