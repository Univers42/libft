#ifndef INPUT_HANDLER_H
# define INPUT_HANDLER_H

# include <stdlib.h>
# include <mlx.h>

# define MAX_EVENTS 256

typedef struct s_event_entry
{
	int		keycode;
	void	(*callback)(void *param);
	void	*param;
}	t_event_entry;

typedef struct s_input_event
{
	t_event_entry	events[MAX_EVENTS];
	int				event_count;
	void			*mlx_ptr;
	void			*win_ptr;
	
	// Method pointers (vtable-like)
	void			(*add_event)(struct s_input_event *self, int keycode, void (*callback)(void *), void *param);
	void			(*prog_hook_events)(struct s_input_event *self, void *win);
	void			(*destroy)(struct s_input_event *self);
}	t_input_event;

// Constructor
t_input_event	*create_event(void);

// Internal callback handlers
int				handle_keypress(int keycode, void *param);
int				handle_close(void *param);

#endif
