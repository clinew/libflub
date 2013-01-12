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


static int flub_malloc_error(struct flub* flub);
static void flub_validate_error_code(unsigned long error_code);


inline struct flub* flub_append_compact(struct flub* flub) {
	// Return the specified flub.
	return flub;
}


struct flub* flub_append_normal(struct flub* flub, char* function_name) {
	void* temp;
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

	// Check for malloc error.
	if (flub_malloc_error(flub)) {
		return flub;
	}

	// Calculate the new length of the stack trace.
	// +2 for delimeter and null byte.
	length = strlen(flub->stack_trace) + strlen(function_name) + 2;

	// Reallocate space for the stack trace.
	temp = realloc(flub->stack_trace, length);
	if (temp == NULL) {
		// Set the last char of the stack trace to '!' instead of ','.
		// This will let the user know that a realloc failed and that
		// part of the stack trace is missing.
		flub->stack_trace[strlen(flub->stack_trace) - 1] = '!';

		// Return the flub.
		return flub;
	}
	flub->stack_trace = temp;

	// Concatenate the specified function name.
	strcat(flub->stack_trace, function_name);

	// Concatenate the delimeter.
	strcat(flub->stack_trace, ",");

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

	// Reuse the grab function.
	flub_grab_normal(flub);

	// Free the flub itself.
	if (flub != (struct flub*)FLUB_ADDRESS_BAD) {
		free(flub);
	}

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

	// Check for malloc error.
	if (flub_malloc_error(flub)) {
		// No memory to free.
		return;
	}

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


/*!	\brief Helper function that allocates space for and assigns the
 * 	specified flub's members.
 *
 * 	The error code is always assigned. The message and stack trace are
 * 	either both assigned or are both not assigned.
 */
static void flub_init_normal(struct flub* flub, char* message,
			    char* function_name, unsigned long error_code) {
	// Validate parameters.
	#ifdef DEBUG_FLUB
	if (flub == NULL || flub == (struct flub*)FLUB_ADDRESS_BAD) {
		fprintf(stderr, "flub_init_normal() failed; specified " \
			"flub was NULL or bad: %p.\n", (void*)flub);
		return;
	}
	if (message == NULL) {
		fprintf(stderr, "flub_init_normal() failed; specified " \
			"message was NULL.\n");
		return;
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_init_normal() failed; specified " \
			"function_name was NULL.\n");
		return;
	}
	flub_validate_error_code(error_code);
	#endif

	// Assign the flub's error code.
	flub->error_code = error_code;

	// Assign the flub's message.
	flub->message = (char*)malloc(sizeof(char) * (strlen(message) + 1));
	if (flub->message == NULL) {
		flub->stack_trace = NULL;
		return;
	}
	strcpy(flub->message, message);

	// Assign the flub's function name.
	// +2 for delimeter and null byte.
	flub->stack_trace = (char*)malloc(sizeof(char) * 
					  (strlen(function_name) + 2));
	if (flub->stack_trace == NULL) {
		goto free_message;
	}
	flub->stack_trace = strcpy(flub->stack_trace, function_name);
	strcat(flub->stack_trace, ",");

	// Return.
	return;

free_message:
	// Free the flub's message.
	free(flub->message);
	flub->message = NULL;

	// Return.
	return;
}


/*!	\brief Helper function that checks whether there was an error
 * 	during dynamic memory allocation for the specified flub.
 *
 * 	Either an error allocating the flub itself or an error allocating
 * 	the flub's members will cause this function to return true.
 *
 * 	\return	1 if there was on error; 0 if there wasn't an error.
 */
static int flub_malloc_error(struct flub* flub) {
	// Validate arguments (obsessively).
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_malloc_error() failed: " \
			"specified flub was NULL.\n");
		return 1;
	}
	#endif

	// Check for malloc error. The message and stack trace are always both
	// malloc'd or neither are malloc'd.
	if (flub == (struct flub*)FLUB_ADDRESS_BAD || flub->message == NULL) {
	    	// Return true.
		return 1;
	}

	// Return false.
	return 0;
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

	// Check for malloc error.
	if (flub_malloc_error(flub)) {
		return NULL;
	}

	// Return the specified flub's message.
	return flub->message;
}


void flub_print_compact(struct flub* flub) {
	// Print the error code.
	fprintf(stderr, "Error Code: %lu.\n", (unsigned long)flub);
}


void flub_print_normal(struct flub* flub) {
	char* cur;

	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_print() failed: struct flub " \
			"is NULL.\n");
		return;
	}
	#endif

	// Check for flub malloc error.
	if (flub == (struct flub*)FLUB_ADDRESS_BAD) {
		// No flub at all; let the user know.
		fprintf(stderr, "ERROR: flub address indicates dynamic " \
			"allocation failed.\n");
		return;
	}

	// Print the flub header to standard error.
	fprintf(stderr, "\n*** FLUB ***\n" \
		"Error Code: %lu\n" \
		"Message: %s\n" \
		"Stack Trace: \n\t",
		flub->error_code,
		flub->message ? flub->message : "<Allocation failed>");
	
	// Print the flub stack trace to standard error.
	cur = flub->stack_trace;
	if (cur == NULL) {
		fprintf(stderr, "<Allocation failed>");
	} else {
		char symbol;

		fprintf(stderr, "-");
		while ((symbol = (*cur++)) != '\0') {
			// Regular character.
			if (symbol != ',' && symbol != '!') {
				fprintf(stderr, "%c", symbol);
			// Delimeter.
			} else {
				// End of the stack trace proper.
				if (*cur == '\0' && symbol == ',') {
					; // Do nothing.
				// Another function or missing function(s).
				} else {
					fprintf(stderr, "\n\t%c",
						(symbol == ',') ? '-' : '!');
				}
			}
		}
	}

	// Print the flub tail to standard error.
	fprintf(stderr, "\n*****************\n\n");
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

	// Check for malloc failure.
	if (flub_malloc_error(flub)) {
		return NULL;
	}

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
		return (struct flub*)FLUB_ADDRESS_BAD;
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_throw() failed: " \
			"function_name argument was NULL.\n");
		return (struct flub*)FLUB_ADDRESS_BAD;
	}
	flub_validate_error_code(error_code);
	#endif

	// Allocate space for a new flub.
	flub = (struct flub*)malloc(sizeof(struct flub));
	if (flub == NULL) {
		return (struct flub*)FLUB_ADDRESS_BAD;
	}

	// Toss the new flub.
	return flub_toss_normal(flub, message, function_name, error_code);
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
	flub_init_normal(flub, message, function_name, error_code);

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
		return 0;
	}
	#endif

	// Check for malloc error.
	if (flub == (struct flub*)FLUB_ADDRESS_BAD) {
		return 0;
	}

	// Return the error code.
	return flub->error_code;
}
