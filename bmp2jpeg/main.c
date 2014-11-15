//
//  main.c
//  bmp2jpeg
//
//  Created by ubuntu-vm on 2014. 11. 11..
//  Copyright (c) 2014??Jaehwan Lee. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bmp.h"
#include "jpegenc.h"

int main (int argc, char** argv)
{
	bmp_context_t bmp;

	if (argc != 2) {
		fprintf (stderr, "Usage: %s bitmap_image.bmp\n", argv[0]);
		return 1;
	}

	init_bmp_context (&bmp, argv[1]);
	//bmp_write_grayscale (&bmp, "grayout.bmp");
	bmp_convert_grayscale (&bmp);
	jpeg_write_grayscale (&bmp, "grayout.jpg");
	deinit_bmp_context (&bmp);

	return 0;
}
