include toolchain-settings.mk
NAME=picfun
SOURCES:= ${SRC_DIR}/main.c \
					${SRC_DIR}/uart.c

# end of normal editable sections

#-MD makes file.c.d with makefile style dependencies

OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=$(DIST_DIR)/$(NAME).elf

all: hex
check: GCC-exists
GCC-exists: ; @which pic30-gcc > /dev/null


	
$(EXECUTABLE): $(OBJECTS)$
	$(CC)  $(EXTRA_CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

.c.o:
	${CC} ${CFLAGS} $< -o $@



hex: $(EXECUTABLE)
	$(BIN2HEX) $(EXECUTABLE) 
	



clean:
	$(RM) $(SRC_DIR)/*.o
	$(RM) $(EXECUTABLE)
	$(RM) dist/$(NAME).hex

program:
	$(PROG) --dev=$(PROG) --hex=dist/$(NAME).hex




