//#define FLUB_COMPACT
#include "../flub.h"


#define ERROR_CODE 1

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

	flub = bar();
	if (flub) {
		//fprintf(stdout, "Error: %lu.\n", flub_yoink(flub));
		
		flub = flub_catch(flub);
	}

	exit(EXIT_SUCCESS);
}
