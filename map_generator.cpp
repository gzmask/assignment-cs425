// map_generator.cpp : create a map to tell the difference of an image
//
// NAME:	  Shulang Lei
// SID:		  200253624
// DATE:	  September 28th, 2013
//

//#include "stdafx.h"		/* keep this ONLY IF using Microsoft Visual Studio */
#include <cstring>
#include <cstdarg>
#include <cstdlib>

#ifndef WIN32
	#include <math.h>
	#include <stdio.h>
#endif


/* image */

typedef unsigned char uchar_t;

#define HEIGHT	256
#define WIDTH	256

uchar_t	inputimage[HEIGHT][WIDTH];
uchar_t outputimage[HEIGHT][WIDTH];
uchar_t coloroutputimage[3][HEIGHT][WIDTH];

void
ProcessImage();

void
ReadImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteColorImage(char*, uchar_t[3][HEIGHT][WIDTH]);


/* math */

#define PI 3.1415926535897932384626433832795

class Complex {
public:
	/* construction/destruction */
	Complex(double r, double i)		{ this->r = r; this->i = i; }
	Complex()						{ r=0.0; i=0.0; }
	~Complex()						{ r=0.0; i=0.0; }

	/* operations */
	Complex operator+(Complex &c)	{ return Complex( r+c.r, i+c.i ); }

	//
	// TODO: Add functions for subtraction, multiplication, modulus, etc.
	//
	
	/* members */
	double r, i;
};


/* error */

void
error(char*, ...);


/* main function */

int main(int argc, char *argv[])
{
	/* initialization */

	memset( inputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );
	memset( outputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );
	memset( coloroutputimage, 0, 3*HEIGHT*WIDTH*sizeof(uchar_t) );


	/* command line arguments */

	if(argc != 2)
	{
#ifdef WIN32
		error("USAGE: %s <image.{bmp|gif|jpg|png}>\n", argv[0]);
#else
		error("USAGE: %s <image.raw>\n", argv[0]);
#endif
	}


	/* read image */

	ReadImage( argv[1], inputimage );


	/* process image */

	ProcessImage();


	/* write image */

#ifdef WIN32
	WriteImage( "output_image.png", outputimage );
#else
	WriteImage( "output_image.raw", outputimage );
#endif
		
	return 0;
}


/* image */

void
ProcessImage()
{
	//
	// TODO: Modify image as needed, saving result into
	// "outputimage" or "coloroutputimage".
	//
}

void
ReadImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

#ifdef WIN32

	CImage img;
	
	if( img.Load( CString(pszPath) ) != S_OK )
		error("ERROR: Could not open image \"%s\".\n", pszPath);

	if( ( img.GetHeight() != HEIGHT ) || ( img.GetWidth() != WIDTH ) )
		error("ERROR: Image \"%s\" is not %dx%d.\n", pszPath, HEIGHT, WIDTH);

	if( img.GetBPP() != 8 )
		error("ERROR: Image \"%s\" is not 8-bit.\n", pszPath);

	for(int i=0; i < HEIGHT; i++)
		for(int j=0; j < WIDTH; j++)
			image[i][j] = (uchar_t) *( (uchar_t*)img.GetPixelAddress(j, i) );

	img.Destroy();

#else

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "rb" ) ) == NULL )
		error("ERROR: Could not open image \"%s\".\n", pszPath);

	if( fread( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW ) <
		( HEIGHT*WIDTH ) )
	{
		error("ERROR: Could not read image \"%s\".\n", pszPath);
	}

	fclose(pRAW);

#endif	/* WIN32 */

}

void
WriteImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

#ifdef WIN32

	CImage	img;
	RGBQUAD	palette[256];

	memset( palette, 0, 256*sizeof(RGBQUAD) );
	
	for(int i=0; i < 256; i++)
	{
		palette[i].rgbRed = (BYTE)i;
		palette[i].rgbGreen = (BYTE)i;
		palette[i].rgbBlue = (BYTE)i;
	}
	
	if( !img.Create(WIDTH, HEIGHT, 8) )
		error("ERROR: Could not create image \"%s\".\n", pszPath);

	img.SetColorTable( 0, 256, palette );

	for(int i=0; i < HEIGHT; i++)
		for(int j=0; j < WIDTH; j++)
			*( (uchar_t*)img.GetPixelAddress(j, i) ) = image[i][j];
	
	if( img.Save( CString(pszPath), Gdiplus::ImageFormatPNG ) != S_OK )
		error("ERROR: Could not write image \"%s\".\n", pszPath);

	img.Destroy();

#else

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error("ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW) <
		( HEIGHT*WIDTH) )
	{
		error("ERROR: Could not write image \"%s\".\n", pszPath);
	}

	fclose(pRAW);

#endif	/* _WIN32_ */

}

void
WriteColorImage(char *pszPath, uchar_t image[3][HEIGHT][WIDTH])
{

#ifdef WIN32

	CImage img;

	if( !img.Create(WIDTH, HEIGHT, 24) )
		error("ERROR: Could not create image \"%s\".\n", pszPath);

	for(int i=0; i < HEIGHT; i++)
	{
		for(int j=0; j < WIDTH; j++)
		{
			img.SetPixelRGB( j,
							 i,
							 image[0][i][j],
							 image[1][i][j],
							 image[2][i][j] );
		}
	}

	if( img.Save( CString(pszPath), Gdiplus::ImageFormatPNG ) != S_OK )
		error("ERROR: Could not write image \"%s\".\n", pszPath);

	img.Destroy();

#else

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error("ERROR: Could not create image \"%s\".\n", pszPath);

	for(int i=0; i < HEIGHT; i++)
		for(int j=0; j < WIDTH; j++)
			for(int k=0; k < 3; k++)
				if( fwrite( &image[k][i][j], sizeof(uchar_t), 1, pRAW ) < 1 )
					error("ERROR: Could not write image \"%s\".\n", pszPath);

	fclose(pRAW);

#endif	/* WIN32 */

}


/* error */

void error(char *psz, ...)
{
	va_list ap;
	
	va_start( ap, psz );
	vfprintf( stderr, psz, ap );
	va_end(ap);

	exit(-1);
}
