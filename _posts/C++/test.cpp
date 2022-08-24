// typecast.cpp

#include <iostream>
const double LITERS_PER_KM_TO_MPG = 19 / 12.4;

int main()
{
	using namespace std;
	
	cout << "주행거리를 킬로미터 단위로 입력하시오 : ";
	double km;
	cin >> km;
	cout << "소비한 휘발유를 리터 단위로 입력하시오 : ";
	double liter;
	cin >> liter;
	cout << "휘발유 1갤런당 "
		 << (liter * 100 / km) * LITERS_PER_KM_TO_MPG << "마일의 거리를 주행했습니다.\n";
	return 0;
}
