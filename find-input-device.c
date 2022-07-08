// SPDX-FileCopyrightText: 2022 Alexander Sosedkin <monk@unboiled.info>
// SPDX-License-Identifier: CC-PDDC

//#include <assert.h>
//#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <libevdev/libevdev.h>


struct libevdev* in_dev = NULL;
int in_dev_fd = 0;

void cleanup(void) {
	if (in_dev)
		libevdev_free(in_dev);
	if (in_dev_fd)
		close(in_dev_fd);
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Specify device name to search for\n");
		exit(1);
	}

	atexit(cleanup);

	char input_device_name[128];
	unsigned i, fails, successes = 0;
	for (i = fails = successes = 0; fails < successes + 8; i++) {
		snprintf(input_device_name, 128, "/dev/input/event%d", i);
		in_dev_fd = open(argv[1], O_RDONLY);
		if (in_dev_fd <= 0) {
			fprintf(stderr, "Cannot open %s, ending enumeration",
				input_device_name);
			continue;
		}
		assert(in_dev_fd > 0);
		int err = libevdev_new_from_fd(in_dev_fd, &in_dev);
		if (err) {
			fprintf(stderr, "Cannot init %s, ending enumeration",
				input_device_name);
			continue;
		}
		if (strcmp(libevdev_get_name(&in_dev), argv[1]) == 0) {
			puts(input_device_name);
			return 0;
		}
	}
	return 7;
}
