/*
 * Copyright 2011-2013 by Wade Cline.
 *
 * Original Author: Wade Cline (clinew)
 * File: flub.c
 * Created: 2011 October 5th by clinew.
 * Last Modified: 2013 January 10th, by clinew.
 * 
 * See: "flub.h".
 */	


#include "flub.h"


inline struct flub* flub_append_compact(struct flub* flub) {
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


void flub_catch_compact(struct flub* flub) {
	// Print the specified flub's message.
	flub_print_compact(flub);

	// Return.
	return;
}


void flub_catch_normal(struct flub* flub) {
	// Validate arugments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_catch() failed: struct flub " \
			"is NULL.\n");
		return;
	}
	#endif

	// Print the flub message.
	flub_print(flub);

	// Free the flub's resources.
	flub_free(flub);

	// Free the flub itself.
	free(flub);

	// Return.
	return;
}


inline unsigned long flub_error_code_get_compact(struct flub* flub) {
	// Just call the compact version of flub yoink.
	return flub_yoink_compact(flub);
}


inline unsigned long flub_error_code_get_normal(struct flub* flub) {
	// Just call the normal version of flub yoink.
	return flub_yoink_normal(flub);
}


inline void flub_free_compact(struct flub* flub) {
	; // Do nothing.
}


void flub_free_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_free() failed: struct flub " \
			"is NULL.\n");
		return;
	}
	#endif

	// Free the specified flub's members.
	free(flub->message);
	free(flub->stack_trace);
}


void flub_grab_compact(struct flub* flub) {
	// Print the flub.
	flub_print_compact(flub);

	// Return.
	return;
}


void flub_grab_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_grab() failed: struct flub " \
			"is NULL.\n");
		return;
	}
	#endif

	// Print the flub.
	flub_print_normal(flub);

	// Free the flub.
	flub_free_normal(flub);

	// Return.
	return;
}


/*!	\brief Helper function that allocates space for and assigns flub
 * 	members.
 *
 * 	\return	0 on success; -1 on failure.
 */
static int flub_init_normal(struct flub* flub, char* message,
			    char* function_name, unsigned long error_code) {
	// Validate parameters.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_init_normal() failed; specified " \
			"flub was NULL.\n");
		return NULL;
	}
	if (message == NULL) {
		fprintf(stderr, "flub_init_normal() failed; specified " \
			"message was NULL.\n");
		return NULL;
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_init_normal() failed; specified " \
			"function_name was NULL.\n");
		return NULL;
	}
	flub_validate_error_code(error_code);
	#endif

	// Assign the new flub's message.
	flub->message = (char*)malloc(sizeof(char) * (strlen(message) + 1));
	if (flub->message == NULL) {
		fprintf(stderr, "flub_init_normal() failed; unable to " \
				"allocate memory for message.\n");
		return -1;
	}
	strcpy(flub->message, message);

	// Assign the new flub's function name.
	flub->stack_trace = (char*)malloc(sizeof(char) * 
					  (strlen(function_name) + 1));
	if (flub->stack_trace == NULL) {
		fprintf(stderr, "flub_init_normal() failed; unable to " \
				"allocate memory for stack trace.\n");
		goto free_message;
	}
	flub->stack_trace = strcpy(flub->stack_trace, function_name);

	// Assign the new flub's error code.
	flub->error_code = error_code;

	// Return success.
	return 0;

free_message:
	// Free the flub's message.
	free(flub->message);

	// Return failure.
	return -1;
}


inline char* flub_message_get_compact(struct flub* flub) {
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


inline char* flub_stack_trace_get_compact(struct flub* flub) {
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


inline struct flub* flub_throw_compact(unsigned long error_code) {
	// A little dirty. Since we specified an unsigned long, cast the long
	// to a struct flub pointer and return the value of the pointer. This
	// will work as long as the successful error code is defined to be 0.
	// Also, don't ever de-reference the structure.
	return (struct flub*)error_code;
}


struct flub* flub_throw_normal(char* message, char* function_name, unsigned long error_code) {
	struct flub* flub;

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
	flub_validate_error_code(error_code);
	#endif

	// Allocate space for the new flub.
	flub = (struct flub*)malloc(sizeof(struct flub));
	if (flub == NULL) {
		fprintf(stderr, "flub_throw() failed; unable to allocate " \
				"memory for struct flub.\n");
		return NULL;
	}

	// Initialize the flub.
	if (flub_init_normal(flub, message, function_name, error_code) == -1) {
		fprintf(stderr, "flub_throw() failed; unable to initialize "
				"flub.\n");
		goto free_flub;
	}

	// Return the new flub.
	return flub;

free_flub:
	// Free the struct flub.
	free(flub);

	// Return NULL.
	return NULL;
}


inline struct flub* flub_toss_compact(struct flub* flub,
				      unsigned long error_code) {
	// Ignore the specified flub and call the throw function.
	return flub_throw_compact(error_code);
}


struct flub* flub_toss_normal(struct flub* flub, char* message,
			      char* function_name, unsigned long error_code) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_toss() failed: specified flub " \
			"was NULL.\n");
		return NULL;
	}
	if (message == NULL) {
		fprintf(stderr, "flub_toss() failed: specified message " \
			"was NULL.\n");
		return NULL;
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_toss() failed: specified " \
			"function_name was NULL.\n");
		return NULL;
	}
	flub_validate_error_code(error_code);
	#endif

	// Initialize the flub.
	if (flub_init_normal(flub, message, function_name, error_code) == -1) {
		fprintf(stderr, "flub_toss() failed: unable to " \
			"initialize flub.\n");
		return NULL;
	}

	// Return the flub.
	return flub;
}


/*!	\brief Validate the specified error code. Zero is the only invalid
 * 	value.
 */
#ifdef DEBUG_FLUB
static void flub_validate_error_code(unsigned long error_code) {
	// Validate the error code.
	if (error_code == 0) {
		fprintf(stderr, "WARNING!\nWARNING!\nWARNING!\n" \
			"Using zero value for error_code of flub " \
			"will almost certainly break compact flub; " \
			"consider using a non-zero error_code.\n");
	}
}
#else
static inline void flub_validate_error_code(unsigned long error_code) {
	; // Do nothing.
}
#endif // DEBUG_FLUB


inline unsigned long flub_yoink_compact(struct flub* flub) {
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
