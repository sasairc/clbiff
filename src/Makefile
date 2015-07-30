#
#    Makefile for clbiff
#

TARGET	= clbiff
SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)
ARCH	= $(shell gcc -print-multiarch)

all: $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c $< -o $@

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install: install-bin

clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
