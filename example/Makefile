DEVICE          = stm32f103c8t6
OPENCM3_DIR     = ../libopencm3
OBJS            += ../logger/logger.o ../sddriver/sdspi.o ../sddriver/sddriver.o test.o

CFLAGS          += -Os -ggdb3
CPPFLAGS		+= -MD
LDFLAGS         += -static -nostartfiles -L../openfat/src
LDLIBS          += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group -lopenfat

include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(OPENCM3_DIR)/mk/gcc-config.mk

.PHONY: clean all

all: binary.elf binary.bin

clean:
	$(Q)$(RM) -rf binary.* *.o
	$(Q)$(RM) -rf binary.* *.d
	$(Q)$(RM) -rf *.ld

	$(Q)$(RM) -rf ../logger/*.o
	$(Q)$(RM) -rf ../logger/*.d

	$(Q)$(RM) -rf ../sddriver/*.o
	$(Q)$(RM) -rf ../sddriver/*.d

include $(OPENCM3_DIR)/mk/genlink-rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk