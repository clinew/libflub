// Uncomment this to enable compact flubs.
//#define FLUB_COMPACT
#include "../flub.h"


#define ERROR_CODE 1
#define BADNESS 0xDEADBEEF


// Static allocation example.
struct flub* hello() {
	static struct flub flub;

	return flub_toss(&flub, "Badness ensued!", BADNESS);
}

struct flub* world() {
	struct flub* flub;

	flub = hello();
	if (flub) {
		return flub;
	}
	return NULL;
}


int main(int argc, char* argv[]) {
	struct flub* flub;

	// Example.
	printf("toss/grab example.\n");
	flub = world();
	if (flub) {
		flub_print(flub);
		flub_grab(flub);
	}

	// Exit the program.
	exit(EXIT_SUCCESS);
}
