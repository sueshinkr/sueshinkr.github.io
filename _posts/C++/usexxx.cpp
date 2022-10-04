// pairs.cpp

#include "xxx.h"
#include <iostream>
#include <string>

template <class T1, class T2>
class Pair
{
	private:
		T1 a;
		T2 b;
	public:
		T1 & first();
		T2 & second();
		T1 first() const { return a; }
		T2 second() const { return b; }
		Pair(const T1 & aval, const T2 & bval) : a(aval), b(bval) {}
		Pair() {}
};

template<class T1, class T2>
T1 & Pair<T1, T2>::first()
{
	return a;
}

template<class T1, class T2>
T2 & Pair<T1, T2>::second()
{
	return b;
}

int main()
{
	using std::cout;
	using std::endl;
	using std::string;
	Pair<string, int> ratings[4] = 
	{
		Pair<string, int>("이동갈비", 5),
		Pair<string, int>("태릉갈비", 4),
		Pair<string, int>("수원갈비", 5),
		Pair<string, int>("LA갈비", 3)
	};

	int joints = sizeof(ratings) / sizeof(Pair<string, int>);
	cout << "등급 : \t 갈비 종류\n";
	for (int i = 0; i < joints; i++)
		cout << ratings[i].second() << " :\t "
			 << ratings[i].first() << endl;
	cout << "조정된 등급 : \n";
	ratings[3].first() = "LA떡갈비";
	ratings[3].second() = 6;
	cout << ratings[3].second() << " :\t "
		 << ratings[3].first() << endl;
	return 0;
}