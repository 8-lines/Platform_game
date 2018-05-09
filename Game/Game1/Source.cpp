#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Classes.h"
#include "Windows.h"
using namespace sf;

int x = 640;
int y = 480;
RenderWindow window(VideoMode(x, y), "Game");

extern const int H = 18;
extern int W = 0;
extern int offsetX = 0;
extern int offsetY = 96;
extern int score = 0;
extern int N = 0;
extern bool gameEnd = false;
extern string TileMap[18] = {};



bool startGame()
{
	map_reset();
	
	Texture enemies, plr, background;
	

	plr.loadFromFile("Images/Player.png");
	enemies.loadFromFile("Images/Enemies.png");
	background.loadFromFile("Images/Game_background.jpg");

	Sprite gameBg(background);
	Sprite gameEnemies(enemies);
	gameBg.setPosition(0, 0);

	float currentFrame=0;

	Player player(plr);
	player.sprite.setTextureRect(IntRect(110,0,-47,68));
	
	Enemy* enemyArr = new Enemy[N];

	srand(time(NULL));
	for (int i=0; i<N; i++)
		{
			int a = 20 + rand() % 165;
			int b = rand() % 2;
			enemyArr[i] = Enemy(enemies,a*32,17*32,b);
			enemyArr[i].sprite.setTextureRect(IntRect(76,0,-32,12));
		}

	Clock clock;

	SoundBuffer bufferJump, bufferHit, bufferPoint;
	
	bufferJump.loadFromFile("Audio/Jump.ogg");
	Sound jump(bufferJump);

	bufferHit.loadFromFile("Audio/Hit.ogg");
	Sound hit(bufferHit);

    Music music;
    music.openFromFile("Audio/Music.wav");
    music.play();

	while (window.isOpen())
	{
		
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time/700;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}


		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.dx = -0.2;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.dx = 0.2;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (player.onGround) { jump.play(); player.dy = -0.65; player.onGround = false; }
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			music.pause();
			int command = Menu(window, 2);
			if (command == 1) { return true; }
			clock.restart();
			music.play();
		}
		

		player.update(time);
		int boost = 0;

		for (int i=0; i<N; i++)
		{
			enemyArr[i].enemyUpdate(time);
			if (player.rect.intersects(enemyArr[i].rect))
			{
				if (enemyArr[i].life) { 
				if (player.dy > 0) { enemyArr[i].dx = 0; boost++; enemyArr[i].life = false; hit.play(); enemyArr[i].sprite.setTextureRect(IntRect(72,12,-32,-12));}
				else { 
						player.life = false; 
						music.stop(); 
						game_over(window);
						Sleep(BYTE(3));
						int command = Menu(window, 1);  
						if (command == 1) { return true; } }
					}
			}
		}
		if (boost>0) 
			player.dy = -0.2;
		
		if ((player.rect.left > x/2) && (player.rect.left < (W*32 - x/2))) offsetX = player.rect.left - x/2;

		window.clear(Color::White);
		window.draw(gameBg);

		map_update(window);
		

		for (int i=0; i<N; i++)
			window.draw(enemyArr[i].sprite);

		window.draw(player.sprite);

		window.display();

			if ((gameEnd == true) && (player.onGround==true)) //≈сли игрок вз€л звезду и стоит на земле (конец игры)
			{
					 map_update(window);
					 window.draw(player.sprite);
					 window.display();
					 level_clear(window);
					 Sleep(BYTE(3));
					 int command = Menu(window, 1);  
					 if (command == 1) { return true; } 
			}
	}

}

void gameRunning()
{ 
	if (startGame()) { gameRunning(); }
}

int main()
{
//	HWND hConsole = GetConsoleWindow();//≈сли компил€тор старый заменить на GetForegroundWindow()
//	ShowWindow(hConsole, SW_HIDE);//собственно пр€чем оконо консоли

	int command = Menu(window, 1); //1 значит что текущей игры нет, 2 - есть
	gameRunning();
	return 0;
}