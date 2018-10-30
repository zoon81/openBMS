PROJECT_NAME := blinky_blank_pca20006
OUTPUT_FILENAME_ = nrf51822_xxaa

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

SDK_ROOT = /home/zoon81/nRF/_tools/nRF51_SDK_10.0.0_dc26b5e
TEMPLATE_PATH = $(SDK_ROOT)/components/toolchain/gcc
LINKER_SCRIPT_FOLDER = s110
LINKER_SCRIPT_NAME = blinky_gcc_nrf51.ld
SoftDevice_PATH = $(SDK_ROOT)/components/softdevice/s110/hex/s110_nrf51_8.0.0_softdevice.hex

include $(TEMPLATE_PATH)/Makefile.posix

MK := mkdir
RM := rm -rf
VERBOSE = 1
#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# debug build?
DEBUG = 1

# debug logs/ trace over UART
TRACE = 0

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'
OPENOCD			:= openocd

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#Project sources
C_SOURCE_FILES = main.c ./ble_bms_service.c ./ble_init.c ./uart.c ./frame.c ./systick.c

# Project includes
INC_PATH  = ./
INC_PATH  += ./config/ ./inc/

	# Libs
C_SOURCE_FILES += \
$(SDK_ROOT)/components/libraries/button/app_button.c \
$(SDK_ROOT)/components/libraries/util/app_error.c \
$(SDK_ROOT)/components/libraries/gpiote/app_gpiote.c \
$(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
$(SDK_ROOT)/components/libraries/timer/app_timer.c \
$(SDK_ROOT)/components/libraries/timer/app_timer_appsh.c \
$(SDK_ROOT)/components/libraries/util/nrf_assert.c \
$(SDK_ROOT)/components/drivers_nrf/delay/nrf_delay.c \
$(SDK_ROOT)/components/drivers_nrf/pstorage/pstorage.c \
$(SDK_ROOT)/components/ble/common/ble_advdata.c \
$(SDK_ROOT)/components/ble/common/ble_conn_params.c \
$(SDK_ROOT)/components/ble/common/ble_srv_common.c \
$(SDK_ROOT)/components/ble/ble_debug_assert_handler/ble_debug_assert_handler.c \
$(SDK_ROOT)/components/toolchain/system_nrf51.c \
$(SDK_ROOT)/components/softdevice/common/softdevice_handler/softdevice_handler.c \
$(SDK_ROOT)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
$(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c \
$(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
$(SDK_ROOT)/components/libraries/uart/retarget.c \
$(SDK_ROOT)/components/libraries/uart/app_uart_fifo.c \
$(SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c \
$(SDK_ROOT)/components/drivers_nrf/timer/nrf_drv_timer.c \


# Softdevice dependent
INC_PATH += $(SDK_ROOT)/components/softdevice/s110/headers/

# Compailer
INC_PATH += $(SDK_ROOT)/components/toolchain/cmsis/include/

# Include folders common to all targets
INC_PATH += $(SDK_ROOT)/components/libraries/util
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/pstorage
INC_PATH += $(SDK_ROOT)/components/toolchain/gcc
INC_PATH += $(SDK_ROOT)/components/toolchain
INC_PATH += $(SDK_ROOT)/components/ble/common
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/common
INC_PATH += $(SDK_ROOT)/components/ble/ble_advertising
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/config
INC_PATH += $(SDK_ROOT)/components/libraries/trace
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/gpiote
INC_PATH += $(SDK_ROOT)/components/ble/device_manager
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/uart
INC_PATH += $(SDK_ROOT)/components/libraries/uart
INC_PATH += $(SDK_ROOT)/components/device
INC_PATH += $(SDK_ROOT)/components/softdevice/common/softdevice_handler
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/delay
INC_PATH += $(SDK_ROOT)/components/libraries/timer
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/hal
INC_PATH += $(SDK_ROOT)/components/libraries/button
INC_PATH += $(SDK_ROOT)/components/libraries/scheduler
INC_PATH += $(SDK_ROOT)/components/libraries/fifo
INC_PATH += $(SDK_ROOT)/components/libraries/fds
INC_PATH += $(SDK_ROOT)/components/ble/peer_manager
INC_PATH += $(SDK_ROOT)/components/libraries/sensorsim
INC_PATH += $(SDK_ROOT)/components/libraries/log
INC_PATH += $(SDK_ROOT)/components/libraries/log/src
INC_PATH += $(SDK_ROOT)/components/ble/ble_error_log
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/ble_flash
INC_PATH += $(SDK_ROOT)/components/libraries/gpiote
INC_PATH += $(SDK_ROOT)/components/ble/ble_debug_assert_handler
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/pstorage
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/pstorage/config/
INC_PATH += $(SDK_ROOT)/components/drivers_nrf/timer


INC_PATHS := $(foreach path, $(INC_PATH), $(addprefix -I, $(path)))

$(info $$var is [${INC_PATHS}])

#assembly files common to all targets
ASM_SOURCE_FILES  = $(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf51.s

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DSOFTDEVICE_PRESENT
CFLAGS += -DNRF51
CFLAGS += -DS110
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DBOARD_PCA20006
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall
CFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -O0
else
CFLAGS += -O3
endif

ifeq ($(TRACE), 1)
CFLAGS += -DENABLE_DEBUG_LOG_SUPPORT
endif

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DNRF51
ASMFLAGS += -DS110
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DBOARD_PCA20006
ASMFLAGS += -DBSP_DEFINES_ONLY

#default target - first one defined
default: clean nrf51822_xxaa

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf51822_xxaa

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf51822_xxaa


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf51822_xxaa: OUTPUT_FILENAME := $(OUTPUT_FILENAME_)
nrf51822_xxaa: LINKER_SCRIPT= $(LINKER_SCRIPT_FOLDER)/$(LINKER_SCRIPT_NAME)
nrf51822_xxaa: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash:
	 $(OPENOCD) -d2 -f ./openocd.cfg -c 'init_reset halt; program $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME_).hex verify; reset; exit'

## Flash softdevice
flash_sd:
	 $(OPENOCD) -d2 -f ./openocd.cfg -c 'init_reset halt; program $(SoftDevice_PATH) 0 verify; reset; exit'