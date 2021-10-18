// File: kernelSe.h
#ifndef _kernelSe_h_
#define _kernelSe_h_
typedef unsigned int Time;

#include "PCB.h"
#include "list.h"

class KernelSem {
public:
	KernelSem(int init = 1);
	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait);
	virtual void signal();

	void timer();
	static void updateAll();

	int val() const; // Returns the current value of the semaphore
private:
	int value;

	void block(Time maxTimeToWait);
	void unblock();
	List<PCB> blocked;

	static List<KernelSem> all;
};

#endif
