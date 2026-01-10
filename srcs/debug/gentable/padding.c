#include "database.h"

void	compute_padding_if_not_too_long(t_pad_ctx *ctx)
{
	size_t	pad;

	if (!ctx || !ctx->buf || ctx->bufsize == 0)
		return ;
	if (!ctx->text)
		ctx->text = "";
	pad = 0;
	if (ctx->width > (size_t)ctx->dlen)
		pad = ctx->width - (size_t)ctx->dlen;
	if (ctx->align == ALIGN_LEFT)
		pad_left(ctx->buf, ctx->text, pad, ctx->bufsize);
	else if (ctx->align == ALIGN_RIGHT)
		pad_right(ctx->buf, ctx->text, pad, ctx->bufsize);
	else
		pad_center(ctx->buf, ctx->text, pad, ctx->bufsize);
}

void	format_cell(t_pad_ctx *ctx)
{
	int		dlen;
	char	tmp[MAX_LINE_LENGTH];

	if (!ctx || !ctx->buf)
		return ;
	if (!ctx->text)
		ctx->text = "";
	dlen = display_width(ctx->text);
	if ((size_t)dlen > ctx->width)
	{
		utf8_truncate_by_display_width(ctx->text, ctx->width,
			tmp, sizeof(tmp));
		ft_strlcpy(ctx->buf, tmp, ctx->bufsize);
		return ;
	}
	ctx->dlen = dlen;
	compute_padding_if_not_too_long(ctx);
}

void	pad_left(char *buf, const char *text, size_t pad, size_t bufsize)
{
	size_t	out;
	size_t	i;
	size_t	text_len;

	if (!buf || bufsize == 0)
		return ;
	if (!text)
		text = "";
	ft_strlcpy(buf, text, bufsize);
	out = ft_strlen(buf);
	text_len = out;
	i = -1;
	while (++i < pad && out + 1 < bufsize)
		buf[out++] = ' ';
	if (out < bufsize)
		buf[out] = '\0';
	else
		buf[bufsize - 1] = '\0';
	(void)text_len;
}

void	pad_right(char *buf, const char *text, size_t pad, size_t bufsize)
{
	size_t	out;
	size_t	i;
	size_t	avail;

	if (!buf || bufsize == 0)
		return ;
	if (!text)
		text = "";
	avail = bufsize;
	out = 0;
	i = 0;
	while (i < pad && out + 1 < avail)
	{
		buf[out++] = ' ';
		i++;
	}
	if (out < avail)
		ft_strlcpy(buf + out, text, avail - out);
	else
		buf[bufsize - 1] = '\0';
}

void	pad_center(char *buf, const char *text, size_t pad, size_t bufsize)
{
	size_t	left;
	size_t	right;
	size_t	out;
	size_t	i;

	if (!buf || bufsize == 0)
		return ;
	if (!text)
		text = "";
	left = pad / 2;
	right = pad - left;
	out = 0;
	i = -1;
	while (++i < left && out + 1 < bufsize)
		buf[out++] = ' ';
	if (out < bufsize)
		ft_strlcpy(buf + out, text, bufsize - out);
	out = ft_strlen(buf);
	i = -1;
	while (++i < right && out + 1 < bufsize)
		buf[out++] = ' ';
	if (out < bufsize)
		buf[out] = '\0';
	else
		buf[bufsize - 1] = '\0';
}
