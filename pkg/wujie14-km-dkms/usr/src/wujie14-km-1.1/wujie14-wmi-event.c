#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/acpi.h>
#include <linux/wmi.h>
#include <linux/version.h>

#include "wujie14-km.h"
#include "wujie14-wmi-event.h"
#include "wujie14-perfmode.h"

static const struct wujie14_wmi_notify_handler_entry handler_tbl[] = {
    {
        .febc_offset = 1,
        .febc_value = 0x11,
        .handler = wujie14_powermode_wmi_event_handler
    },
    {
        .febc_offset = 1,
        .febc_value = 0x12,
        .handler = wujie14_powermode_wmi_event_handler
    },
    {
        .febc_offset = 1,
        .febc_value = 0x13,
        .handler = wujie14_powermode_wmi_event_handler
    },
    {}
};

static inline wujie14_wmi_notify_handler locate_handler(
    u8* response_buffer
){
    const struct wujie14_wmi_notify_handler_entry* cur;
    wujie14_wmi_notify_handler ret_handler = NULL;
    cur = handler_tbl;
    while(cur->febc_offset){
        if (response_buffer[cur->febc_offset] == cur->febc_value){
            ret_handler = cur->handler;
            break;
        }
        cur++;
    }
    return ret_handler;
}

static void wujie14_handle_wmi_event_data(
    union acpi_object* obj,
    struct wujie14_private* priv
)
{
    wujie14_wmi_notify_handler handler;

    if (!obj || obj->type != ACPI_TYPE_BUFFER) {
        dev_err(&priv->pdev->dev, "unexpected WMI event data\n");
        return;
    }

    handler = locate_handler(obj->buffer.pointer);
    if (handler == NULL){
        return;
    }
    handler(priv); 
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 12, 0)
static void wujie14_wmi_event_handler(
    union acpi_object* data,
    void* context
)
{
    struct wujie14_private* priv = context;

    wujie14_handle_wmi_event_data(data, priv);
}
#else
static void wujie14_wmi_event_handler(
    u32 value,
    void* context
)
{
    struct acpi_buffer response = {ACPI_ALLOCATE_BUFFER, NULL};
    acpi_status status;
    struct wujie14_private* priv = context;

    status = wmi_get_event_data(value, &response);
    if (ACPI_FAILURE(status)) {
        dev_err(&priv->pdev->dev, "get event data failed\n");
        return;
    }

    wujie14_handle_wmi_event_data(response.pointer, priv);
    ACPI_FREE(response.pointer);
}
#endif

int wujie14_wmi_event_init(struct wujie14_private* priv)
{
    acpi_status status;
    status = wmi_install_notify_handler(
        WUJIE14_IP3WMIEVENT_GUID,
        wujie14_wmi_event_handler,
        priv);
    if (ACPI_FAILURE(status)){
        dev_err(&priv->pdev->dev, "get event data failed\n");
        return -ENODEV;
    }
    return 0;
}

void wujie14_wmi_event_exit(struct wujie14_private* priv)
{
    wmi_remove_notify_handler(WUJIE14_IP3WMIEVENT_GUID);
}
