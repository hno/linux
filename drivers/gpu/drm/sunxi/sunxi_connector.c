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
#include "sunxi_disp_regs.h"

static int sunxi_drm_connector_get_modes(struct drm_connector *connector)
{
	struct drm_display_mode *mode;
	struct edid *edid;
	int ret = 0;

	mode = drm_mode_create(connector->dev);
	if (mode == NULL)
		return 0;

	edid = drm_get_edid(connector, &sunxi_hdmi_i2c_adapter);
	if (edid) {
		drm_mode_connector_update_edid_property(connector, edid);
		ret = drm_add_edid_modes(connector, edid);
		connector->display_info.raw_edid = NULL;
		kfree(edid);
	}

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
	void __iomem *hdmi_base = (void __iomem *)0xf1c16000;
	if (readl(HDMI_HPD) & 0x1)
		return connector_status_connected;

	return connector_status_disconnected;
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
