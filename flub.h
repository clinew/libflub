/* 
 * Copyright 2011-2013 by Wade Cline.
 *
 * Original Author: Wade Cline (clinew)
 * File: flub.h
 * Created: 2011 October 5th by clinew.
 *
 * This file contains a C structure and appropriate functions to allow
 * error-checking and detection. It's structure is similar to that of
 * exceptions in newer programming languages. See the function headers
 * or the README file for more details.
 */


#ifndef flub_H
#define flub_H


// Debugging.
#ifdef DEBUG
#define DEBUG_FLUB
#endif


#include <execinfo.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// These macros take the specified flub functions and replace them with the
// appropriate compact/normal versions of the functions, depending on whether
// the user has specified compact flubs or not.
#ifdef FLUB_COMPACT
#define flub_error_get(flub) \
	flub_error_get_compact(flub)
#define flub_grab(flub) \
	flub_grab_compact(flub)
#define flub_message_get(flub) \
	flub_message_get_compact(flub)
#define flub_print(flub) \
	flub_print_compact(flub)
#define flub_backtrace_get(flub) \
	flub_backtrace_get_compact(flub)
#define flub_toss(flub, message, error) \
	flub_toss_compact(flub, error)
#define flub_yoink(flub) \
	flub_yoink_compact(flub)
#else
#define flub_error_get(flub) \
	flub_error_get_normal(flub)
#define flub_grab(flub) \
	flub_grab_normal(flub)
#define flub_message_get(flub) \
	flub_message_get_normal(flub)
#define flub_print(flub) \
	flub_print_normal(flub)
#define flub_backtrace_get(flub) \
	flub_backtrace_get_normal(flub)
#define flub_toss(flub, message, error) \
	flub_toss_normal(flub, message, error)
#define flub_yoink(flub) \
	flub_yoink_normal(flub)
#endif


// The maximum number of elements in the backtrace.
#define FLUB_BACKTRACE_COUNT 32


/*!	\brief An error-reporting structure that can be returned from a
 *	function in order to provide debugging information to the caller.
 */
struct flub {
	//! Contains an error message detailing what caused the flub.
	char* message;
	//! Contains the stack trace from where the flub occurred.
	char** backtrace;
	//! Contains the error code of the type of flub. Zero -must- indicate
	//  success in order for compact errors to be reported properly.
	unsigned long error;
};


/*!	\brief Casts the specified flub pointer back into the error code it
 *	represents and returns that value.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_error_get() when compact flubs are set.
 *
 *	\return The error code embeded within the specified flub pointer.
 */
inline unsigned long flub_error_get_compact(struct flub* flub);


/*!	\brief Returns the error codes of the specified flub.
 *
 * 	\warning Do not call this function yourself; it will be called
 *	automatically by flub_error_get() when compact flubs are not set.
 *
 * 	\return The error code of the specified flub; 0 if dynamic allocation
 * 	failed.
 */
inline unsigned long flub_error_get_normal(struct flub* flub);


/*!	\brief Pretty prints the specified flub's error code to standard error.
 *
 * 	\warning Do not call this function yourself; it will be called 
 * 	automatically by flub_grab() when compact flubs are set.
 *	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 */
void flub_grab_compact(struct flub* flub);


/*!	\brief Pretty prints the specified flub's message and stack trace to
 * 	stderr and then frees the resources associated with the specified flub.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_grab() when compact flubs are not set.
 */
void flub_grab_normal(struct flub* flub);


/*!	\brief Returns NULL.
 *
 * 	There's no string here, so there is likewise nothing to return.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_message_get() when compact flubs are set.
 *
 *	\return NULL.
 */
inline char* flub_message_get_compact(struct flub* flub);


/*!	\brief Returns the specified flub's message.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_message_get() when compact flubs are not set.
 *
 *	\return The specified flub's message.
 */
char* flub_message_get_normal(struct flub* flub);


/*!	\brief Prints the error code of the specified flub to standard error.
 *
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_print() when compact flubs are set.
 */
void flub_print_compact(struct flub* flub);


/*!	\brief Prints the specified flub to standard error.
 *
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_print() when compact flubs are not set.
 */
void flub_print_normal(struct flub* flub);


/*!	\brief Returns NULL.
 *
 * 	The pointer is just an error code and doesn't actually contain
 * 	a stack trace; return NULL.
 *
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_backtrace_get() when compact flubs are set.
 *
 * 	\return NULL.
 */
inline char** flub_backtrace_get_compact(struct flub* flub);


/*!	\brief Returns the specified flub's stack trace.
 *
 * 	Considering that this is just a raw string, I have no idea why you
 * 	would actually want this, but, just in case...
 *
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_backtrace_get() when compact flubs are not set.
 *
 * 	\return The specified flub's stack trace.
 */
char** flub_backtrace_get_normal(struct flub* flub);


/*!	\brief Initializes the specified flub with the specified error code.
 *
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_toss() when compact flubs are set.
 *
 * 	\return	The error code cast as a struct flub pointer.
 */
inline struct flub* flub_toss_compact(struct flub* flub,
				      unsigned long error);


/*!	\brief Initializes the specified flub with the specified values.
 *
 * 	\param[in]	flub		The flub whose members to initialize.
 *	\param[in] 	error	The error code associated with the 
 *					flub.
 *	\param[in]	message		A message describing what triggered 
 *					the flub.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_toss() when compact flubs are not set.
 *
 * 	\return	A pointer to the initialized flub.
 */
struct flub* flub_toss_normal(struct flub* flub, char* message,
			unsigned long error);


/*!	\brief Get the error code of the specified flub.
 *
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_throw() when compact flubs are set.
 *
 *	\return The error code of the specified flub.
 */
inline unsigned long flub_yoink_compact(struct flub* flub);


/*!	\brief Get the error code of the specified flub.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_throw() when compact flubs are set.
 *
 * 	\return	The error code of the specified flub; 0 if dynamic allocation
 * 	failed.
 */
unsigned long flub_yoink_normal(struct flub* flub);


#endif // flub_H
