DESIGN_NAME=xtea
ABSTRACTION_LEVEL=UT

EXECUTABLE=bin/heterogeneous_platform.x

SYSTEMC=$$SYSTEMC_HOME
SYSTEMC_AMS=$$SCAMS_HOME

PLATFORM= $(shell uname -s)
ARCH=$(shell getconf LONG_BIT)

ifeq ($(PLATFORM),Darwin)
SYSC_LIBDIR := $(SYSTEMC)/lib-macosx
SCAMS_LIBDIR := $(SYSTEMC_AMS)/lib-macosx
endif
ifeq ($(PLATFORM),Linux)
SYSC_LIBDIR := $(SYSTEMC)/lib-linux
SCAMS_LIBDIR := $(SYSTEMC_AMS)/lib-linux
endif

ifeq ($(ARCH),64)
SYSC_LIBDIR := $(SYSC_LIBDIR)64
SCAMS_LIBDIR := $(SCAMS_LIBDIR)64
endif

LIBS = $(SCAMS_LIBDIR)/libsystemc-ams.a   $(SYSC_LIBDIR)/libsystemc.a -lpthread

INCDIR = -I./include/ -I. -I$(SYSTEMC)/include/ -I$(SYSTEMC_AMS)/include

CXX=g++
CFLAGS= -c -g
LD=g++

all: $(EXECUTABLE)

$(EXECUTABLE): obj/main.o \
	obj/controller.o \
	obj/watertank.o \
	obj/iface_watertank.o \
	obj/watertank_transactor.o \
	obj/xtea_transactor.o \
	obj/valve.o \
	obj/iface_valve.o \
	obj/w_system.o \
	obj/xtea_RTL.o

	@echo Building $@
	@$(LD) -o $@ $(INCDIR) $^ $(LIBS)

obj/main.o: src/main.cpp
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/watertank.o: src/watertank.cpp include/watertank.hh
		@echo 'Building file: $<'
		@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/controller.o: src/controller.cpp include/controller.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<


obj/iface_watertank.o: src/iface_watertank.cpp include/iface_watertank.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/watertank_transactor.o: src/watertank_transactor.cpp include/watertank_transactor.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/xtea_transactor.o: src/xtea_transactor.cpp include/xtea_transactor.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/valve.o: src/valve.cpp include/valve.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/iface_valve.o: src/iface_valve.cpp include/iface_valve.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/w_system.o: src/w_system.cpp include/w_system.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

obj/xtea_RTL.o: src/xtea_RTL.cpp include/xtea_RTL.hh
	@echo 'Building file: $<'
	@$(CXX) $(CFLAGS) $(INCDIR) -o $@ $<

clean:
	@echo Cleaning
	@rm -f obj/*.o *~
	@rm -f waterwave.vcd
	@rm -f clock.vcd
	@rm -f $(EXECUTABLE)


.PHONY: all clean
