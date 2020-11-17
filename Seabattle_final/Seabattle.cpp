#include "Seabattle.h"

Seabattle::Seabattle(int mode)
{
	srand(time(0));
	// ���������� ����� ��� ��������� ���������
	font.loadFromFile("..\\calibri.ttf");
	Init(mode);
}

void Seabattle::Init(int mode)
{
	player = mode;

	// ��������� ���� ������� ������
	field_player1 = new int* [FIELD_SIZE];
	field_player2 = new int* [FIELD_SIZE];

	Fill_player_fields(field_player1);
	Fill_player_fields(field_player2);

	// ��� ����������� �� TCP
	TcpSocket socket;
	TcpListener listener;
	std::size_t received;
	IpAddress ip;
	Packet get_cell;
	Packet send_cell;
	int cell;

	switch (mode)
	{
	case 1: // ������
	{
		// �����������
		ip = IpAddress::getLocalAddress();
		socket.connect(ip, 2000);

		for (int i = 0; i < FIELD_SIZE; i++)
			for (int j = 0; j < FIELD_SIZE; j++)
			{
				socket.receive(get_cell);
				get_cell >> cell;
				field_player1[i][j] = cell;
				get_cell.clear();

				socket.receive(get_cell);
				get_cell >> cell;
				field_player2[i][j] = cell;
				get_cell.clear();
			}
	}
	case 2: // ������
	{

		// �����������
		ip = "localhost";
		listener.listen(2000);
		listener.accept(socket);

		for (int i = 0; i < FIELD_SIZE; i++)
			for (int j = 0; j < FIELD_SIZE; j++)
			{
				cell = field_player1[i][j];
				send_cell << cell;
				socket.send(send_cell);
				send_cell.clear();

				cell = field_player2[i][j];
				send_cell << cell;
				socket.send(send_cell);
				send_cell.clear();
			}
	}
	default:
		break;
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			std::cout << field_player1[i][j] << " ";
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			std::cout << field_player2[i][j] << " ";
		std::cout << std::endl;
	}
}

void Seabattle::Fill_player_fields(int** field)
{

	for (int i = 0; i < FIELD_SIZE; i++) // ��������� ������� ���� - ��������� ������ �����
	{
		field[i] = new int[FIELD_SIZE];
		for (int j = 0; j < FIELD_SIZE; j++)
			field[i][j] = 0;
	}

	// ��������� �������
	int rand_row; // ����� ���� ������
	int rand_column; // ����� ������� ������
	int rand_direction; // �����������
	bool is_cells_empty; // �������� �� ��������� ������ 
	int ships_number[5]; // ������� �� ������������� �� ���� ��������. ������ �� 5 ��� �������� �������� ����� ��������

	int ship_length = 4; // �������� ��������� � 4-� ���������

	for (int i = 4; i >= 1; i--) // ���������� �������� ������ �����
		ships_number[i] = 5 - i;

	for (int i = 0; i < SHIPS_NUMBER; i++)
	{

		is_cells_empty = false;

		if (ships_number[ship_length] == 0) // ����������� ����� ���������������� �������
			ship_length -= 1;

		while (!is_cells_empty) // ����� ��������� ������ ��� ������� � �������� �� �����������
		{
			rand_direction = rand() % 2; // ����������� ������� ������ ��� ���� (����������� � ���������)

			// ���������� ��� � ������� � ������ ����� � ����������� �������
			if (rand_direction == direction::right)
			{
				rand_row = rand() % 10;
				rand_column = rand() % (10 - ship_length + 1);
			}
			else if (rand_direction == direction::down)
			{
				rand_row = rand() % (10 - ship_length + 1);
				rand_column = rand() % 10;
			}

			// �������� ������ �� ����������� ������� �� �����������
			if (rand_direction == direction::right)
			{
				is_cells_empty = true;
				for (int i = 0; i < ship_length; i++)
					if (field[rand_row][rand_column + i] != 0)
						is_cells_empty = false;
			}
			else if (rand_direction == direction::down)
			{
				is_cells_empty = true;
				for (int i = 0; i < ship_length; i++)
					if (field[rand_row + i][rand_column] != 0)
						is_cells_empty = false;
			}
		}

		// ���������� ������ ������� � �� ���������
		//222222
		//211112
		//222222
		if (rand_direction == direction::right) // �� �����������
		{
			int row_up = rand_row - 1;
			int row_down = rand_row + 1;
			int column_right = rand_column + ship_length;
			int column_left = rand_column - 1;

			for (int i = 0; i < ship_length; i++) // ���������� ������� � ������ � ����� �������
			{
				field[rand_row][rand_column + i] = 1;
				if (row_up >= 0)
					field[row_up][rand_column + i] = 2;
				if (row_down <= 9)
					field[row_down][rand_column + i] = 2;
			}
			if (column_right <= 9) // ���������� ������ ����� � ������ ������� �� ���������
				for (int i = 0; i < 3; i++)
					if (rand_row - 1 + i <= 9 && rand_row - 1 + i >= 0)
						field[rand_row - 1 + i][column_right] = 2;
			if (column_left >= 0)
				for (int i = 0; i < 3; i++)
					if (rand_row - 1 + i <= 9 && rand_row - 1 + i >= 0)
						field[rand_row - 1 + i][column_left] = 2;
		}
		else if (rand_direction == direction::down) // �� ��������� 
		{
			int row_up = rand_row - 1;
			int row_down = rand_row + ship_length;
			int column_right = rand_column + 1;
			int column_left = rand_column - 1;

			for (int i = 0; i < ship_length; i++)
			{
				field[rand_row + i][rand_column] = 1;
				if (column_right <= 9)
					field[rand_row + i][column_right] = 2;
				if (column_left >= 0)
					field[rand_row + i][column_left] = 2;
			}
			if (row_up >= 0)
				for (int i = 0; i < 3; i++)
					if (rand_column - 1 + i <= 9 && rand_column - 1 + i >= 0)
						field[row_up][rand_column - 1 + i] = 2;
			if (row_down <= 9)
				for (int i = 0; i < 3; i++)
					if (rand_column - 1 + i <= 9 && rand_column - 1 + i >= 0)
						field[row_down][rand_column - 1 + i] = 2;
		}

		ships_number[ship_length]--;
	}
}

void Seabattle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	sf::Color color = sf::Color(200, 100, 200);

	// ������ ����� �������� ����
	sf::RectangleShape shape(sf::Vector2f(DRAW_FIELD_SIZE, DRAW_FIELD_SIZE));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(color);
	shape.setFillColor(sf::Color::Transparent);
	target.draw(shape, states);

	// �������������� ����� ��� ��������� ���� ������
	shape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(color);
	shape.setFillColor(Color::Transparent);

	sf::Text text("", font, 25);
	text.setFillColor(Color(200, 100, 200));
	text.setPosition(100, 100);

	if (player == 1)
	{
		if (pl1_hits == POINTS_FOR_WIN)
		{
			text.setString("YOU WIN");
			target.draw(text, states);
		}
		else if (pl2_hits == POINTS_FOR_WIN)
		{
			text.setString("YOU LOSER HAHA");
			target.draw(text, states);
		}
	}
	if (player == 2)
	{
		if (pl2_hits == POINTS_FOR_WIN)
		{
			text.setString("YOU WIN");
			target.draw(text, states);
		}
		else if (pl1_hits == POINTS_FOR_WIN)
		{
			text.setString("YOU LOSER HAHA");
			target.draw(text, states);
		}
	}

	if (pl1_hits < POINTS_FOR_WIN && pl2_hits < POINTS_FOR_WIN)
	{
		if (player == 2)
			for (int i = 0; i < FIELD_SIZE; i++)
			{
				for (int j = 0; j < FIELD_SIZE; j++)
				{
					shape.setOutlineColor(color);

					if (field_player1[i][j] == 0 || field_player1[i][j] == 2)
						shape.setFillColor(Color::Transparent);
					else if (field_player1[i][j] == 1)
						shape.setFillColor(Color(128, 128, 128));
					else if (field_player1[i][j] == 3)
						shape.setFillColor(Color::Red);
					else if (field_player1[i][j] == 4)
						shape.setFillColor(Color(250, 250, 100, 80));

					// ���������� ������� ������ ��� ���������
					Vector2f position(i * CELL_SIZE + 5.f, j * CELL_SIZE + 5.f);
					shape.setPosition(position);
					target.draw(shape, states);
				}
			}
		else if (player == 1)
			for (int i = 0; i < FIELD_SIZE; i++)
			{
				for (int j = 0; j < FIELD_SIZE; j++)
				{
					shape.setOutlineColor(color);

					if (field_player1[i][j] == 0 || field_player1[i][j] == 1 || field_player1[i][j] == 2)
						shape.setFillColor(Color::Transparent);
					else if (field_player1[i][j] == 3)
						shape.setFillColor(Color::Red);
					else if (field_player1[i][j] == 4)
						shape.setFillColor(Color(250, 250, 100, 80));

					// ���������� ������� ������ ��� ���������
					Vector2f position(i * CELL_SIZE + 5.f, j * CELL_SIZE + 5.f);
					shape.setPosition(position);
					target.draw(shape, states);
				}
			}

		//
		// ������ ����
		//

		color = sf::Color(57, 255, 20);

		// ������ ����� �������� ����
		//sf::RectangleShape shape(sf::Vector2f(DRAW_FIELD_SIZE, DRAW_FIELD_SIZE));
		shape.setSize(sf::Vector2f(DRAW_FIELD_SIZE, DRAW_FIELD_SIZE));
		shape.setOutlineThickness(2.f);
		shape.setOutlineColor(color);
		shape.setFillColor(sf::Color::Transparent);
		shape.setPosition(sf::Vector2f(DRAW_FIELD_SIZE + 10.f, 0));
		target.draw(shape, states);

		// �������������� ����� ��� ��������� ���� ������
		shape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
		shape.setOutlineThickness(2.f);
		shape.setOutlineColor(color);
		shape.setFillColor(Color::Transparent);

		if (player == 1)
			for (int i = 0; i < FIELD_SIZE; i++)
			{
				for (int j = 0; j < FIELD_SIZE; j++)
				{
					shape.setOutlineColor(color);

					if (field_player2[i][j] == 0 || field_player2[i][j] == 2)
						shape.setFillColor(Color::Transparent);
					else if (field_player2[i][j] == 1)
						shape.setFillColor(Color(128, 128, 128));
					else if (field_player2[i][j] == 3)
						shape.setFillColor(Color::Red);
					else if (field_player2[i][j] == 4)
						shape.setFillColor(Color(250, 250, 100, 80));

					// ���������� ������� ������ ��� ���������
					Vector2f position(DRAW_FIELD_SIZE + 10.f + i * CELL_SIZE + 5.f, j * CELL_SIZE + 5.f);
					shape.setPosition(position);
					target.draw(shape, states);
				}
			}
		else if (player == 2)
			for (int i = 0; i < FIELD_SIZE; i++)
			{
				for (int j = 0; j < FIELD_SIZE; j++)
				{
					shape.setOutlineColor(color);

					if (field_player2[i][j] == 0 || field_player2[i][j] == 1 || field_player2[i][j] == 2)
						shape.setFillColor(Color::Transparent);
					else if (field_player2[i][j] == 3)
						shape.setFillColor(Color::Red);
					else if (field_player2[i][j] == 4)
						shape.setFillColor(Color(250, 250, 100, 80));

					// ���������� ������� ������ ��� ���������
					Vector2f position(DRAW_FIELD_SIZE + 10.f + i * CELL_SIZE + 5.f, j * CELL_SIZE + 5.f);
					shape.setPosition(position);
					target.draw(shape, states);
				}
			}
	}
}

