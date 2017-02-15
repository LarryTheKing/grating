/* Larry King 2017 
	Generates a circular polarization grating
	and exports as a vector graphics file (svg)
	(Uses svg rectangles)

	Compile using gcc grating2.cpp -lm -o grating2
	Run using ./grating2
*/

#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define FILENAME "out2.svg"

/* All units in micrometers unless otherwise noted */
#define RADIUS	22000
#define STEP	7
#define RWIDTH	5

/* Conversion factor millimeters/micrometer */
#define MM_UM	0.001

typedef int_fast64_t	DATA_TX;
typedef double			DATA_TY;

struct Rectangle
{

public:
	DATA_TX x;
	DATA_TX width;
	DATA_TY y;
	DATA_TY height;

	Rectangle(const DATA_TX x, const DATA_TY y, const DATA_TX width, const DATA_TY height)
		: x(x), y(y), width(width), height(height) {}
};

Rectangle calcRectangle(const DATA_TX x, const DATA_TX width = RWIDTH, const DATA_TX radius = RADIUS);

void printHeader(FILE * const pFile, const DATA_TX radius = RADIUS);

void printFooter(FILE * const pFile);

void printRectangle(FILE * const pFile, const Rectangle rectangle);

int main()
{
	DATA_TX const diameter = 2 * RADIUS;
	DATA_TX const step = STEP;
	DATA_TX const width = RWIDTH;

	FILE * pFile = fopen(FILENAME, "w");
	if (!pFile) {
		perror("Error opening file");
		return -1;
	}

	printHeader(pFile, RADIUS);

	for (DATA_TX x = step; x < diameter; x += step)
	{
		printRectangle(pFile, calcRectangle(x));
	}

	printFooter(pFile);

	fclose(pFile);

	return 0;
}

Rectangle calcRectangle(const DATA_TX x, const DATA_TX width, const DATA_TX radius)
{
	const DATA_TX   xCoord  = (x < radius ? radius - x: x - radius);
	const DATA_TY	halfHeight = sqrt(radius * radius - xCoord * xCoord);
	
	return Rectangle(
		x, (DATA_TY)(radius - halfHeight),
		width, 2 * halfHeight);

}

#define SVG_HEADER_1 "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
#define SVG_HEADER_2 "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%.3fmm\" height=\"%.3fmm\">\n"
void printHeader(FILE * const pFile,const DATA_TX radius)
{
	const double size = 2.0 * radius * MM_UM;

	fprintf(pFile, SVG_HEADER_1);
	fprintf(pFile, SVG_HEADER_2, size, size);
}

void printFooter(FILE * const pFile)
{
	fprintf(pFile, "</svg>\n");
}

#define SVG_RECTANGLE_STRING "\t<rect x=\"%.3fmm\" y=\"%fmm\" width=\"%.3fmm\" height=\"%fmm\" fill=\"black\"/>\n"
void printRectangle(FILE * const pFile, const Rectangle rectangle)
{
	double const x = rectangle.x 	  * MM_UM;
	double const y = rectangle.y 	  * MM_UM;
	double const w = rectangle.width  * MM_UM;
	double const h = rectangle.height * MM_UM;

	fprintf(pFile, SVG_RECTANGLE_STRING, x, y, w, h);
}