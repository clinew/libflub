/*
 * Copyright 2011-2012 by Wade Cline.
 *
 * Original Author: Wade Cline (clinew)
 * File: flub.c
 * Created: 2011 October 5th by clinew.
 * Last Modified: 2012 August 11th, by clinew.
 * 
 * See: "flub.h".
 */	


#include "flub.h"


struct flub* flub_append_compact(struct flub* flub) {
	// Return the specified flub.
	return flub;
}


struct flub* flub_append_normal(struct flub* flub, char* function_name) {
	size_t length;

	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_append() failed: flub is " \
			"NULL.\n");
		return NULL;
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_append() failed: function_name " \
			"is NULL.\n");
		return NULL;
	}
	#endif

	// Calculate the new length of the stack trace.
	// +4 for extra whitespace characters and null byte.
	length = strlen(flub->stack_trace) + strlen(function_name) + 4;

	// Reallocate space for the stack trace.
	if (realloc(flub->stack_trace, length) == NULL) {
		fprintf(stderr, "flub_append() failed: unable to reallocate " \
				"space.\n");
		return flub;
	}

	// Add a newline, tab, and list marker to the flub's stack trace.
	strcat(flub->stack_trace, "\n\t-");

	// Add the specified function name to the flub's stack trace.
	strcat(flub->stack_trace, function_name);

	// Return the flub.
	return flub;
}


struct flub* flub_catch_compact(struct flub* flub) {
	// Print the specified flub's message.
	flub_print_compact(flub);

	// Return no error.
	return 0;
}


struct flub* flub_catch_normal(struct flub* flub) {
	// Validate arugments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_catch() failed: struct flub " \
			"is NULL.\n");
		return NULL;
	}
	#endif

	// Print the flub message.
	flub_print(flub);

	// Free the flub and return NULL.
	return flub_free(flub);
}


unsigned long flub_error_code_get_compact(struct flub* flub) {
	// Just call the compact version of flub yoink.
	return flub_yoink_compact(flub);
}


unsigned long flub_error_code_get_normal(struct flub* flub) {
	// Just call the normal version of flub yoink.
	return flub_yoink_normal(flub);
}

struct flub* flub_free_compact(struct flub* flub) {
	// Return success. Nothing needs to be done.
	return NULL;
}


struct flub* flub_free_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_free() failed: struct flub " \
			"is NULL.\n");
		return NULL;
	}
	#endif

	// Free the specified flub's members.
	free(flub->message);
	free(flub->stack_trace);

	// Free the flub itself.
	free(flub);

	// Return success.
	return NULL;
}


char* flub_message_get_compact(struct flub* flub) {
	// Return nothing; there's no string to return.
	return NULL;
}


char* flub_message_get_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_message_get() failed: " \
			"struct flub is NULL.\n");
		return NULL;
	}
	#endif

	// Return the specified flub's message.
	return flub->message;
}


void flub_print_compact(struct flub* flub) {
	// Print the error code.
	fprintf(stderr, "Error Code: %lu.\n", (unsigned long)flub);
}


void flub_print_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_print() failed: struct flub " \
			"is NULL.\n");
		return;
	}
	#endif

	// Print the flub to standard error.
	fprintf(stderr, "\n*** FLUB ***\n" \
		"Error Code: %lu\n" \
		"Message: %s\n" \
		"Stack Trace: \n\t-%s\n" \
		"*****************\n\n", \
		flub->error_code, flub->message, 
		flub->stack_trace);
}


char* flub_stack_trace_get_compact(struct flub* flub) {
	// Return NULL; there's no actual stack trace attached.
	return NULL;
}


char* flub_stack_trace_get_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_stack_trace_get() failed; struct flub " \
			"is NULL.\n");
		return NULL;
	}
	#endif

	// Return the stack trace.
	return flub->stack_trace;
}


struct flub* flub_throw_compact(unsigned long error_code) {
	// A little dirty. Since we specified an unsigned long, cast the long
	// to a struct flub pointer and return the value of the pointer. This
	// will work as long as the successful error code is defined to be 0.
	// Also, don't ever de-reference the structure.
	return (struct flub*)error_code;
}


struct flub* flub_throw_normal(char* message, char* function_name, unsigned long error_code) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (message == NULL) {
		fprintf(stderr, "flub_throw() failed: message " \
			"arugment was NULL.\n");
		return NULL;
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_throw() failed: " \
			"function_name argument was NULL.\n");
		return NULL;
	}
	#endif

	// Check for zero error code.
	#ifdef DEBUG_FLUB
	if (error_code == 0) {
		fprintf(stderr, "WARNING!\nWARNING!\nWARNING!\n" \
			"Using zero value for error_code of flub " \
			"will almost certainly break compact flub; " \
			"consider using a non-zero error_code.\n");
	}
	#endif

	// Allocate space for the new flub.
	struct flub* flub = (struct flub*)malloc(sizeof(struct flub));
	if (flub == NULL) {
		fprintf(stderr, "flub_throw() failed; unable to allocate " \
				"memory for struct flub.\n");
		return NULL;
	}

	// Assign the new flub's message.
	flub->message = (char*)malloc(sizeof(char) * (strlen(message) + 1));
	if (flub->message == NULL) {
		fprintf(stderr, "flub_throw() failed; unable to allocate " \
				"memory for message.\n");
		goto free_flub;
	}
	strcpy(flub->message, message);

	// Assign the new flub's function name.
	flub->stack_trace = (char*)malloc(sizeof(char) * 
					  (strlen(function_name) + 1));
	if (flub->stack_trace == NULL) {
		fprintf(stderr, "flub_throw() failed; unable to allocate " \
				"memory for stack trace.\n");
		goto free_message;
	}
	flub->stack_trace = strcpy(flub->stack_trace, function_name);

	// Assign the new flub's error code.
	flub->error_code = error_code;

	// Return the new flub.
	return flub;

free_message:
	// Free the flub's message.
	free(flub->message);

free_flub:
	// Free the struct flub.
	free(flub);

	// Return NUll.
	return NULL;
}


unsigned long flub_yoink_compact(struct flub* flub) {
	// Cast the flub back into its true form and return it!
	return (unsigned long)flub;
}


unsigned long flub_yoink_normal(struct flub* flub) {
	// Validate parameters.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_yoink failed: " \
			"flub was NULL.\n");
		return NULL;
	}
	#endif

	// Return the error code.
	return flub->error_code;
}
