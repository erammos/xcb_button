#include <stdio.h>
#include <stdlib.h>
#include "gui/gui.h"
int main(void)
{
	gui_window * wnd = gui_create_window();

	gui_dispose_window(wnd);

	return 0;
}
