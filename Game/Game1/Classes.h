#include <SFML/Graphics.hpp>
#include "windows.h"
using namespace sf;
using namespace std;

extern const int H;
extern int W;
extern int offsetX;
extern int offsetY;
extern string TileMap[18];	


void gameRunning();
int Menu(RenderWindow & window, int n);
void MenuCr(RenderWindow & window);
bool startGame();
void map_reset();
void map_update(RenderWindow & window);
void scores_input();
void game_over(RenderWindow & window);
void level_clear(RenderWindow & window);
void Sleep(BYTE time);



class Player
{

public:

	float dx,dy;
	FloatRect rect;
	bool life;
	bool onGround;
	Sprite sprite;
	float currentFrame;
	
	
	Player (Texture &image)
	{
		sprite.setTexture(image);
		rect = FloatRect(7*32,17*32-68,47,68);

		dx=dy=0;
		currentFrame = 0;
		life = true;
	}

	void update(float time);
	
	void Collision(int);
};

class Enemy
{

public:

	Enemy(): dx(0), dy(0), rect(), life(false), sprite(), currentFrame(0) {};
	float dx,dy;
	FloatRect rect;
	bool life;
	Sprite sprite;
	float currentFrame;
	
	Enemy (Texture &image, int x, int y, bool s)
	{
		sprite.setTexture(image);
		rect = FloatRect(x,y-12,32,12);

		if (s == true) dx = 0.07;
			else dx = -0.07;

		dy = 0;
		currentFrame = 0;
		life = true;
	}

	void enemyUpdate(float time);
	
	void enemyCollision();


};
