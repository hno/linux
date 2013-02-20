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


/* Use drm_fb_cma_helper 3.7 onwards */
static struct fb_ops drm_fbdev_cma_ops = {
	.owner          = THIS_MODULE,
	.fb_fillrect    = sys_fillrect,
	.fb_copyarea    = sys_copyarea,
	.fb_imageblit   = sys_imageblit,
	.fb_check_var   = drm_fb_helper_check_var,
	.fb_set_par     = drm_fb_helper_set_par,
	.fb_blank       = drm_fb_helper_blank,
	.fb_pan_display = drm_fb_helper_pan_display,
	.fb_setcmap     = drm_fb_helper_setcmap,
};

static int sunxi_fb_find_or_create_single(struct drm_fb_helper *helper,
		struct drm_fb_helper_surface_size *sizes)
{
	int err;
	struct fb_info *fbi;
	struct sunxi_drm_device *sdev = helper->dev->dev_private;
	struct drm_framebuffer *fb = &sdev->fb;

	fbi = framebuffer_alloc(0, helper->dev->dev);
	if (NULL == fbi)
		return -ENOMEM;

	err = fb_alloc_cmap(&fbi->cmap, 256, 0);
	if (0 != err)
		goto error;

	helper->fbdev = fbi;
	helper->fb = fb;

	fbi->par = helper;
	fbi->fbops = &drm_fbdev_cma_ops;
	fbi->flags = FBINFO_FLAG_DEFAULT;

	drm_fb_helper_fill_fix(fbi, fb->pitch, fb->depth);
	drm_fb_helper_fill_var(fbi, helper, fb->width, fb->height);

	return 1; /* negative==error, 0==?? */

error:
	framebuffer_release(fbi);
	return err;
}

static struct drm_fb_helper_funcs sunxi_fb_helper_funcs = {
	.fb_probe = sunxi_fb_find_or_create_single,
};


static void sunxi_user_framebuffer_destroy(struct drm_framebuffer *fb)
{
	drm_framebuffer_cleanup(fb);
}

static int sunxi_user_framebuffer_create_handle(struct drm_framebuffer *fb,
						struct drm_file *file,
						unsigned int *handle)
{
	if (handle)
		*handle = 0;

	pr_info("sunxi_user_framebuffer_create_handle\n");
	return 0;
}

static const struct drm_framebuffer_funcs sunxi_fb_funcs = {
	.destroy = sunxi_user_framebuffer_destroy,
	.create_handle = sunxi_user_framebuffer_create_handle,
};


static struct drm_framebuffer *
sunxi_drm_fb_create(struct drm_device *dev,
		struct drm_file *file_priv,
		struct drm_mode_fb_cmd *mode_cmd)
{
	int err;
	struct sunxi_drm_device *sdev = dev->dev_private;
	pr_info("sunxi_drm_fb_create\n");

	switch (mode_cmd->bpp) {
	case 8:
	case 16:
	case 24:
	case 32:
		break;
	default:
		return NULL;
	}

	pr_info("sunxi_drm_fb_create\n");
	err = drm_framebuffer_init(dev, &sdev->fb, &sunxi_fb_funcs);
	if (err) {
		DRM_ERROR("framebuffer init failed %d\n", err);
		return NULL;
	}

	drm_helper_mode_fill_fb_struct(&sdev->fb, mode_cmd);

	return &sdev->fb;
}

static struct drm_mode_config_funcs sunxi_drm_mode_config_funcs = {
	.fb_create = sunxi_drm_fb_create,
};


int init_framebuffer(struct sunxi_drm_device *sdev)
{
	int err;
	sdev->pdev->mode_config.min_width = 0;
	sdev->pdev->mode_config.min_height = 0;
	sdev->pdev->mode_config.max_width = 4095;
	sdev->pdev->mode_config.max_height = 4095;
	sdev->pdev->mode_config.funcs = &sunxi_drm_mode_config_funcs;

	sdev->fb_helper.funcs = &sunxi_fb_helper_funcs;

	err = drm_fb_helper_init(sdev->pdev, &sdev->fb_helper, 1 , 1);
	if (0 != err)
		return err;

	drm_fb_helper_single_add_all_connectors(&sdev->fb_helper);
	drm_fb_helper_initial_config(&sdev->fb_helper, 32);
	return 0;
}

void destroy_framebuffer(struct sunxi_drm_device *sdev)
{
	unregister_framebuffer(sdev->fb_helper.fbdev);
	drm_fb_helper_fini(&sdev->fb_helper);
}
