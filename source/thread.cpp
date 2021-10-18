#include "thread.h"
#include "pcb.h"
#include "kernel.h"

Thread::Thread(StackSize stackSize, Time kvant) {
	hardLock();//Kernel::lock();
	myPCB = new PCB(this, stackSize, kvant);
	hardUnlock();//Kernel::unlock();
}

void Thread::start() {
	myPCB->start();
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getID() {
	return myPCB->getID();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getPCBById(id)->myThread;
}

void Thread::dispatch() {
	Kernel::dispatch();
}

Thread::~Thread() {
	waitToComplete();
	hardLock();
	delete myPCB;
	hardUnlock();
}
