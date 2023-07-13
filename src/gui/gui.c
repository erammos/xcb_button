#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#define gui_IMPORT
#include "gui.h"

/* Private macros and constants */
/* Private types */
/* Declaration of private opaque struct */
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_gcontext_t;
typedef void (*function)();
struct gui_window {
  xcb_window_t window;
  xcb_connection_t *connection;
};

xcb_rectangle_t rectangle[] = {{10, 50, 40, 20}};
/* Private global variables */
/* Private functions */
/* Implementation of the public functions */
void gui_dispose_window(void *p) {
  printf("Destroy window \n");
  gui_window *ptr = p;
  xcb_disconnect(ptr->connection);
  free(p);
}

gui_window *gui_create_window(int x, int y, int width, int height) {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);
  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;
  uint32_t window = xcb_generate_id(connection);
  uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[2] = {0xffffff, XCB_EVENT_MASK_EXPOSURE};
  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, x,
                    y, width, height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, mask, values);
  xcb_map_window(connection, window);
  gui_window *ptr = malloc(sizeof(gui_window));
  ptr->connection = connection;
  ptr->window = window;
  printf("Create window \n");

  return ptr;
}

void gui_draw_rect(void *p) { gui_window *ptr = p; };

gui_event gui_poll_events(void *p) {
  gui_window *ptr = p;
  xcb_generic_event_t *event = xcb_poll_for_event(ptr->connection);
  gui_event e = {.type = 0};
  if (event == NULL)
    return e;
  switch (event->response_type & ~0x80) {
  case XCB_EXPOSE:
    printf("expose \n");
    break;
  default:
    break;
  }
  free(event);
  e.type = 66;
  return e;
}

void gui_update_window(void *p) {
  gui_window *ptr = p;
  xcb_flush(ptr->connection);
}
