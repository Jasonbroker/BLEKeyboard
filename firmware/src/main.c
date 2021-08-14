/**
 * Copyright (c) 2012 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup ble_sdk_app_hids_keyboard_main main.c
 * @{
 * @ingroup ble_sdk_app_hids_keyboard
 * @brief HID Keyboard Sample Application main file.
 *
 * This file contains is the source code for a sample application using the HID, Battery and Device
 * Information Services for implementing a simple keyboard functionality.
 * Pressing Button 0 will send text 'hello' to the connected peer. On receiving output report,
 * it toggles the state of LED 2 on the mother board based on whether or not Caps Lock is on.
 * This application uses the @ref app_scheduler.
 *
 * Also it would accept pairing requests from any peer device.
 */


/**@brief Function for application main entry.
 */

#include <stdint.h>
#include "ble_service.h"

#include "log/kb_nrf_print.h"
#include "nrf_log_ctrl.h"
#include "app_scheduler.h"
#include "nrf_pwr_mgmt.h"
#include "app_timer.h"
#include "kb_nrf_driver.h"
#include "keyboard.h"
#include "powermgr/powermgr.h"
#include "kb_rgb.h"

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
void idle_state_handle(void)
{
    app_sched_execute();
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
void timers_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing power management.
 */
void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

static void keyboard_scan_handler(void* p_context)
{
    keyboard_task();
}

APP_TIMER_DEF(m_keyboard_scan_timer_id);

void init_and_start_scan_timer(void)
{
    ret_code_t err_code = app_timer_create(&m_keyboard_scan_timer_id,
        APP_TIMER_MODE_REPEATED,
        keyboard_scan_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_keyboard_scan_timer_id,  APP_TIMER_TICKS(10), NULL);
    APP_ERROR_CHECK(err_code);

}




int main(void)
{
    bool erase_bonds = false;

    // Initialize.
    log_init();

    timers_init();
    scheduler_init();

    power_management_init();

    ble_stack_init();
    gap_params_init();
    gatt_init();
    advertising_init();
    services_init();

    conn_params_init();
    buffer_init();
    peer_manager_init();


    // Start execution.
    kb_nrf_print("HID Keyboard example started.");

    advertising_start(erase_bonds);

    keyboard_init();
    host_set_driver(&kb_nrf_driver);
    init_and_start_scan_timer();
    kb_power_mgr_init();
    kb_power_mgr_start();

    // 启动rgb灯效
    rgb3741_twi_init();

    // Enter main loop.
    for (;;)
    {
        idle_state_handle();
    }
}


/**
 * @}
 */
