// vect3_shared_ptr.cpp

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Review {
	std::string title;
	int rating;
	int price;
};

bool operator<(const std::shared_ptr<Review> & r1, const std::shared_ptr<Review> & r2);
bool worseThan(const std::shared_ptr<Review> & r1, const std::shared_ptr<Review> & r2);
bool worseprice(const std::shared_ptr<Review> & r1, const std::shared_ptr<Review> & r2);
bool FillReview(Review & rr);
void ShowReview(const std::shared_ptr<Review> & rr);

int main()
{
	using namespace std;

	vector<shared_ptr<Review>> books;
	Review temp;

	while (FillReview(temp))
	{
		shared_ptr<Review> pt (new Review(temp));
		books.push_back(pt);
	}
	if (books.size() > 0)
	{
		cout << "감사합니다. 당신은 다음과 같이 "
			 << books.size() << "개의 책 등급을 입력하셨습니다.\n"
			 << "등급\t제목\n";
		for_each(books.begin(), books.end(), ShowReview);

		vector<shared_ptr<Review>> books_origin = books;

		while (true)
		{
			cout << "어느 정렬 방식을 선택하시겠습니까?\n";
			cout << "1.원래 순서\t2.알파벳 순서\t3.등급 오름차순\n";
			cout << "4.등급 내림차순\t5.가격 오름차순\t6.가격 내림차순\n";
			cout << "주어진 방식 이외의 입력을 받을시 프로그램이 종료됩니다.\n";

			int n;
			cin >> n;
			if (n < 1 || n > 6)
			{
				cout << "프로그램을 종료합니다.\n";
				return 0;
			}

			switch(n)
			{
				case 1:
					cout << "원래 순서 : \n등급\t제목\t가격\n"; 
					for_each(books_origin.begin(), books_origin.end(), ShowReview);
					break;
				case 2:
					sort(books.begin(), books.end());
					cout << "책 제목을 기준으로 정렬 : \n등급\t제목\t가격\n";
					for_each(books.begin(), books.end(), ShowReview);
					break;
				case 3:
					sort(books.begin(), books.end(), worseThan);
					cout << "등급 오름차순 정렬 : \n등급\t제목\t가격\n";
					for_each(books.begin(), books.end(), ShowReview);
					break;
				case 4:
					sort(books.rbegin(), books.rend(), worseThan);
					cout << "등급 오름차순 정렬 : \n등급\t제목\t가격\n";
					for_each(books.begin(), books.end(), ShowReview);
					break;
				case 5:
					sort(books.begin(), books.end(), worseprice);
					cout << "가격 오름차순 정렬 : \n등급\t제목\t가격\n";
					for_each(books.begin(), books.end(), ShowReview);
					break;
				case 6:
					sort(books.rbegin(), books.rend(), worseprice);
					cout << "가격 오름차순 정렬 : \n등급\t제목\t가격\n";
					for_each(books.begin(), books.end(), ShowReview);
					break;

			}
		}
	}
	else
		cout << "프로그램을 종료합니다.\n";
	return 0;
}

bool operator<(const std::shared_ptr<Review> & r1, const std::shared_ptr<Review> & r2)
{
	if (r1->title < r2->title)
		return true;
	else if (r1->title == r2->title && r1->rating < r2->rating)
		return true;
	else
		return false;
}

bool worseThan(const std::shared_ptr<Review> & r1, const std::shared_ptr<Review> & r2)
{
	if (r1->rating < r2->rating)
		return true;
	else
		return false;
}

bool worseprice(const std::shared_ptr<Review> & r1, const std::shared_ptr<Review> & r2)
{
	if (r1->price < r2->price)
		return true;
	else
		return false;
}

bool FillReview(Review & rr)
{
	std::cout << "책 제목을 입력하십시오(끝내려면 quit를 입력) : ";
	std::getline(std::cin, rr.title);
	if (rr.title == "quit")
		return false;
	std::cout << "책 등급(0-10)을 입력하십시오 : ";
	std::cin >> rr.rating;
	if (!std::cin)
		return false;
	std::cout << "책 가격을 입력하십시오 : ";
	std::cin >> rr.price;
	if (!std::cin)
		return false;
	while (std::cin.get() != '\n')
		continue;
	return true;
}

void ShowReview(const std::shared_ptr<Review> & rr)
{
	std::cout << rr->rating << "\t" << rr->title << "\t" << rr->price << std::endl;
}