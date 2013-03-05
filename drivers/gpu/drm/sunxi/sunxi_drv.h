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

#ifndef _SUNXI_DRV_H_
#define _SUNXI_DRV_H_

#define DRIVER_NAME		"sunxi_drm"
#define DRIVER_DESC		"DRM module for Sunxi devices"
#define DRIVER_ALIAS    "platform:sunxi_drm"
#define DRIVER_DATE		"20130902"
#define DRIVER_VERSION  "0.1"
#define DRIVER_MAJOR 0
#define DRIVER_MINOR 1
#define DRIVER_PATCHLEVEL 0

#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_helper.h>
#include <linux/i2c.h>

struct sunxi_drm_device {
	struct drm_device *pdev;

	struct drm_crtc crtc;
	struct drm_encoder encoder;
	struct drm_connector connector;
	struct drm_framebuffer fb;
	struct drm_fb_helper fb_helper;
};

extern struct i2c_adapter sunxi_hdmi_i2c_adapter;

int init_crtc(struct sunxi_drm_device *sdev);
void destroy_crtc(struct sunxi_drm_device *sdev);

int hdmi_i2c_sunxi_probe(struct platform_device *dev);
int hdmi_i2c_sunxi_remove(struct platform_device *dev);

int init_connector(struct sunxi_drm_device *sdev, struct drm_encoder *encoder);
void destroy_connector(struct sunxi_drm_device *sdev);

int init_encoder(struct sunxi_drm_device *sdev);
void destroy_encoder(struct sunxi_drm_device *sdev);
int init_framebuffer(struct sunxi_drm_device *sdev);
void destroy_framebuffer(struct sunxi_drm_device *sdev);

#endif /* _SUNXI_DRV_H_ */
