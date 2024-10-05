package server

import (
	"errors"
	"net/http"
	"net/http/httptest"
	"testing"

	qt "github.com/frankban/quicktest"
	"github.com/pkulik0/bufro/api/internal/mock"
	"github.com/pkulik0/bufro/api/internal/model"
	"go.uber.org/mock/gomock"
)

func TestHandler(t *testing.T) {
	testCases := []struct {
		name  string
		setup func(*mock.MockBufro)
		call  func(*qt.C, *server)
	}{
		{
			name: "get ok",
			setup: func(b *mock.MockBufro) {
				buf := &model.Buf{
					ID:     "abc",
					Type:   model.TypeImage,
					Data:   []byte{1, 2, 3},
					UserID: 123,
				}
				b.EXPECT().GetBuf(gomock.Any(), "abc").Return(buf, nil)
			},
			call: func(c *qt.C, s *server) {
				w := httptest.NewRecorder()
				r := httptest.NewRequest("GET", "/b/abc", nil)
				r.SetPathValue("id", "abc")

				s.handlerGet(w, r)
				c.Assert(w.Code, qt.Equals, http.StatusOK)
				c.Assert(w.Header().Get("Content-Type"), qt.Equals, "image/png")
			},
		},
		{
			name: "get error",
			setup: func(b *mock.MockBufro) {
				b.EXPECT().GetBuf(gomock.Any(), "abc").Return(nil, errors.New("error"))
			},
			call: func(c *qt.C, s *server) {
				w := httptest.NewRecorder()
				r := httptest.NewRequest("GET", "/b/abc", nil)
				r.SetPathValue("id", "abc")

				s.handlerGet(w, r)
				c.Assert(w.Code, qt.Equals, http.StatusInternalServerError)

				body := w.Body.String()
				c.Assert(body, qt.Contains, "internal server error")
			},
		},
	}

	c := qt.New(t)
	for _, tc := range testCases {
		c.Run(tc.name, func(c *qt.C) {
			ctrl := gomock.NewController(c)

			b := mock.NewMockBufro(ctrl)
			s := NewServer(b)

			tc.setup(b)
			tc.call(c, s)
		})
	}
}
