#include "Kernel.h"
#include "KernelEv.h"
#include "SCHEDULE.h"
#include "ivtEntry.h"

List<KernelEv> KernelEv::all = List<KernelEv>();

KernelEv::KernelEv(IVTNo ivtNo) {
	hardLock();
	blocked = 0;
	this->ivtNo = ivtNo;
	owner = (PCB*)Kernel::running;

	IVTEntry::getIVTEntryById(ivtNo)->addEvent(this);

	all.push(this);
	hardUnlock();
}

KernelEv::~KernelEv() {
	hardLock();
	if(blocked) {
		owner->state = PCB::RUNNABLE;
		Scheduler::put(owner);
	}

	delete IVTEntry::getIVTEntryById(ivtNo);

	all.remove(this);
	hardUnlock();
}

void KernelEv::wait() {
	hardLock();
	if(Kernel::running == owner) {
		if (!blocked) {
			block();
			Kernel::dispatch();
		} else {
			blocked = 1;
		}
	}
	hardUnlock();
}

void KernelEv::signal() {
	hardLock();
	if(blocked) {
		unblock();
		Kernel::dispatch();
	}
	hardUnlock();
}

void KernelEv::block() {
	blocked = 1;
	owner->state = PCB::BLOCKED;
}

void KernelEv::unblock() {
	blocked = 0;
	owner->state = PCB::RUNNABLE;
	Scheduler::put(owner);
}
