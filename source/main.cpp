#include "kernel.h"
#include "list.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	Kernel::inic();

	int ret = userMain(argc, argv);

	Kernel::restore();

	return ret;
}
