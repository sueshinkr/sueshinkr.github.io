// exc_mean.h

#ifndef XXX_H_
#define XXX_H_
#include <iostream>

class bad_hmean
{
	private:
		double v1;
		double v2;
	public:
		bad_hmean(double a = 0, double b = 0) : v1(a), v2(b) {}
		void mesg();
};

inline void bad_hmean::mesg()
{
	std::cout << "hmean(" << v1 << ", " << v2 << ") : "
			  << "잘못된 매개변수 : a = -b\n";
}

class bad_gmean
{
	public:
		double v1;
		double v2;
		bad_gmean(double a = 0, double b = 0) : v1(a), v2(b) {}
		const char * mesg();
};

inline const char * bad_gmean::mesg()
{
	return "gmean() 매개변수들은 >= 0 이어야 합니다.\n";
}

#endif