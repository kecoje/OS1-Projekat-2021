// File: kernelEv.h
#ifndef _kernelEv_h_
#define _kernelEv_h_
typedef unsigned int Time;

#include "pcb.h"
#include "list.h"
#include "event.h"

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	virtual ~KernelEv();

	virtual void wait();
	virtual void signal();
private:
	friend class IVTEntry;

	int blocked;
	IVTNo ivtNo;
	PCB* owner;

	void block();
	void unblock();

	static List<KernelEv> all;
};

#endif
