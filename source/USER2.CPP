//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    user.cpp
//   Date:    Maj 2021
//===========================================================================//
#include <iostream.h>
#include <stdlib.h>
#include <assert.h>
#include "kernel.h"
#include "keyevent.h"
#include "bounded.h"
#include "user.h"
#include "intLock.h"
#include <event.h>
#include "semaphor.h"
#include "SCHEDULE.H"

class A: public Thread {
public:
	A() :
			Thread(defaultStackSize, 40) {
	}
protected:
	void run();
};

void A::run() {
	int i, k, j;
	for (i = 0; i < 30; ++i) {
		Kernel::lock();
		cout << "u a() i = " << i << endl;
		Kernel::unlock();
		for (k = 0; k < 10000; ++k)
			for (j = 0; j < 30000; ++j)
				;
	}
}

class B: public Thread {
public:
	B() :
			Thread(defaultStackSize, 20) {
	}
protected:
	void run();
};
void B::run() {
	int i, k, j;
	for (i = 0; i < 30; ++i) {
		Kernel::lock();
		cout << "u b() i = " << i << endl;
		Kernel::unlock();
		for (k = 0; k < 10000; ++k)
			for (j = 0; j < 30000; ++j)
				;
	}
}

int userMain2(int argc, char * argv[]) {

	Kernel::lock();

	A a = A();
	a.start();
	cout << "napravio a" << endl;

	B b = B();
	b.start();
	cout << "napravio b" << endl;

	Kernel::unlock();

	for (int i = 0; i < 30; ++i) {
		Kernel::lock();
		cout<<"main "<<i<<endl;
		Kernel::unlock();

		for (int j = 0; j < 30000;
				++j
				)
				for (int k = 0; k < 30000;
				++k
			);
		}

	a.waitToComplete();
	b.waitToComplete();

	cout << "Happy End" << endl;

	return 0;
}
