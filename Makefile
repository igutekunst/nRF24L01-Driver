include toolchain-settings.mk
NAME=picfun
SOURCES:= ${SRC_DIR}/main.c \
					${SRC_DIR}/uart.c

# end of normal editable sections



OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=$(DIST_DIR)/$(NAME).elf

check: GCC-exists
GCC-exists: ; @which pic30-gcc > /dev/null


all: $(SOURCES) $(EXECUTABLE) $(hex)
	
$(EXECUTABLE): $(OBJECTS)$
	$(CC)  $(EXTRA_CFLAGS) $(OBJECTS) -$(LDFLAGS) -o $(EXECUTABLE)

.c.o:
	${CC} ${CFLAGS} $< -o $@



hex: $(EXECUTABLE)
	$(BIN2HEX) $(EXECUTABLE) -omf=elf
	

program:


clean:
	rm $(SRC_DIR)/*.o
	rm $(EXECUTABLE)



