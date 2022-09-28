class Marine
{
	public:
		int _hp;
		static int s_att;
};

int Marine::s_att = 0;

int main()
{
	Marine::s_att = 6;

	Marine m1;
	m1._hp = 35;
	
	Marine m2;
	m2._hp = 14;

	Marine::s_att = 7;
}