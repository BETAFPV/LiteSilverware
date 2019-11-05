
SDIR = .
VPATH = $(SDIR)/Silverware/src:$(SDIR)/Utilities:$(SDIR)/Libraries/STM32F0xx_StdPeriph_Driver/src:$(SDIR)/Libraries/CMSIS/Device/ST/STM32F0xx/Source/Templates/arm
SRC_C = $(wildcard $(SDIR)/Silverware/src/*.c) \
	$(wildcard $(SDIR)/Utilities/*.c) \
	$(wildcard $(SDIR)/Libraries/STM32F0xx_StdPeriph_Driver/src/*.c)
SRC_CXX = $(wildcard $(SDIR)/Silverware/src/*.cpp)
SRC_S = $(SDIR)/Libraries/CMSIS/Device/ST/STM32F0xx/Source/Templates/arm/startup_stm32f031.s

CFLAGS := -I$(DIR)/Silverware/src -I$(SDIR)/Libraries/CMSIS/Device/ST/STM32F0xx/Include -I $(SDIR)/Libraries/CMSIS/Include -I $(SDIR)/Utilities -I $(SDIR)/Libraries/STM32F0xx_StdPeriph_Driver/inc

CPU = --cpu Cortex-M0

CFLAGS   += $(CPU) -D__EVAL -D__MICROLIB -g -O2 --apcs=interwork --split_sections -D__UVISION_VERSION="524" -DUSE_STDPERIPH_DRIVER -DSTM32F031 --fpmode=fast
ASMFLAGS := $(CPU) --pd "__EVAL SETA 1" -g --apcs=interwork --pd "__MICROLIB SETA 1" --pd "__UVISION_VERSION SETA 524" --xref
LDFLAGS  := $(CPU) --library_type=microlib --ro-base 0x08000000 --entry 0x08000000 --rw-base 0x20000000 --entry Reset_Handler --first __Vectors --strict --info summarysizes

SRCS     := $(SRC_C) $(SRC_CXX) $(SRC_S)
ODIR     = $(SDIR)/obj

OBJS 	 = $(addprefix $(ODIR)/, $(notdir $(SRC_C:.c=.o) $(SRC_S:.s=.o) $(SRC_CXX:.cpp=.o)))

export ARM_TOOL_VARIANT = mdk_lite
export ARMCC5_ASMOPT = --diag_suppress=9931
export ARMCC5_CCOPT = --diag_suppress=9931
export ARMCC5_LINKOPT = --diag_suppress=9931
export CPU_TYPE = STM32F030F4
export CPU_VENDOR = STMicroelectronics
export CPU_CLOCK = 0x00B71B00
export UV2_TARGET = BWhoop

.PHONY: default all
default: silverware.hex

$(VERBOSE).SILENT:

$(OBJS): | $(ODIR)

$(ODIR):
	@mkdir -p $@

$(ODIR)/%.o: %.cpp
	@echo " + Compiling '$(notdir $<)'"
	armcc --cpp $(CFLAGS) --depend=$(@:.o=.dep) -c -o $@ $<

$(ODIR)/%.o: %.c
	@echo " + Compiling '$(notdir $<)'"
	armcc --c99 $(CFLAGS) --depend=$(@:.o=.dep) -c -o $@ $<

$(ODIR)/%.o: %.s
	@echo " + Compiling '$(notdir $<')"
	armasm $(ASMFLAGS) -o $@ $<

silverware.hex: silverware.axf
	fromelf $< --i32combined --output $@

silverware.axf: $(OBJS)
	armlink $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -Rf $(ODIR) silverware.axf silverware.hex

-include $(OBJS:.o=.dep)
