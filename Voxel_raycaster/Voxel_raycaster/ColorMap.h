#ifndef COLORMAP_H
#define COLORMAP_H

inline double interpolate(double val, double y0, double x0, double y1, double x1) {
	return (val - x0)*(y1 - y0) / (x1 - x0) + y0;
}


inline void color_map2(double& R, double& G, double& B, double current_value, double max_value)
{
	// our interval starts at 0.0
	double LEFT_END = -0.5;
	double ONE_FIFTH = max_value / 5.0;
	double TWO_FITHS = max_value * 2.0 / 5.0;
	double THREE_FIFTHS = max_value * 3.0 / 5.0;
	double FOUR_FIFTHS = max_value * 4.0 / 5.0;
	double& RIGHT_END = max_value;


	if (current_value <= max_value / 5.0)
	{
		R = 0.0;
		G = 0.0;
		B = interpolate(current_value, 0.0,  LEFT_END, 1.0, ONE_FIFTH);	
	}
	else if(current_value <= max_value *  2.0/ 5.0)
	{
		R = 0.0;
		G = interpolate(current_value, 0.0, ONE_FIFTH, 1.0, TWO_FITHS );
		B = 1.0;
	}
	else if (current_value <= max_value *  3.0 / 5.0)
	{
		R = interpolate(current_value, 0.0, TWO_FITHS, 1.0, THREE_FIFTHS);
		G = 1.0;
		B = interpolate(current_value, 1.0 , TWO_FITHS, 0.0, THREE_FIFTHS);	
	}
	else if (current_value <= max_value *  4.0 / 5.0)
	{
		R = 1.0;
		//interpolate(double val, double y0, double x0, double y1, double x1)
		G = interpolate(current_value, 1.0, THREE_FIFTHS, 0.0, FOUR_FIFTHS);
		B = 0.0;		
	} 
	else
	{
		//	interpolate(double val, double y0, double x0, double y1, double x1) 
		R = interpolate(current_value, 1.0, FOUR_FIFTHS, 0.0, RIGHT_END);
		G = 0.0;
		B = 0.0;	
	}

}


typedef struct {
	double r, g, b;
} MAP_COLOUR;

inline MAP_COLOUR GetColour(double v, double vmin, double vmax)
{
	MAP_COLOUR c = { 1.0,1.0,1.0 }; // white
	double dv;

	if (v < vmin)
		v = vmin;
	if (v > vmax)
		v = vmax;
	dv = vmax - vmin;

	if (v < (vmin + 0.25 * dv)) {
		c.r = 0;
		c.g = 4 * (v - vmin) / dv;
	}
	else if (v < (vmin + 0.5 * dv)) {
		c.r = 0;
		c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
	}
	else if (v < (vmin + 0.75 * dv)) {
		c.r = 4 * (v - vmin - 0.5 * dv) / dv;
		c.b = 0;
	}
	else {
		c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
		c.b = 0;
	}

	return(c);
}


#endif