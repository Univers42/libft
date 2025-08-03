#include "input_handler.h"

// Forward declarations of methods
static void	add_event_method(t_input_event *self, int keycode, void (*callback)(void *), void *param);
static void	prog_hook_events_method(t_input_event *self, void *win);
static void	destroy_method(t_input_event *self);

t_input_event	*create_event(void)
{
	t_input_event	*event;
	int				i;

	event = malloc(sizeof(t_input_event));
	if (!event)
		return (NULL);
	
	// Initialize event table to NULL
	i = 0;
	while (i < MAX_EVENTS)
	{
		event->events[i].keycode = 0;
		event->events[i].callback = NULL;
		event->events[i].param = NULL;
		i++;
	}
	
	event->event_count = 0;
	event->mlx_ptr = NULL;
	event->win_ptr = NULL;
	
	// Set method pointers (vtable-like)
	event->add_event = add_event_method;
	event->prog_hook_events = prog_hook_events_method;
	event->destroy = destroy_method;
	
	return (event);
}

static void	add_event_method(t_input_event *self, int keycode, void (*callback)(void *), void *param)
{
	if (!self || self->event_count >= MAX_EVENTS || !callback)
		return ;
	
	self->events[self->event_count].keycode = keycode;
	self->events[self->event_count].callback = callback;
	self->events[self->event_count].param = param;
	self->event_count++;
}

static void	prog_hook_events_method(t_input_event *self, void *win)
{
	if (!self || !win)
		return ;
	
	self->win_ptr = win;
	// Assuming win structure contains mlx_ptr - adjust as needed
	// self->mlx_ptr = ((t_win *)win)->mlx_ptr;
	
	mlx_hook(win, 2, 1L<<0, handle_keypress, self);  // KeyPress event
	mlx_hook(win, 17, 1L<<17, handle_close, self);   // Close event
}

static void	destroy_method(t_input_event *self)
{
	if (self)
		free(self);
}

int	handle_keypress(int keycode, void *param)
{
	t_input_event	*event_system;
	int				i;

	event_system = (t_input_event *)param;
	if (!event_system)
		return (0);
	
	i = 0;
	while (i < event_system->event_count)
	{
		if (event_system->events[i].keycode == keycode && event_system->events[i].callback)
		{
			event_system->events[i].callback(event_system->events[i].param);
			return (0);
		}
		i++;
	}
	return (0);
}

int	handle_close(void *param)
{
	t_input_event	*event_system;

	event_system = (t_input_event *)param;
	if (event_system && event_system->mlx_ptr)
	{
		// Clean exit
		exit(0);
	}
	return (0);
}
