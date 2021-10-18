#ifndef _pcb_h_
#define _pcb_h_

#include "list.h"
#include "thread.h"
#include "kernel.h"

class PCB {
public:
	typedef enum State {
				NEW, RUNNABLE, BLOCKED, WAITING, TERMINATED
			};

	unsigned sp;
	unsigned ss;
	unsigned bp;
	Time kvant;
	Time timeToWait;
	StackSize stackSize;
	unsigned* stack;
	State state;

	Thread* myThread;
	List<PCB> waitingToComplete;

	PCB(Thread* thread, StackSize stackSize, Time kvant);
	virtual ~PCB();

	void start();
	void waitToComplete();
	void allocateStack();
	ID getID();
	static PCB* getPCBById(ID id);

	friend class Kernel;
	friend class Thread;
	friend class Scheduler;

	unsigned int pid;
	static unsigned int nextID;
};

#endif
