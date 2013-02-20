/*
 * Copyright (c) 2013 Jari Helaakoski <tekkuli@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */


#include "sunxi_drv.h"



static void sunxi_drm_crtc_dpms(struct drm_crtc *crtc, int mode)
{
	switch (mode) {
	case DRM_MODE_DPMS_ON:
		break;
	case DRM_MODE_DPMS_STANDBY:
	case DRM_MODE_DPMS_SUSPEND:
	case DRM_MODE_DPMS_OFF:
		break;
	}
}

static int sunxi_drm_crtc_mode_set_base(struct drm_crtc *crtc, int x, int y,
			   struct drm_framebuffer *old_fb)
{
	return 0;
}

static int sunxi_drm_crtc_mode_set(struct drm_crtc *crtc,
			   struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode,
			   int x, int y, struct drm_framebuffer *old_fb)
{
	return 0;
}

static bool sunxi_drm_crtc_mode_fixup(struct drm_crtc *crtc,
				     struct drm_display_mode *mode,
				     struct drm_display_mode *adjusted_mode)
{
	return true;
}

static void sunxi_drm_crtc_mode_prepare(struct drm_crtc *crtc)
{

}

static void sunxi_drm_crtc_mode_commit(struct drm_crtc *crtc)
{

}

static void sunxi_crtc_disable(struct drm_crtc *crtc)
{

}


static const struct drm_crtc_helper_funcs crtc_helper_funcs = {
	.dpms = sunxi_drm_crtc_dpms,
	.mode_fixup = sunxi_drm_crtc_mode_fixup,
	.prepare = sunxi_drm_crtc_mode_prepare,
	.commit = sunxi_drm_crtc_mode_commit,
	.mode_set = sunxi_drm_crtc_mode_set,
	.mode_set_base = sunxi_drm_crtc_mode_set_base,
	.disable = sunxi_crtc_disable,
};


static int sunxi_drm_crtc_page_flip(
		struct drm_crtc *crtc,
		struct drm_framebuffer *fb,
		struct drm_pending_vblank_event *event)
{
	return 0;
}

static const struct drm_crtc_funcs crtc_funcs = {
		.destroy = drm_crtc_cleanup,
		.set_config = drm_crtc_helper_set_config,
		.page_flip = sunxi_drm_crtc_page_flip,
};


int init_crtc(struct sunxi_drm_device *dev)
{
	int ret = 0;

	drm_crtc_init(dev->pdev, &dev->crtc, &crtc_funcs);
	if (ret != 0)
		return ret;

	drm_crtc_helper_add(&dev->crtc, &crtc_helper_funcs);
	return ret;
}


void destroy_crtc(struct sunxi_drm_device *dev)
{

}
