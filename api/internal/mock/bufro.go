// Code generated by MockGen. DO NOT EDIT.
// Source: github.com/pkulik0/bufro/api/internal/bufro (interfaces: Bufro)
//
// Generated by this command:
//
//	mockgen -destination=../mock/bufro.go -package=mock . Bufro
//

// Package mock is a generated GoMock package.
package mock

import (
	context "context"
	reflect "reflect"

	model "github.com/pkulik0/bufro/api/internal/model"
	gomock "go.uber.org/mock/gomock"
)

// MockBufro is a mock of Bufro interface.
type MockBufro struct {
	ctrl     *gomock.Controller
	recorder *MockBufroMockRecorder
}

// MockBufroMockRecorder is the mock recorder for MockBufro.
type MockBufroMockRecorder struct {
	mock *MockBufro
}

// NewMockBufro creates a new mock instance.
func NewMockBufro(ctrl *gomock.Controller) *MockBufro {
	mock := &MockBufro{ctrl: ctrl}
	mock.recorder = &MockBufroMockRecorder{mock}
	return mock
}

// EXPECT returns an object that allows the caller to indicate expected use.
func (m *MockBufro) EXPECT() *MockBufroMockRecorder {
	return m.recorder
}

// CreateBuf mocks base method.
func (m *MockBufro) CreateBuf(arg0 context.Context, arg1 *model.Buf) error {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "CreateBuf", arg0, arg1)
	ret0, _ := ret[0].(error)
	return ret0
}

// CreateBuf indicates an expected call of CreateBuf.
func (mr *MockBufroMockRecorder) CreateBuf(arg0, arg1 any) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "CreateBuf", reflect.TypeOf((*MockBufro)(nil).CreateBuf), arg0, arg1)
}

// GetBuf mocks base method.
func (m *MockBufro) GetBuf(arg0 context.Context, arg1 string) (*model.Buf, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "GetBuf", arg0, arg1)
	ret0, _ := ret[0].(*model.Buf)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// GetBuf indicates an expected call of GetBuf.
func (mr *MockBufroMockRecorder) GetBuf(arg0, arg1 any) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetBuf", reflect.TypeOf((*MockBufro)(nil).GetBuf), arg0, arg1)
}

// GetUserBufs mocks base method.
func (m *MockBufro) GetUserBufs(arg0 context.Context, arg1 string, arg2, arg3 int) ([]model.Buf, error) {
	m.ctrl.T.Helper()
	ret := m.ctrl.Call(m, "GetUserBufs", arg0, arg1, arg2, arg3)
	ret0, _ := ret[0].([]model.Buf)
	ret1, _ := ret[1].(error)
	return ret0, ret1
}

// GetUserBufs indicates an expected call of GetUserBufs.
func (mr *MockBufroMockRecorder) GetUserBufs(arg0, arg1, arg2, arg3 any) *gomock.Call {
	mr.mock.ctrl.T.Helper()
	return mr.mock.ctrl.RecordCallWithMethodType(mr.mock, "GetUserBufs", reflect.TypeOf((*MockBufro)(nil).GetUserBufs), arg0, arg1, arg2, arg3)
}
