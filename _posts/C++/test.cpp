// choicess.cpp

#include <iostream>
#include <array>

int main()
{
	using namespace std;
	array<double, 3> run;

	cout << "첫번째 달리기 결과를 입력하시오 : ";
	cin >> run[0];
	cout << "두번째 달리기 결과를 입력하시오 : ";
	cin >> run[1];
	cout << "세번째 달리기 결과를 입력하시오 : ";
	cin >> run[2];

	cout << "총 세번의 달리기의 평균은 " << (run[0] + run[1] + run[2]) / 3 << "입니다.\n";
	return 0;
}
