// map_generator.cpp : create a map to tell the difference of an image
// Tested Environment1: Ubuntu 13.04 Linux i686 with g++ 4.7.3
// Tested Environment2: Darwin OSX X86_64 with g++ 4.2.1
// NAME:	  Shulang Lei
// SID:		  200253624
// DATE:	  September 28th, 2013

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
uchar_t outputimage[BLOCKHEIGHT][BLOCKWIDTH];
uchar_t coloroutputimage[3][HEIGHT][WIDTH];

void
ProcessImage();

void
ReadImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteImage(char*, uchar_t[BLOCKHEIGHT][BLOCKWIDTH]);


//get block grey level according to image axis (x,y)
uchar_t
GetBlockLevelFromImage(
    uchar_t/*index representing which block of the image on X*/, 
    uchar_t/*index representing which block of the image on Y*/, 
    uchar_t[HEIGHT][WIDTH]/*the inputing image*/);

//convert source image x axis to x block axis
uchar_t
BlockXFromImg(
    uchar_t/*index of image on X*/); 

//convert source image y axis to y block axis
uchar_t
BlockYFromImg(
    uchar_t/*index of image on Y*/); 

/* math */
#define PI 3.1415926535897932384626433832795

/* error */
void
error(char*, ...);

/* main function */

int main(int argc, char *argv[])
{
	/* initialization */

	memset( inputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );
	memset( outputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );


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
	for(int i=0; i < BLOCKHEIGHT; i++)
          for(int j=0; j < BLOCKWIDTH; j++)
            outputimage[i][j] = GetBlockLevelFromImage(j, i, inputimage);
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
WriteImage(char *pszPath, uchar_t image[BLOCKHEIGHT][BLOCKWIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char *)"ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), BLOCKHEIGHT*BLOCKWIDTH, pRAW) <
		( BLOCKHEIGHT*BLOCKWIDTH) )
	{
		error((char *)"ERROR: Could not write image \"%s\".\n", pszPath);
	}

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
  int max_diff = 0;

  if (xEnd > WIDTH || yEnd > HEIGHT) /* boundary check */
    return max_diff;

  for(int i=yOffset; i < yEnd; i++) /* compare every pair of pixels within the block*/
    for(int j=xOffset; j < xEnd; j++)
      for(int k=yOffset; k < yEnd; k++)
        for(int l=xOffset; l < xEnd; l++)
          if (max_diff < (image[i][j] - image[k][l]))
            max_diff = (image[i][j] - image[k][l]); /* assgin the largest difference to be the block value */

  return max_diff;
}

uchar_t
BlockXFromImg(
    uchar_t x/*index of image on X*/)
{
  int unitWidth = WIDTH/BLOCKWIDTH;
  return (int)x/unitWidth;
}

uchar_t
BlockYFromImg(
    uchar_t y/*index of image on Y*/)
{
  int unitHeight = HEIGHT/BLOCKHEIGHT;
  return (int)y/unitHeight;
}
