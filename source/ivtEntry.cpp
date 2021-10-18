#include"ivtEntry.h"
#include"kernelEv.h"
#include<DOS.H>

IVTEntry* IVTEntry::all[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine) {
	this->ivtNo = ivtNo;
	this->kernelEv = 0;
	this->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	all[ivtNo] = this;
}
IVTEntry::~IVTEntry() {
	this->kernelEv = 0;
	setvect(ivtNo, oldRoutine);
}

void IVTEntry::signal() {
	if (kernelEv) {
		kernelEv->signal();
	}
}

void IVTEntry::addEvent(KernelEv* ev){
	this->kernelEv = ev;
}

IVTEntry* IVTEntry::getIVTEntryById(IVTNo ivtNo) {
	return all[ivtNo];
}

void IVTEntry::invokeOldRoutine() {
	if (oldRoutine)
		oldRoutine();
}
