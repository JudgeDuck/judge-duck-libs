# Run 'make V=1' to turn on verbose commands, or 'make V=0' to turn them off.
ifeq ($(V),1)
override V =
endif
ifeq ($(V),0)
override V = @
endif

UNAME_S := $(shell uname -s)

USER_CFLAGS := -pipe -fno-builtin -O2 -std=gnu99 -static -Wall -Wno-format -Wno-unused -m32 -fno-stack-protector
USER_CFLAGS += -I../libducknet_include

USER_CXXFLAGS := -pipe -fno-builtin -O2 -fno-omit-frame-pointer -static -Wall -Wno-format -Wno-unused -m32 -fno-stack-protector
USER_CXXFLAGS += -I../libducknet_include

OBJDIR := .

PREFIX := 
ifeq ($(UNAME_S),Darwin)
	PREFIX := i386-elf-
endif

CC := $(PREFIX)gcc
CXX := $(PREFIX)g++
AS := $(PREFIX)as
AR := $(PREFIX)ar

LIB_HEADERS := ../libducknet_include/*.h ducknet_impl.h

LIB_SRCFILES :=	\
			ducknet.cpp \
			ducknet_impl.cpp \
			utils.cpp \
			phy.cpp \
			ether.cpp \
			arp.cpp \
			ipv4.cpp \
			icmp.cpp \
			udp.cpp \



LIB_OBJFILES := $(patsubst %.c, $(OBJDIR)/%.o, $(LIB_SRCFILES))
LIB_OBJFILES := $(patsubst %.cpp, $(OBJDIR)/%.o, $(LIB_SRCFILES))
LIB_OBJFILES := $(patsubst %.S, $(OBJDIR)/%.o, $(LIB_OBJFILES))

all: $(OBJDIR)/libducknet.a

.SUFFIXES:

.DELETE_ON_ERROR:

.PRECIOUS: %.o


.PHONY: FORCE



$(OBJDIR)/%.o: %.c $(LIB_HEADERS)
	@echo + cc[USER] $<
	$(V)$(CC)  $(USER_CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.cpp $(LIB_HEADERS)
	@echo + cxx[USER] $<
	$(V)$(CXX)  $(USER_CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.S $(LIB_HEADERS)
	@echo + as[USER] $<
	$(V)$(CC)  $(USER_CFLAGS) -c -o $@ $<

$(OBJDIR)/libducknet.a: $(LIB_OBJFILES)
	@echo + ar $@
	$(V)$(AR) r $@ $(LIB_OBJFILES)

clean:
	rm -rf *.d *.o *.a
