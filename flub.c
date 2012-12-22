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


/*!	\brief Internal string append function.
 *
 * 	\return Pointer to a newly allocated character array containing the 
 *	concatenation of the specified head and tail.
 */
char* flub_string_append(char* head, char* tail) {
	char* appended_string;
	int head_length;
	int tail_length;

	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (head == NULL || tail == NULL) {
		fprintf(stderr, "flub_string_append failed: Destination " \
			"or source string was null.\n");
		return NULL;
	}
	#endif

	// Get the length of each string.
	head_length = strlen(head);
	tail_length = strlen(tail);

	// Allocate space for the two appended strings; equal to the length of
	// both strings combined, plus one for the null character.
	appended_string = (char*)malloc((sizeof(char) * (head_length + 
			  tail_length)) + 1);
	if (appended_string == NULL) {
		fprintf(stderr, "flub_string_append failed: Unable to " \
			"allocate space for the new string.\n");
		return NULL;
	}

	// Copy the head char array to the newly allocated char array.
	strcpy(appended_string, head);

	// Concatenate the tail char array to the newly allocated char array.
	strcat(appended_string, tail);

	// Return the concatenated char array.
	return appended_string;
}


/*!	\brief Internal string copy function.
 *
 * 	\return	Pointer to a newly-allocated character array.
 */
char* flub_string_copy(char* source) {
	char* copied_string = NULL;
	int length = -1;

	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (source == NULL) {
		fprintf(stderr, "flub_string_copy failed: Specified " \
			"string was NULL.\n");
		return NULL;
	}
	#endif

	// Get the length of the string.
	length = strlen(source);

	// Allocate memory for the new char array.
	copied_string = (char*)malloc((sizeof(char) * length) + 1);
	if (copied_string == NULL) {
		fprintf(stderr, "flub_string_copy failed: Unable to " \
			"allocate memory.\n");
		return NULL;
	}

	// Copy the source char array into the newly allocated char array.
	strcpy(copied_string, source);

	// Return the newly allocated char array.
	return copied_string;
}


struct flub* flub_append_compact(struct flub* flub) {
	// Return the specified flub.
	return flub;
}


struct flub* flub_append_normal(struct flub* flub, char* function_name) {
	char* temp_string;

	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_append failed: flub is " \
			"NULL.\n");
		exit(EXIT_FAILURE);
	}
	if (function_name == NULL) {
		fprintf(stderr, "flub_append failed: function_name " \
			"is NULL.\n");
		exit(EXIT_FAILURE);
	}
	#endif

	// Add a newline, tab, and list marker to the flub's stack trace.
	// TODO: Refactor into using realloc().
	temp_string = flub->stack_trace;
	flub->stack_trace = flub_string_append(temp_string, "\n\t-");
	if (flub->stack_trace == NULL) {
		fprintf(stderr, "flub_append failed: unable to append " \
			"\"\\n\".\n");
		exit(EXIT_FAILURE);
	}
	free(temp_string);
	
	// Add the specified function name to the flub's stack trace.
	// TODO: Refactor using realloc().
	temp_string = flub->stack_trace;
	flub->stack_trace = flub_string_append(temp_string, 
							 function_name);
	if (flub->stack_trace == NULL) {
		fprintf(stderr, "flub_append failed: unable to append " \
			"function_name.\n");
		exit(EXIT_FAILURE);
	}
	free(temp_string);

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
		fprintf(stderr, "flub_catch failed: struct flub " \
			"is NULL.\n");
		exit(EXIT_FAILURE);
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
		fprintf(stderr, "flub_free failed: struct flub " \
			"is NULL.\n");
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
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
		fprintf(stderr, "struct flub_throw failed: message " \
			"arugment was NULL.\n");
		exit(EXIT_FAILURE);
	}
	if (function_name == NULL) {
		fprintf(stderr, "struct flub_throw failed: " \
			"function_name argument was NULL.\n");
		exit(EXIT_FAILURE);
	}
	#endif

	// Check for zero error code.
	if (error_code == 0) {
		fprintf(stderr, "WARNING!\nWARNING!\nWARNING!\n" \
			"Using zero value for error_code of flub " \
			"will almost certainly break compact flub; " \
			"consider using a non-zero error_code.\n");
	}

	// Allocate space for the new flub.
	struct flub* flub = (struct flub*)malloc(
				       sizeof(struct flub));
	if (flub == NULL) {
		fprintf(stderr, "Malloc failed during flub creation.");
		exit(EXIT_FAILURE);
	}

	// Assign the new flub's message.
	flub->message = flub_string_copy(message);
	if (flub->message == NULL) {
		fprintf(stderr, "flub_throw failed: " \
			"flub_string_copy returned NULL when copying " \
			"message.\n");
		exit(EXIT_FAILURE);
	}

	// Assign the new flub's function name.
	flub->stack_trace = flub_string_copy(function_name);
	if (flub->message == NULL) {
		fprintf(stderr, "struct flub_throw failed: " \
			"flub_string_copy returned NULL when copying " \
			"function_name.\n");
		exit(EXIT_FAILURE);
	}

	// Assign the new flub's error code.
	flub->error_code = error_code;

	// Return the new flub.
	return flub;
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
		exit(EXIT_FAILURE);
	}
	#endif

	// Return the error code.
	return flub->error_code;
}
