package main

/*
#cgo CFLAGS: -I../src
#cgo LDFLAGS: -L../build/lib -lin3
#include "init/in3_init.h"
#include "core/client/client.h"
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func main() {
	in3Client := C.in3_for_chain_auto_init(0x5)

	rpc := C.CString("{\"method\":\"in3_createKey\", \"params:\":[]}")

	result := C.in3_client_exec_req(in3Client, rpc)
	C.free(unsafe.Pointer(rpc))
	fmt.Println(result)
}
