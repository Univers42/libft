#include "window.h"
#include <unistd.h> // for sleep and usleep

int main(void)
{
	t_Window *win = Window_new(800, 600, "Test Window");

	if (!win)
		return (1);
	win->vtable->init(win);

	usleep(200000); // Wait 0.2s to ensure window is mapped
	win->vtable->set_resizable(win); // Now allow manual resizing

	// Optionally, call again after 1s for stubborn window managers
	usleep(1000000);
	win->vtable->set_resizable(win);

	while (1)
	{
		sleep(1);
		// The window remains open and resizable until the user closes it or presses Ctrl+C
	}

	win->vtable->destroy(win);
	return (0);
}
