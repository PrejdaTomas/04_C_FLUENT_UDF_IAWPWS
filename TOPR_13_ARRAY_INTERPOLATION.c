#include "TOPR_13_ARRAY_INTERPOLATION.h"
double interpolation1D_linear(Array1D xValues, Array1D yValues, double x)
{
    fprintfWrapper_newline_head("interpolation1D_linear: start! -> %lf", x);
    double xDiff, yDiff, slope, intercept;
    unsigned int this,  next;
    unsigned int closest = findClosest1D(x, xValues);
    if (closest != xValues.last )
    {
        this = closest;
        next = this + 1;
    }

    else
    {
        this = closest - 1;
        next = this + 1;
    }

    xDiff = xValues.array[next] - xValues.array[this];
    yDiff = yValues.array[next] - yValues.array[this];
    slope = yDiff/xDiff;
    intercept = yValues.array[this];
    double result = slope*(x - xValues.array[this]) + intercept;
    fprintfWrapper_newline_tail("interpolation1D_linear: success -> [%lf, %lf]", x, result);
    return result;
}


double interpolation2D_bilinear(Array2D arrayIpt, double x, double y, enum tableColumn selectedColumn, Array1D uniqueX, Array1D uniqueY)
{
	fprintfWrapper_newline_head("interpolation2D_bilinear: start -> [%lf %lf %d]", x, y, selectedColumn);
	if ((x < arrayIpt.array[0][0]) || (y > arrayIpt.array[arrayIpt.lastRow][0]))
	{
		fprintfWrapper_newline_tail("interpolation2D_bilinear: failure -> extrapolation (in 1. independent) variable not supported! %lf not in <%lf, %lf>",
		x, arrayIpt.array[0][0], arrayIpt.array[arrayIpt.lastRow][0]
		);
		exit(1);
	}

	if ((y < arrayIpt.array[0][1]) || (y > arrayIpt.array[arrayIpt.lastRow][1]))
	{
		fprintfWrapper_newline_tail("interpolation2D_bilinear: failure -> extrapolation (in 2. independent) variable not supported! %lf not in <%lf, %lf>",
		y, arrayIpt.array[0][1], arrayIpt.array[arrayIpt.lastRow][1]
		);
		exit(1);
	}

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: passed the range checks!");
	double result;


	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: finding the closest value using the binary search in the read table!");
	int closest_eff	= findClosest2D_efficient(x, y, uniqueX, uniqueY);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: creating the bounding recangle to the closest value index: %d!", closest_eff);
	Array2D rect	= getRectangle(closest_eff, arrayIpt, uniqueY.count);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: finding the closest value in the read table!");

	double *point_11 = rect.array[TL]; //point11
	double *point_21 = rect.array[BL]; //point21
	double *point_12 = rect.array[TR]; //point12
	double *point_22 = rect.array[BR]; //point22
	
	double x1 = point_11[T]; //point_11.x
	double y1 = point_11[P]; //point_11.y
	double x2 = point_22[T]; //point_22.x
	double y2 = point_22[P]; //point_22.y

	if ((x1 == x2) || (y1 == y2))
	{
		fprintfWrapper_newline_tail("interpolation2D_bilinear: failure -> not a rectangle!");
		exit(1);
	}



	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: points:");
	fprintfWrapper_newline_mid_lower("point_11[%lf, %lf, %lf]", point_11[T], point_11[P], point_11[selectedColumn]);
	fprintfWrapper_newline_mid_lower("point_21[%lf, %lf, %lf]", point_21[T], point_21[P], point_21[selectedColumn]);
	fprintfWrapper_newline_mid_lower("point_12[%lf, %lf, %lf]", point_12[T], point_12[P], point_12[selectedColumn]);
	fprintfWrapper_newline_mid_lower("point_22[%lf, %lf, %lf]", point_22[T], point_22[P], point_22[selectedColumn]);
	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: x1: %lf, x2: %lf, y1: %lf, y2: %lf", x1, x2, y1, y2);
	


	double dxdy = (x2-x1)*(y2-y1);
	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: dxdy: %lf", dxdy);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: interpolation members"); 
	double left_top		= point_11[selectedColumn] * (x2 - x) * (y2 - y);
	fprintfWrapper_newline_mid_lower("left_top -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_11[selectedColumn],x2,x,y2,y,
	point_11[selectedColumn], (x2 - x), (y2 - y),
	point_11[selectedColumn] * (x2 - x) * (y2 - y)
	);

	double right_top	= point_21[selectedColumn] * (x - x1) * (y2 - y);
	fprintfWrapper_newline_mid_lower("right_top -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_21[selectedColumn],x,x1,y2,y,
	point_21[selectedColumn], (x - x1), (y2 - y),
	point_21[selectedColumn] * (x - x1) * (y2 - y)
	);

	double left_bot		= point_12[selectedColumn] * (x2 - x) * (y - y1);
	fprintfWrapper_newline_mid_lower("left_bot -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_12[selectedColumn],x2,x,y,y1,
	point_12[selectedColumn], (x2 - x), (y - y1),
	point_12[selectedColumn] * (x2 - x) * (y - y1)
	);

	double right_bot	= point_22[selectedColumn] * (x - x1) * (y - y1);
	fprintfWrapper_newline_mid_lower("right_bot -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_22[selectedColumn],x,x1,y,y1,
	point_22[selectedColumn], (x - x1), (y - y1),
	point_22[selectedColumn] * (x - x1) * (y - y1)
	);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: [[%lf, %lf], [%lf, %lf]]", left_top/dxdy, right_top/dxdy, left_bot/dxdy, right_bot/dxdy);
	result = (left_top + right_top + left_bot + right_bot)/dxdy;

	fprintfWrapper_newline_tail("interpolation2D_bilinear: success! -> %lf", result);
	return result;
}