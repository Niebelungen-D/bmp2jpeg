#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

int main (int argc, char** argv)
{
    int i, j;
    int fd;

    fd = open ("grayout512.bmp", O_RDONLY);

    unsigned char buf[54] = {0, };

    read (fd, buf, 54);

	for (i=0; i<54; i++) {
		printf ("index %d: 0x%x\n", i, buf[i]);
	}

    // info, little endian
    unsigned int width;
    memcpy (&width, &buf[18], sizeof (int));
    printf ("width: %u\n", width);

    unsigned int height;
    memcpy (&height, &buf[22], sizeof (int));
    printf ("height: %u\n", height);

    unsigned short bits;
    memcpy (&bits, &buf[28], sizeof (short));
    printf ("bits: %u\n", bits);

	return 0;
}
