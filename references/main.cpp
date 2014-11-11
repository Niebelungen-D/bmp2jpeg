/***************************************************************************/
/*                                                                         */
/*  File: main.cpp                                                         */
/*  Autor: bkenwright@xbdev.net                                            */
/*  URL: www.xbdev.net                                                     */
/*                                                                         */
/***************************************************************************/
/*
	Jpeg File Format Explained
*/
/***************************************************************************/

#include <windows.h>

#include <stdio.h>		// sprintf(..), fopen(..)
#include <stdarg.h>     // So we can use ... (in dprintf)


#include "loadjpg.h"	// ConvertJpgFile(..)

#include "savejpg.h"    // SaveJpgFile(..)

/***************************************************************************/
/*                                                                         */
/* FeedBack Data                                                           */
/*                                                                         */
/***************************************************************************/

//Saving debug information to a log file
void dprintf(const char *fmt, ...) 
{
/*
	va_list parms;
	char buf[256];

	// Try to print in the allocated space.
	va_start(parms, fmt);
	vsprintf (buf, fmt, parms);
	va_end(parms);

	// Write the information out to a txt file
	FILE *fp = fopen("output.txt", "a+");
	fprintf(fp, "%s", buf);
	fclose(fp);
*/
}// End dprintf(..)


 
/***************************************************************************/
/*                                                                         */
/* Entry Point                                                             */
/*                                                                         */
/***************************************************************************/


int __stdcall WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{

	// Create a jpg from a bmp
	//SaveJpgFile("smiley.bmp", "ex.jpg");

	// Create a bmp from a jpg
	ConvertJpgFile("smiley.jpg", "smiley.bmp");

	//ConvertJpgFile("imageJPEG3_1.jpg", "imageJPEG3_1.bmp");
	//ConvertJpgFile("imageJPEG3_2.jpg", "imageJPEG3_2.bmp");

	//ConvertJpgFile("test.jpg", "test.bmp");

	//ConvertJpgFile("cross.jpg", "cross.bmp");

	return 0;
}// End WinMain(..)


