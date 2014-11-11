//
//  jpegenc.h
//  bmp2jpeg
//
//  abstract: encodes grayscale image of 512x512 from bmp context
//
//  Created by ubuntu-vm on 2014. 11. 11..
//  Copyright (c) 2014??Jaehwan Lee. All rights reserved.
//


#ifndef __jpegenc_h__
#define __jpegenc_h__

#include "bmp.h"

#define JPEG_QUALITY_PERCENT 100

int jpeg_write_grayscale (bmp_context_t* context, const char* dest);

#endif //__jpegenc_h__