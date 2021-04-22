
#define OLC_PGE_APPLICATION
#define OLC_PGEX_ANIMSPR 
#include "olcPixelGameEngine.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <dos.h>
#include <string>
#include "olcPGEX_AnimatedSprite.h"


// THis will replace the main, adding structure to the game 
using namespace std;

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}
	struct shape
	{
		float x;
		float y;
		float dx;
		float dy;
		float angle;
		int nSize;
	};

	struct stats
	{
		int health;
		int points;
	};

	// intializing vectors: bullets, minions, tiles? 
	vector<shape> bullet;
	vector<shape> minion;
	vector<shape> tiles;
	// initalizing main characters 
	shape keith;
	shape player;
	// main character stats
	stats keithstat;
	stats playerstat;

	// Sprite stuff 
	olc::Sprite* sprPlayer = new olc::Sprite("./Gnome2.png");
	olc::Sprite* sprMinion = new olc::Sprite("./monster.png");
	olc::Sprite* sprKeith = new olc::Sprite("./Keith.png");


public:

	bool OnUserCreate() override
	{
		// Clean screen 
		Clear(olc::BLANK); 

		// initial plaayer spot 
		player.x = 150.0f; player.y = 120.0f; player.dx = 0.0f; player.dy = 0.0f; player.nSize = 16;
		keith.x = 200.0f; keith.y = 200.0f; keith.dx = 10.0f; keith.dy =10.0f; keith.nSize = 32;
		// intializing 2 minions
		minion.push_back({ 100.0f,100.0f,45.0f,30.0f,0.0f,(int)16 });
		minion.push_back({ 120.0f,120.0f,-15.0f,28.0f,0.0f,(int)16 });
		// initial tiles 
		tiles.push_back({ 0.0f,0.0f,0.0f,0.0f,0.0f,int(4) });
		// initalizing screen set up
		int nHeight = 200;
		int nWidth = 200;
		SetScreenSize(nHeight, nWidth);
		// initializing the player statitics
		keithstat.health = 100; 
		playerstat.health = 100;
		playerstat.points = 0; 
		return true;

	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);
		float speed = 75.0f;
		// player movements 
		if (GetKey(olc::Key::LEFT).bHeld) { player.angle = 4.71f; player.dx = speed; player.dy = speed; }
		if (GetKey(olc::Key::RIGHT).bHeld) { player.angle = 1.57f; player.dx = speed; player.dy = speed; }
		if (GetKey(olc::Key::UP).bHeld) { player.angle = 3.14f; player.dx = speed; player.dy = speed; }
		if (GetKey(olc::Key::DOWN).bHeld) { player.angle = 0.0f; player.dx = speed; player.dy = speed; }
		if (GetKey(olc::Key::UP).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
		if (GetKey(olc::Key::DOWN).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
		if (GetKey(olc::Key::LEFT).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
		if (GetKey(olc::Key::RIGHT).bReleased) { player.dx = 0.0f; player.dy = 0.0f; }
		player.x += player.dx * sin(player.angle) * fElapsedTime;
		player.y += player.dy * cos(player.angle) * fElapsedTime;
		// keeping player in the bounds of the screen
		if (player.x > 182 || player.x < 0) { player.x = player.x - player.dx * sin(player.angle) * fElapsedTime; }
		if (player.y > 182 || player.y < 0) { player.y = player.y - player.dy * cos(player.angle) * fElapsedTime; }
		// random generation of minion movement

	


		// drawing the 2 minions 
		for (auto& m : minion)
		{
			m.x += m.dx * fElapsedTime;
			m.y += m.dy * fElapsedTime;
			m.nSize = 4;
			//DrawCircle(m.x, m.y, m.nSize, olc::DARK_CYAN);
			// minion sprite 
			DrawSprite(m.x, m.y, sprMinion);

			// minion bouncing off the walls 
			if (m.x > 192 || m.x < 0) { m.dx = -1 * m.dx; } // vertical wall 
			if (m.y > 192 || m.y < 0) { m.dy = -1 * m.dy; } // horizantal wall

			// minion collision with Sprite, right now just bounces off 
			if (IsPointInsideRect(player.x, player.y, player.x + 16, player.y + 16, m.x, m.y))
			{
				m.dx = -1 * m.dx;
				m.dy = -1 * m.dy; 
				playerstat.health = playerstat.health - 1;
			}
		}

		// drawing bullets 
		if (GetKey(olc::Key::SPACE).bPressed) { bullet.push_back({ player.x+8,player.y+8, 100.0f * sin(player.angle),100.0f * cos(player.angle), 0.0f, (int)4 }); }
		for (auto& b : bullet)
		{
			b.x += b.dx * fElapsedTime;
			b.y += b.dy * fElapsedTime;
			DrawCircle(b.x, b.y, 2, olc::WHITE);
			// checking collision with minions 
			for (auto& m : minion)
			{
				if (IsPointInsideCircle(m.x, m.y, m.nSize, b.x, b.y))
				{
					//remove bullet 
					b.x = -100;
					// remove minion 
					m.y = -100;
					playerstat.points += 1;
				}
			}
		}

		// placing tiles
		for (auto& t : tiles)
		{
			// drawing initial targets 
			for (int k = 0; k < 80; k = k + 10) {
				t.x = (float)k;
				t.y = 50;
				DrawRect(t.x, t.y, 10, 10, olc::WHITE);
				// adding another tile set 
	
				// Check player collision with tiles 
				if (IsPointInsideRect(t.x, t.y, t.x+16, t.y+16, player.x,player.y)|| IsPointInsideRect(t.x, t.y, t.x + 16, t.y + 16, player.x + player.nSize, player.y + player.nSize)) // fix for all sides of circles?? 
				{
					player.x = player.x - player.dx * sin(player.angle) * fElapsedTime;
					player.y = player.y - player.dy * cos(player.angle) * fElapsedTime;
				}
				// bullet collision with tile 
				for (auto& b : bullet)
				{					
						if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, b.x, b.y))
						{
							b.dx = -1 * b.dx;
							b.dy = -1 * b.dy;
						}
				}
				// minion collision with tile 
				for (auto& m : minion)
				{
					if (IsPointInsideRect(t.x, t.y, t.x + 10, t.y + 10, m.x, m.y))
					{
						m.dx = -1 * m.dx;
						m.dy = -1 * m.dy; 
					}
				}
			}
		}

		// Keiths movement when player gets all the minions 
		if (playerstat.points == 2) {
			keith.angle = (player.y-keith.y) / (player.x-keith.x);
			if (player.x > keith.x)
			{
				keith.x += keith.dx * cos(keith.angle) * fElapsedTime;
				keith.y += keith.dy * sin(keith.angle) * fElapsedTime;
			}
			else
			{
				keith.x += -1* keith.dx * cos(keith.angle) * fElapsedTime;
				keith.y += -1* keith.dy * sin(keith.angle) * fElapsedTime;
			}
		}

		// draw player
		// DrawCircle(player.x, player.y, player.nSize, olc::WHITE);

		DrawPartialSprite(player.x, player.y, sprPlayer, 0, 0, 16, 16);

		// Drawing keith if comes to that point 
		if (playerstat.points == 2) {
			DrawPartialSprite(keith.x, keith.y, sprKeith, 0, 0, 32, 32);
		}
			

		// score and health box 
		DrawRect(165, 8, 24, 12, olc::WHITE);

		// update health and score stat 
		std::stringstream ss;
		ss << playerstat.health;
		DrawString(170, 10, ss.str(), olc::WHITE,1);
		std::stringstream sc;
		sc << playerstat.points;
		DrawString(170, 20, sc.str(), olc::WHITE);
		if (playerstat.health == 0) {
			return false;
		}

		return true;
	}

	bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
	}

	bool IsPointInsideRect(float x, float y, float xplus, float yplus, float xcheck, float ycheck)
	{
		return (xcheck > x && xcheck <xplus && ycheck>y && ycheck < yplus);
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}