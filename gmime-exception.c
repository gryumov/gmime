/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* gmime-execpetion.c : exception utils */

/* 
 *
 * Authors: Bertrand Guiheneuf <bertrand@helixcode.com>
 *
 * Copyright 1999, 2000 Helix Code, Inc. (http://www.helixcode.com)
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gmime-exception.h"


/**
 * g_mime_exception_new: allocate a new exception object. 
 * 
 * Create and returns a new exception object.
 * Returns the newly allocated exception object.
 **/
GMimeException *
g_mime_exception_new (void)
{
	GMimeException *ex;
	
	ex = g_new (GMimeException, 1);
	ex->desc = NULL;
	
	/* set the Exception Id to NULL */
	ex->id = GMIME_EXCEPTION_NONE;
	
	return ex;
}


/**
 * g_mime_exception_init: init a (statically allocated) exception.
 * @ex: exception
 * 
 * Init an exception. This routine is mainly
 * useful when using a statically allocated
 * exception.
 **/
void
g_mime_exception_init (GMimeException *ex)
{
	ex->desc = NULL;
	
	/* set the Exception Id to NULL */
	ex->id = GMIME_EXCEPTION_NONE;
}


/**
 * g_mime_exception_clear: Clear an exception
 * @exception: the exception object
 * 
 * Clear an exception, that is, set the exception ID to
 * GMIME_EXCEPTION_NONE and free the description text. If the
 * exception is NULL, this funtion just returns.
 **/
void 
g_mime_exception_clear (GMimeException *exception)
{
	if (!exception)
		return;
	
	/* free the description text */
	if (exception->desc)
		g_free (exception->desc);
	exception->desc = NULL;
	
	/* set the Exception Id to NULL */
	exception->id = GMIME_EXCEPTION_NONE;
}


/**
 * g_mime_exception_free: Free an exception 
 * @exception: The exception object to free
 * 
 * Free an exception object. If the exception is NULL, nothing is
 * done, the routine simply returns.
 **/
void 
g_mime_exception_free (GMimeException *exception)
{
	if (!exception)
		return;
	
	/* free the description text */
	if (exception->desc)
		g_free (exception->desc);
	
	/* free the exeption itself */
	g_free (exception);
}


/**
 * g_mime_exception_set: set an exception 
 * @ex: exception object 
 * @id: exception id 
 * @desc: textual description of the exception
 * 
 * Set the value of an exception. The exception id is 
 * a unique number representing the exception. The 
 * textual description is a small text explaining 
 * what happened and provoked the exception.
 *
 * When @ex is NULL, nothing is done, this routine
 * simply returns.
 *
 **/
void
g_mime_exception_set (GMimeException *ex,
		      ExceptionId id,
		      const char *desc)
{
	/* if no exception is given, do nothing */
	if (!ex)
		return;
	
	ex->id = id;
	
	/* remove the previous exception description */
	if (desc != ex->desc) {
		g_free (ex->desc);
		ex->desc = g_strdup (desc);
	}
}


/**
 * g_mime_exception_setv: set an exception 
 * @ex: exception object 
 * @id: exception id 
 * @format: format of the description string. The format string is
 * used as in printf().
 * @Varargs: arguments
 * 
 * Set the value of an exception. The exception id is 
 * a unique number representing the exception. The 
 * textual description is a small text explaining 
 * what happened and provoked the exception. 
 * In this version, the string is created from the format 
 * string and the variable argument list.
 *
 * It is safe to say:
 *   g_mime_exception_setv (ex, ..., g_mime_exception_get_description (ex), ...);
 *
 * When @ex is NULL, nothing is done, this routine
 * simply returns.
 **/
void
g_mime_exception_setv (GMimeException *ex,
		       ExceptionId id,
		       const char *format, 
		       ...)
{
	va_list args;
	char *buf;
	
	/* if no exception is given, do nothing */
	if (!ex)
		return;
	
	buf = ex->desc;
	
	/* create the temporary exception string */
	va_start(args, format);
	ex->desc = g_strdup_vprintf (format, args);
	va_end (args);
	
	g_free (buf);
	
	ex->id = id;
}


/**
 * g_mime_exception_xfer: transfer an exception
 * @ex_dst: Destination exception object 
 * @ex_src: Source exception object
 * 
 * Transfer the content of an exception from
 * an exception object to another. 
 * The destination exception receives the id and
 * the description text of the source exception. 
 **/
void 
g_mime_exception_xfer (GMimeException *ex_dst,
		       GMimeException *ex_src)
{
	if (ex_dst->desc)
		g_free (ex_dst->desc);
	
	ex_dst->id = ex_src->id;
	ex_dst->desc = ex_src->desc;
	
	ex_src->id = GMIME_EXCEPTION_NONE;
	ex_src->desc = NULL;
}


/**
 * g_mime_exception_get_id: get the exception id
 * @ex: The exception object
 * 
 * Return the id of an exception. 
 * If @ex is NULL, return GMIME_EXCEPTION_NONE;
 * 
 * Return value: Exception ID.
 **/
ExceptionId
g_mime_exception_get_id (GMimeException *ex)
{
	if (ex)
		return ex->id;
	else 
		return GMIME_EXCEPTION_NONE;
}


/**
 * g_mime_exception_get_description: get the description of an exception.
 * @ex: The exception object
 * 
 * Return the exception description text. 
 * If @ex is NULL, return NULL;
 * 
 * Return value: Exception description text.
 **/
const char *
g_mime_exception_get_description (GMimeException *ex)
{
	if (ex)
		return ex->desc;
	else 
		return NULL;
}
