/** @file main.c
 *  @brief An XCB Demo.
 *
 *  Renders Two buttons and two texts in imgui style
 *
 *  @author Elias Rammos <rammoselias@gmail.com>
 */

#include "gui/gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main (void)
{
  gui_window *wnd = gui_create_window (0, 0, 200, 200);
  while (!gui_quit (wnd))
    {

      if (gui_button (wnd, 100, 100, 100, 100, "Click 1"))
        {
          gui_text (wnd, 180, 80, "6x13", "Click 1");
        }

      if (gui_button (wnd, 210, 100, 100, 100, "Click 2"))
        {
          gui_text (wnd, 180, 80, "6x13", "Click 2");
        }

      gui_update_window (wnd);
    }
  gui_dispose_window (wnd);

  return 0;
}
