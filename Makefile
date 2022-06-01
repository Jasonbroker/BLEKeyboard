
PROJECT_NAME     := ble_keyboard
MCU = 52832
TARGETS := nrf$(MCU)_xxaa
OUTPUT_DIRECTORY := .build
debug := yes
ifeq ($(strip $(debug)), yes)
bl_suffix = _nobl
endif

ifeq ($(strip $(MCU)), 52810)
my_softdevice = s112
else # ifeq ($(strip $(MCU)), 52832) 默认832
my_softdevice = s132
endif
softdevice_hex = $(SDK_ROOT)/components/softdevice/$(my_softdevice)/hex/$(my_softdevice)_nrf52_7.2.0_softdevice.hex

hex_name = nrf$(MCU)_xxaa.hex

SDK_ROOT := ../nRF5_SDK_17.1.0

ifeq ($(strip $(debug)), yes) 
SRC_FILES += \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
		$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c

	CFLAGS += -DDEBUG
	CFLAGS += -DDEBUG_NRF
	CFLAGS += -DNRF_LOG_BACKEND_RTT_ENABLED
	CFLAGS += -DNRF_FPRINTF_ENABLED
	CFLAGS += -DNRF_LOG_ENABLED=1
	CFLAGS += -DNRF_LOG_DEFAULT_LEVEL=4

	ASMFLAGS += -DDEBUG
	ASMFLAGS += -DDEBUG_NRF
	ASMFLAGS += -DNRF_LOG_BACKEND_RTT_ENABLED
	ASMFLAGS += -DNRF_FPRINTF_ENABLED
	ASMFLAGS += -DNRF_LOG_ENABLED=1
	ASMFLAGS += -DNRF_LOG_DEFAULT_LEVEL=4

endif



PROJ_DIR := src

