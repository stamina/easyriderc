PROJECT = easyrider
BUILD_DIR = bin

SHARED_DIR = ./include
CFILES = easyrider.c
CFILES += easy_state.c
CFILES += easy_util.c
CFILES += easy_cmd.c
CFILES += easy_log.c
CFILES += easy_clock.c
CFILES += easy_gpio.c
CFILES += easy_debug.c
CFILES += easy_usart.c
CFILES += easy_spi.c
CFILES += easy_i2c.c
CFILES += easy_buzz.c
CFILES += easy_gps.c
CFILES += easy_imu.c
CFILES += easy_ble.c
CFILES += easy_cfg.c
CFILES += easy_flash.c
CFILES += easy_adc.c
CFILES += easy_rpm.c
CFILES += easy_alarm.c

# TODO - you will need to edit these two lines!
DEVICE=stm32f446re
OOCD_FILE = 

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=./libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
