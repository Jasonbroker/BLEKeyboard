
PROJECT_NAME     := ble_keyboard
MCU = 52832
TARGETS := nrf$(MCU)_xxaa
OUTPUT_DIRECTORY := .build

SDK_ROOT := ../nRF5_SDK_17.1.0

PROJ_DIR := src

SRC_FILES += $(PROJ_DIR)/main.c

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

LIB_FILES += -lc -lnosys -lm

# Default target - first one defined
# default: nrf$(MCU)_xxaa

$(foreach target, $(TARGETS), $(call define_target, $(target)))