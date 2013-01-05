/* 
 * Copyright 2011-2013 by Wade Cline.
 *
 * Original Author: Wade Cline (clinew)
 * File: flub.h
 * Created: 2011 October 5th by clinew.
 * Last Modified: 2013 January 4th, by clinew.
 *
 * This file contains a C structure and appropriate functions to allow
 * error-checking and detection. It's structure is similar to that of
 * exceptions in newer programming languages. See the function headers
 * or the README file for more details.
 */


#ifndef flub_H
#define flub_H


#ifdef DEBUG
#define DEBUG_FLUB
#endif


#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// These macros take the specified flub functions and replace them with the
// appropriate compact/normal versions of the functions, depending on whether
// the user has specified compact flubs or not.
#ifdef FLUB_COMPACT
#define flub_append(flub, function_name) \
	flub_append_compact(flub)
#define flub_catch(flub) \
	flub_catch_compact(flub)
#define flub_error_code_get(flub) \
	flub_error_code_get_compact(flub)
#define flub_free(flub) \
	flub_free_compact(flub)
#define flub_message_get(flub) \
	flub_message_get_compact(flub)
#define flub_print(flub) \
	flub_print_compact(flub)
#define flub_stack_trace_get(flub) \
	flub_stack_trace_get_compact(flub)
#define flub_throw(message, function_name, error_code) \
	flub_throw_compact(error_code)
#define flub_yoink(flub) \
	flub_yoink_compact(flub)
#else
#define flub_append(flub, function_name) \
	flub_append_normal(flub, function_name)
#define flub_catch(flub) \
	flub_catch_normal(flub)
#define flub_error_code_get(flub) \
	flub_error_code_get_normal(flub)
#define flub_free(flub) \
	flub_free_normal(flub)
#define flub_message_get(flub) \
	flub_message_get_normal(flub)
#define flub_print(flub) \
	flub_print_normal(flub)
#define flub_stack_trace_get(flub) \
	flub_stack_trace_get_normal(flub)
#define flub_throw(message, function_name, error_code) \
	flub_throw_normal(message, function_name, error_code)
#define flub_yoink(flub) \
	flub_yoink_normal(flub)
#endif


/*!	\brief An error-reporting structure that can be returned from a
 *	function in order to provide debugging information to the caller.
 */
struct flub {
	//! Contains an error message detailing what caused the flub.
	char* message;
	//! Contains the stack trace from where the flub occurred.
	char* stack_trace;
	//! Contains the error code of the type of flub. Zero -must- indicate
	//  success in order for compact errors to be reported properly.
	unsigned long error_code;
};


/*!	\brief Simply returns the pointer to the specified flub.
 *
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_append() when compact flubs are set.
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 *
 * 	\return	The pointer to the specified flub.
 */
struct flub* flub_append_compact(struct flub* flub);


/*!	\brief Appends the specified function name to the specified flub's
 *	stack trace and returns the pointer to the specified flub. 
 *	
 *	\note Will exit the program on error, which is most likely to occurr
 *	from a malloc failure.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_append() when compact flubs are not set.
 *
 *	\return	The pointer to the specified flub.
 */
struct flub* flub_append_normal(struct flub* flub, char* function_name);


/*!	\brief Prints the specified flub's error code to standard error and
 * 	then returns 0.
 *
 * 	\warning Do not call this function yourself; it will be called 
 * 	automatically by flub_catch() when compact flubs are set.
 *	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 * 	
 * 	\return The pointer to the specified flub.
 */
struct flub* flub_catch_compact(struct flub* flub);


/*!	\brief Prints the specified flub's message and stack trace to standard 
 *	error and then frees the specified flub. Returns NULL (allowing one to
 *	easily reset a local variable).
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_catch() when compact flubs are not set.
 *
 *	\return	NULL.
 */
struct flub* flub_catch_normal(struct flub* flub);


/*!	\brief Casts the specified flub pointer back into the error code it
 *	represents and returns that value.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_error_code_get() when compact flubs are set.
 *
 *	\return The error code embeded within the specified flub pointer.
 */
unsigned long flub_error_code_get_compact(struct flub* flub);


/*!	\brief Returns the error codes of the specified flub.
 *
 * 	\warning Do not call this function yourself; it will be called
 *	automatically by flub_error_code_get() when compact flubs are not set.
 *
 * 	\return The error code of the specified flub.
 */
unsigned long flub_error_code_get_normal(struct flub* flub);


/*!	\brief Returns 0.
 *
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_free() when compact flubs are set.
 *
 * 	\return	0.
 */
struct flub* flub_free_compact(struct flub* flub);


/*!	\brief Frees any resources associated with the specified flub.
 *
 *	\note Returns NULL so that the calling code need not reset its local
 *	variable on a following line.
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_free() when compact flubs are not set.
 *
 *	\return	NULL.
 */
struct flub* flub_free_normal(struct flub* flub);


/*!	\brief Returns NULL.
 *
 * 	There's no string here, so there is likewise nothing to return.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_message_get() when compact flubs are set.
 *
 *	\return NULL.
 */
char* flub_message_get_compact(struct flub* flub);


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
 * 	automatically by flub_stack_trace_get() when compact flubs are set.
 *
 * 	\return NULL.
 */
char* flub_stack_trace_get_compact(struct flub* flub);


/*!	\brief Returns the specified flub's stack trace.
 *
 * 	Considering that this is just a raw string, I have no idea why you
 * 	would actually want this, but, just in case...
 *
 * 	\warning Do not call this function yourself; it will be called
 * 	automatically by flub_stack_trace_get() when compact flubs are not set.
 *
 * 	\return The specified flub's stack trace.
 */
char* flub_stack_trace_get_normal(struct flub* flub);


/*!	\brief Initializes a flub with the specified error code.
 *
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_throw() when compact flubs are set.
 *
 * 	\return	The error code cast as a struct flub pointer.
 */
struct flub* flub_throw_compact(unsigned long error_code);


/*!	\brief Initializes a flub with the specified values. This will
 *	implicitly allocate space, so make sure to free the space via
 *	flub_catch().
 *
 *	\param[in] 	error_code	The error code associated with the 
 *					flub.
 *	\param[in]	message		A message describing what triggered 
 *					the flub.
 *	\param[in]	function_name	The name of the function in which 
 *					the flub was thrown.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_throw() when compact flubs are not set.
 *	\warning Remember to free the flub using flub_catch() or
 *	flub_free().
 *
 * 	\return	A pointer to the newly-initialized flub.
 */
struct flub* flub_throw_normal(char* message, char* function_name, 
			unsigned long error_code);


/*!	\brief Get the error code of the specified flub.
 *
 * 	\warning The pointer to the flub is really an unsigned long error code
 * 	in disguise. Dereferencing it will cause undefined behaviour.
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_throw() when compact flubs are set.
 *
 *	\return The error code of the specified flub.
 */
unsigned long flub_yoink_compact(struct flub* flub);


/*!	\brief Get the error code of the specified flub.
 *
 *	\warning Do not call this function yourself; it will be called
 *	automatically by flub_throw() when compact flubs are set.
 *
 * 	\return	The error code of the specified flub.
 */
unsigned long flub_yoink_normal(struct flub* flub);


#endif // flub_H
