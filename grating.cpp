/* Larry King 2017 
	Generates a circular polarization grating
	and exports as a vector graphics file (svg)

	Compile using gcc grating.cpp -lm -o grating
	Run using ./grating
*/

#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define FILENAME "out.svg"

/* All units in micrometers unless otherwise noted */
#define RADIUS	22000
#define STEP	7
#define LWIDTH	5

/* Conversion factor millimeters/micrometer */
#define MM_UM	0.001

typedef int_fast64_t	DATA_TX;
typedef double			DATA_TY;

struct Point
{

public:
	DATA_TX x;
	DATA_TY y;

	Point(const DATA_TX x, const DATA_TY y)
		: x(x), y(y) {}
};

struct Line
{

public:
	Point p1;
	Point p2;

	Line(const Point p1, const Point p2)
		: p1(p1), p2(p2) {}
};

Line calcLine(const DATA_TX x, const DATA_TX radius = RADIUS);

void printHeader(FILE * const pFile, const DATA_TX radius = RADIUS);

void printFooter(FILE * const pFile);

void printLine(FILE * const pFile, const Line line, const DATA_TX width = LWIDTH);

int main()
{
	DATA_TX const twoRadius = 2 * RADIUS;
	DATA_TX const step = STEP;
	DATA_TX const width = LWIDTH;

	FILE * pFile = fopen(FILENAME, "w");
	if (!pFile) {
		perror("Error opening file");
		return -1;
	}

	printHeader(pFile, RADIUS);

	for (DATA_TX x = 1; x < twoRadius; x += step)
	{
		printLine(pFile, calcLine(x));
	}

	printFooter(pFile);

	fclose(pFile);

	return 0;
}

Line calcLine(const DATA_TX x, const DATA_TX radius)
{
	const DATA_TX   xCoord  = (x < radius ? radius - x: x - radius);
	double const	yOffset = sqrt(radius * radius - xCoord * xCoord);
	
	return Line(
		Point(x, (DATA_TY)(radius - yOffset)),
		Point(x, (DATA_TY)(radius + yOffset)));
}

#define SVG_HEADER_1 "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
#define SVG_HEADER_2 "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%fmm\" height=\"%fmm\">\n"
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

#define SVG_LINE_STRING "\t<line x1=\"%fmm\" y1=\"%fmm\" x2=\"%fmm\" y2=\"%fmm\" stroke=\"black\" stroke-width=\"%fmm\"/>\n"
void printLine(FILE * const pFile, const Line line, const DATA_TX width)
{
	double const x1 = line.p1.x * MM_UM;
	double const y1 = line.p1.y * MM_UM;
	double const x2 = line.p2.x * MM_UM;
	double const y2 = line.p2.y * MM_UM;
	double const w = width * MM_UM;

	fprintf(pFile, SVG_LINE_STRING, x1, y1, x2, y2, w);
}