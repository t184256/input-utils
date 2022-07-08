// SPDX-FileCopyrightText: 2022 Alexander Sosedkin <monk@unboiled.info>
// SPDX-License-Identifier: CC-PDDC

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
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
	unsigned i, fails = 0, successes = 0;
	for (i = 0; i < 1024 && fails < successes / 2 + 4; i++) {  // overscan
		snprintf(input_device_name, 128, "/dev/input/event%d", i);
		in_dev_fd = open(input_device_name, O_RDONLY);
		if (in_dev_fd <= 0) {
			fails++;
			continue;
		}
		int err = libevdev_new_from_fd(in_dev_fd, &in_dev);
		if (err) {
			fprintf(stderr, "Cannot init %s, skipping\n",
				input_device_name);
			fails++;
			continue;
		}
		if (strcmp(libevdev_get_name(in_dev), argv[1]) == 0) {
			puts(input_device_name);
			return 0;
		}
		successes++;
	}
	return 7;
}
