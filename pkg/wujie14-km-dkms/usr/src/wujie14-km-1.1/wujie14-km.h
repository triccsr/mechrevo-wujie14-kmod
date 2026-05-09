#ifndef WUJIE14_KM_H
#define WUJIE14_KM_H

#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#if !defined(NO_PLATFORM_PROFILE)
#include <linux/platform_profile.h>
#endif

static const struct dmi_system_id wujie14_allowlist[] = {
    {
        .ident = "WUJIE 14",
        .matches = {
            DMI_MATCH(DMI_SYS_VENDOR, "MECHREVO"),
            DMI_MATCH(DMI_PRODUCT_NAME, "WUJIE 14"),
            DMI_MATCH(DMI_PRODUCT_SKU, "WUJIE 14")
        }
    }
};


struct wujie14_private{
    struct platform_device* pdev;
#if !defined(NO_PLATFORM_PROFILE)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 14, 0)
    struct device* ppdev;
#else
    struct platform_profile_handler pphandler;
#endif
#endif
};

int wujie_plat_probe(struct platform_device *pdev);
void wujie_plat_remove(struct platform_device *pdev);

#endif
