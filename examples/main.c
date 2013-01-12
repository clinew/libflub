// Uncomment this to enable compact flubs.
//#define FLUB_COMPACT
#include "../flub.h"


#define ERROR_CODE 1
#define BADNESS 0xDEADBEEF


// Static allocation example.
struct flub* hello() {
	char* function_name = "hello()";
	static struct flub flub;

	return flub_toss(&flub, "Badness ensued!", function_name, BADNESS);
}

struct flub* world() {
	struct flub* flub;
	char* function_name = "world()";

	flub = hello();
	if (flub) {
		return flub_append(flub, function_name);
	}

	return NULL;
}


// Dynamic allocation example.
struct flub* foo() {
	char* function_name = "foo()";
	struct flub* flub;

	return flub_throw("An error occurred!", function_name, ERROR_CODE);
}

struct flub* bar() {
	char* function_name = "bar()";
	struct flub* flub;

	flub = foo();
	if (flub) {
		return flub_append(flub, function_name);
	}

	return NULL;
}


int main(int argc, char* argv[]) {
	struct flub* flub;

	// Dynamic allocation example.
	printf("throw/catch example.\n");
	flub = bar();
	if (flub) {
		//fprintf(stdout, "Error: %lu.\n", flub_yoink(flub));
		
		flub_catch(flub);
	}

	// Static allocation example.
	printf("toss/grab example.\n");
	flub = world();
	if (flub) {
		flub_grab(flub);
	}

	// Exit the program.
	exit(EXIT_SUCCESS);
}
