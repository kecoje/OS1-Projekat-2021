// File: idleThre.h
#ifndef _idleThread_h_
#define _idleThread_h_

#include "thread.h"
#include "pcb.h"
#include "kernel.h"

class IdleThread: public Thread {
public:
	IdleThread() :
			Thread(64, 1) {
	}

	void start();
protected:
	void run() {
		while(1);
	}
};

void IdleThread::start() {
	hardLock();//Kernel::lock();
	myPCB->allocateStack();
	myPCB->state = PCB::RUNNABLE;
	hardUnlock();//Kernel::unlock();
}

#endif // _idleThread_h_
