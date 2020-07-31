##############################################################################
##############################################################################
##                                                                          ##
##   Title:         Generic makefile for embedded system projects           ##
##                                                                          ##
##   Author:        Nathan Jones, nathancharlesjones@gmail.com              ##
##                                                                          ##
##   Description:   Builds simple projects for embedded systems. Supports   ##
##                  native- and cross-compilation (though no embedded       ##
##                  target has been set). Can build debug and release       ##
##                  versions (the difference being which flags are passed   ##
##                  to the compiler). Can package build output into a zip   ##
##                  file. A simple library is also compiled and linked.     ##
##                                                                          ##
##   Usage:         First, update the lists of source files and include     ##
##                  directories for your project. The following rules or    ##
##                  goals require BUILD and TARGET to be specified on the   ##
##                  command line: all (or none), clean, and zip. "Clean"    ##
##                  will remove a specific build folder; "purify" will      ##
##                  remove all build folders. "Print" will display the      ##
##                  lists that are being used in the makefile (for debug    ##
##                  purposes).                                              ##
##                                                                          ##
##   License:       MIT                                                     ##
##                                                                          ##
##                  Copyright (c) 2020 Nathan Charles Jones                 ##
##                                                                          ##
##                  Permission is hereby granted, free of charge, to any    ##
##                  person obtaining a copy of this software and            ##
##                  associated documentation files (the "Software"), to     ##
##                  deal in the Software without restriction, including     ##
##                  without limitation the rights to use, copy, modify,     ##
##                  merge, publish, distribute, sublicense, and/or sell     ##
##                  copies of the Software, and to permit persons to whom   ##
##                  the Software is furnished to do so, subject to the      ##
##                  following conditions:                                   ##
##                                                                          ##
##                  The above copyright notice and this permission notice   ##
##                  shall be included in all copies or substantial portions ##
##                  of the Software.                                        ##
##                                                                          ##
##                  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF   ##
##                  ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED ##
##                  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A     ##
##                  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT     ##
##                  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR    ##
##                  ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN    ##
##                  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    ##
##                  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE    ##
##                  OR OTHER DEALINGS IN THE SOFTWARE.                      ##
##                                                                          ##
##############################################################################
##############################################################################

##############################################################################
#                                                                            #
#  Command line switches:                                                    #
#   - V: 0 (quiet), 1 (verbose) (can also use "--trace")                     #
#   - BUILD: test, debug, release                                            #
#   - TARGET: x86, STM32F1                                                   #
#   - ZIP_SUFFIX: Custom string to append to the end of zip files            #
#                 created using the "zip" make goal.                         #
#                                                                            #
##############################################################################

# Verbose flag. Using the "--trace" flag offers additional verbosity.
ifeq ($(V),1)
Q :=
else
Q := @
endif

# Check if the make goals contain all, clean, zip, or the default rule;
# used to determine if the BUILD and TARGET variables need to be set.
ifeq ($(MAKECMDGOALS),)
MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP = 1
else ifneq ($(filter all clean zip, $(MAKECMDGOALS)),)
MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP = 1
else ifeq ($(filter-out purify print, $(MAKECMDGOALS)),)
MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP = 0
else
$(error Unreachable)
endif

# BUILD starts empty and an error is thrown if it isn't defined on the command
# line to be either "test", "debug", or "release" AND the make goals include
# either "all", "clean", "zip" or is empty (which defaults to "all").
BUILD ?=
# Check that BUILD only contains the keywords
ifeq ($(BUILD),test)
else ifeq ($(BUILD),debug)
else ifeq ($(BUILD),release)
# Do nothing; these are valid targets for BUILD.
# Check that the make goals do not contain all, clean, zip, or the default rule;
# we only evaluate this if the first conditionals are false, so if either of
# these are true then we need to throw an error.
else ifeq ($(MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP),1)
$(error ERROR: BUILD variable not set. Options are: test, debug, or release)
else ifeq ($(filter-out purify print, $(MAKECMDGOALS)),)
# Do nothing; neither "purify" nor "print" use the BUILD variable
else
$(error Unreachable)
endif

# TARGET starts empty and an error is thrown if it isn't defined on the command
# line to be either "test", "debug", or "release" AND the make goals include
# either "all", "clean", "zip" or is empty (which defaults to "all").
TARGET ?=
# This is the embedded target, referenced throughout the rest of this Makefile.
MCU := STM32F1
# The EXECUTABLE variable below is recursively expanded since the variable
# BUILD_FOLDER isn't defined until later in the Makefile.
ifeq ($(TARGET),x86)
EXECUTABLE = $(BUILD_FOLDER)/$(BUILD).elf
else ifeq ($(TARGET),$(MCU))
EXECUTABLE = $(BUILD_FOLDER)/$(BUILD).bin
# Check that the make goals do not contain all, clean, zip, or the default rule;
# we only evaluate this if the first conditionals are false, so if this is
# true then we need to throw an error.
else ifeq ($(MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP),1)
$(error ERROR: TARGET variable not set. Options are: x86 or $(MCU))
else ifeq ($(filter-out purify print, $(MAKECMDGOALS)),)
# Do nothing; neither "purify" nor "print" use the TARGET variable
else
$(error Unreachable)
endif

