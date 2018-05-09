#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include "Classes.h"
#include "windows.h"
using namespace sf;

int ground = 400;

extern const int H;
extern int W;
extern int offsetX;
extern int offsetY;
extern int score;
extern int N;
extern bool gameEnd;
extern string TileMap[18];	



void Player::update(float time)
{
	if (life == true) 
	{
	rect.left += dx*time;
	Collision(0);
	if (!onGround) 
		dy += 0.001*time;
	rect.top += dy*time;
	onGround = false;
	Collision(1);

	currentFrame += 0.005*time;

	if (currentFrame > 4) currentFrame = 0; 
	if (dx<0) sprite.setTextureRect(IntRect(63*int(currentFrame),0,47,68));
	if (dx>0) sprite.setTextureRect(IntRect(63*int(currentFrame)+47,0,-47,68));
	}
	sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	
	dx = 0;

}



void Player::Collision(int dir)
{	
	static SoundBuffer bufferPoint;
	static Sound point(bufferPoint);
	static int c = 0;

	if (c==0) {bufferPoint.loadFromFile("Audio/Point.ogg"); c++; }

	for (int i=rect.top/32;i<(rect.top+rect.height)/32;i++)
		for (int j=rect.left /32;j<(rect.left +rect.width)/32;j++)
		{
			if (i>=H) i = H-1;;
			if (j>=W) j = W-1;
			if ((TileMap[i][j]!=' ') && (TileMap[i][j]!='0') && (TileMap[i][j]!='*'))
			{
				if ((dy>0) && (dir==1)) 
				{ rect.top = i*32 - rect.height; dy=0; onGround = true; }
				if ((dy<0) && (dir==1)) 
					{ rect.top = i*32 + 32; dy=0; }
				if ((dx>0) && (dir==0))  
					rect.left = j*32 - rect.width;
				if ((dx<0) && (dir==0)) 
					rect.left = j*32 + 32;
			}

			if (TileMap[i][j]=='0')
			{
				TileMap[i][j] = ' ';
				point.play();
				score++;
			}

			if (TileMap[i][j]=='*')
			{
				TileMap[i][j] = ' ';
			    gameEnd = true;
				score += 3;
			}

		}
}



void Enemy::enemyUpdate(float time)
{
	if (life == true) 
	{
		rect.left += dx*time;
		enemyCollision();

		currentFrame += 0.005*time;
		if (currentFrame >= 3) currentFrame = 0; 
		if (dx>0) sprite.setTextureRect(IntRect(4+36*int(currentFrame)+32,0,-32,12));
		if (dx<0) sprite.setTextureRect(IntRect(4+36*int(currentFrame),0,32,12));

	}
	sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}
	


