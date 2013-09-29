// map_generator.cpp : create a map to tell the difference of an image
// Execution Environment: Ubuntu 13.04 Linux i686 with g++ 4.7.3
// NAME:	  Shulang Lei
// SID:		  200253624
// DATE:	  September 28th, 2013
//

#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <math.h>
#include <stdio.h>

/* image */

typedef unsigned char uchar_t;

#define HEIGHT	256
#define WIDTH	256
#define BLOCKHEIGHT	8
#define BLOCKWIDTH	8

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

uchar_t
GetBlockLevelFromImage(
    uchar_t/*index representing which block of the image on X*/, 
    uchar_t/*index representing which block of the image on Y*/, 
    uchar_t[HEIGHT][WIDTH]/*the inputing image*/);


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
void
debug(char*, ...);


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
		error((char *)"USAGE: %s <image.raw>\n",  argv[0]);
	}


	/* read image */

	ReadImage( argv[1], inputimage );


	/* process image */

	ProcessImage();


	/* write image */

	WriteImage((char *)"output_image.raw", outputimage );
		
	return 0;
}


/* image */

void
ProcessImage()
{
	//
	// TODO: Modify image as needed, saving result into
	// "outputimage" or "coloroutputimage".
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(0, 0, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(1, 1, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(2, 2, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(3, 3, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(4, 4, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(5, 5, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(6, 6, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(7, 7, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(8, 8, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(9, 9, inputimage));
	debug((char *)"DEBUG: the block level is \"%u\".\n", GetBlockLevelFromImage(10, 10, inputimage));
}

void
ReadImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "rb" ) ) == NULL )
		error((char *)"ERROR: Could not open image \"%s\".\n", pszPath);

	if( fread( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW ) <
		( HEIGHT*WIDTH ) )
	{
		error((char *)"ERROR: Could not read image \"%s\".\n", pszPath);
	}

	fclose(pRAW);
}

void
WriteImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char *)"ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW) <
		( HEIGHT*WIDTH) )
	{
		error((char *)"ERROR: Could not write image \"%s\".\n", pszPath);
	}

	fclose(pRAW);

}

void
WriteColorImage(char *pszPath, uchar_t image[3][HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char *)"ERROR: Could not create image \"%s\".\n", pszPath);

	for(int i=0; i < HEIGHT; i++)
		for(int j=0; j < WIDTH; j++)
			for(int k=0; k < 3; k++)
				if( fwrite( &image[k][i][j], sizeof(uchar_t), 1, pRAW ) < 1 )
					error((char *)"ERROR: Could not write image \"%s\".\n", pszPath);

	fclose(pRAW);
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

/*debug*/
void debug(char *psz, ...)
{
	va_list ap;
	
	va_start( ap, psz );
	vfprintf( stdout, psz, ap );
	va_end(ap);
}

uchar_t GetBlockLevelFromImage(
    uchar_t blocknumX/*index representing which block of the image on X*/, 
    uchar_t blocknumY/*index representing which block of the image on Y*/, 
    uchar_t image[HEIGHT][WIDTH]/*the inputing image*/)
{
  int unitWidth = WIDTH/BLOCKWIDTH;
  int unitHeight = HEIGHT/BLOCKHEIGHT;
  int xOffset = blocknumX*unitWidth;
  int yOffset = blocknumY*unitHeight;
  int xEnd = xOffset+unitWidth-1;
  int yEnd = yOffset+unitHeight-1;
  if (xEnd > WIDTH || yEnd > HEIGHT) {
    return 0;
  }

  int max_diff = 0;
  for(int i=xOffset; i < xEnd; i++)
    for(int j=yOffset; j < yEnd; j++)
      for(int k=xOffset; k < xEnd; k++)
        for(int l=yOffset; l < yEnd; l++)
          if (max_diff < (image[i][j] - image[k][l]))
            max_diff = (image[i][j] - image[k][l]);

  //debug((char *)"DEBUG: the x start is \"%u\".\n", xOffset);
  //debug((char *)"DEBUG: the y start is \"%u\".\n", yOffset);
  //debug((char *)"DEBUG: the x end is \"%u\".\n", xEnd);
  //debug((char *)"DEBUG: the y end is \"%u\".\n", yEnd);
  debug((char *)"DEBUG: the max_diff is \"%u\".\n", max_diff);
  return 1;
}