# Give users the option to add a suffix to the package created by the "zip"
# target. If they do, prefix it with an underscore in order to separate it
# from the default package filename.
ZIP_SUFFIX ?=
ifneq ($(ZIP_SUFFIX),)
UNDERSCORE_ZIP_SUFFIX := _$(ZIP_SUFFIX)
endif

##############################################################################
#                                                                            #
#  File lists:                                                               #
#   - Source files for final executable (SRC_FILES)                          #
#     - NOTE: There are two different SRC_FILES lists based on which target  #
#       is being built.                                                      #
#   - Assembly files for final executable (ASM_FILES)                        #
#   - Object files for final executable (SRC_OBJ_FILES & ASM_OBJ_FILES)      #
#   - Dependency files for every object (DEP_FILES)                          #
#                                                                            #
##############################################################################

# List of C source files. Putting this in a separate variable, with a file on
# each line, makes it easy to add files later (and makes it easier to see
# additions in pull requests). Larger projects might use a wildcard to locate
# source files automatically.

# Source files common to all targets.
SRC_FILES := \
	source/main.c \
	source/sin_lut.c \
	libraries/PolynomialApproximations_Ganssle/sincos.c

# Source and assembly files specific to one or more targets.
ifeq ($(MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP),0)
# Do nothing; we don't need a list of source files for the remaining goals
else ifeq ($(TARGET),x86)
SRC_FILES += \
    hardware/x86/x86.c
else ifeq ($(TARGET),$(MCU))
SRC_FILES += \
	hardware/STM32F1/source/STM32F1.c \
	hardware/STM32F1/source/stm32f1xx_hal_msp.c \
	hardware/STM32F1/source/stm32f1xx_it.c \
	hardware/STM32F1/source/system_stm32f1xx.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
ASM_FILES := \
	hardware/STM32F1/source/startup_stm32f103xb.s
else
$(error Unreachable)
endif

# Generate a list of .o files from the .c files. Prefix them with the build
# folder to output the files there. These variables are recursive because
# the variable BUILD_FOLDER isn't assigned until later in the Makefile.
SRC_OBJ_FILES = $(addprefix $(BUILD_FOLDER)/,$(SRC_FILES:.c=.o))
ASM_OBJ_FILES = $(addprefix $(BUILD_FOLDER)/,$(ASM_FILES:.s=.o))

# Generate a list of depfiles, used to track includes. The file name is the same
# as the object files with the .d extension added. This variable is recursively
# expanded since SRC_OBJ_FILES and ASM_OBJ_FILES both rely on BUILD_FOLDER, which
# isn't defined until later in the Makefile.
DEP_FILES = $(SRC_OBJ_FILES:.o=.d) $(ASM_OBJ_FILES:.o=.d)

##############################################################################
#                                                                            #
#  Compiler options                                                          #
#                                                                            #
##############################################################################

# Set the default compiler
CC := gcc

# The CFLAGS variable below is recursively expanded since the variable
# COMMON_FLAGS isn't defined until later in the Makefile.
CFLAGS = \
	$(COMMON_FLAGS)

# Check which target we are building for, in order to set the proper compiler and CFLAGS.
ifeq ($(MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP),0)
# Do nothing; we don't need to specify a GCC prefix or CFLAGS for the remaining goals
else ifeq ($(TARGET),x86)
# Do nothing; default settings are fine for building for x86.
else ifeq ($(TARGET),$(MCU))
GCC_PREFIX := arm-none-eabi-
CFLAGS += \
	-DUSE_HAL_DRIVER \
	-DSTM32F103xB \
	$(MCU_FLAGS)
else
$(error Unreachable)
endif

# List of include dirs. These are put into CFLAGS.
INCLUDE_DIRS := \
    include/ \
    hardware/include/ \
    hardware/STM32F1/include \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Inc \
	libraries/STM32CubeF1/STM32F1xx_HAL_Driver/Inc/Legacy \
	libraries/STM32CubeF1/CMSIS/Device/ST/STM32F1xx/Include \
	libraries/STM32CubeF1/CMSIS/Include \
	libraries/STM32CubeF1/CMSIS/Include \
	libraries/Assert \
	libraries/Error \
	libraries/FixedPoint_ARM \
	libraries/PolynomialApproximations_Ganssle

