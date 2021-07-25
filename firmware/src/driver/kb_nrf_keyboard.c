#include "kb_nrf_keyboard.h"
#include "ble_service.h"
#include "kb_nrf_print.h"

bool kb_send_report(report_keyboard_t *report) {
    kb_nrf_print("report %x", report->raw);
    keys_send(8, report->raw);
    return true;
}
