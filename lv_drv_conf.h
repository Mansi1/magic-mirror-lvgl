#ifndef LV_DRV_CONF_H
#define LV_DRV_CONF_H

#include "lv_conf.h"

#if 1 

#ifndef USE_DRM
#  define USE_DRM      1
#endif

#if USE_DRM
#  define DRM_CARD          "/dev/dri/card0"
#  define DRM_CONNECTOR_ID  33
#  define DRM_DEPTH         32
#  define DRM_FORMAT        DRM_FORMAT_XRGB8888
#endif

#define USE_FBDEV      0
#define USE_SDL        0

#endif 
#endif