bool Seabattle::Player_turn(int x, int y, int player_turn)
{
	bool hit = false; // ��������� ��� ������

	if (player_turn == 1) // ���� �������� ������ �����
	{
		if ((x >= 0 && x <= 9) && (y >= 0 && y <= 9))
		{
			if (field_player1[x][y] == 3 || field_player1[x][y] == 4) // ���� �� ������ ��� ��������, ����� �� ��������� ��� ����� �������
				hit = true;

			if (field_player1[x][y] == 1)
			{
				field_player1[x][y] = 3;
				hit = true;
				pl1_hits++;
			}
			else if (field_player1[x][y] == 0 || field_player1[x][y] == 2)
				field_player1[x][y] = 4;
		}
	}
	else //if (position.x > DRAW_FIELD_SIZE + 10) // ���� �������� ������ ����� � �� ������� ����
	{
		if ((x >= 0 && x <= 9) && (y >= 0 && y <= 9))
		{
			if (field_player2[x][y] == 3 || field_player2[x][y] == 4)
				hit = true;

			if (field_player2[x][y] == 1)
			{
				field_player2[x][y] = 3;
				pl2_hits++;
				hit = true;
			}
			else if (field_player2[x][y] == 0 || field_player2[x][y] == 2)
				field_player2[x][y] = 4;
		}
	}

	return hit;
}