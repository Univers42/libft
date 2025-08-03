#include "input_handler.h"
#include "window.h"
#include <stdio.h>

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
	{
		printf("❌ prog_hook_events_method: NULL parameter\n");
		return ;
	}
	
	// Store the window pointer directly (raw MLX window pointer)
	self->win_ptr = win;
	self->mlx_ptr = NULL; // We don't need to store mlx_ptr for events
	
	printf("🔗 Setting up event hooks...\n");
	printf("   win_ptr=%p\n", win);
	
	printf("🎯 Hooking keyboard events...\n");
	mlx_key_hook(win, handle_keypress, self);
	printf("🎯 Hooking close events...\n");
	mlx_hook(win, 17, 1L<<17, handle_close, self);
	printf("✅ Event hooks established successfully\n");
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
	
	printf("🔑 Key pressed: %d\n", keycode);
	
	i = 0;
	while (i < event_system->event_count)
	{
		if (event_system->events[i].keycode == keycode && event_system->events[i].callback)
		{
			printf("✅ Found matching callback for keycode %d\n", keycode);
			event_system->events[i].callback(event_system->events[i].param);
			return (0);
		}
		i++;
	}
	printf("⚠️  No callback found for keycode %d\n", keycode);
	return (0);
}

int	handle_close(void *param)
{
	(void)param;
	exit(0);
}
