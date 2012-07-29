/* 
 * Copyright 2011-2012 by Wade Cline.
 *
 * Original Author: Wade Cline (clinew)
 * File: flub.h
 * Created: 2011 October 5th by clinew.
 * Last Modified: 2012 July 28th, by clinew.
 *
 * This file contains a C structure and appropriate functions to allow
 * error-checking and detection. It's structure is similar to that of
 * flubs in newer programming languages. See the function headers
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


/*!	\brief An error-reporting structure that can be returned from a
 *	function in order to provide debugging information to the caller.
 */
struct flub {
	//! Contains an error message detailing what caused the flub.
	char* message;
	//! Contains the stack trace from where the flub occurred.
	char* stack_trace;
	//! Contains the error code of the type of flub.
	int error_code;
};


/*!	\brief Appends the specified function name to the specified flub's
 *	stack trace and returns the pointer to the specified flub. 
 *	
 *	\note Will exit the program on error, which is most likely to occurr
 *	from a malloc failure.
 *
 *	\return	Returns a pointer to the specified flub.
 */
struct flub* flub_append(struct flub* flub, char* function_name);


/*!	\brief Prints the specified flub's message and stack trace to standard 
 *	error and then frees the specified flub. Returns NULL (allowing one to
 *	easily reset a local variable).
 *
 *	\note Implemented by calling flub_print and then flub_free.
 *	
 *	\return	NULL.
 */
struct flub* flub_catch(struct flub* flub);


/*!	\brief Frees any resources associated with the specified flub.
 *
 *	\note Returns NULL so that the calling code need not reset its local
 *	variable on a following line.
 *
 *	\return	NULL.
 */
struct flub* flub_free(struct flub* flub);


/*!	\brief Prints the specified flub to standard error.
 */
void flub_print(struct flub* flub);


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
 */
struct flub* flub_throw(char* message, char* function_name, int error_code);


#endif
