// surface_image.cpp : create a 511 by 511 image of the surface given by the function f(x,y) = 1 - sin(sqrt(x*x+y*y))
// Tested Environment1: Darwin OSX X86_64 with g++ 4.2.1
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

#define HEIGHT	512
#define WIDTH	512

uchar_t	inputimage[HEIGHT][WIDTH];
uchar_t outputimage[HEIGHT][WIDTH];
uchar_t coloroutputimage[3][HEIGHT][WIDTH];

void
ProcessImage();

void
WriteImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteColorImage(char*, uchar_t[3][HEIGHT][WIDTH]);

double 
target_function(double);

double
domain_of(int, int);

uchar_t
codomain_of(double);

//8 path length from point x to point y
int len8path(int/*x1*/, int/*x2*/, int/*y1*/, int/*y1*/);


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
  // TODO: Modify image as needed, saving result into
  // "outputimage" or "coloroutputimage".
  //debug((char *)"DEBUG: domain is \"%f\".\n", ydomain_of(0));
  //debug((char *)"DEBUG: domain is \"%f\".\n", ydomain_of(255));
  //debug((char *)"DEBUG: domain is \"%f\".\n", ydomain_of(256));
  //debug((char *)"DEBUG: domain is \"%f\".\n", ydomain_of(511));
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

void
WriteColorImage(char *pszPath, uchar_t image[3][HEIGHT][WIDTH])
{
	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char*)"ERROR: Could not create image \"%s\".\n", pszPath);

	for(int i=0; i < HEIGHT; i++)
		for(int j=0; j < WIDTH; j++)
			for(int k=0; k < 3; k++)
				if( fwrite( &image[k][i][j], sizeof(uchar_t), 1, pRAW ) < 1 )
					error((char*)"ERROR: Could not write image \"%s\".\n", pszPath);

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

/*debug*/
void 
debug(char *psz, ...)
{
	va_list ap;
	va_start( ap, psz );
	vfprintf( stdout, psz, ap );
	va_end(ap);
}

double target_function(double x) 
{
  return 1-sin(sqrt(x*x+x*x));
}

double
domain_of(int x, int y)
{  
  if (x<0 || x>WIDTH) 
    error((char*)"ERROR: x index out of boundaries \"%i\".\n", x);
  if (y<0 || y>HEIGHT) 
    error((char*)"ERROR: y index out of boundaries \"%i\".\n", y);
  int longest = ((HEIGHT/2) > (WIDTH/2)) ? (HEIGHT/2) : (WIDTH/2);
  double unit = PI/2/longest;
  int distance_to_center = len8path(WIDTH/2, HEIGHT/2, x, y);
  return distance_to_center*unit;
}

uchar_t
codomain_of(double target)
{
  if (target<0.0 || target>1.0) 
    error((char*)"ERROR: target function out of boundaries \"%f\".\n", target);
  double unit = 1.0 / 255;
  return target/unit;
}

//8 path length from point x to point y
int len8path(int x1/*x1*/, int x2/*x2*/, int y1/*y1*/, int y2/*y1*/)
{
  int xdiff = std::abs(x1-y1);
  int ydiff = std::abs(x2-y2);
  if (xdiff > ydiff)
    return xdiff;
  return ydiff;
}
