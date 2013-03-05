#
# * Copyright (c) 2013 Jari Helaakoski <tekkuli@gmail.com>
# *
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 2 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful, but WITHOUT
# * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# * more details.
#

ccflags-y = -Iinclude/drm

sunxi_drm-y := sunxi_drv.o \
	sunxi_connector.o \
	sunxi_crtc.o \
	sunxi_encoder.o \
	sunxi_fb.o \
	sunxi_hdmi_i2c.o

obj-$(CONFIG_DRM_SUNXI)   += sunxi_drm.o


