#edit this file to configure the toolchain

#toolchain path
C30_DIR := /Applications/microchip/mplabc30/v3.30c/bin

#edit the PIC type
PIC_TYPE = 24F16KA102

# Code language. I don't really know how to magically add more
LANGUAGE:= c


#Heap Size. Change this to nonzero if you want to use malloc(), etc
HEAP_SIZE=0


#Serial Port
USERPORT := `ls /dev/tty.usb*`

SHELL	= /bin/sh
INCLUDE = -I${INC_DIR}
CFLAGS  = -g \
					-c \
					-Werror-implicit-function-declaration \
					-Wshadow			\
					-mcpu=${PIC_TYPE} \
					${INCLUDE} 

LINKER_SCRIPT = p${PIC_TYPE}.gld
EXTRA_CFLAGS =  -mcpu=${PIC_TYPE}

LDFLAGS = -Wl,--no-check-sections,--heap=${HEAP_SIZE},-T${LINKER_SCRIPT}



MKDIR=mkdir -p
RM=rm -f
MV=mv
CP=cp

TOP_DIR 		= .
SRC_DIR  		= ${TOP_DIR}/src
OBJECT_DIR 	= ${TOP_DIR}/build
DIST_DIR			= ${TOP_DIR}/dist
LIB_DIR  		= ${TOP_DIR}/lib
INC_DIR 		= ${TOP_DIR}/include


CC=${C30_DIR}/pic30-gcc
AS=${C30_DIR}/pic30-as
LD=${C30_DIR}/pic30-ld
AR=${C30_DIR}/pic30-ar
BIN2HEX=${C30_DIR}/pic30-bin2hex