# Source files common to all targets
SRC_FILES += \
  $(SDK_ROOT)/components/libraries/util/app_error.c \
  $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
  $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
  $(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
  $(SDK_ROOT)/components/libraries/timer/app_timer2.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/libraries/crc16/crc16.c \
  $(SDK_ROOT)/components/libraries/timer/drv_rtc.c \
  $(SDK_ROOT)/components/libraries/fds/fds.c \
  $(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
  $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
  $(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
  $(SDK_ROOT)/components/libraries/atomic_flags/nrf_atflags.c \
  $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
  $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
  $(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage.c \
  $(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage_sd.c \
  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
  $(SDK_ROOT)/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
  $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
  $(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
  $(SDK_ROOT)/components/libraries/sortlist/nrf_sortlist.c \
  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
  $(SDK_ROOT)/components/boards/boards.c \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c \
  $(SDK_ROOT)/modules/nrfx/soc/nrfx_atomic.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c \
  $(SDK_ROOT)/components/ble/peer_manager/auth_status_tracker.c \
  $(SDK_ROOT)/components/ble/common/ble_advdata.c \
  $(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
  $(SDK_ROOT)/components/ble/common/ble_conn_params.c \
  $(SDK_ROOT)/components/ble/common/ble_conn_state.c \
  $(SDK_ROOT)/components/ble/ble_link_ctx_manager/ble_link_ctx_manager.c \
  $(SDK_ROOT)/components/ble/common/ble_srv_common.c \
  $(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c \
  $(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c \
  $(SDK_ROOT)/components/ble/peer_manager/id_manager.c \
  $(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
  $(SDK_ROOT)/components/ble/nrf_ble_qwr/nrf_ble_qwr.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_database.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_id.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_manager.c \
  $(SDK_ROOT)/components/ble/peer_manager/peer_manager_handler.c \
  $(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c \
  $(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c \
  $(SDK_ROOT)/components/ble/peer_manager/security_manager.c \
  $(SDK_ROOT)/external/utf_converter/utf.c \
  $(SDK_ROOT)/components/ble/ble_services/ble_bas/ble_bas.c \
  $(SDK_ROOT)/components/ble/ble_services/ble_dis/ble_dis.c \
  $(SDK_ROOT)/components/ble/ble_services/ble_hids/ble_hids.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_ble.c \
  $(SDK_ROOT)/components/softdevice/common/nrf_sdh_soc.c \
  $(SDK_ROOT)/components/libraries/uart/retarget.c \
  $(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
  $(SDK_ROOT)/components/libraries/uart/app_uart_fifo.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_saadc.c \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/print_internal.c

INC_FOLDERS += \
  $(SDK_ROOT)/components/ble/ble_services/ble_ancs_c \
  $(SDK_ROOT)/components/ble/ble_services/ble_ias_c \
  $(SDK_ROOT)/components/libraries/pwm \
  $(SDK_ROOT)/components/libraries/usbd/class/cdc/acm \
  $(SDK_ROOT)/components/libraries/usbd/class/hid/generic \
  $(SDK_ROOT)/components/libraries/usbd/class/msc \
  $(SDK_ROOT)/components/libraries/usbd/class/hid \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/components/ble/ble_services/ble_gls \
  $(SDK_ROOT)/components/libraries/fstorage \
  $(SDK_ROOT)/components/libraries/mutex \
  $(SDK_ROOT)/components/libraries/gpiote \
  $(SDK_ROOT)/components/libraries/bootloader/ble_dfu \
  $(SDK_ROOT)/components/boards \
  $(SDK_ROOT)/components/ble/ble_advertising \
  $(SDK_ROOT)/external/utf_converter \
  $(SDK_ROOT)/components/ble/ble_services/ble_bas_c \
  $(SDK_ROOT)/modules/nrfx/drivers/include \
  $(SDK_ROOT)/components/libraries/experimental_task_manager \
  $(SDK_ROOT)/components/ble/ble_services/ble_hrs_c \
  $(SDK_ROOT)/components/libraries/queue \
  $(SDK_ROOT)/components/libraries/pwr_mgmt \
  $(SDK_ROOT)/components/ble/ble_dtm \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/components/ble/ble_services/ble_rscs_c \
  $(SDK_ROOT)/components/ble/common \
  $(SDK_ROOT)/components/ble/ble_services/ble_lls \
  $(SDK_ROOT)/components/ble/ble_services/ble_bas \
  $(SDK_ROOT)/components/libraries/mpu \
  $(SDK_ROOT)/components/libraries/experimental_section_vars \
  $(SDK_ROOT)/components/ble/ble_services/ble_ans_c \
  $(SDK_ROOT)/components/libraries/slip \
  $(SDK_ROOT)/components/libraries/delay \
  $(SDK_ROOT)/components/libraries/csense_drv \
  $(SDK_ROOT)/components/libraries/memobj \
  $(SDK_ROOT)/components/ble/ble_services/ble_nus_c \
  $(SDK_ROOT)/components/softdevice/common \
  $(SDK_ROOT)/components/ble/ble_services/ble_ias \
  $(SDK_ROOT)/components/libraries/usbd/class/hid/mouse \
  $(SDK_ROOT)/components/libraries/low_power_pwm \
  $(SDK_ROOT)/components/ble/ble_services/ble_dfu \
  $(SDK_ROOT)/external/fprintf \
  $(SDK_ROOT)/components/libraries/svc \
  $(SDK_ROOT)/components/libraries/atomic \
  $(SDK_ROOT)/components \
  $(SDK_ROOT)/components/libraries/scheduler \
  $(SDK_ROOT)/components/libraries/cli \
  $(SDK_ROOT)/components/ble/ble_services/ble_lbs \
  $(SDK_ROOT)/components/ble/ble_services/ble_hts \
  $(SDK_ROOT)/components/ble/ble_services/ble_cts_c \
  $(SDK_ROOT)/components/libraries/crc16 \
  $(SDK_ROOT)/components/libraries/util \
  $(PROJ_DIR) \
  $(SDK_ROOT)/components/libraries/usbd/class/cdc \
  $(SDK_ROOT)/components/libraries/csense \
  $(SDK_ROOT)/components/libraries/balloc \
  $(SDK_ROOT)/components/libraries/ecc \
  $(SDK_ROOT)/components/libraries/hardfault \
  $(SDK_ROOT)/components/ble/ble_services/ble_cscs \
  $(SDK_ROOT)/components/libraries/hci \
  $(SDK_ROOT)/components/libraries/usbd/class/hid/kbd \
  $(SDK_ROOT)/components/libraries/timer \
  $(SDK_ROOT)/integration/nrfx \
  $(SDK_ROOT)/components/libraries/sortlist \
  $(SDK_ROOT)/components/libraries/spi_mngr \
  $(SDK_ROOT)/components/libraries/led_softblink \
  $(SDK_ROOT)/components/libraries/sdcard \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(SDK_ROOT)/components/ble/ble_link_ctx_manager \
  $(SDK_ROOT)/components/ble/ble_services/ble_nus \
  $(SDK_ROOT)/components/libraries/twi_mngr \
  $(SDK_ROOT)/components/ble/ble_services/ble_hids \
  $(SDK_ROOT)/components/libraries/strerror \
  $(SDK_ROOT)/components/libraries/crc32 \
  $(SDK_ROOT)/components/libraries/usbd/class/audio \
  $(SDK_ROOT)/components/libraries/sensorsim \
  $(SDK_ROOT)/components/ble/peer_manager \
  $(SDK_ROOT)/components/libraries/mem_manager \
  $(SDK_ROOT)/components/libraries/ringbuf \
  $(SDK_ROOT)/components/ble/ble_services/ble_tps \
  $(SDK_ROOT)/components/ble/ble_services/ble_dis \
  $(SDK_ROOT)/components/ble/nrf_ble_gatt \
  $(SDK_ROOT)/components/ble/nrf_ble_qwr \
  $(SDK_ROOT)/components/libraries/gfx \
  $(SDK_ROOT)/components/libraries/button \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/components/libraries/twi_sensor \
  $(SDK_ROOT)/integration/nrfx/legacy \
  $(SDK_ROOT)/components/libraries/usbd \
  $(SDK_ROOT)/external/segger_rtt \
  $(SDK_ROOT)/components/libraries/atomic_fifo \
  $(SDK_ROOT)/components/ble/ble_services/ble_lbs_c \
  $(SDK_ROOT)/components/libraries/crypto \
  $(SDK_ROOT)/components/ble/ble_racp \
  $(SDK_ROOT)/components/libraries/fds \
  $(SDK_ROOT)/components/libraries/atomic_flags \
  $(SDK_ROOT)/components/ble/ble_services/ble_hrs \
  $(SDK_ROOT)/components/ble/ble_services/ble_rscs \
  $(SDK_ROOT)/components/libraries/stack_guard \
  $(SDK_ROOT)/components/libraries/log/src \
  $(SDK_ROOT)/components/libraries/fifo \
  $(SDK_ROOT)/components/libraries/uart \
  $(PROJ_DIR)/config \


SRC_FILES += $(PROJ_DIR)/i2c_master.c \
            $(PROJ_DIR)/timer.c \
            $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_twi.c

CFLAGS += -DSSD1306OLED

#### mcu spec
ifeq ($(strip $(MCU)), 52810)
SRC_FILES += \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52810.S \
	$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52810.c \

INC_FOLDERS += \
	$(SDK_ROOT)/components/softdevice/s112/headers/nrf52 \
	$(SDK_ROOT)/components/softdevice/s112/headers \


else ifeq ($(strip $(MCU)), 52832)
SRC_FILES += \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52.S \
	$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52.c \

INC_FOLDERS += \
	$(SDK_ROOT)/components/softdevice/s132/headers/nrf52 \
	$(SDK_ROOT)/components/softdevice/s132/headers
endif

# Optimization flags
OPT = -Os -g3

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DAPP_TIMER_V2
CFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF$(MCU)_XXAA
CFLAGS += -DNRF_SD_BLE_API_VERSION=7
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -DBL_SETTINGS_ACCESS_ONLY
CFLAGS += -DNRF_DFU_TRANSPORT_BLE=1
CFLAGS += -DNRF_DFU_SVCI_ENABLED
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

CFLAGS += -DNRFX_SAADC_ENABLED=1
CFLAGS += -DSAADC_ENABLED=1

# C++ flags common to all targets
CXXFLAGS += $(OPT)
# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -DAPP_TIMER_V2
ASMFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF$(MCU)_XXAA
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=7
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DBL_SETTINGS_ACCESS_ONLY
ASMFLAGS += -DNRF_DFU_SVCI_ENABLED
ASMFLAGS += -DNRF_DFU_TRANSPORT_BLE=1

ASMFLAGS += -DNRFX_SAADC_ENABLED=1
ASMFLAGS += -DSAADC_ENABLED=1

ifeq ($(strip $(MCU)), 52810)
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DS112
CFLAGS += -DFLOAT_ABI_SOFT
CFLAGS += -mfloat-abi=soft

ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DNRF52_PAN_74
ASMFLAGS += -DS112
ASMFLAGS += -DFLOAT_ABI_SOFT
ASMFLAGS += -mfloat-abi=soft

CFLAGS += -DNRF_SDH_CLOCK_LF_SRC=1
CFLAGS += -DNRF_SDH_CLOCK_LF_RC_CTIV=0
CFLAGS += -DNRF_SDH_CLOCK_LF_RC_TEMP_CTIV=0
CFLAGS += -DNRF_SDH_CLOCK_LF_ACCURACY=7

# fake 832
# CFLAGS += -DDEVELOP_IN_NRF52832
# CFLAGS += -DNRFX_COREDEP_DELAY_US_LOOP_CYCLES=3
# ASMFLAGS += -DDEVELOP_IN_NRF52832
# ASMFLAGS += -DNRFX_COREDEP_DELAY_US_LOOP_CYCLES=3

else ifeq ($(strip $(MCU)), 52833)
#### fixme #######################
CFLAGS += -DBOARD_PCA10100
CFLAGS += -DS113
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

ASMFLAGS += -DBOARD_PCA10100
ASMFLAGS += -DS113
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# 这句千万不能忘了，不然会报错
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

# 蓝牙相关
CFLAGS += -DNRF_SDH_CLOCK_LF_SRC=0
CFLAGS += -DNRF_SDH_CLOCK_LF_RC_CTIV=16
CFLAGS += -DNRF_SDH_CLOCK_LF_RC_TEMP_CTIV=2
CFLAGS += -DNRF_SDH_CLOCK_LF_ACCURACY=1
CFLAGS += -DNRFX_CLOCK_ENABLED=1
CFLAGS += -DNRFX_CLOCK_CONFIG_LF_SRC=0

ASMFLAGS += -DNRF_SDH_CLOCK_LF_SRC=0
ASMFLAGS += -DNRF_SDH_CLOCK_LF_RC_CTIV=16
ASMFLAGS += -DNRF_SDH_CLOCK_LF_RC_TEMP_CTIV=2
ASMFLAGS += -DNRF_SDH_CLOCK_LF_ACCURACY=1
ASMFLAGS += -DNRFX_CLOCK_ENABLED=1
ASMFLAGS += -DNRFX_CLOCK_CONFIG_LF_SRC=0

else # 832

CFLAGS += -DBOARD_PCA10040
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DS132
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DNRF52

ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DNRF52_PAN_74
ASMFLAGS += -DS132
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DNRF52

LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS += -DNRF_SDH_CLOCK_LF_SRC=1
CFLAGS += -DNRF_SDH_CLOCK_LF_RC_CTIV=0
CFLAGS += -DNRF_SDH_CLOCK_LF_RC_TEMP_CTIV=0
CFLAGS += -DNRF_SDH_CLOCK_LF_ACCURACY=7

endif

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

ifeq ($(strip $(MCU)), 52810)
nrf52810_xxaa: CFLAGS += -D__HEAP_SIZE=2048
nrf52810_xxaa: CFLAGS += -D__STACK_SIZE=2048
nrf52810_xxaa: ASMFLAGS += -D__HEAP_SIZE=2048
nrf52810_xxaa: ASMFLAGS += -D__STACK_SIZE=2048
else ifeq ($(strip $(MCU)), 52833)
nrf52833_xxaa: CFLAGS += -D__HEAP_SIZE=2048
nrf52833_xxaa: CFLAGS += -D__STACK_SIZE=2048
nrf52833_xxaa: ASMFLAGS += -D__HEAP_SIZE=2048
nrf52833_xxaa: ASMFLAGS += -D__STACK_SIZE=2048
else
nrf52832_xxaa: CFLAGS += -D__HEAP_SIZE=8192
nrf52832_xxaa: CFLAGS += -D__STACK_SIZE=8192
nrf52832_xxaa: ASMFLAGS += -D__HEAP_SIZE=8192
nrf52832_xxaa: ASMFLAGS += -D__STACK_SIZE=8192

endif


$(OUTPUT_DIRECTORY)/nrf52832_xxaa.out: \
  LINKER_SCRIPT  := nrf$(MCU)_xxaa$(bl_suffix).ld

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

LIB_FILES += -lc -lnosys -lm

# Default target - first one defined
default: nrf$(MCU)_xxaa

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash flash_softdevice erase

flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/$(hex_name)
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/$(hex_name) --sectorerase --log --verify
	nrfjprog -f nrf52 --reset

flash_softdevice:
	@echo Flashing: $(my_softdevice)_nrf52_7.2.0_softdevice.hex
	nrfjprog -f nrf52 --program $(softdevice_hex) --sectorerase --verify
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall