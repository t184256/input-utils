CFLAGS+=$(shell pkg-config --cflags libevdev)
LDFLAGS+=$(shell pkg-config --libs libevdev)

all: wait_for_keypress
