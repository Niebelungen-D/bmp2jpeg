//
//  bmp.c
//  bmp2jpeg
//
//  Created by ubuntu-vm on 2014. 11. 11..
//  Copyright (c) 2014??Jaehwan Lee. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bmp.h"


void init_bmp_context (bmp_context_t* context, char* filepath)
{
	uint32_t i, j;
	FILE* fp = NULL;
	bmp_header_t header;

	// open file
	if ((fp = fopen (filepath, "r")) == NULL) {
		fprintf (stderr, "cannot open %s\n", filepath);
		exit (1);
 	}

	// read header
	memset (&header, 0x00, sizeof (bmp_header_t));
	fread (&header.signature, 1, BMP_HEADER_SIZE, fp);

	// check size
	if (header.bmp_width != 512 || header.bmp_height != 512) {
		fprintf (stderr, "bitmap must be 512x512\n");
		exit (1);
	}

	// check color components
	if (header.bpp != 24) {
		fprintf (stderr, "bitmap must be 24bit\n");
		exit (1);
	}

	// print info
	printf ("%s: %ux%u %ubpp\n", filepath, header.bmp_width, 
		header.bmp_height, header.bpp);

	context->width = context->height = 512;

	// init context
    uint32_t width = context->width;
    uint32_t height = context->height;

    context->channel_red = (uint8_t*)malloc (width * height);
    context->channel_green = (uint8_t*)malloc (width * height);
    context->channel_blue = (uint8_t*)malloc (width * height);

	// read rgb channels
	uint8_t* scanline = (uint8_t*)malloc (width * 3);
	for (i=0; i<height; i++) {
		// 
		uint32_t idx = (height - i - 1) * width;

		// read a scanline of rgb components
		fread (scanline, 1, width * 3, fp);

		// extract rgb components
		for (j=0; j<width; j++) {
			context->channel_blue[idx + j] = scanline[j * 3];
			context->channel_green[idx + j] = scanline[j * 3 + 1];
			context->channel_red[idx + j] = scanline[j * 3 + 2];
		}
	}

	// release resources
	free (scanline);
	close (fp);

	printf ("bmp read done.\n");
}

void deinit_bmp_context (bmp_context_t* context)
{
	free (context->channel_red);
	context->channel_red = NULL;
	free (context->channel_green);
	context->channel_green = NULL;
	free (context->channel_blue);
	context->channel_blue = NULL;
}
