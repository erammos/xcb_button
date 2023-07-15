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

#include <stdbool.h>
#include <stdint.h>
/* Constants declaration */

/* Type declarations here */
typedef struct gui_window gui_window;
typedef struct gui_event {
  uint32_t type;
} gui_event;
/* Global variables declarations */

/* Fuction prototypes here */

/**
 * Allocates a new gui_Type.
 * @return New gui_Type.
 */
EXTERN void gui_dispose_window(void *);
EXTERN gui_window *gui_create_window(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height);
EXTERN gui_event gui_poll_events(void *);
EXTERN void gui_update_window(void *);
EXTERN void gui_draw_rect(void *p, uint32_t x, uint32_t y, uint32_t width,
                          uint32_t height, uint32_t color);
EXTERN bool gui_button(void *p, uint32_t x, uint32_t y, uint32_t width,
                       uint32_t height, const char *text);
#undef EXTERN
#endif
