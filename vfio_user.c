  GNU nano 2.9.8                                                                                                                                                                                                                             vfio_user.c                                                                                                                                                                                                                                       

#include<stdio.h>
#include <linux/vfio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>

int main (void) {

    int fd, group, rv, device, i,data;
    unsigned char *ptr;
    struct vfio_group_status group_status = { .argsz = sizeof(group_status) };
    struct vfio_device_info device_info = { .argsz = sizeof(device_info) };

    fd = open("/dev/vfio/vfio", O_RDWR);
    if (fd < 0) {
        printf("negative fd in opening /dev/vfio/vfio %d\n",fd);
        return -1;
    }

    group = open("/dev/vfio/57", O_RDWR);
    if (group < 0) {
        printf("negative fd in opening /dev/vfio/57 %d\n",fd);
        return -1;
    }

    rv = ioctl(group, VFIO_GROUP_GET_STATUS, &group_status);
    if (rv < 0) {
        printf("ioctl with VFIO_GROUP_GET_STATUS failed\n");

    }
    if (!(group_status.flags & VFIO_GROUP_FLAGS_VIABLE))
        printf("Group is not viable (ie, not all devices bound for vfio)\n");

    rv = ioctl(group, VFIO_GROUP_SET_CONTAINER, &fd); /*we can try commenting it out*/
    if (rv < 0) {
        printf("ioctl with VFIO_GROUP_SET_CONTAINER failed\n");

    }
    
    /* Enable the IOMMU model we want */
    ioctl(fd, VFIO_SET_IOMMU, VFIO_TYPE1_IOMMU);

    device = ioctl(group, VFIO_GROUP_GET_DEVICE_FD, "0000:42:00.0");
    /* Test and setup the device */
    ioctl(device, VFIO_DEVICE_GET_INFO, &device_info);

    printf("device_info.num_regions %d\n",device_info.num_regions);

    for (i = 0; i < device_info.num_regions; i++) {
        struct vfio_region_info reg = { .argsz = sizeof(reg) };

        reg.index = i;

        ioctl(device, VFIO_DEVICE_GET_REGION_INFO, &reg);

       // printf("size[%d] %d\n",i,reg.size);
        if (i == 0) {
            ptr = mmap (NULL, reg.size, PROT_READ | PROT_WRITE, MAP_SHARED, device, reg.offset);
            if (!ptr) {
                printf("mmap failed: exiting\n");
                return -1;
            }

            printf ("value = 0x%x\n", *(unsigned int *)(ptr + 0x2414) );
            //read(group + reg.offset + 0x1024, &data,4);
        }
	/* Setup mappings... read/write offsets, mmaps
         * For PCI devices, config space is a region */
}

    return 0;
}