# Prefix the include dirs with '-I' when passing them to the compiler
CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

# Flags to generate the .d dependency-tracking files when we compile.  It's
# named the same as the target file, swapping the .o extension with .d. This
# variable is recursively expanded since it references the target ($@).
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

##############################################################################
#                                                                            #
#  Assembler options                                                         #
#                                                                            #
##############################################################################

# Set the default assembler
AS := gcc

# The ASFLAGS variable below is recursively expanded since the variables
# COMMON_FLAGS and MCU_FLAGS aren't defined until later in the Makefile.
ASFLAGS = \
	-x assembler-with-cpp \
	$(COMMON_FLAGS) \
	$(MCU_FLAGS)

##############################################################################
#                                                                            #
#  Linker options                                                            #
#                                                                            #
##############################################################################

# Our project requires some linker flags: garbage collect sections, output a
# .map file. LDFLAGS is recursively expanded because it includes the target
# variable ($@).
LDFLAGS = \
    -Wl,--gc-sections,-Map,$(basename $@).map,--cref

LDLIBS := \
    -lm

# Check which target we are building for, in order to set the proper linker settings.
ifeq ($(MAKE_GOALS_INCLUDE_ALL_CLEAN_OR_ZIP),0)
# Do nothing; we don't need to specify LDFLAGS or LDLIBS for the remaining goals
else ifeq ($(TARGET),x86)
# Do nothing; default settings are fine for building for x86.
else ifeq ($(TARGET),$(MCU))
LDFLAGS += \
	$(MCU_FLAGS) \
	--specs=nano.specs \
	-T$(LDSCRIPT)
LDLIBS += \
	-lc \
    -lnosys
else
$(error Unreachable)
endif

LDSCRIPT := hardware/STM32F1/linker-script/STM32F103C8Tx_FLASH.ld

##############################################################################
#                                                                            #
#  Automatically created variables                                           #
#                                                                            #
##############################################################################

# Compiler/assembler flags common to all builds and targets. COMMON_FLAGS is recursively
# expanded because it includes the variable for the first prerequisite ($<); the variable
# __FILENAME__ is defined here in order for it to be used by an implementation of assert.
COMMON_FLAGS = \
	-std=c11 \
    -Wall \
    -ffunction-sections \
    -fdata-sections \
    -fstack-usage \
    -D__FILENAME__=\"$(notdir $<)\"

# Check value of BUILD and MAKECMDGOALS to determine which CFLAGS we need
# to add.
ifneq ($(MAKECMDGOALS),)
# Do nothing; we're not executing the default rule (all) so we don't care
# which set of CFLAGS we add.
else ifneq ($(filter all, $(MAKECMDGOALS)),)
# Do nothing; we're not executing the default rule (all) so we don't care
# which set of CFLAGS we add.
# If this is a release build, compile for performance.
else ifeq ($(BUILD),release)
$(info BUILD == release; using -O2 flag instead of -Og & -g3)
COMMON_FLAGS += -O2
# If this is a test or debug build, compile for debuggability.
else ifneq ($(filter test debug, $(BUILD)),)
$(info BUILD != release; using -Og & -g3 flags instead of -O2)
COMMON_FLAGS += -Og -g3
else
$(error Unreachable)
endif

# MCU-specific flag for the compiler, assembler, and linker
MCU_FLAGS := \
	-mcpu=cortex-m3 \
	-mthumb

# The build folder, for all generated output. This should normally be included
# in a .gitignore rule.
BUILD_FOLDER := build/$(BUILD)_$(TARGET)

# File where build info (such as date and software versions) should be placed.
# See the make target "$(BUILD_FOLDER)/$(BUILD).elf" for usage. This variable
# is defined recursively because it references the target ($@).
BUILD_INFO_FILE = $(dir $@)build-info.txt

# Get the current date from the shell, in YYYY-MM-DD formate. Used to
# create zip files.
CURRENT_DATE := $(shell date +%F)

# Generate the build tree (list of folders for all output files) based on
# the directory information from the list of objects. "sort" is used to
# remove duplicate files from the resulting list, not necessarily because
# the list needs to be sorted.
BUILD_TREE := $(sort $(dir $(SRC_OBJ_FILES)) $(dir $(ASM_OBJ_FILES)))

##############################################################################
#                                                                            #
#  Makefile goals                                                            #
#                                                                            #
##############################################################################

# Default all rule will build the target, which here is an executable
.PHONY:
all: $(EXECUTABLE)

# The rule for building the target executable, without debug information.
$(BUILD_FOLDER)/$(BUILD).bin: $(BUILD_FOLDER)/$(BUILD).elf | $(BUILD_TREE)
	$(GCC_PREFIX)objcopy -O binary -S $< $@

