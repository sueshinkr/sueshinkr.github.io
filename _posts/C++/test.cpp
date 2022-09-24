// ptr_exercise

#include <iostream>
using namespace std;

struct StatInfo
{
	int hp;
	int att;
	int def;
};

void EnterLobby();
StatInfo CreatePlayer();
void CreateMonster(StatInfo* info);
bool StartBattle(StatInfo* player, StatInfo* monster);

int main()
{
	EnterLobby();
	return 0;
}

void EnterLobby()
{
	cout << "로비에 입장했습니다." << endl;
	
	StatInfo player;
	player = CreatePlayer();

	StatInfo monster;
	CreateMonster(&monster);

	bool victory = StartBattle(&player, &monster);
	if (victory)
		cout << "승리!" << endl;
	else
		cout << "패배!" << endl;
}

StatInfo CreatePlayer()
{
	StatInfo pl;

	cout << "플레이어 생성" << endl;
	pl.hp = 100;
	pl.att = 10;
	pl.def = 2;

	return pl;
}

void CreateMonster(StatInfo* info)
{
	cout << "몬스터 생성" << endl;
	info->hp = 40;
	info->att = 8;
	info->def = 1;
}

bool StartBattle(StatInfo* player, StatInfo* monster)
{
	while (true)
	{
		int damage = player->att - monster->def;
		if (damage < 0)
			damage = 0;
		
		monster->hp -= damage;
		if (monster->hp < 0)
			monster->hp = 0;
		
		cout << "몬스터 HP : " << monster->hp << endl;
		if (monster->hp == 0)
			return true;

		damage = monster->att - player->def;
		if (damage < 0)
			damage = 0;
		
		player->hp -= damage;
		if (player->hp < 0)
			player->hp = 0;

		cout << "플레이어 HP : " << player->hp << endl;
		if (player->hp == 0)
			return false;
	}
}