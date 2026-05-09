#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dmi.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/version.h>


#include "wujie14-km.h"
#include "wujie14-perfmode.h"
#include "wujie14-wmi-event.h"
#include "wujie14-kb.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Xu");
MODULE_DESCRIPTION("mechrevo wujie 14 driver km");

struct wujie14_private priv;
static struct platform_device *wujie14_pdev;


int wujie_plat_probe(struct platform_device *pdev)
{
	const struct dmi_system_id *dmi_match;	
	int err;
	priv.pdev = pdev;
	// do dmi match first
	dmi_match = dmi_first_match(wujie14_allowlist);
	if (dmi_match == NULL) {
		dev_err(&pdev->dev, "Unsupported laptop model.\n");
		return -EINVAL;
	}
	err = wujie14_platform_profile_init(&priv);
	if (err) {
		dev_err(&pdev->dev, "Platform profile initialization failed.\n");
		return err;
	}
	err = wujie14_powermode_sysfs_init(&priv);
	if (err) {
		dev_err(&pdev->dev, "Powermode sysfs initialization failed.\n");
		return err;
	}
	err = wujie14_wmi_event_init(&priv);
	if (err) {
		return err;
	}
	err = wujie14_kbd_sysfs_init(&priv);
	if (err) {
		return err;
	}

	dev_info(&pdev->dev, "WUJIE14 driver initialized\n");
	return 0;
}

void wujie_plat_remove(struct platform_device *pdev)
{
	wujie14_platform_profile_exit(&priv);
	wujie14_powermode_sysfs_exit(&priv);
	wujie14_wmi_event_exit(&priv);
	wujie14_kbd_sysfs_exit(&priv);
}


static struct platform_driver wujie14_platdriver = {
	.probe = wujie_plat_probe,
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 11, 0)
	.remove_new = wujie_plat_remove,
#else
	.remove = wujie_plat_remove,
#endif
	.driver = {
		.name   = "wujie14",
	},
};

static int __init wujie14_init(void)
{
	int err;
	err = platform_driver_register(&wujie14_platdriver);
	if (err) {
		return err;
	}
	wujie14_pdev = platform_device_register_simple("wujie14", PLATFORM_DEVID_NONE, NULL, 0);
	if (IS_ERR(wujie14_pdev)) {
		err = PTR_ERR(wujie14_pdev);
		platform_driver_unregister(&wujie14_platdriver);
		return err;
	}
	return err;
	
}

static void __exit wujie14_exit(void)
{
	platform_device_unregister(wujie14_pdev);
	platform_driver_unregister(&wujie14_platdriver);
}

module_init(wujie14_init);
module_exit(wujie14_exit);
