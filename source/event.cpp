#include "kernel.h"
#include "event.h"
#include "SCHEDULE.h"
#include "kernelEv.h"

Event::Event(IVTNo ivtNo) {
	hardLock();
	myImpl = new KernelEv(ivtNo);
	hardUnlock();
}

Event::~Event() {
	hardLock();//Kernel::lock();
	delete myImpl;
	hardUnlock();//Kernel::unlock();
}

void Event::wait() {
	myImpl->wait();
	return;
}

void Event::signal() {
	myImpl->signal();
	return;
}
