#include "kernel.h"
#include "thread.h"
#include "dos.H"
#include "stdio.H"
#include "idleThre.h"
#include "SCHEDULE.H"
#include "kernelSe.h"

volatile PCB* Kernel::running = 0;
PCB* Kernel::idle = 0;
volatile int Kernel::brojac = 10; //kvant orginalne niti - overridujem u inic
volatile int Kernel::zahtevana_promena_konteksta = 0;
volatile unsigned int Kernel::lockFlag = 1;
Inter Kernel::oldTimer = 0; // stara prekidna rutina
List<PCB> Kernel::allPcbs = List<PCB>();
extern void tick();

void Kernel::dispatch() { // sinhrona promena konteksta
	hardLock();
	zahtevana_promena_konteksta = 1;
	timer();
	hardUnlock();
}

void Kernel::lock() {
	lockFlag--;
}
void Kernel::unlock() {
	lockFlag++;
	if (zahtevana_promena_konteksta) {
		dispatch();
	}
}

void interrupt Kernel::timer(...) {
	static volatile unsigned tsp, tss, tbp;
	if (!zahtevana_promena_konteksta) {
		brojac--;
		tick();
		KernelSem::updateAll();
		asm int 60h;
	}
	if (brojac == 0 || zahtevana_promena_konteksta) {
		if(lockFlag == 1) {
			zahtevana_promena_konteksta = 0;
			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			if(running != idle && running->state == PCB::RUNNABLE)
				Scheduler::put((PCB*) running);
			running = Scheduler::get();
			if(!running) {
				running = idle;
			}

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			brojac = running->kvant;

			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		} else zahtevana_promena_konteksta = 1;

	}

}

void Kernel::inic() {
	hardLock();
	oldTimer = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldTimer);

	//stackSize moze da bude 0 zato sto se nikada ne poziva start na ovu nit
	Thread* main = new Thread(0, 1);
	//Kada dam main niti da se izvrsava beskonocano Scheduler nece da je promeni
	//Scheduler moze da vradi korumpirani PCB ako je prazan i to pravi NVM error
	running = main->myPCB;
	running->state = PCB::RUNNABLE;
	brojac = running->kvant;

	Thread* idleT = new IdleThread();
	Kernel::idle = idleT->myPCB;
	idle->start();
	hardUnlock();
}

void Kernel::restore() {
	hardLock();
	setvect(0x08, oldTimer);
	
//	delete kMain;
//	delete myMain;
//	delete idle;
	hardUnlock();
}

void Kernel::runWrapper() {
	running->myThread->run();
	hardLock();

	running->state = PCB::TERMINATED;

	while(((PCB*)running)->waitingToComplete.isNotEmpty()) {
		PCB* pcb = running->waitingToComplete.pop();
		pcb->state = PCB::RUNNABLE;
		Scheduler::put(pcb);
	}
	hardUnlock();
	dispatch();
}
