/**
 * @file
 * @author Elias Rammos <rammoselias@gmail.com>
 * @version $Date$
 */

#ifndef GUI_H
#define GUI_H

#ifdef gui_IMPORT
	#define EXTERN
#else
	#define EXTERN extern
#endif

/* Constants declaration */

/* Type declarations here */
typedef struct gui_window gui_window;

/* Global variables declarations */

/* Fuction prototypes here */

/**
 * Allocates a new gui_Type.
 * @return New gui_Type.
 */
EXTERN void gui_dispose_window(void *);
EXTERN gui_window * gui_create_window(void);
#undef EXTERN
#endif
