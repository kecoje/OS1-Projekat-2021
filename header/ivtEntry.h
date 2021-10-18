// File: ivtEntry.h
#ifndef _ivtEntry_h_
#define _ivtEntry_h_

#include "event.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal(); \
	if (callOld) \
		newEntry##numEntry.invokeOldRoutine(); \
}

typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	virtual ~IVTEntry();

	void signal();
	void addEvent(KernelEv *ev);

	static IVTEntry* getIVTEntryById(IVTNo ivtNo);

	void invokeOldRoutine();
private:
	IVTNo ivtNo;
	KernelEv* kernelEv;
	pInterrupt oldRoutine;

	static IVTEntry* all[256];
};

#endif
