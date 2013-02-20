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

static int sunxi_drm_connector_get_modes(struct drm_connector *connector)
{
	/*struct sunxi_drm_device *sdev = connector->dev->dev_private;*/
	struct drm_display_mode *mode;

	mode = drm_mode_create(connector->dev);
	if (mode == NULL)
		return 0;
/*
	mode->type = DRM_MODE_TYPE_PREFERRED | DRM_MODE_TYPE_DRIVER;
	mode->clock = sdev->pdata->panel.mode.clock;
	mode->hdisplay = sdev->pdata->panel.mode.hdisplay;
	mode->hsync_start = sdev->pdata->panel.mode.hsync_start;
	mode->hsync_end = sdev->pdata->panel.mode.hsync_end;
	mode->htotal = sdev->pdata->panel.mode.htotal;
	mode->vdisplay = sdev->pdata->panel.mode.vdisplay;
	mode->vsync_start = sdev->pdata->panel.mode.vsync_start;
	mode->vsync_end = sdev->pdata->panel.mode.vsync_end;
	mode->vtotal = sdev->pdata->panel.mode.vtotal;
	mode->flags = sdev->pdata->panel.mode.flags;

	drm_mode_set_name(mode);
	drm_mode_probed_add(connector, mode);

	connector->display_info.width_mm = sdev->pdata->panel.width_mm;
	connector->display_info.height_mm = sdev->pdata->panel.height_mm;
*/
	return 1;
}

static int sunxi_drm_connector_mode_valid(struct drm_connector *connector,
		struct drm_display_mode *mode)
{
	return MODE_OK;
}

static struct drm_encoder *
sunxi_drm_connector_best_encoder(struct drm_connector *connector)
{
	return connector->encoder;
}


static const struct drm_connector_helper_funcs connector_helper_funcs = {
	.get_modes = sunxi_drm_connector_get_modes,
	.mode_valid = sunxi_drm_connector_mode_valid,
	.best_encoder = sunxi_drm_connector_best_encoder,
};

static void sunxi_drm_connector_destroy(struct drm_connector *connector)
{
	drm_sysfs_connector_remove(connector);
	drm_connector_cleanup(connector);
}

static enum drm_connector_status
sunxi_drm_connector_detect(struct drm_connector *connector, bool force)
{
	return connector_status_connected;
}

static const struct drm_connector_funcs connector_funcs = {
	.dpms = drm_helper_connector_dpms,
	.detect = sunxi_drm_connector_detect,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = sunxi_drm_connector_destroy,
};

int init_connector(struct sunxi_drm_device *dev, struct drm_encoder *encoder)
{
	int ret = 0;

	drm_connector_init(dev->pdev, &dev->connector,
			&connector_funcs, DRM_MODE_CONNECTOR_HDMIA);
	if (ret != 0)
		return ret;

	drm_connector_helper_add(&dev->connector, &connector_helper_funcs);

	ret = drm_sysfs_connector_add(&dev->connector);
	if (ret != 0)
		return ret;


	ret = drm_mode_connector_attach_encoder(&dev->connector, encoder);
	dev->connector.encoder = encoder;

	return ret;
}

void destroy_connector(struct sunxi_drm_device *dev)
{

}
