/**
 * @file    app_ui.h
 * @brief   Minimal Astra UI bring-up wrapper.
 */

#ifndef _APP_UI_H_
#define _APP_UI_H_

#include "bsp_key.h"

void app_ui_init(void);
void app_ui_task(void);
void app_ui_handle_key(bsp_key_id_enum key, uint8 pressed, bsp_key_event_enum event, uint32 now_ms);

#endif /* _APP_UI_H_ */
