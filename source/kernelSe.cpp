#include "Kernel.h"
#include "KernelSe.h"
#include "SCHEDULE.h"

List<KernelSem> KernelSem::all = List<KernelSem>();

KernelSem::KernelSem(int init) : value(init) {
	hardLock();
	all.push(this); // ili pushBack
	hardUnlock();
}

KernelSem::~KernelSem() {
	hardLock();
	while(blocked.isNotEmpty()) {
		unblock();
	}
	all.remove(this);
	hardUnlock();
}

int KernelSem::wait(Time maxTimeToWait) {
	hardLock();
	int deblockedOnSignal = 1;
	value--;
	if(value < 0) {
		block(maxTimeToWait);
		if(Kernel::running->timeToWait == 0 && maxTimeToWait != 0) {
			//Nit nije deblokirana na signal
			deblockedOnSignal = 0;
			value++;
		}
	}
	if(deblockedOnSignal) Kernel::running->timeToWait = 0;
	hardUnlock();
	return deblockedOnSignal; //vrati koliko je cekao?
}

void KernelSem::signal() {
	hardLock();

	value++;
	if(value<=0)
		unblock();

	hardUnlock();
	return;
}

void KernelSem::timer() {
	List<PCB>::Elem* jumpTo = 0;
	List<PCB>::Elem* curr = blocked.tail;
	while (curr && (PCB*)curr->data) {
		PCB* pcb = curr->data;
		//syncPrintf("[p:%d]", pcb->pid);
		if(pcb->timeToWait > 0) {
			pcb->timeToWait--;
			if(pcb->timeToWait == 0){
				jumpTo = curr->prev; //ili next
				blocked.remove(pcb);
				pcb->state = PCB::RUNNABLE;
				Scheduler::put(pcb);
				//syncPrintf(" u:%d)", pcb->pid);
				if(jumpTo) curr = jumpTo;
				else curr = blocked.tail;
			} else {
				curr = curr->next;
			}
		} else {
			curr = curr->next;
		}
	}
}

void KernelSem::updateAll() {
	hardLock();
	List<KernelSem>::Elem* curr = KernelSem::all.head;
	for (curr; curr != 0; curr = curr->prev) {
		KernelSem* ksem = curr->data;
		ksem->timer();
	}
	hardUnlock();
}

void KernelSem::block(Time maxTimeToWait) {
	Kernel::running->state = PCB::BLOCKED;
	Kernel::running->timeToWait = maxTimeToWait;
	blocked.push((PCB*)Kernel::running);
	Kernel::dispatch();
}

void KernelSem::unblock() {
	if(blocked.isEmpty())
		return;
	PCB* unblocked = blocked.popBack();
	unblocked->state = PCB::RUNNABLE;
	unblocked->timeToWait = 0;
	Scheduler::put(unblocked);
}


int KernelSem::val() const {
	return value;
}
