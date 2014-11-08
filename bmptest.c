#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct __bmp_header {

	unsigned short signature;
	unsigned int bmp_file_size;
	unsigned short reserve_1;
	unsigned short reserve_2;
	unsigned int soi_offset;
	unsigned int bmp_header_size;
	unsigned int bmp_width;
	unsigned int bmp_height;
	unsigned short number_of_planes;
	unsigned short bpp; // number of bits per pixel
	unsigned int compression_type;
	unsigned int img_data_size;
	unsigned int horizontal_ppm;
	unsigned int vertical_ppm;
	unsigned int number_of_colors;
	unsigned int number_of_important_colors;

} bmp_context;

int main (int argc, char** argv)
{
	int i, j;
	int fd, outfd;

	fd = open ("iutest512.bmp", O_RDONLY);
    outfd = open ("grayout512.bmp", O_WRONLY | O_CREAT);

	unsigned char buf[54] = {0, };
	
	read (fd, buf, 54);
    write (outfd, buf, 54);

	bmp_context bc;
	memcpy (&bc.bmp_width, &buf[18], sizeof (int));
	memcpy (&bc.bmp_height, &buf[22], sizeof (int));
	memcpy (&bc.bpp, &buf[28], sizeof (short));

	// info, little endian
	unsigned int width = bc.bmp_width;
	printf ("width: %u\n", width);

	unsigned int height = bc.bmp_height;
	printf ("height: %u\n", height);

	unsigned short bits = bc.bpp;
	printf ("bits: %u\n", bits);


	// RGB
	unsigned char** buf_r = (unsigned char**)malloc (sizeof (char*) * height);
	unsigned char** buf_g = (unsigned char**)malloc (sizeof (char*) * height);
	unsigned char** buf_b = (unsigned char**)malloc (sizeof (char*) * height);
	for (i=0; i<height; i++) {
		buf_r[i] = (unsigned char*)malloc (sizeof (char) * width);
		buf_g[i] = (unsigned char*)malloc (sizeof (char) * width);
		buf_b[i] = (unsigned char*)malloc (sizeof (char) * width);
	}

	unsigned char* buf_scanline = 
		(unsigned char*)malloc (sizeof (char) * width * 3);

	int ret;
	for (i=0; i<height; i++) {
		// BGR, from bottom
		ret = read (fd, buf_scanline, sizeof (char) * width * 3);
		//printf ("read ? %d\n", ret);
		for (j=0; j<width; j++) {
			buf_b[height - i - 1][j] = buf_scanline[j * 3];
			buf_g[height - i - 1][j] = buf_scanline[j * 3 + 1];
			buf_r[height - i - 1][j] = buf_scanline[j * 3 + 2];
		}
	}

	// convert grayscale
	unsigned char** gray = (unsigned char**)malloc (sizeof (char*) * height);
	for (i=0; i<height; i++) {
		gray[i] = (unsigned char*)malloc (sizeof (char) * width);
	}
	
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			gray[i][j] = 
			(unsigned char)((0.2126f * (float)buf_r[i][j]) + 
							0.7152f * (float)buf_g[i][j] + 
							0.0722f * (float)buf_b[i][j]);
		}
	}

	// write out
	// BGR, from bottom
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			write (outfd, &gray[height - i - 1][j], sizeof (char));
			write (outfd, &gray[height - i - 1][j], sizeof (char));
			write (outfd, &gray[height - i - 1][j], sizeof (char));
		}
	}

	close (fd);
	close (outfd);

	return 0;
}

