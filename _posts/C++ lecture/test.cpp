#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// find()
	{
		int number = 50;
		vector<int>::iterator itFind = find(v.begin(), v.end(), number);
		if (itFind == v.end())
			cout << "not found" << endl;
		else
			cout << "found" << endl;
	}

	// find_it()
	{
		struct CanDivideBy11
		{
			bool operator()(int n)
			{
				return (n % 11) == 0;
			}
		};
		vector<int>::iterator itFind = find_if(v.begin(), v.end(), CanDivideBy11());
		if (itFind == v.end())
			cout << "not found" << endl;
		else
			cout << "found" << endl;
	}

	// count_if()
	{
		struct isOdd
		{
			bool operator()(int n)
			{
				return (n % 2) != 0;
			}
		};
		int n = count_if(v.begin(), v.end(), isOdd());
		cout << n << endl;
	
	// all_of() / any_of() / none_of()

		bool b1 = all_of(v.begin(), v.end(), isOdd());
		bool b2 = any_of(v.begin(), v.end(), isOdd());
		bool b3 = none_of(v.begin(), v.end(), isOdd());
		cout << b1 << "\t" << b2 << "\t" << b3 << endl;
	}

	// for_each()
	{
		struct MultiplyBy3
		{
			void operator()(int& n)
			{
				n = n * 3;
			}
		};
		for_each(v.begin(), v.end(), MultiplyBy3());
	}

	// remove() / remove_if()
	{
		struct isOdd
		{
			bool operator()(int n)
			{
				return (n % 2) != 0;
			}
		};
		vector<int>::iterator it = remove_if(v.begin(), v.end(), isOdd());
		// 1 4 3 5 8 2 => 4 8 2 5 8 2

		v.erase(it, v.end());
		//v.erase(remove_if(v.begin(), v.end(), isOdd());
	}

	return 0;
}