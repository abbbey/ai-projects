#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>

void get_disk_info(char *disk_name) {
    // Open the specified disk
    int fd = open(disk_name, O_RDONLY);
    if (fd < 0) {
        perror("Error opening disk");
        return;
    }

    // Retrieve the IDENTIFY DEVICE data for the disk
    struct hd_driveid id;
    if (ioctl(fd, HDIO_GET_IDENTITY, &id) < 0) {
        perror("Error retrieving IDENTIFY DEVICE data");
        close(fd);
        return;
    }

    // Print the manufacturer and model name of the disk
    printf("Manufacturer: %.40s\n", id.model);
    printf("Model: %.40s\n", id.model);

    // Print the size of the disk
    unsigned long long size = (unsigned long long)id.lba_capacity * (unsigned long long)id.sector_bytes;
    printf("Size: %llu bytes\n", size);

    // Print the connection type of the disk
    printf("Connection type: ");
    if (id.config & 0x0080) {
        printf("ATA\n");
    } else if (id.config & 0x0040) {
        printf("ATAPI\n");
    } else {
        printf("unknown\n");
    }

    // Close the disk
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <disk name>\n", argv[0]);


         exit(1);
        }
    // Get the disk name from the command line argument
    char *disk_name = argv[1];

    // Retrieve and print the manufacturer, model name, size, and connection type of the disk
    get_disk_info(disk_name);

    return 0;
}
