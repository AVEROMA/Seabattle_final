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

// Для отрисовки
const int DRAW_FIELD_SIZE = 460;
const int CELL_SIZE = 45;

enum direction { right, down };

class Seabattle : public sf::Drawable, public sf::Transformable
{
protected:
	int **field_player1; // Игровое поле 1
	int **field_player2; // Игровое поле 2
	int player; //Игрок 1 или 2 для отрисовки своего поля
	sf::Font font;
public:
	Seabattle(int);
	void Init(int);
	void Fill_player_fields(int**);
	bool Player_turn(Vector2i, int);
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