void Enemy::enemyCollision()
{
	for (int i=rect.top/32;i<(rect.top+rect.height)/32;i++)
	for (int j=rect.left /32;j<(rect.left +rect.width)/32;j++)
	{
		if (i>=H) i = H-1;
		if (j>=W) j = W-1;
		if (TileMap[i][j]!=' ')
		{
			if (dx>0)  
			{
				rect.left = j*32 - rect.width;
				dx = -dx;
			} else
			if (dx<0) 
			{
				rect.left = j*32 + 32;
				dx = -dx;
			}
		}
	}
}



 int Menu(RenderWindow & window, int n) {
	Texture menuButton1, menuButton2, menuButton3, menuButton4, aboutTexture, menuBackground;
	menuButton1.loadFromFile("Images/Menu_new.png");
	menuButton2.loadFromFile("Images/Menu_continue.png");
	menuButton3.loadFromFile("Images/Menu_credits.png");
	menuButton4.loadFromFile("Images/Menu_quit.png");
	menuBackground.loadFromFile("Images/Menu_background.jpg");
	Sprite menu1(menuButton1), menu2(menuButton2), menu3(menuButton3), menu4(menuButton4), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;

	menuBg.setPosition(0, 0);
	menu1.setPosition(200, 130);
	menu2.setPosition(200, 200);
	menu3.setPosition(200, 270);
	menu4.setPosition(200, 340);
	
	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menu4.setColor(Color::White);

		menuNum = 0;
		window.clear();

		if (IntRect(200, 130, 244, 36).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(200, 200, 244, 36).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(200, 270, 244, 36).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }
		if (IntRect(200, 340, 244, 36).contains(Mouse::getPosition(window))) { menu4.setColor(Color::Blue); menuNum = 4; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { isMenu = false; return 1; }//если нажали первую кнопку, то выходим из меню 

			if ((menuNum == 2) && (n == 2)) {isMenu = false; return 2;}

			if (menuNum == 3) { MenuCr(window); }

			if (menuNum == 4)  { window.close(); isMenu = false; return 3; } //выход из игры

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(menu4);

		window.display();
	}

}



 void MenuCr(RenderWindow & window)
 {
	 window.clear();
	 Texture menuCrButton, menuCrBackground;
	 menuCrButton.loadFromFile("Images/MenuCr_quit.png");
	 menuCrBackground.loadFromFile("Images/Menu_background.jpg");
	 Sprite menuCr(menuCrButton), menuCrBg(menuCrBackground);
	 menuCrBg.setPosition(0, 0);
	 menuCr.setPosition(455, 400);
	 bool isMenuCr = true;
	 bool menuExit;


	 fstream playerScores;
	 playerScores.open("Other/Player_scores.txt", ios::in);

	 Font font;
	 font.loadFromFile("Other/CyrilicOld.ttf");
	 string tempStr;

		 Text scores[5];
		 for (int i=0; i<5; i++)
		 {
			 getline(playerScores,tempStr);
			 Text temp(tempStr, font, 22);
			 scores[i] = temp;
			 scores[i].setStyle(sf::Text::Bold);
			 scores[i].setPosition(40, 270+i*30);
		 }

	 playerScores.close();

	 Text credits[4];
	 tempStr = "Автор игры:";
	 Text temp(tempStr, font, 24);
	 credits[0] = temp;
	 credits[0].setStyle(sf::Text::Bold | sf::Text::Underlined);
	 credits[0].setPosition(40, 80);
	 temp.setString("Будлов Егор Олегович, 1 курс, ПМ-ПУ, группа 132");
	 credits[1] = temp;
	 credits[1].setStyle(sf::Text::Bold);
	 credits[1].setPosition(40, 110);
	 temp.setString("2017");
	 credits[2] = temp;
	 credits[2].setStyle(sf::Text::Bold);
	 credits[2].setPosition(40, 140);
	 temp.setString("Таблица рекордов:");
	 credits[3] = temp;
	 credits[3].setStyle(sf::Text::Bold | sf::Text::Underlined);
	 credits[3].setPosition(210, 200);
	 

	 while (isMenuCr)
	{
		menuCr.setColor(Color::White);
		menuExit = false;

		window.clear();

		if (IntRect(455,  400, 128, 35).contains(Mouse::getPosition(window))) { menuCr.setColor(Color::Blue); menuExit = true;}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuExit == true)  { window.clear(); isMenuCr = false; return; } //выход из игры

		}

		window.draw(menuCrBg);
		window.draw(menuCr);

		for (int i=0; i<4; i++) 
			window.draw(credits[i]);

		for (int i=0; i<5; i++) 
			window.draw(scores[i]);

		window.display();
	}

 }