# The rule for building the target executable, with debug information.
$(BUILD_FOLDER)/$(BUILD).elf: $(SRC_OBJ_FILES) $(ASM_OBJ_FILES) | $(BUILD_TREE)
	@echo Linking $(notdir $@)
	$(Q) $(GCC_PREFIX)$(CC) $(LDFLAGS) $^ $(LDLIB_DIRS) $(LDLIBS) -o $@
	@# Echo with only one right arrow is used to overwrite any previous version of build-info.txt
	$(Q) echo "Build date:\t\t\t$(shell date)" > $(BUILD_INFO_FILE)
	@# Echo with TWO right arrows is used to append to the newly created build-info.txt
	$(Q) echo "Build account:\t\t$(shell whoami)@$(shell hostname)" >> $(BUILD_INFO_FILE)
	$(Q) echo "Build machine:\t\t$(shell uname -p) running $(shell uname -o), release $(shell uname -r), \
	version $(shell uname -v)." >> $(BUILD_INFO_FILE)
	$(Q) echo "Make version info:\t$(shell $(MAKE) -v | head -n 2)" >> $(BUILD_INFO_FILE)
	$(Q) echo "GCC version info:\t$(shell $(GCC_PREFIX)$(CC) --version | head -n 1)" >> $(BUILD_INFO_FILE)
	$(Q) echo "GNU binutils info:\t$(shell $(GCC_PREFIX)$(AR) -V | head -n 1)" >> $(BUILD_INFO_FILE)
	$(Q) echo "\t\t\t\t\t$(shell $(GCC_PREFIX)size -V | head -n 1)" >> $(BUILD_INFO_FILE)
	$(Q) echo "\t\t\t\t\t$(shell $(GCC_PREFIX)objcopy -V | head -n 1)" >> $(BUILD_INFO_FILE)
	$(Q) $(GCC_PREFIX)size $@

# The rule for compiling the SRC_FILES into OBJ_FILES
$(BUILD_FOLDER)/%.o: %.c $(BUILD_FOLDER)/%.d | $(BUILD_TREE)
	@echo Compiling $(notdir $<)
	$(Q) $(GCC_PREFIX)$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# The rule for compiling the ASM_FILES into OBJ_FILES
$(BUILD_FOLDER)/%.o: %.s $(BUILD_FOLDER)/%.d | $(BUILD_TREE)
	@echo Assembling $(notdir $<)
	$(Q) $(GCC_PREFIX)$(CC) $(ASFLAGS) $(DEPFLAGS) -c $< -o $@

# Create the directory structure into which all build artifacts will be placed.
$(BUILD_TREE):
	$(Q) mkdir -p $@

.PHONY: zip
# Package the specified build folder into a zip file.
zip: all
	# Reference: https://www.andrewheiss.com/blog/2020/01/10/makefile-subdirectory-zips/
	cd build && zip -FSr $(BUILD)_$(TARGET)_$(CURRENT_DATE)$(UNDERSCORE_ZIP_SUFFIX).zip $(BUILD)_$(TARGET)/

.PHONY: print
print:
	@echo --BUILD_FOLDER--
	@echo $(BUILD_FOLDER)
	@echo --SRC_FILES--
	@echo $(SRC_FILES)
	@echo --LIB_FILES--
	@echo $(LIB_FILES)
	@echo --SRC_OBJ_FILES--
	@echo $(SRC_OBJ_FILES)
	@echo --LIB_OBJ_FILES--
	@echo $(LIB_OBJ_FILES)
	@echo --BUILD_TREE--
	@echo $(BUILD_TREE)
	@echo --DEP_FILES--
	@echo $(DEP_FILES)
	@echo --INCLUDE_DIRS--
	@echo $(INCLUDE_DIRS)
	@echo --CFLAGS--
	@echo $(CFLAGS)
	@echo --LDFLAGS--
	@echo $(LDFLAGS)
	@echo --LIB_NAME--
	@echo $(LIB_NAME)
	@echo --LIB_FILE_NAME--
	@echo $(LIB_FILE_NAME)
	@echo --LIB_DIR--
	@echo $(LIB_DIR)
	@echo --LDLIBS--
	@echo $(LDLIBS)
	@echo --LDLIB_DIRS--
	@echo $(LDLIB_DIRS)

# Since all our generated output is placed into the build folder, our clean rule
# is simple. Prefix the recipe line with '-' to not error if the build folder
# doesn't exist (the -f flag for rm also has this effect)
.PHONY: clean
clean:
	$(Q) $(RM) -r $(BUILD_FOLDER)/

.PHONY: purify
purify:
	$(Q) $(RM) -r build/

# Empty target so Make doesn't throw an error if the dependency files don't exist
$(DEP_FILES):

# Include the dependency tracking files
-include $(DEP_FILES)