//configuration
#define DRIVE_SIZE(X) "1.4T"
#define PARTITION_NUMBER(X) "1"
#define MOUNT_POINT(X) "/sda1"

#define DRIVE_NAME_SIZE(X) X(4)
#define COMMAND_BUFFER_SIZE(X) X(32)

//code
#define GREP_COMMAND(size) "lsblk | grep " size
#include <stdlib.h>
#include <stdio.h>
int main() {

	FILE *fp;
	char buf[4];
	fp = popen(GREP_COMMAND(DRIVE_SIZE()), "r");
	if (fp == NULL) {
		printf("Failed to locate" DRIVE_SIZE() " drive!, exiting\n");
		exit(1);
	}

	if(fgets(buf, DRIVE_NAME_SIZE(), fp) != NULL) {
		printf(DRIVE_SIZE() " drive identified as %s!\n", buf);
	}
	char command [COMMAND_BUFFER_SIZE()];
	snprintf(command, sizeof(command), "sudo mount /dev/%s" PARTITION_NUMBER() " " MOUNT_POINT(), buf);
	printf("mount command requires sudo privileges, may require password\n");
	int result = system(command);
	if (result == -1) {
		printf("Command execution failed! Failed to mount to /sdb1\n");
	} else {
		printf("command executed!\n");
	}
		printf("command '%s' exited with status %d\n", command, result);
	pclose(fp);
	return 0;
}
