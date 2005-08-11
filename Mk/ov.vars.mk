# Each Inputmethod should define a IMID as it's identifier.
# It'll be used as a part of data installation path.

ECHO=/bin/echo
RM=/bin/rm -f
CP=/bin/cp
MKDIR=/bin/mkdir -p

DATA_INST_PATH?=$(INST_PATH)/$(IMID)

INC=-I./include -I/usr/local/include

CFLAGS=-O2 -Wall $(INC)

GCC=/usr/bin/gcc
GPP=/usr/bin/g++
LIBTOOL=$(shell ./find-libtool.pl)

CCACHE=ccache
ifdef USE_CCACHE
CC=$(CCACHE) $(GCC)
CPP=$(CCACHE) $(GPP)
else
CC=$(GCC)
CPP=$(GPP)
endif

VPATH+= ../../Source/
DATA?=

