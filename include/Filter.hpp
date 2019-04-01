/* 
 * header:	Filter.hpp
 * description: header file for Filter.c++ which provides the functions for
 * 		filtering data.
 * author: 	Samuel Hild
 */

/*
 * function:	preflightFilter()
 * description:	averaging filter for the vehicle on the ground
 * input(s):	int n, The index of the new estimation
 * 		float previous, The previously filtered value
 *		float next, The most recently collected raw data
 * output(s):	float filtered, The new filtered value
 * author: 	Samuel Hild
 */
float preflightFilter(int n, float previous, float next);

/*
 * function:	burnFilter()
 * description:	kalman filter for the vehicle during the burn
 * input(s):	float previous, The previously filtered value
 *		float next, The most recently collected raw data
 * output(s):	float filtered, The new filtered value
 * author: 	Samuel Hild
 */
float burnFilter(float previous, float next);

/*
 * function:	ascentFilter()
 * description:	kalman filter for the vehicle ascending
 * input(s):	float previous, The previously filtered value
 *		float next, The most recently collected raw data
 * output(s):	float filtered, The new filtered value
 * author: 	Samuel Hild
 */
float ascentFilter(float previous, float next);

/*
 * function:	apogeeFilter()
 * description: filter for the vehicle near apogee
 * input(s):	float previous, The previously filtered value
 *		float next, The most recently collected raw data
 * output(s):	float filtered, The new filtered value
 * author: 	Samuel Hild
 */
float apogeeFilter(float previous, float next);

/*
 * function:	drogueFilter()
 * description:	kalman filter for the vehicle descending on drogue chute
 * input(s):	float previous, The previously filtered value
 *		float next, The most recently collected raw data
 * output(s):	float filtered, The new filtered value
 * author: 	Samuel Hild
 */
float drogueFilter(float previous, float next);

/*
 * function:	mainFilter()
 * description:	kalman filter for the vehicle descending on main chute
 * input(s):	float previous, The previously filtered value
 *		float next, The most recently collected raw data
 * output(s):	float filtered, The new filtered value
 * author: 	Samuel Hild
 */
float mainFilter(float previous, float next);
