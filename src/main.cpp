#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>

#define WIDTH 40
#define HEIGHT 15

struct Pos {
	int x, y;
	Pos() = default;
	Pos(int x, int y) : x(x), y(y) {}

	// Overload operator == for Pos structure
	bool operator==(const Pos& pos2) const {
		return ((this->x == pos2.x) && (this->y == pos2.y));
	}
};

struct Segment {
	std::string symbol; // Symblo S or O
	Pos position; // position of segment
};

class Game {
private:
	HANDLE console_ = GetStdHandle(STD_OUTPUT_HANDLE);
	enum Color { RED = 4, GREEN = 2, GRAY = 8, WHITE = 15 };
	enum Direction { UP, DOWN, LEFT, RIGHT } dir_;
	std::vector<Segment> snake_;
	Pos fruit_pos_;
	bool alive_, pause_ = false;

public:
	int score;

private:
	// Init class fields
	void init() {
		hideCursor();
		system("cls");
		alive_ = true;
		dir_ = Direction::RIGHT; // Start snake direction
		fruit_pos_ = { WIDTH/2, HEIGHT/2 }; // Start fruit position
		snake_ = { {"S", {1, 1}} }; // Put snake head into snake vector
		score = 0;
	}

	// Hide cursor
	void hideCursor() {
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(consoleHandle, &info);
	}

	// Print colored text to console
	void printColoredText(int color, const std::string& text) {
		SetConsoleTextAttribute(console_, color);
		std::cout << text;
		SetConsoleTextAttribute(console_, Color::WHITE);
	}

	// Clear board
	void clearBoard(int x, int y) {
		COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
		SetConsoleCursorPosition(console_, coord);
	}

	// Draw board
	void drawBoard() {
		clearBoard(0, 0);
		for (size_t i = 0; i < WIDTH; ++i) printColoredText(Color::GRAY, "#");
		std::cout << std::endl;

		for (size_t i = 0; i < HEIGHT; ++i) {
			for (size_t j = 0; j < WIDTH; ++j) {
				if (j == 0 || j == WIDTH - 1) printColoredText(Color::GRAY, "#");
				else if (snake_[0].position == Pos(j, i)) printColoredText(Color::GREEN, snake_[0].symbol);
				else if (fruit_pos_ == Pos(j, i)) printColoredText(Color::RED, "F");
				else {
					bool can_print = false;
					for (size_t k = 1; k < snake_.size(); ++k)
						if (snake_[k].position == Pos(j, i)) {
							can_print = true;
							printColoredText(Color::GREEN, snake_[k].symbol);
						}
					
					if (!can_print) std::cout << ' ';
				}
			}
			std::cout << std::endl;
		}

		for (size_t i = 0; i < WIDTH; ++i) printColoredText(Color::GRAY, "#");
		std::cout << "\nSCORE: " << score;
	}

	// Handle direction of the snake
	void handleDir() {
		switch (dir_) {
			case Game::UP: snake_[0].position.y--; break;
			case Game::DOWN: snake_[0].position.y++; break;
			case Game::LEFT: snake_[0].position.x--; break;
			case Game::RIGHT: snake_[0].position.x++; break;
		}
		handleBarrier();
	}

	// Check if snake in the board
	void handleBarrier() {
		if (snake_[0].position.x >= WIDTH - 1 || snake_[0].position.x <= 0 || snake_[0].position.y >= HEIGHT || snake_[0].position.y < 0) {
			alive_ = false;
		}
	}

	// Update snake segments position
	void updateSegments() {
		for (size_t i = snake_.size() - 1; i > 0; --i)
			snake_[i].position = snake_[i-1].position;
	}

	// Get input
	void checkInput() {
		if (_kbhit()) {
			switch (_getch()) {
				case 'w': 
					if (snake_.size() - 1 > 0) {
						if (dir_ != Direction::DOWN) dir_ = Direction::UP;
					} else dir_ = Direction::UP;
					break;
				case 'a': 
					if (snake_.size() - 1 > 0) {
						if (dir_ != Direction::RIGHT) dir_ = Direction::LEFT;
					} else dir_ = Direction::LEFT;
					break;
				case 's': 
					if (snake_.size() - 1 > 0) {
						if (dir_ != Direction::UP) dir_ = Direction::DOWN;
					} else dir_ = Direction::DOWN;
					break;
				case 'd': 
					if (snake_.size() - 1 > 0) {
						if (dir_ != Direction::LEFT) dir_ = Direction::RIGHT;
					} else dir_ = Direction::RIGHT;
					break;
				case 'p':
					pause_ = (pause_) ? false : true;
					break;
			}
		}
	}

	// Spawn fruit randomly
	void spawnFruit() {
		if (snake_[0].position == fruit_pos_) {
			score += 10; // increase score
			fruit_pos_ = { rand()%WIDTH, rand()%HEIGHT }; // set new fruit position
			snake_.push_back({ "O", {0, 0} }); // push new segment to snake_
		}
	}

	// Check if snake head not on their tail
	void checkIfIsItTail() {
		for (size_t i = 2; i < snake_.size(); ++i)
			if (snake_[0].position == snake_[i].position)
				alive_ = false;
	}

public:
	// Start the game
	void start() {
		init();
		while (alive_) {
			checkInput();
			if (pause_) continue;
			drawBoard();
			updateSegments();
			spawnFruit();
			handleDir();
			checkIfIsItTail();

			Sleep(200);
		}
	}
};

int main() {
	Game app;
	app.start();
	return 0;
}