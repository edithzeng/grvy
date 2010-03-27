//--------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS GRVY Toolkit
//
// GRVY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRVY is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRVY.  If not, see <http://www.gnu.org/licenses/>.
//
// -------------------------------------------------------------------------
//
// grvy.h: Basic API Definitions
//
// $Id$
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#ifndef GRVY_H_
#define GRVY_H_

#ifdef __cplusplus
extern "C" {
#endif

  /*! \page apiC C/C++ Library Interface
   *
   * This section outlines the available GRVY functions for C/C++.  To
   * use, make sure to \#include <grvy.h> within your source code
   * files. Several simple examples using these functions are provided
   * in the examples section.  Functions which have an integer return
   * value return "0" upon success. To illustrate the organizatinal
   * flexibility of the input file parsing, a
   * \ref inputFile "sample input file" is also provided.
   * <hr>
   */

  /*
   * Basic API Definition for ASCII input file parsing
   */

  /* C Interface */

  extern int grvy_input_fopen               (const char *filename);
  extern int grvy_input_fclose              ();

  extern int grvy_input_fread_char          (const char* var, char **value);
  extern int grvy_input_fread_char_ivec     (const char* var, char **value,  int elem);

  extern int grvy_input_fread_int           (const char* var, int *value);
  extern int grvy_input_fread_int_vec       (const char* var, int *value,    int nelems);
  extern int grvy_input_fread_int_ivec      (const char* var, int *value,    int elem);

  extern int grvy_input_fread_float         (const char* var, float *value);
  extern int grvy_input_fread_float_vec     (const char* var, float *value,  int nelems);
  extern int grvy_input_fread_float_ivec    (const char *var, float *value,  int elem);

  extern int grvy_input_fread_double        (const char* var, double *value);
  extern int grvy_input_fread_double_vec    (const char* var, double *value, int nelems);
  extern int grvy_input_fread_double_ivec   (const char* var, double *value, int elem);

  extern int grvy_input_fdump               ();
  extern int grvy_input_fdump_delim         (const char* prefix);
  extern int grvy_input_fdump_file          (const char* prefix, const char *filename);

  /* \subpage input_register Default Value Registration Functions */

  /* Input variable registration functions (to provide default input
     values and backwards compatibility support) */

  extern int grvy_input_register_int        (const char *var,     int value);
  extern int grvy_input_register_float      (const char *var,   float value);
  extern int grvy_input_register_double     (const char *var,  double value);
  extern int grvy_input_register_char       (const char *var,   char *value);

  extern int grvy_input_register_get_int    (const char *var,    int *value);
  extern int grvy_input_register_get_float  (const char *var,  float *value);
  extern int grvy_input_register_get_double (const char *var, double *value);
  extern int grvy_input_register_get_char   (const char *var,  char **value);

  /* Error Message Output Control */

  extern void grvy_input_toggle_messages(int flag);

  /*
   * Basic API Definition for Performance Timers
   */

  extern double grvy_timer                ();
  extern void   grvy_timer_init           (const char *id);
  extern void   grvy_timer_reset          ();
  extern void   grvy_timer_finalize       ();
  extern void   grvy_timer_begin          (const char *id);
  extern void   grvy_timer_end            (const char *id);
  extern double grvy_timer_elapsedseconds (const char *id);
  extern double grvy_timer_elapsed_global ();
  extern void   grvy_timer_summarize      ();

  /*
   * Basic API Definition for Math Utilities
   */

  extern int grvy_double_isnan            (double *val);
  extern int grvy_double_isinf            (double *val);

  extern int grvy_float_isnan             (float  *val);
  extern int grvy_float_isinf             (float  *val);

  /*
   * Logging Utilities
   */

  /* Available logging priorities; roughly mimics syslog priority levels but with
   * fewer options. Also provides a NOLOG option to disable all log messages. 
   */
  
  typedef enum {GRVY_NOLOG  =  -1,
		GRVY_FATAL  =   0,
		GRVY_ERROR  = 100,
		GRVY_WARN   = 200,
		GRVY_INFO   = 300,
		GRVY_DEBUG  = 400,
		GRVY_ALL    = 500
  } PriorityLevel;

  extern void grvy_log_setlevel           (int priority);
  extern void grvy_log                    (int loglevel, const char *mesg);

  /*
   * Miscellaneous Utilities
   */

  extern int    grvy_check_file_path      (const char *pathname);
  extern int    grvy_create_unique_dir    (      char *name_template);
  extern int    grvy_create_scratch_dir   (      char *name_template);



  /*
   * Doxygen API Definitions
   */

  /*! \page apiC
   * \section input Input Parsing Routines
   *
   * <b>Open/close GRVY input file:</b>
   * \code
   * int grvy_input_fopen             (const char *filename);
   * int grvy_input_fclose            ();
   * \endcode
   *
   * <b>Read a single int, float, or double for input keyword \e var:</b>
   * \code
   * int grvy_input_fread_int         (const char* var, int    *value);
   * int grvy_input_fread_float       (const char* var, float  *value);
   * int grvy_input_fread_double      (const char* var, double *value);
   * \endcode
   *
   * <b>Read a vector of  ints, floats, or doubles for input keyword \e var:</b>
   * \code
   * int grvy_input_fread_int_vec     (const char* var, int    *value, int nelems);
   * int grvy_input_fread_float_vec   (const char* var, float  *value, int nelems);
   * int grvy_input_fread_double_vec  (const char* var, double *value, int nelems);
   * \endcode
   *
   * \section timing Performance Timing Routines
   * \code
   * double grvy_timer                ();
   * void   grvy_timer_init           (const char *id);
   * void   grvy_timer_reset          ();
   * void   grvy_timer_finalize       ();
   * void   grvy_timer_begin          (const char *id);
   * void   grvy_timer_end            (const char *id);
   * double grvy_timer_elapsedseconds (const char *id);
   * double grvy_timer_elapsed_global ();
   * void   grvy_timer_summarize      ();
   * \endcode
   *
   * \section misc Miscellaneous Routines
   * <b>grvy_check_file_path: Check desired output \e pathname and create any non-existing parent directories.</b>
   * \code
   * int    grvy_check_file_path      (const char *pathname);
   * \endcode
   * <b>grvy_create_unique_dir: Create a new, unique directory with 0700 permissions using the provided template name.  The last six characters of name_template must be XXXXXX, and name_template is modified with the unique name upon return.</b>
   * \code
   * int    grvy_create_unique_dir      (char *name_template);
   * \endcode
   * <b>grvy_create_scratch_dir: Identical to grvy_create_unique_dir, except the new directory and all of its contents are deleted when the process exits.</b>
   * \code
   * int    grvy_create_scratch_dir     (char *name_template);
   * \endcode
   */


#ifdef __cplusplus
}
#endif

#endif