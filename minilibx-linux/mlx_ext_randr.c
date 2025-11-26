/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_ext_randr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:33:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 15:43:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_int.h"
#include <unistd.h>
#include <stdio.h>
#include <X11/extensions/Xrandr.h>

static RRMode *saved_mode_singleton(void)
{
  static RRMode mode = 0;
  return (&mode);
}

static RROutput find_connected_output(Display *display, XRRScreenResources *res,
                                      XRROutputInfo **out_info)
{
  int i;

  *out_info = NULL;
  i = res->noutput;
  while (i--)
  {
    *out_info = XRRGetOutputInfo(display, res, res->outputs[i]);
    if ((*out_info)->connection == RR_Connected)
      return (res->outputs[i]);
    XRRFreeOutputInfo(*out_info);
  }
  return (0);
}

/* internal context grouping parameters for the helper */
typedef struct s_randr_ctx
{
  t_xvar *xvar;
  t_win_list *win;
  XRRScreenResources *res;
  XRROutputInfo *o_info;
  XID output_xid;
  int fullscreen;
  int watt_w;
  int watt_h;
} t_randr_ctx;

static int select_best_mode(t_randr_ctx *ctx)
{
  int i;
  int j;
  int best = -1;

  i = ctx->o_info->nmode;
  while (i--)
  {
    j = ctx->res->nmode;
    while (j--)
    {
      if (ctx->res->modes[j].id == ctx->o_info->modes[i])
        if (ctx->res->modes[j].width >= ctx->watt_w && ctx->res->modes[j].height >= ctx->watt_h &&
            (best == -1 || ctx->res->modes[best].width > ctx->res->modes[j].width ||
             ctx->res->modes[best].height > ctx->res->modes[j].height))
          best = j;
    }
  }
  return best;
}

static int perform_apply_crtc(t_randr_ctx *ctx, RRMode mode_candidate, int best)
{
  RRMode *saved_mode = saved_mode_singleton();
  XRRCrtcInfo *crtc;

  crtc = XRRGetCrtcInfo(ctx->xvar->display, ctx->res, ctx->o_info->crtc);
  if (!crtc)
    return (-1);
  *saved_mode = crtc->mode;
  XRRSetCrtcConfig(ctx->xvar->display, ctx->res, ctx->o_info->crtc, CurrentTime, 0, 0,
                  mode_candidate, crtc->rotation, &ctx->output_xid, 1);
  if (ctx->fullscreen)
    printf("found mode : %d x %d\n Status %d\n", ctx->res->modes[best].width, ctx->res->modes[best].height, 0);
  else
    printf("back previous mode\n");
  XMoveWindow(ctx->xvar->display, ctx->win->window, 0, 0);
  XMapRaised(ctx->xvar->display, ctx->win->window);
  if (ctx->fullscreen)
    XGrabKeyboard(ctx->xvar->display, ctx->win->window, False, GrabModeAsync, GrabModeAsync, CurrentTime);
  else
  {
    XUngrabPointer(ctx->xvar->display, CurrentTime);
    XUngrabKeyboard(ctx->xvar->display, CurrentTime);
  }
  XSync(ctx->xvar->display, False);
  sleep(1);
  XRRFreeCrtcInfo(crtc);
  return (0);
}

static int apply_mode(t_randr_ctx *ctx, int best)
{
  RRMode mode_candidate;
  RRMode *saved_mode;

  if (best < 0)
    return (-1);
  mode_candidate = ctx->o_info->modes[best];
  saved_mode = saved_mode_singleton();
  if (!ctx->fullscreen && *saved_mode == -1)
    mode_candidate = ctx->o_info->modes[0];
  if (!ctx->fullscreen)
    mode_candidate = *saved_mode;
  return (perform_apply_crtc(ctx, mode_candidate, best));
}

static int prepare_and_apply_mode(t_randr_ctx *ctx)
{
  int best;

  best = select_best_mode(ctx);
  if (best < 0)
    return (-1);
  return (apply_mode(ctx, best));
}

int mlx_ext_fullscreen(t_xvar *xvar, t_win_list *win, int fullscreen)
{
  XWindowAttributes watt;
  XRRScreenResources *res;
  XRROutputInfo *o_info;
  XID output_xid;

  if (!XGetWindowAttributes(xvar->display, win->window, &watt))
    return (0);
  res = XRRGetScreenResources(xvar->display, xvar->root);
  if (!res)
    return (0);
  output_xid = find_connected_output(xvar->display, res, &o_info);
  if (!output_xid)
  {
    XRRFreeScreenResources(res);
    return (0);
  }
  {
    t_randr_ctx ctx = {xvar, win, res, o_info, output_xid, fullscreen, watt.width, watt.height};
    if (prepare_and_apply_mode(&ctx) < 0)
    {
      XRRFreeOutputInfo(o_info);
      XRRFreeScreenResources(res);
      return (0);
    }
  }
  XRRFreeOutputInfo(o_info);
  XRRFreeScreenResources(res);
  return (0);
}
