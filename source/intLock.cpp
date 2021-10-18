#include "intLock.h"
#include "kernel.h"

int mutex_glb = 1;

void mutex_glb_wait(){
 sloop:asm{
	mov ax, 0
	xchg ax, mutex_glb 	
}
 if(_AX ==0){
	Kernel::dispatch();
	asm jmp sloop;
 }
}
