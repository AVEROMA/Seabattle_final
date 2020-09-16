#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "Seabattle.h"

using namespace sf;
using namespace std;

int menu(RenderWindow& window)
{
	Texture newGameTexture, serverTexture, exitGameTexture, aboutTexture, textureBackground;
	serverTexture.loadFromFile("..\\images\\server.png");
	newGameTexture.loadFromFile("..\\images\\new_game.png");
	aboutTexture.loadFromFile("..\\images\\about_game.png");
	exitGameTexture.loadFromFile("..\\images\\exit_game.png");
	textureBackground.loadFromFile("..\\images\\background_3.png");
	Sprite	menuNewGame(newGameTexture),
		menuServerNewGame(serverTexture),
		menuExitGame(exitGameTexture),
		menuAbout(aboutTexture),
		menuBackground(textureBackground);

	RectangleShape line_with_thickness(Vector2f(440.f, 5.f));
	line_with_thickness.rotate(90.f);
	line_with_thickness.setFillColor(Color(200, 100, 200));
	line_with_thickness.move(280, 15);

	menuServerNewGame.scale(0.3, 0.3);
	menuNewGame.scale(0.3, 0.3);
	menuExitGame.scale(0.3, 0.3);
	menuAbout.scale(0.3, 0.3);

	bool isMenu = true;
	int menuNum = 0;
	menuNewGame.setPosition(100, 30);
	menuServerNewGame.setPosition(100, 90);
	menuExitGame.setPosition(100, 150);
	menuAbout.setPosition(100, 300);
	menuBackground.setPosition(330, 10);

	Font font;
	font.loadFromFile("..\\calibri.ttf");
	sf::Text text("", font, 25);
	text.setFillColor(Color(200, 100, 200));

	while (isMenu)
	{
		menuNewGame.setColor(Color::White);
		menuServerNewGame.setColor(Color::White);
		menuExitGame.setColor(Color::White);
		menuAbout.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(0, 0, 0));

		if (IntRect(100, 30, 150, 50).contains(Mouse::getPosition(window))) { menuNewGame.setColor(Color(57, 255, 20)); menuNum = 1; }
		if (IntRect(100, 90, 150, 50).contains(Mouse::getPosition(window))) { menuServerNewGame.setColor(Color(57, 255, 20)); menuNum = 2; }
		if (IntRect(100, 300, 300, 150).contains(Mouse::getPosition(window))) { menuAbout.setColor(Color(57, 255, 20)); menuNum = 3; }
		if (IntRect(100, 160, 150, 50).contains(Mouse::getPosition(window))) { menuExitGame.setColor(Color(57, 255, 20)); menuNum = 4; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{

			if (menuNum == 1 || menuNum == 2) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 3)
			{
				text.setString("SEABATTLE GAME\n\n" 
					"Made by \nAveryanov Roman(c) \nShumkin Vladislav(c)\nNSTU, \nFaculty of Automation and Computer Engineering\n\n"
					"Game rules are simple\n\n"
					"If you want to exit just press ESC"
				);
				text.setPosition(300, 50);
				window.draw(line_with_thickness);
				window.draw(text);
				window.draw(menuAbout);
				window.display();
				while (!Keyboard::isKeyPressed(Keyboard::Escape));
			}
			if (menuNum == 4) { window.close(); isMenu = false; }
		}
		window.draw(menuBackground);
		window.draw(menuNewGame);
		window.draw(menuServerNewGame);
		window.draw(menuAbout);
		window.draw(menuExitGame);

		window.display();
	}
	Sleep(2000);
	return menuNum;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	// Создаем окно размером 940 на 470 и частотой обновления 30 кадров в секунду
	RenderWindow window(VideoMode(940, 470), "ITSA FUCKIN SEABATTLE YOU MOTHERFUCKER");
	window.setFramerateLimit(30);

	Font font; 
	font.loadFromFile("..\\calibri.ttf");

	int player_turn = 2; // Ход игрока 1 или 2
	int player; // Игрок 1 или 2
	bool hit = true; // Попадание или промах
	int mode; // Для включения клиента или сервера
	
	mode = menu(window);
	player = mode;

	Sleep(2000);

	Seabattle game(mode);
	game.setPosition(5.f, 5.f);

	// Для подключения по TCP
	TcpSocket socket;
	TcpListener listener;
	std::size_t received;
	IpAddress ip;
	Packet get_packet;
	Packet send_packet;
	int cell;
	Vector2i get_pos;

	if (player == 1)
	{
		cout << endl << "client" << endl;
		window.setTitle("client");
		// Подключение
		ip = IpAddress::getLocalAddress();
		socket.connect(ip, 2000);
	}
	else if (player == 2)
	{
		cout << endl << "server" << endl;
		window.setTitle("server");
		// Подключение
		ip = "localhost";
		listener.listen(2000);
		listener.accept(socket);
	}
	
	// Главный цикл приложения: выполняется, пока открыто окно
	while (window.isOpen())
	{

		if (player == 2 && player_turn == 1)
		{
			int x, y;
			socket.receive(get_packet);
			get_packet >> x >> y;
			get_packet.clear();

			cout << player << " 21SHOOT " << player_turn << " x:" << x << "y:" << y << endl;

			hit = game.Player_turn(x, y, player_turn);
			if (!hit) // Если игрок попал, он делает еще один выстрел
				player_turn = 2;
		}
		else if (player == 1 && player_turn == 2)
		{
			int x, y;
			socket.receive(get_packet);
			get_packet >> x >> y;
			get_packet.clear();

			cout << player << " 12SHOOT " << player_turn << " x:" << x << "y:" << y << endl;

			hit = game.Player_turn(x, y, player_turn);
			if (!hit) // Если игрок попал, он делает еще один выстрел
				player_turn = 1;
		}

		// Обрабатываем события в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
			{
				// тогда закрываем его
				window.close();
			}

			// Выстрел
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
				{
					Vector2i position = Mouse::getPosition(window); 
					
					cout << player << " CHECK " << player_turn << " " << position.x << " " << position.y << endl;

					int add_position;
					if (player_turn == 1) // Если поле второго игрока, то добавить смещение поля от начала окна
						add_position = 0;
					else
						add_position = DRAW_FIELD_SIZE + 10;

					// Определение координат клетки
					int x = (position.x - add_position) / (CELL_SIZE + 1);
					int y = position.y / (CELL_SIZE + 1);

					cout << player << " CHECK2 " << player_turn << " " << x << " " << y << endl;

					if (player_turn == 1 && player == 1 && position.x < 470) // Ход первого игрока и выстрел по его полю?
					{
						hit = game.Player_turn(x, y, player_turn); // Вызов функции выстрела, возвращает попадание или промах

						cout << endl << " shooting " << endl;

						send_packet << x << y;
						socket.send(send_packet);
						send_packet.clear();

						if (!hit) // Если игрок попал, он делает еще один выстрел
							player_turn = 2; 
					}
					if (player_turn == 2 && player == 2 && position.x > 480)
					{
						hit = game.Player_turn(x, y, player_turn);

						cout << endl << " shooting " << endl;

						send_packet << x << y;
						socket.send(send_packet);
						send_packet.clear();

						if (!hit)
							player_turn = 1;
					}
				}
			}

			window.clear();
			window.draw(game);
			window.display();

		}
	}

	return 0;

}