////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           DREAMCLOUD PROJECT                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef DREAMCLOUD__UTILITIES_CLIB__MATH_HXX
#define DREAMCLOUD__UTILITIES_CLIB__MATH_HXX

inline double pairing_function(double x, double y) {
	return (((x + y) * (x + y + 1)) / 2) + y;
}

inline int gcd(unsigned long a, unsigned long b) {
	if (b == 0)
		return a;
	else
		return gcd(b, a % b);
}

inline int lcm(unsigned long a, unsigned long b) {
	return (a * b) / gcd(a, b);
}

#endif

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  END OF FILE.                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
