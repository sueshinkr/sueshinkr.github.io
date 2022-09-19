// strngbad.h

#include <iostream>
#ifndef XXX_H_
#define XXX_H_

class StringBad
{
	private:
		char * str;
		int len;
		static int num_strings;
	public:
		StringBad(const char * s);
		StringBad();
		~StringBad();
		friend std::ostream & operator<<(std::ostream & os, const StringBad & st);
};

#endif