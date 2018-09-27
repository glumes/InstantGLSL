//
// Created by glumes on 2018/9/3.
//

#include "file_fast_map.h"

const char *FileFastMap::fastRead(const char *path) {

    int fd;
    char *mapped;
    struct stat sb;

    if ((fd = open(path, O_RDWR)) < 0) {
        perror("open failed");
        return nullptr;
    }

    if ((fstat(fd, &sb)) == -1) {
        perror("fstat");
        return nullptr;
    }

    if ((mapped = (char *) mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
        (void *) -1) {
        perror("mmap error");
        return nullptr;
    }

    close(fd);

    return mapped;
}





