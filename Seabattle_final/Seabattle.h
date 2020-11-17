#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <Windows.h>
#include <string> 

using namespace sf;
using namespace std;

const int FIELD_SIZE = 10; 
const int FIELD_SQUARE = FIELD_SIZE * FIELD_SIZE;
const int SHIPS_NUMBER = 10;
const int POINTS_FOR_WIN = 20;

// ��� ���������
const int DRAW_FIELD_SIZE = 460;
const int CELL_SIZE = 45;

enum direction { right, down };

class Seabattle : public sf::Drawable, public sf::Transformable
{
protected:
	int **field_player1; // ������� ���� 1
	int **field_player2; // ������� ���� 2
	int player; //����� 1 ��� 2 ��� ��������� ������ ����
	int pl1_hits = 0; // ���������� ��������� ������ 1
	int pl2_hits = 0; // ���������� ��������� ������ 2
	sf::Font font;
public:
	Seabattle(int);
	void Init(int);
	void Fill_player_fields(int**);
	bool Player_turn(int, int, int);
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

