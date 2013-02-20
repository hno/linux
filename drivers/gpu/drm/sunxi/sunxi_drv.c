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

/**
 * @file sunxi_drv.c
 * Implementation of the Linux device driver entrypoints for sunxi DRM
 */
#include <linux/vermagic.h>
#include "sunxi_drv.h"

static struct platform_device *dev0;

static void sunxi_drm_preclose(struct drm_device *dev,
		struct drm_file *file_priv)
{
}

static void sunxi_drm_lastclose(struct drm_device *dev)
{
}

static int sunxi_drm_suspend(struct drm_device *dev, pm_message_t state)
{
	return 0;
}

static int sunxi_drm_resume(struct drm_device *dev)
{
	return 0;
}

static int sunxi_drm_load(struct drm_device *dev, unsigned long chipset)
{
	int err = 0;
	struct sunxi_drm_device *sdev;
	sdev = kzalloc(sizeof(struct sunxi_drm_device), GFP_KERNEL);
	if (sdev == NULL)
		return -ENOMEM;

	dev->dev_private = sdev;
	sdev->pdev = dev;

	drm_mode_config_init(dev);

	err = init_crtc(sdev);
	if (err != 0)
		goto error_handler;

	err = init_encoder(sdev);
	if (err != 0)
		goto error_handler;

	err = init_connector(sdev, &sdev->encoder);
	if (err != 0)
		goto error_handler;

	err = init_framebuffer(sdev);
	if (err != 0)
		goto error_handler;

	/*drm_helper_disable_unused_functions(dev);*/

	pr_info("sunxi_drm load success!\n");
	return 0;

error_handler:
	kfree(sdev);
	return err;
}

static int sunxi_drm_unload(struct drm_device *dev)
{
	struct sunxi_drm_device *sdev = dev->dev_private;
	destroy_framebuffer(sdev);
	destroy_connector(sdev);
	destroy_encoder(sdev);
	destroy_crtc(sdev);
	drm_mode_config_cleanup(dev);

	kfree(sdev);
	pr_info("sunxi_drm unload success!\n");
	return 0;
}

static struct drm_driver driver = {
	.driver_features = DRIVER_BUS_PLATFORM | DRIVER_MODESET,
	.load = sunxi_drm_load,
	.unload = sunxi_drm_unload,
	.context_dtor = NULL,
	.reclaim_buffers = NULL,
	.reclaim_buffers_idlelocked = NULL,
	.preclose = sunxi_drm_preclose,
	.lastclose = sunxi_drm_lastclose,
	.suspend = sunxi_drm_suspend,
	.resume = sunxi_drm_resume,
	.ioctls = NULL,
	.fops = {
		 .owner = THIS_MODULE,
		 .open = drm_open,
		 .release = drm_release,
		 .unlocked_ioctl = drm_ioctl,
		 .mmap = drm_mmap,
		 .poll = drm_poll,
		 .fasync = drm_fasync,
	},
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

static int __devinit sunxi_platform_drm_probe(struct platform_device *dev)
{
	return drm_platform_init(&driver, dev);
}

static int sunxi_platform_drm_remove(struct platform_device *dev)
{
	drm_platform_exit(&driver, dev);
	return 0;
}

static int sunxi_platform_drm_suspend(struct platform_device *dev,
		pm_message_t state)
{
	return 0;
}

static int sunxi_platform_drm_resume(struct platform_device *dev)
{
	return 0;
}


static struct platform_driver platform_drm_driver = {
	.probe = sunxi_platform_drm_probe,
	.remove = __devexit_p(sunxi_platform_drm_remove),
	.suspend = sunxi_platform_drm_suspend,
	.resume = sunxi_platform_drm_resume,
	.driver = {
		.owner = THIS_MODULE,
		.name = DRIVER_NAME,
	},
};

static int __init sunxi_platform_drm_init(void)
{
	dev0 = platform_device_register_simple("sunxi_drm", 0, NULL, 0);
	return platform_driver_register(&platform_drm_driver);
}

static void __exit sunxi_platform_drm_exit(void)
{
	platform_driver_unregister(&platform_drm_driver);
	platform_device_unregister(dev0);
}

#ifdef MODULE
module_init(sunxi_platform_drm_init);
#else
late_initcall(sunxi_platform_drm_init);
#endif
module_exit(sunxi_platform_drm_exit);

MODULE_DESCRIPTION("DRM module for sunxi platform");
MODULE_AUTHOR("Jari Helaakoski <tekkuli@gmail.com>");
MODULE_LICENSE("GPL");

