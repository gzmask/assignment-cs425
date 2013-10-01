// surface_image.cpp : create a 511 by 511 image of the surface given by the function f(x) = 1 - sin(sqrt(x*x+x*x))
// Tested Environment: Darwin OSX X86_64 with g++ 4.2.1
//
// NAME:	  Shulang Lei
// SID:		  200253624
// DATE:	  September 29th, 2013
//

#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <math.h>
#include <stdio.h>

/* image */

typedef unsigned char uchar_t;

#define HEIGHT  512
#define WIDTH   512	

uchar_t	inputimage[HEIGHT][WIDTH];
uchar_t outputimage[HEIGHT][WIDTH];

void
ProcessImage();

void
WriteImage(char*, uchar_t[HEIGHT][WIDTH]);

double 
target_function(double); // this is the function that we are drawing out of

double
domain_of(int, int); //this converts the image pixel numbers to proper domain of the target function [0,1]

uchar_t
codomain_of(double); //this converts the target_function output to prpoer range of acceptable grey levels [0,255]

int len8path(int/*x1*/, int/*x2*/, int/*y1*/, int/*y1*/); //calculates the 8-path length from point x to point y

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


	/* process image */
	ProcessImage();


	/* write image */
	WriteImage((char*)"output_image.raw", outputimage );
		
	return 0;
}


/* image */

void
ProcessImage()
{
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++)
      outputimage[i][j]=codomain_of(target_function(domain_of(j,i))); 
}

void
WriteImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char*)"ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW) <
		( HEIGHT*WIDTH) )
	{
		error((char*)"ERROR: Could not write image \"%s\".\n", pszPath);
	}

	fclose(pRAW);
}

/* error */
void 
error(char *psz, ...)
{
	va_list ap;
	va_start( ap, psz );
	vfprintf( stderr, psz, ap );
	va_end(ap);
	exit(-1);
}

double target_function(double x) 
{
  return 1-sin(sqrt(x*x+x*x));
}

double
domain_of(int x, int y)
{  
  if (x<0 || x>WIDTH) /* boundary check*/
    error((char*)"ERROR: x index out of boundaries \"%i\".\n", x);
  if (y<0 || y>HEIGHT) /* boundary check*/
    error((char*)"ERROR: y index out of boundaries \"%i\".\n", y);
  int longest = ((HEIGHT/2) > (WIDTH/2)) ? (HEIGHT/2) : (WIDTH/2);
  double unit = (PI/2)/longest;
  int distance_to_center = len8path(WIDTH/2, HEIGHT/2, x, y);
  return distance_to_center*unit;
}

uchar_t
codomain_of(double target)
{
  if (target<0.0 || target>1.0) /* boundary check*/
    error((char*)"ERROR: target function out of boundaries \"%f\".\n", target);
  int longest = ((HEIGHT/2) > (WIDTH/2)) ? (HEIGHT/2) : (WIDTH/2);
  double unit = 1.0 / (longest-1);
  return target/unit;
}

int len8path(int x1/*x1*/, int x2/*x2*/, int y1/*y1*/, int y2/*y1*/)
{
  int xdiff = std::abs(x1-y1);
  int ydiff = std::abs(x2-y2);
  if (xdiff > ydiff)
    return xdiff;
  return ydiff;
}
