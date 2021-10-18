#ifndef _kernel_h_
#define _kernel_h_

#include "list.h"
#include "pcb.h"
class PCB;

// Zabranjuje prekide
#define hardLock() { asm{pushf; cli;}}
// Dozvoljava prekide
#define hardUnlock() {asm{popf;}}

typedef void interrupt (*Inter)(...);

class Kernel {
public:
	static void inic();
	static void restore();
	static void dispatch();
	static void interrupt timer(...);

	static void lock();
	static void unlock();

	static volatile PCB* running;
	static PCB* idle;
	static volatile int brojac;
	static volatile int zahtevana_promena_konteksta;
	static volatile unsigned int lockFlag;
	static Inter oldTimer;

private:
	friend class PCB;
	friend class Scheduler;

	static void runWrapper();

	static List<PCB> allPcbs;
};

#endif
