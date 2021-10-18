// File: thread.h
#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
//defaultStack size najvice utice na rad programa sa 2^15 maks tredova je 15
const StackSize defaultStackSize = 32768;//4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
class PCB;

class Thread {
public:
	void start();
	void waitToComplete();

	virtual ~Thread();

	ID getID();

	static Thread* getThreadById(ID id);
	static void dispatch();
protected:
	PCB* myPCB;

	friend class PCB;
	friend class Kernel;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice);
	virtual void run() {
	}
private:

};
void dispatch();
#endif
