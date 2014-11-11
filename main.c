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

int main (int argc, char** argv)
{
	bmp_context_t bmp;

	init_bmp_context (&bmp, "iutest512.bmp");
	bmp_write_grayscale (&bmp, "grayout.bmp");
	deinit_bmp_context (&bmp);

	return 0;
}