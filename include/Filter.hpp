/* 
 * header:	Filter.h
 * description: header file for Filter.c++ which provides the functions for
 * 		filtering data.
 * author: 	Samuel Hild
 */

/*
 * function:	preflightFilter()
 * description:	averaging filter for the vehicle on the ground
 * input(s):	double previous, The previously filtered value
 *		double next, The most recently collected raw data
 * output(s):	double filtered, The new filtered value
 * author: 	Samuel Hild
 */
double preflightFilter(double previous, double next);

/*
 * function:	burnFilter()
 * description:	kalman filter for the vehicle during the burn
 * input(s):	double previous, The previously filtered value
 *		double next, The most recently collected raw data
 * output(s):	double filtered, The new filtered value
 * author: 	Samuel Hild
 */
double burnFilter(double previous, double next);

/*
 * function:	ascentFilter()
 * description:	kalman filter for the vehicle ascending
 * input(s):	double previous, The previously filtered value
 *		double next, The most recently collected raw data
 * output(s):	double filtered, The new filtered value
 * author: 	Samuel Hild
 */
double ascentFilter(double previous, double next);

/*
 * function:	apogeeFilter()
 * description: filter for the vehicle near apogee
 * input(s):	double previous, The previously filtered value
 *		double next, The most recently collected raw data
 * output(s):	double filtered, The new filtered value
 * author: 	Samuel Hild
 */
double apogeeFilter(double previous, double next);

/*
 * function:	drogueFilter()
 * description:	kalman filter for the vehicle descending on drogue chute
 * input(s):	double previous, The previously filtered value
 *		double next, The most recently collected raw data
 * output(s):	double filtered, The new filtered value
 * author: 	Samuel Hild
 */
double drogueFilter(double previous, double next);

/*
 * function:	mainFilter()
 * description:	kalman filter for the vehicle descending on main chute
 * input(s):	double previous, The previously filtered value
 *		double next, The most recently collected raw data
 * output(s):	double filtered, The new filtered value
 * author: 	Samuel Hild
 */
double mainFilter(double previous, double next);
