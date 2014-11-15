
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <jpeglib.h>

/* we will be using this uninitialized pointer later to store raw, uncompressd image */
uint8_t *raw_image = NULL;

/* dimensions of the image we want to write */
int width;
int height;
int bytes_per_pixel;   /* or 1 for GRACYSCALE images */
int color_space; /* or JCS_GRAYSCALE for grayscale images */

#define BMP_HEADER_SIZE 54
typedef struct __bmp_header {

  uint16_t padding;
  uint16_t signature;
  uint32_t bmp_file_size;
  uint16_t reserve_1;
  uint16_t reserve_2;
  uint32_t soi_offset;
  uint32_t bmp_header_size;
  uint32_t bmp_width;
  uint32_t bmp_height;  
  uint16_t number_of_planes;
  uint16_t bpp;
  uint32_t compression_type;
  uint32_t img_data_size;
  uint32_t horizontal_ppm;
  uint32_t vertical_ppm;
  uint32_t number_of_colors;
  uint32_t number_of_important_colors;

} bmp_header_t;

int write_bmp_file( char *filename )
{
    bmp_header_t bh;

    memset (&bh, 0, BMP_HEADER_SIZE); /* sets everything to 0 */

    //bh.filesize  =   calculated size of your file (see below)
    //bh.reserved  = two zero bytes
    memcpy (&bh.signature, "BM", 2);

    bh.bmp_file_size    = BMP_HEADER_SIZE + 3 * width * height;
    bh.soi_offset       = 0x36;//  (for 24 bit images)
    bh.bmp_header_size  = 0x28;//  (for 24 bit images)
    bh.bmp_width        = width ;//in pixels of your image
    bh.bmp_height       = height;// in pixels of your image
    bh.number_of_planes = 0x01;//(for 24 bit images)
    bh.bpp              = 0x18;//(for 24 bit images)
    bh.compression_type = 0;//  (no compression)
    bh.img_data_size    = bh.bmp_file_size - BMP_HEADER_SIZE;

    

    FILE* bmpfile = fopen(filename, "wb");
    if (bmpfile == NULL) {
      printf("Error opening output file\n");
      /* -- close all open files and free any allocated memory -- */
      exit (1);
    }
    
    // write header
    fwrite (&bh.signature, 1, BMP_HEADER_SIZE, bmpfile);

    // write image
    uint32_t bytes_per_line = 3 * width;
    uint32_t bytes_per_pixel = 3;
    int32_t line, x;

    uint8_t* line_buffer;
    line_buffer = (uint8_t *) calloc(1, bytes_per_line);
    if (line_buffer == NULL) {
        printf ("Error allocating memory\n");
        free(raw_image);
         /* -- close all open files and free any allocated memory -- */
         exit (1);   
    }

    for (line = height-1; line >= 0; line --) {
        /* fill line line_buffer with the image data for that line */
        for( x =0 ; x < width; x++ ) {
            *(line_buffer + x * bytes_per_pixel) = 
                *(raw_image + (x + line * width) * bytes_per_pixel + 2);
            *(line_buffer+x*bytes_per_pixel+1) = 
                *(raw_image + (x + line * width) * bytes_per_pixel + 1);
            *(line_buffer+x*bytes_per_pixel+2) = 
                *(raw_image + (x + line * width) * bytes_per_pixel + 0);
        }

        /* remember that the order is BGR and if width is not a multiple
         * of 4 then the last few bytes may be unused
         */
        fwrite(line_buffer, 1, bytes_per_line, bmpfile);
    }

    free (line_buffer);
    fclose (bmpfile);
}



/**
 * read_jpeg_file Reads from a jpeg file on disk specified by filename and saves into the 
 * raw_image buffer in an uncompressed format.
 * 
 * \returns positive integer if successful, -1 otherwise
 * \param *filename char string specifying the file name to read from
 *
 */
int read_jpeg_file( char *filename )
{
    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1];

    FILE *infile = fopen( filename, "rb" );
    unsigned long location = 0;
    int i = 0;

    if ( !infile )
    {
      printf("Error opening jpeg file %s\n!", filename );
      return -1;
    }
    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );
    /* reading the image header which contains image information */
    jpeg_read_header( &cinfo, TRUE );
    /* Uncomment the following to output image information, if needed. */

    printf( "JPEG File Information: \n" );
    printf( "Image width and height: %d pixels and %d pixels.\n", width=cinfo.image_width, height=cinfo.image_height );
    printf( "Color components per pixel: %d.\n", bytes_per_pixel = cinfo.num_components );
    printf( "Color space: %d.\n", cinfo.jpeg_color_space );

    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    /* allocate memory to hold the uncompressed image */
    raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    /* read one scan line at a time */
    while( cinfo.output_scanline < cinfo.image_height )
    {
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );
        for( i=0; i<cinfo.image_width*cinfo.num_components;i++) 
        raw_image[location++] = row_pointer[0][i];
    }
    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    free( row_pointer[0] );
    fclose( infile );
    /* yup, we succeeded! */
    return 1;
}


int main (int argc,char **argv)
{
    int x,y;

    if(argc != 3){ 
      printf("Usage: %s source.jpg dest.bmp\n",argv[0]); 
      return -1; 
    }
    x = y = 0;

    /* Try opening a jpeg*/
    if( read_jpeg_file( argv[1] ) > 0 ) {
      write_bmp_file( argv[2] );
    } else { 
      return -1;
    }

    free(raw_image);
    return 0;
}
