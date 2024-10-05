package server

import "net/http"

type respWriter struct {
	http.ResponseWriter
	status int
}

func (w *respWriter) WriteHeader(status int) {
	w.status = status
	w.ResponseWriter.WriteHeader(status)
}

func (w *respWriter) Status() int {
	if w.status == 0 {
		return http.StatusOK
	}
	return w.status
}
