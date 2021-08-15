#include <stdint.h>

#include "nrf.h"
#include "string.h"
#include "nrfx_twi.h"
#include "queue.h"
#include "nrf_drv_twi.h"
#include "kb_nrf_print.h"

#define MAX_I2C_CHANNEL NRFX_TWI_ENABLED_COUNT

//////////////////////////////////////////////////////////
#include "i2c_master.h"
static uint8_t i2c_address;
static nrfx_twi_t m_twi = {
    .p_twi = NRF_TWI0,
    .drv_inst_idx = NRFX_TWI0_INST_IDX,
};
// nrf adapter layer
void twi_init(uint8_t sda, uint8_t scl);
void twi_uninit();
void kb_nrf_i2c_write(nrfx_twi_t* channel, uint8_t addr, uint8_t len, const uint8_t* data, uint8_t cmd, bool sequence);

void i2c_init(void) {
    static bool is_initialised = false;
    if (!is_initialised) {
        is_initialised = true;
        twi_init(I2C1_SDA, I2C1_SCL);
        if (&m_twi == NULL)
            APP_ERROR_HANDLER(1);
    }
}

// give addr when start
i2c_status_t i2c_start(uint8_t address) {
    i2c_address = address;
    nrfx_twi_enable(&m_twi);
    return I2C_STATUS_SUCCESS;
}


void i2c_stop(void) {
  twi_uninit(m_twi);
}


i2c_status_t i2c_transmit(uint8_t address, const uint8_t* data, uint16_t length, uint16_t timeout) {
    i2c_address = address;
    
    kb_nrf_i2c_write(&m_twi, address, length, data, 0x40, true);
  
    //msg_t status = i2cMasterTransmitTimeout(&I2C_DRIVER, (i2c_address >> 1), data, length, 0, 0, TIME_MS2I(timeout));
    //return chibios_to_qmk(&status);
    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout) {
    i2c_address = address;
    //i2cStart(&I2C_DRIVER, &i2cconfig);
    //msg_t status = i2cMasterReceiveTimeout(&I2C_DRIVER, (i2c_address >> 1), data, length, TIME_MS2I(timeout));
    //return chibios_to_qmk(&status);
    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t* data, uint16_t length, uint16_t timeout) {
    i2c_address = devaddr;
    //i2cStart(&I2C_DRIVER, &i2cconfig);

    //uint8_t complete_packet[length + 1];
    //for (uint8_t i = 0; i < length; i++) {
    //    complete_packet[i + 1] = data[i];
    //}
    //complete_packet[0] = regaddr;

    //msg_t status = i2cMasterTransmitTimeout(&I2C_DRIVER, (i2c_address >> 1), complete_packet, length + 1, 0, 0, TIME_MS2I(timeout));
    //return chibios_to_qmk(&status);

    kb_nrf_i2c_write(&m_twi, i2c_address, length, data, 0x40, true);
    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
    i2c_address = devaddr;
    //i2cStart(&I2C_DRIVER, &i2cconfig);
    //msg_t status = i2cMasterTransmitTimeout(&I2C_DRIVER, (i2c_address >> 1), &regaddr, 1, data, length, TIME_MS2I(timeout));
    //return chibios_to_qmk(&status);
    return I2C_STATUS_SUCCESS;
}


//////////////////////////////////////////////////////////
struct tx_item {
    uint8_t len; // 发送数据长度
    uint8_t index; // 当前数据index
    uint8_t addr; // 目标地址
    uint8_t cmd;
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
QUEUE(struct tx_item, twi_tx_buff, 20);

struct i2c_channel {
    uint8_t sda;
    uint8_t scl;
    nrfx_twi_t twi;
};

static uint8_t buff[2];

static void shared_i2c_tx_item_send(struct tx_item* item)
{
    if (item->flag.sequence) {
        nrfx_twi_tx(item->channel, item->addr, item->flag.is_pointer ? item->data.data_pointer : item->data.data, item->len, false);
        item->index = item->len;
    } else {

        buff[0] = item->cmd;
        if (item->flag.is_pointer) {
            buff[1] = item->data.data_pointer[item->index];
        } else {
            buff[1] = item->data.data[item->index];
        }

        nrfx_twi_tx(item->channel, item->addr, buff, 2, false);
        item->index++;
    }
    // 数据长度一致，标志为已发送
    if (item->index >= item->len)
        item->flag.data_send = true;
}

void kb_nrf_i2c_write(nrfx_twi_t* channel, uint8_t addr, uint8_t len, const uint8_t* data, uint8_t cmd, bool sequence)
{
    kb_nrf_print("i2c bus start %x", addr);
    struct tx_item item = {
        .len = len,
        .index = 0,
        .addr = addr,
        .cmd = cmd,
        .channel = channel
    };

    item.flag.data_send = false;
    item.flag.sequence = sequence;

    // 数据较短，尝试直接复制
    if (len <= 4) {
        item.flag.is_pointer = false;
        memcpy(item.data.data, data, len);
    } else {
        item.flag.is_pointer = true;
        item.data.data_pointer = data;
    }

    twi_tx_buff_push(item);

    if (!nrfx_twi_is_busy(channel)) {
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
          kb_nrf_print("i2c bus evt addr nack");
          break;
    case NRFX_TWI_EVT_DATA_NACK:
    // todo try to resend
          kb_nrf_print("i2c bus evt data nack");
          break;
    case NRFX_TWI_EVT_DONE:
        kb_nrf_print("i2c bus evt done");
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
    case NRFX_TWI_EVT_BUS_ERROR:
    kb_nrf_print("i2c bus error");
      break;
    default:
        break;
    }
}

void twi_init(uint8_t sda, uint8_t scl)
{
    kb_nrf_print("i2c bus init data %d clock %d", sda, scl);
    const nrfx_twi_config_t config = {
        .scl = scl,
        .sda = sda,
        .frequency = NRF_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
    };

    ret_code_t err_code = nrfx_twi_init(&m_twi, &config, shared_i2c_handler, NULL);
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief TWI uninitialization.
 */
void twi_uninit(void)
{
    nrfx_twi_uninit(&m_twi);
}

// device detector
 #define TWI_ADDRESSES      127
void device_detector()
{
    ret_code_t err_code;
    uint8_t address = i2c_address;
    uint8_t sample_data = 'a';
    bool detected_device = false;
    kb_nrf_print("i2c bus start %x", address);
  err_code = nrfx_twi_tx(&m_twi, address, &sample_data, sizeof(sample_data), false);





}