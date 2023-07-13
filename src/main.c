#include <stdio.h>
#include <stdlib.h>
#include "gui/gui.h"
int main(void)
{
	gui_window * wnd = gui_create_window(0,0,100,100);
	while(1)
	{
	  gui_event e = gui_poll_events(wnd);         
	  gui_update_window(wnd);
	}
	gui_dispose_window(wnd);

	return 0;
}
