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
static bool isPressed = false;
/* Declaration of private opaque struct */
typedef uint32_t xcb_window_t;
typedef uint32_t xcb_gcontext_t;
typedef void (*function)();
struct gui_window {
  xcb_screen_t *screen;
  xcb_connection_t *connection;
  xcb_window_t window;
  xcb_gcontext_t gc;
  xcb_colormap_t colormap;
  xcb_generic_event_t *event;
};
/* Private global variables */
/* Private functions */
static inline int check_request(xcb_connection_t *c, xcb_void_cookie_t cookie,
                                const char *msg) {
  xcb_generic_error_t *error = xcb_request_check(c, cookie);
  if (error) {
    fprintf(stderr, "%s error: %d\n", msg, error->error_code);
    xcb_disconnect(c);
    return 0;
  }
  return 1;
}

static xcb_gc_t gc_font_get(xcb_connection_t *c, xcb_screen_t *screen,
                            xcb_window_t window, const char *font_name) {
  uint32_t value_list[3];
  xcb_void_cookie_t cookie_font;
  xcb_void_cookie_t cookie_gc;
  xcb_font_t font;
  xcb_gcontext_t gc;
  uint32_t mask;

  font = xcb_generate_id(c);
  cookie_font = xcb_open_font_checked(c, font, strlen(font_name), font_name);
  check_request(c, cookie_font, "Can't Open Font");
  gc = xcb_generate_id(c);
  mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
  value_list[0] = screen->black_pixel;
  value_list[1] = screen->white_pixel;
  value_list[2] = font;
  cookie_gc = xcb_create_gc_checked(c, gc, window, mask, value_list);
  check_request(c, cookie_gc, "Can't create gc");
  cookie_font = xcb_close_font_checked(c, font);
  check_request(c, cookie_font, "Can't close font");
  return gc;
}

static void text_draw(xcb_connection_t *c, xcb_screen_t *screen,
                      xcb_window_t window, int16_t x1, int16_t y1,
                      const char *label) {
  xcb_void_cookie_t cookie_gc;
  xcb_void_cookie_t cookie_text;
  xcb_gcontext_t gc;
  uint8_t length;

  length = strlen(label);
  gc = gc_font_get(c, screen, window, "6x13");
  cookie_text = xcb_image_text_8_checked(c, length, window, gc, x1, y1, label);
  check_request(c, cookie_text, "Can't paste text");
  cookie_gc = xcb_free_gc(c, gc);
  check_request(c, cookie_gc, "Can't free gc");
}

static void button_draw(gui_window *ptr, int16_t x, int16_t y, int16_t width,
                        int16_t height, const char *label) {
  xcb_void_cookie_t cookie_gc;
  xcb_void_cookie_t cookie_rect;
  xcb_void_cookie_t cookie_text;
  xcb_gcontext_t gc;
  uint8_t length;
  length = strlen(label);
  int16_t text_width = 6 * length;
  gc = gc_font_get(ptr->connection, ptr->screen, ptr->window, "6x13");

  xcb_rectangle_t rectangle[] = {{x, y, width, height}};
  cookie_rect = xcb_poly_fill_rectangle(ptr->connection, ptr->window, ptr->gc,
                                        1, rectangle);
  check_request(ptr->connection, cookie_rect, "Can't draw rectangle");
  cookie_text = xcb_image_text_8_checked(ptr->connection, length, ptr->window,
                                         gc, x + (width - text_width) / 2,
                                         y + height / 2, label);
  check_request(ptr->connection, cookie_text, "Can't paste text");
  cookie_gc = xcb_free_gc(ptr->connection, gc);
  check_request(ptr->connection, cookie_gc, "Can't free gc");
}
/* Implementation of the public functions */
void gui_dispose_window(void *p) {
  printf("Destroy window \n");
  gui_window *ptr = p;
  xcb_disconnect(ptr->connection);
  free(p);
}

gui_window *gui_create_window(uint32_t x, uint32_t y, uint32_t width,
                              uint32_t height) {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);
  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;
  uint32_t window = xcb_generate_id(connection);
  uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
  uint32_t values[3] = {
      0,
      XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS |
          XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_BUTTON_RELEASE |
          XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION,
      screen->default_colormap};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root, x,
                    y, width, height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, mask, values);
  xcb_map_window(connection, window);
  gui_window *ptr = malloc(sizeof(gui_window));
  ptr->connection = connection;
  ptr->window = window;
  ptr->colormap = screen->default_colormap;
  ptr->screen = screen;
  printf("Create window \n");

  xcb_gcontext_t gc = xcb_generate_id(connection);
  values[0] = screen->white_pixel;
  values[1] = 0;
  xcb_create_gc(connection, gc, screen->root,
                XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES, values);
  ptr->gc = gc;

  return ptr;
}

void gui_draw_rect(void *p, uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height, uint32_t color) {
  gui_window *ptr = p;
  xcb_rectangle_t rectangle[] = {{x, y, width, height}};
  uint32_t values[1] = {color};
  xcb_change_gc(ptr->connection, ptr->gc, XCB_GC_FOREGROUND, values);
  xcb_poly_fill_rectangle(ptr->connection, ptr->window, ptr->gc, 1, rectangle);
};
bool gui_button(void *p, uint32_t x, uint32_t y, uint32_t width,
                uint32_t height, const char *text) {
  gui_window *ptr = p;
  if (ptr->event) {
    switch (ptr->event->response_type & ~0x80) {
    case XCB_EXPOSE: {

      if (isPressed) {
        button_draw(ptr, x + 5, y + 5 , width - 10, height - 10, text);
      } else {

        button_draw(ptr, x, y, width, height, text);
      }
      break;
    }
    case XCB_BUTTON_RELEASE: {

      xcb_button_press_event_t *ev;
      ev = (xcb_button_press_event_t *)ptr->event;
      if (isPressed && ev->event_x >= (x) && ev->event_x <= (x + width) &&
          ev->event_y >= (y) && ev->event_y <= (y + height)) {
        printf("Button Release\n ");
        xcb_clear_area(ptr->connection, 1, ptr->window, x  , y , width , height );
        isPressed = false;
        return true;
      }
      break;
    }
    case XCB_BUTTON_PRESS: {
      xcb_button_press_event_t *ev;
      ev = (xcb_button_press_event_t *)ptr->event;
      if (!isPressed && ev->event_x >= (x) && ev->event_x <= (x + width) &&
          ev->event_y >= (y) && ev->event_y <= (y + height)) {
        isPressed = true;
        printf("Button Press\n ");
        xcb_clear_area(ptr->connection, 1, ptr->window, x, y, width, height);
        return true;
      }
      break;
    }
    }
  }

  return false;
}
bool gui_quit(void *p) {
  gui_window *ptr = p;
  xcb_generic_event_t *e = xcb_poll_for_event(ptr->connection);
  ptr->event = e;
  if (e) {
    switch (e->response_type & ~0x80) {
    case XCB_KEY_RELEASE: {
      xcb_key_release_event_t *ev;
      ev = (xcb_key_release_event_t *)e;
      switch (ev->detail) {
        /* ESC */
      case 9:
        free(e);
        return true;
      }
    }
    }
  }
  return false;
}

void gui_update_window(void *p) {
  gui_window *ptr = p;
  free(ptr->event);
  xcb_flush(ptr->connection);
}