void map_reset()
 {
	ifstream Map;
	gameEnd = false;
	score = 0;
	offsetX = 0;
	offsetY = 96;
	Map.open("Other/Map1.txt");
	string temp;
	Map >> N;
	getline(Map,temp);

	for (int i=0; i<H; i++)
			 getline(Map,TileMap[i]);
	W = TileMap[0].length();
	Map.close();
 }

 void map_update(RenderWindow & window)
 {
	 
	static Texture textures;
	static Sprite gameTextures(textures);
	
	static Texture numbers;
	static Sprite gameNumbers(numbers);
	
	static int c = 0;

	if (c==0) 
	{
		textures.loadFromFile("Images/Sprite_list.png");
		numbers.loadFromFile("Images/Numbers.png");
		c++;
	}

			for (int i=0; i<H; i++)
			 for (int j=0; j<W; j++)
			  { 
				  int sdvx = 0;
				  int sdvy = 0;

				if (TileMap[i][j]=='W')  gameTextures.setTextureRect( IntRect(0,204,32,32) );

				if (TileMap[i][j]=='w')  gameTextures.setTextureRect( IntRect(34,204,32,32) );
				
				
				if (TileMap[i][j]=='1')  gameTextures.setTextureRect( IntRect(0,0,32,32) );

				if (TileMap[i][j]=='2')  gameTextures.setTextureRect( IntRect(34,0,32,32) );

				if (TileMap[i][j]=='3')  gameTextures.setTextureRect( IntRect(68,0,32,32) );

				if (TileMap[i][j]=='4')  gameTextures.setTextureRect( IntRect(0,34,32,32) );

				if (TileMap[i][j]=='5')  gameTextures.setTextureRect( IntRect(34,34,32,32) );

				if (TileMap[i][j]=='6')  gameTextures.setTextureRect( IntRect(68,34,32,32) );

				if (TileMap[i][j]=='7')  gameTextures.setTextureRect( IntRect(0,68,32,32) );

				if (TileMap[i][j]=='8')  gameTextures.setTextureRect( IntRect(34,68,32,32) );

				if (TileMap[i][j]=='9')  gameTextures.setTextureRect( IntRect(68,68,32,32) );


				if (TileMap[i][j]=='l')  gameTextures.setTextureRect( IntRect(0,102,32,32) );

				if (TileMap[i][j]=='m')  gameTextures.setTextureRect( IntRect(34,102,32,32) );

				if (TileMap[i][j]=='r')  gameTextures.setTextureRect( IntRect(68,102,32,32) );


				if (TileMap[i][j]=='v')  gameTextures.setTextureRect( IntRect(102,102,32,32) );

				if (TileMap[i][j]=='0')  gameTextures.setTextureRect( IntRect(0,262,32,24) );


				if (TileMap[i][j]=='T')  gameTextures.setTextureRect( IntRect(102,0,32,32) );

				if (TileMap[i][j]=='|')  { gameTextures.setTextureRect( IntRect(104,34,28,32) ); sdvx = 2; }

				if (TileMap[i][j]=='!')  { gameTextures.setTextureRect( IntRect(104,68,28,32) ); sdvx = 2; }


				if (TileMap[i][j]=='{') {  gameTextures.setTextureRect( IntRect(212,78,24,22) ); sdvx = 8; sdvy = 10; }

				if (TileMap[i][j]=='}') {  gameTextures.setTextureRect( IntRect(238,78,28,22) ); sdvy = 10; }

				if (TileMap[i][j]=='[') { gameTextures.setTextureRect( IntRect(212,102,24,32) ); sdvx = 8; }

				if (TileMap[i][j]==']')  gameTextures.setTextureRect( IntRect(238,102,28,32) );


				if (TileMap[i][j]=='*')  gameTextures.setTextureRect( IntRect(35,262,30,30) );


				if (TileMap[i][j]==' ') continue;

  			    gameTextures.setPosition(j*32 - offsetX + sdvx,i*32 - offsetY + sdvy) ; 
		        window.draw(gameTextures);
		     }


			 if (score/10 == 0) { gameNumbers.setTextureRect( IntRect(6,4,24,36) ); }
				 else { gameNumbers.setTextureRect( IntRect(34,4,24,36) );  }

			 gameNumbers.setPosition(560,40) ; 
			 window.draw(gameNumbers);

			 int s = score%10;
			 if (s<5) { gameNumbers.setTextureRect( IntRect(6+28*s,4,24,36) ); }
				else { gameNumbers.setTextureRect( IntRect(6+28*(s-5),46,24,36) ); }

			 gameNumbers.setPosition(588,40) ; 
			 window.draw(gameNumbers);


 }


 void scores_input()
 {
	 fstream playerScores;
	 playerScores.open("Other/Player_scores.txt", ios::in);
	 string scores[5];
	 int pos = 6;
	 int scTemp = 0;
	 for (int i=0; i<5; i++)
	 {
			 int temp = 0;
			 getline(playerScores,scores[i]);
			 int length = scores[i].length();
			 temp = (int(scores[i][length-2])-48)*10+(int(scores[i][length-1]-48)); //извлекаем кол-во очков из строки
			 if ((i<pos) && (score>temp)) 
			 {
				 scTemp = score;
				 pos = i;
			 }
	 }
	 playerScores.close();

	 if (pos<6)
	 {
		 int t = 4;
		 int length;
		 while (t>pos) //если нужно, сдвигаем список
		 {
			 length = scores[t].length();
			 scores[t][length-2] = scores[t-1][length-2];
			 scores[t][length-1] = scores[t-1][length-1];
			 t--;
		 }

		 length = scores[pos].length();
		 scores[pos].erase(length-2, 2);
		 scores[pos] = scores[pos] + char((scTemp/10)+48) + char((scTemp%10)+48);
	 }

	 playerScores.open("Other/Player_scores.txt", ios::out);
	 playerScores.clear();

	 for (int i=0; i<5; i++)
		 playerScores << scores[i] << endl;

	 playerScores.close();
 }



 void game_over(RenderWindow & window)
 {
	static Texture end_game;
	static Sprite gameOver(end_game);
	static int c = 0;

	if (c==0) 
	{
		end_game.loadFromFile("Images/Game_over.png");
		c++;
	}


	gameOver.setPosition(130,170) ; 
	
	
	gameOver.setTextureRect( IntRect(10,6,384,84) );

  	scores_input();

     window.draw(gameOver);
	 window.display();
 }



 void level_clear(RenderWindow & window)
 {
	static Texture lvl_cleared;
	static Sprite gameWin(lvl_cleared);
	static int c = 0;

	if (c==0) 
	{
		lvl_cleared.loadFromFile("Images/Level_complete.png");
		c++;
	}


	gameWin.setPosition(130,120) ; 
	
	
	gameWin.setTextureRect( IntRect(0,0,384,150) );

  	scores_input();

     window.draw(gameWin);
	 window.display();
 }



 void Sleep(BYTE time)
{
	Sleep((DWORD)time*1000);
	return;
}


 

 