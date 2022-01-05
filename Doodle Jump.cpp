#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 700), "Pikachu Jump", sf::Style::Default);
	window.setFramerateLimit(60);

	sf::Texture backgroundTexture;
	sf::Texture playerTexture;
	sf::Texture platformTexture;
	sf::Texture mainmenubgTexture;

	backgroundTexture.loadFromFile("images/background.png");
	playerTexture.loadFromFile("images/pikachu.png");
	platformTexture.loadFromFile("images/platform.png");
	mainmenubgTexture.loadFromFile("images/mainmenubg.png");

	sf::Sprite background(backgroundTexture);
	sf::Sprite player(playerTexture);
	sf::Sprite platform(platformTexture);
	sf::Sprite mainmenubg(mainmenubgTexture);

	sf::Font font;
	font.loadFromFile("font/BaksoSapi.otf");

	sf::Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setString("Game Over!");
	gameoverText.setCharacterSize(70);
	gameoverText.setFillColor(sf::Color::Red);

	sf::Text restartText;
	restartText.setFont(font);
	restartText.setString("Press enter to restart");
	restartText.setCharacterSize(25);
	restartText.setFillColor(sf::Color::Blue);

	sf::Text tomainmenuText;
	tomainmenuText.setFont(font);
	tomainmenuText.setString("Press space to back to main menu");
	tomainmenuText.setCharacterSize(25);
	tomainmenuText.setFillColor(sf::Color::Blue);

	sf::Text mainmenuText1;
	mainmenuText1.setFont(font);
	mainmenuText1.setString("PIKACHU");
	mainmenuText1.setCharacterSize(80);
	mainmenuText1.setFillColor(sf::Color::Blue);

	sf::Text mainmenuText2;
	mainmenuText2.setFont(font);
	mainmenuText2.setString("JUMP");
	mainmenuText2.setCharacterSize(80);
	mainmenuText2.setFillColor(sf::Color::Blue);

	sf::Text playText;
	playText.setFont(font);
	playText.setString("Press enter to play");
	playText.setCharacterSize(30);
	playText.setFillColor(sf::Color::Magenta);

	sf::Text exitText1;
	exitText1.setFont(font);
	exitText1.setString("Press escape to exit game");
	exitText1.setCharacterSize(30);
	exitText1.setFillColor(sf::Color::Magenta);

	sf::Text exitText2;
	exitText2.setFont(font);
	exitText2.setString("Press escape to exit game");
	exitText2.setCharacterSize(25);
	exitText2.setFillColor(sf::Color::Blue);

	mainmenuText1.setPosition(80, 90);
	mainmenuText2.setPosition(135, 160);
	playText.setPosition(87, 550);
	exitText1.setPosition(45, 580);

	mainmenu:
		sf::Music mainmenusong;
		if (!mainmenusong.openFromFile("sound/mainmenusong.ogg"))
			return -1;
		mainmenusong.play();
		mainmenusong.setLoop(true);
		mainmenusong.setVolume(50);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.draw(mainmenubg);
			window.draw(mainmenuText1);
			window.draw(mainmenuText2);
			window.draw(playText);
			window.draw(exitText1);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				goto game;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				return 0;
		}

	game:
		sf::Music music;
		if (!music.openFromFile("sound/music.ogg"))
			return -1;
		mainmenusong.stop();
		music.play();
		music.setLoop(true);

		sf::SoundBuffer buffer;
		buffer.loadFromFile("sound/jump.wav");
		sf::Sound sound;
		sound.setBuffer(buffer);
		sound.setVolume(30);

		sf::Text scoreText;
		scoreText.setFont(font);
		scoreText.setCharacterSize(30);
		scoreText.setFillColor(sf::Color::Blue);

		sf::Vector2u platformPosition[10];
		std::uniform_int_distribution<unsigned> x(0, 500 - platformTexture.getSize().x);
		std::uniform_int_distribution<unsigned> y(100, 890);
		std::default_random_engine e(time(0));
		for (size_t i = 0; i < 10; ++i)
		{
			platformPosition[i].x = x(e);
			platformPosition[i].y = y(e);
		}
		int playerX = 250;
		int playerY = 151;
		float dy = 0;
		int height = 150;
		int score = 0;

		const int PLAYER_LEFT_BOUNDING_BOX = 20;
		const int PLAYER_RIGHT_BOUNDING_BOX = 40;
		const int PLAYER_BOTTOM_BOUNDING_BOX = 65;

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				playerX -= 4;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				playerX += 4;

			if (playerX > 500)
				playerX = 0;
			if (playerX < -40)
				playerX = window.getSize().x - playerTexture.getSize().x;

			if (playerY == height && dy < (-1.62))
			{
				score += 1;
				scoreText.setString(" Score: " + std::to_string(score));
			}

			dy += 0.2;
			playerY += dy;

			if (playerY < height)
				for (size_t i = 0; i < 10; ++i)
				{
					playerY = height;
					platformPosition[i].y -= dy;
					if (platformPosition[i].y > 700)
					{
						platformPosition[i].y = 0;
						platformPosition[i].x = x(e);
					}
				}

			for (size_t i = 0; i < 10; ++i)
			{
				if ((playerX + PLAYER_RIGHT_BOUNDING_BOX > platformPosition[i].x)
					&& (playerX + PLAYER_LEFT_BOUNDING_BOX < platformPosition[i].x + platformTexture.getSize().x)
					&& (playerY + PLAYER_BOTTOM_BOUNDING_BOX > platformPosition[i].y)
					&& (playerY + PLAYER_BOTTOM_BOUNDING_BOX < platformPosition[i].y + platformTexture.getSize().y)
					&& (dy > 0))
				{
					sound.play();
					dy = -10;
				}
			}
			player.setPosition(playerX, playerY);
			window.draw(background);
			window.draw(player);

			for (size_t i = 0; i < 10; ++i)
			{
				platform.setPosition(platformPosition[i].x, platformPosition[i].y);
				window.draw(platform);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				goto game;

			if (playerY > 800)
			{
				music.stop();
				if (!music.openFromFile("sound/gameover.wav"))
					return -1;
				music.play();
				music.setLoop(false);
				music.setVolume(50);

				gameoverText.setPosition(40, 150);
				scoreText.setPosition(170, 300);
				tomainmenuText.setPosition(30, 500);
				restartText.setPosition(95, 530);
				exitText2.setPosition(80, 560);

				goto gameover;
			}
			window.draw(scoreText);
			window.display();
		}
	gameover:
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.draw(background);
			window.draw(gameoverText);
			window.draw(scoreText);
			window.draw(tomainmenuText);
			window.draw(restartText);
			window.draw(exitText2);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				goto mainmenu;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				goto game;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				return 0;
		}
	return 0;
}