

#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H
#include <stdbool.h>

    void log_init(void);
    void timers_init(void);
    void buttons_leds_init(bool * p_erase_bonds);
    void power_management_init(void);
    void ble_stack_init(void);
    void scheduler_init(void);
    void gap_params_init(void);
    void gatt_init(void);
    void advertising_init(void);
    void services_init(void);
    void sensor_simulator_init(void);
    void conn_params_init(void);
    void buffer_init(void);
    void peer_manager_init(void);

    // Start execution.
    void timers_start(void);
    void advertising_start(bool erase_bonds);


    void keys_send(uint8_t key_pattern_len, uint8_t * p_key_pattern);
#endif