#include "gui/gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void) {
  gui_window *wnd = gui_create_window(0, 0, 50, 50);
  while (!gui_quit(wnd)) {
    if (gui_button(wnd, 100, 100, 100, 100, "Click 1")) {
      printf("Click 1\n");
    }
    if (gui_button(wnd, 210, 100, 100, 100, "Click 2")) {
      printf("Click 2\n");
    }
    gui_update_window(wnd);
  }
  gui_dispose_window(wnd);

  return 0;
}
