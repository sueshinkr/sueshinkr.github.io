// Game.h

class Player; // 전방선언
class Field;

class Game
{
	public:
		Game();
		~Game();
		void Init();
		void Update();
		void CreatePlayer();
	private:
		Player* _player;
		Field* _field;
}; 