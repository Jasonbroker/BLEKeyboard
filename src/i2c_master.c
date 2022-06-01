#include <stdint.h>

#include "nrf.h"
#include "string.h"
#include "nrfx_twi.h"
#include "queue.h"
#include "print.h"

static const nrfx_twi_t m_twi_master = NRFX_TWI_INSTANCE(0);

struct tx_item {
    uint16_t len; // 发送数据长度
    uint8_t index; // 当前数据index
    uint8_t addr; // 目标地址
    struct {
        bool is_pointer : 1; // 数据位是否是指针
        bool data_send : 1; // 数据是否发送完毕
        bool sequence : 1; // 是否以连续方式发送，若否则
    } flag;
    union data_or_pointer {
        const uint8_t* data_pointer; // 数据指针
        uint8_t data[4]; // 数据本体
    } data;
    nrfx_twi_t* channel; // 使用的物理通道
};
QUEUE(struct tx_item, twi_tx_buff, 30);

static void shared_i2c_tx_item_send(struct tx_item* item)
{

    nrfx_twi_tx(&m_twi_master, item->addr, item->flag.is_pointer ? item->data.data_pointer : item->data.data, item->len, false);
    printf("send cmd %x", item->data.data[1]);
    item->index = item->len;

    // 数据长度一致，标志为已发送
    if (item->index >= item->len)
        item->flag.data_send = true;
}

void i2c_transmit(uint8_t addr, const uint8_t* data, uint8_t len, bool sequence)
{
    struct tx_item item = {
        .len = len,
        .index = 0,
        .addr = addr,
    };

    item.flag.data_send = false;

    // 数据较短，尝试直接复制
    if (len <= 4) {
        item.flag.is_pointer = false;
        memcpy(item.data.data, data, len);
    } else {
        item.flag.is_pointer = true;
        item.data.data_pointer = data;
    }

    twi_tx_buff_push(item);

    if (!nrfx_twi_is_busy(&m_twi_master)) {
        // 如果当前等待队列为空，则尝试发送
        shared_i2c_tx_item_send(twi_tx_buff_peek());
    }
}

/**
 * @brief 事件处理
 * 
 * @param p_event 
 * @param p_context 
 */
static void shared_i2c_handler(nrfx_twi_evt_t const* p_event, void* p_context)
{
    switch (p_event->type) {
    case NRFX_TWI_EVT_ADDRESS_NACK:
        printf("add nack");
    case NRFX_TWI_EVT_DATA_NACK:
        printf("evt nack");
    case NRFX_TWI_EVT_DONE:
        printf("evt done nack %d", p_event->xfer_desc.type);
        if (p_event->xfer_desc.type == NRFX_TWI_XFER_TX) {
            // 当前队列不空
            if (!twi_tx_buff_empty()) {
                struct tx_item* item = twi_tx_buff_peek();
                // 如果这个是已经发送过的
                if (item->flag.data_send) {
                    // 就出队列
                    twi_tx_buff_pop();
                    // 然后尝试发送下一个
                    if (!twi_tx_buff_empty()) {
                        shared_i2c_tx_item_send(twi_tx_buff_peek());
                    }
                } else {
                    // 不然就尝试发送
                    shared_i2c_tx_item_send(item);
                }
            }
        }
        break;
    default:
        break;
    }
}

int i2c_init(uint8_t sda, uint8_t scl)
{
    // 找不到就新建一个
    nrfx_twi_config_t config = {
        .scl = scl,
        .sda = sda,
        .frequency = NRF_TWI_FREQ_400K,
        .interrupt_priority = APP_IRQ_PRIORITY_LOW
    };

    uint32_t err_code = nrfx_twi_init(&m_twi_master, &config, shared_i2c_handler, NULL);
    if (err_code != NRF_SUCCESS)
        return 0;

    nrfx_twi_enable(&m_twi_master);
    return 1;
}