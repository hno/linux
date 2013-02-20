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
#include <drm/drm_crtc_helper.h>

/*
static const struct drm_encoder_helper_funcs encoder_helper_funcs = {
	.dpms = sunxi_drm_encoder_dpms,
	.mode_fixup = sunxi_drm_encoder_mode_fixup,
	.prepare = sunxi_drm_encoder_mode_prepare,
	.commit = sunxi_drm_encoder_mode_commit,
	.mode_set = sunxi_drm_encoder_mode_set,
};
*/

static void sunxi_drm_encoder_destroy(struct drm_encoder *encoder)
{
	drm_encoder_cleanup(encoder);
}

static const struct drm_encoder_funcs encoder_funcs = {
	.destroy = sunxi_drm_encoder_destroy,
};

int init_encoder(struct sunxi_drm_device *dev)
{
	int ret = 0;

	/* TCON1 */
	drm_encoder_init(dev->pdev, &dev->encoder,
			&encoder_funcs, DRM_MODE_ENCODER_TMDS);
	if (ret != 0)
		return ret;


	/*drm_encoder_helper_add(dev->encoder, &encoder_helper_funcs);*/

	dev->encoder.possible_crtcs = 1;

	return ret;
}


void destroy_encoder(struct sunxi_drm_device *dev)
{

}

