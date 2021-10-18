#include "pcb.h"
#include "kernel.h"
#include "thread.h"
#include "DOS.h"
#include "SCHEDULE.h"

unsigned int PCB::nextID = 0;

PCB::PCB(Thread* thread, StackSize stackSize, Time kvant) {
	//hardLock();
	this->myThread = thread;
	this->waitingToComplete = List<PCB>();
	this->kvant = kvant;
	this->timeToWait = 0; //?
	this->stackSize = stackSize / sizeof(unsigned);
	this->pid = nextID++;
	this->state = PCB::NEW;
	Kernel::allPcbs.push(this);
	//hardUnlock();
}

PCB::~PCB() {
	//hardLock();
	state = PCB::TERMINATED;
	Kernel::allPcbs.remove(this);
	delete stack;
	//hardUnlock();
}

ID PCB::getID() {
	return pid;
}

PCB* PCB::getPCBById(ID id) {
	List<PCB>::Elem* curr = Kernel::allPcbs.head;
	PCB* found = 0;
	for (curr; curr != 0; curr = curr->prev) {
		PCB* pcb = curr->data;
		if (pcb->getID() == id) {
			found = pcb;
		}
	}
	return found;
}

void PCB::start() {
	hardLock();//Kernel::lock();
	allocateStack();
	this->state = RUNNABLE;
	Scheduler::put((PCB*) this);
	hardUnlock();//Kernel::unlock();
}

void PCB::waitToComplete() {
	// && this != Kernel::kernelMain
	if(this != Kernel::running && this != Kernel::idle && this->state != PCB::NEW && this->state != PCB::TERMINATED){
		if(Kernel::running->blockedLevel == 0) {
			Kernel::running->state = PCB::BLOCKED;
		}
		Kernel::running->blockedLevel--;
		waitingToComplete.push((PCB*) Kernel::running);
		Kernel::dispatch();
	}
}

void PCB::allocateStack() {
	stack = new unsigned[stackSize];
	stack[stackSize-1] = 0x200; //setovan I fleg u pocetnom PSW-u za nit
	stack[stackSize-2] = FP_SEG(&Kernel::runWrapper);
	stack[stackSize-3] = FP_OFF(&Kernel::runWrapper);
	sp = FP_OFF(stack + stackSize - 12); //svi sacuvani registri pri ulasku u interrupt rutinu
	ss = FP_SEG(stack + stackSize - 12);
	bp = sp;
}
