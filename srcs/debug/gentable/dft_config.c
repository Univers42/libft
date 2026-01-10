#include "database.h"

t_format_style	default_style(void)
{
	t_format_style	style;

	/* align defaults */
	style.number_align = ALIGN_RIGHT;
	style.string_align = ALIGN_LEFT;

	/* title/header/footer colors: choose a calm palette */
	style.title = make_color(60, 120, 200);         /* soft blue */
	style.header = make_color(40, 45, 55);          /* dark slate */
	style.footer = make_color(120, 120, 160);       /* muted lavender */

	/* row foreground colors (text color for even/odd rows) */
	style.even_row = make_color(30, 30, 30);        /* dark text for even rows */
	style.odd_row = make_color(45, 45, 45);         /* slightly lighter text for odd rows */

	/* subtle background tones to reduce contrast */	
	style.background_header = make_background(20, 40, 70);   /* deep muted blue */
	style.background_cell = make_background(250, 250, 250);  /* very light gray for cells */
	style.background_body = make_background(245, 247, 250);  /* pale bluish background for body */
	style.background_footer = make_background(235, 235, 240);/* soft footer background */

	return (style);
}

t_display_config	default_config(void)
{
	t_display_config	cfg;

	cfg.show_title = true;
	cfg.show_header = true;
	cfg.show_footer = true;
	cfg.show_row_numbers = false;
	cfg.show_left_attributes = false;
	cfg.alternating_colors = true;
	cfg.use_bold_header = true;
	cfg.col_separator = ',';
	return (cfg);
}
