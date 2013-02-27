/*
 * Copyright 2011-2013 by Wade Cline.
 *
 * Original Author: Wade Cline (clinew)
 * File: flub.c
 * Created: 2011 October 5th by clinew.
 * 
 * See: "flub.h".
 */	


#include "flub.h"


static void flub_validate_error(unsigned long error);


inline unsigned long flub_error_get_compact(struct flub* flub) {
	// Just call the compact version of flub yoink.
	return flub_yoink_compact(flub);
}


inline unsigned long flub_error_get_normal(struct flub* flub) {
	// Just call the normal version of flub yoink.
	return flub_yoink_normal(flub);
}


inline void flub_grab_compact(struct flub* flub) {
	// Do nothing.
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

	// Free the specified flub's message.
	if (flub->message) {
		free(flub->message);
	}

	// Free the specified flub's backtrace.
	if (flub->backtrace) {
		free(flub->backtrace);
	}

	// Return.
	return;
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
	int i;

	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_print() failed: struct flub " \
			"is NULL.\n");
		return;
	}
	#endif

	// Print the flub header to standard error.
	fprintf(stderr, "\n*** FLUB ***\n" \
		"Error Code: %lu\n" \
		"Message: %s\n" \
		"Backtrace: \n",
		flub->error,
		flub->message ? flub->message : "<Allocation failed>");
	
	// Print the flub stack trace to standard error.
	if (flub->backtrace) {
		for (i = 0; i < FLUB_BACKTRACE_COUNT && flub->backtrace[i];
		     i++) {
			fprintf(stderr, "%s\n", flub->backtrace[i]);
		}
	}

	// Print the flub tail to standard error.
	fprintf(stderr, "\n*****************\n\n");
}


inline char** flub_backtrace_get_compact(struct flub* flub) {
	// Return NULL; there's no actual stack trace attached.
	return NULL;
}


char** flub_backtrace_get_normal(struct flub* flub) {
	// Validate arguments.
	#ifdef DEBUG_FLUB
	if (flub == NULL) {
		fprintf(stderr, "flub_backtrace_get() failed; struct flub " \
			"is NULL.\n");
		return NULL;
	}
	#endif

	// Return the backtrace.
	return flub->backtrace;
}


inline struct flub* flub_toss_compact(struct flub* flub,
				      unsigned long error) {
	// Ignore the specified flub and just return the error cast to a
	// flub pointer.
	return (struct flub*)error;
}


struct flub* flub_toss_normal(struct flub* flub, char* message,
			      unsigned long error) {
	void* buffer[FLUB_BACKTRACE_COUNT];
	int count;

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
	flub_validate_error(error);
	#endif

	// Assign the flub's error code.
	flub->error = error;

	// Assign the flub's message.
	flub->message = (char*)malloc(sizeof(char) * (strlen(message) + 1));
	if (flub->message) {
		strcpy(flub->message, message);
	}

	// Assign the flub's backtrace.
	count = backtrace((void**)&buffer, FLUB_BACKTRACE_COUNT);
	flub->backtrace = backtrace_symbols((void**)&buffer, count);
	if (count < FLUB_BACKTRACE_COUNT) {
		flub->backtrace[count] = NULL;
	}

	// Return the flub.
	return flub;
}


/*!	\brief Validate the specified error code. Zero is the only invalid
 * 	value.
 */
#ifdef DEBUG_FLUB
static void flub_validate_error(unsigned long error) {
	// Validate the error code.
	if (error == 0) {
		fprintf(stderr, "WARNING!\nWARNING!\nWARNING!\n" \
			"Using zero value for error of flub " \
			"will almost certainly break compact flub; " \
			"consider using a non-zero error.\n");
	}
}
#else
static inline void flub_validate_error(unsigned long error) {
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

	// Return the error code.
	return flub->error;
}
