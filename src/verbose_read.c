#include <stdio.h>

int verbose_read(unsigned long file_position, int bytes_read) {

    printf("%s: [file_position=%lu] Read: %d\n", __FUNCTION__, file_position, bytes_read);
    fflush(stdout);

    return 0;

}

