#include "semaphor.h"
#include "kernelSe.h"
#include "kernel.h"

Semaphore::Semaphore(int init) {
	hardLock();
	myImpl = new KernelSem(init);
	hardUnlock();
}

Semaphore::~Semaphore() {
	hardLock();
	delete myImpl;
	hardUnlock();
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	myImpl->signal();
	return;
}

int Semaphore::val() const {
	return myImpl->val();
}
