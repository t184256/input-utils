#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <libevdev/libevdev.h>


struct libevdev* in_dev = NULL;
int in_dev_fd = 0;

void cleanup(void) {
	if (in_dev) {
		libevdev_grab(in_dev, LIBEVDEV_UNGRAB);
		libevdev_free(in_dev);
	}
	if (in_dev_fd) {
		close(in_dev_fd);
	}
}

struct input_event read_event(void) {
	struct input_event iev;
	int res;
	int flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;
	do {
		res = libevdev_next_event(in_dev, flags, &iev);
	} while (!(res == LIBEVDEV_READ_STATUS_SUCCESS && iev.type == EV_KEY));
	return iev;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Specify a device to wait for a keypress on\n");
		exit(1);
	}

	atexit(cleanup);
	in_dev_fd = open(argv[1], O_RDONLY);
	assert(in_dev_fd > 0);
	int err = libevdev_new_from_fd(in_dev_fd, &in_dev);
	assert(!err);
	err = libevdev_grab(in_dev, LIBEVDEV_GRAB);
	assert(!err);

	struct input_event iev;
	do { iev = read_event(); } while (!iev.value);  // wait for press
	do { iev = read_event(); } while (iev.value); // wait for release
	return 0;
}
