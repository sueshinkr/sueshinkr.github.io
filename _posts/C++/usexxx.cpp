// useemp.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Store
{
	private:
		std::ofstream& os;
	public:
		Store(std::ofstream & fout) : os(fout) {}
		~Store() {}
		Store & operator()(const std::string & str)
		{
			size_t len = str.length();
			os.write((char*)&len, sizeof(size_t));
			os.write(str.data(), len);
			return *this;
		}
};

void ShowStr(const std::string &);
void GetStrs(std::ifstream& fin, std::vector<std::string>& v);


int main()
{
	using namespace std;
	vector<string> vostr;
	string temp;

	cout << "문자열들을 입력하십시오 (끝내려면 빈 줄 입력) : \n";
	while (getline(cin, temp) && temp[0] != '\0')
		vostr.push_back(temp);
	cout << "다음과 같이 입력하셨습니다.\n";
	for_each(vostr.begin(), vostr.end(), ShowStr);

	ofstream fout("strings.dat", ios_base::out | ios_base::binary);
	for_each(vostr.begin(), vostr.end(), Store(fout));
	fout.close();

	vector<string> vistr;
	ifstream fin("strings.dat", ios_base::in | ios_base::binary);
	if (!fin.is_open())
	{
		cerr << "입력을 위한 파일을 열 수 없습니다.\n";
		exit(EXIT_FAILURE);
	}
	GetStrs(fin, vistr);
	cout << "\n파일로부터 읽은 문자열들은 다음과 같습니다 : \n";
	for_each(vistr.begin(), vistr.end(), ShowStr);

	return 0;
}

void ShowStr(const std::string & str)
{
	std::cout << str << std::endl;
}

void GetStrs(std::ifstream& fin, std::vector<std::string>& v)
{
	size_t len = 0;
	std::string str;
	char ch;

	while (fin.peek() && !fin.eof())
	{
		fin.read((char*)&len, sizeof(size_t));
		for (size_t i = 0; i < len; i++)
		{
			fin.read(&ch, sizeof(char));
			str.push_back(ch);
		}
		v.push_back(str);
		str.clear();
	}
	fin.close();
}