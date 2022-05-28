
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

PROJ_DIR := src

SRC_FILES += $(PROJ_DIR)/main.c

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