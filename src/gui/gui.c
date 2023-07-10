#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define gui_IMPORT
#include "gui.h"

/* Private macros and constants */
/* Private types */
/* Declaration of private opaque struct */
struct gui_window {
	/* Specific fields of gui_Type here. */
};

/* Private global variables */
/* Private functions */
/* Implementation of the public functions */
void gui_dispose_window(void *p)
{
	printf("Destroy window \n");
}

gui_window * gui_create_window()
{
	printf("Create window \n");
	return NULL;
}

