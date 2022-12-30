#include "Game.h"

// Private functions
void Game::initVariables()
{
	this->window = nullptr;

	// Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 50;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 8;
	this->mouseHeld = false;
}
void Game::initWindow()
{
	this->videoMode.height = 1000;
	this->videoMode.width = 1200;

	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}
void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Roboto-Regular.ttf"))
		std::cout << "Error::Game::initFonts::Failed to load font" << "\n";
}
void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
}

// Constructors / Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
}
Game::~Game()
{
    std::cout << "Points: " << this->points << "\n";
	delete this->window;
}

// Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}
const bool Game::getEndGame() const
{
	return this->endGame;
}

// Functions
void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	// Randomize enemy type
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(85.f, 85.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100, 100));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	}

	// Spawn the enemy
	this->enemies.push_back(this->enemy);
}
void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}
void Game::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::updateText()
{
	std::stringstream ss;

    ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health;

	this->uiText.setString(ss.str());
}
void Game::updateEnemies()
{
	// Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;  // Reset the timer
		}
		else
			this->enemySpawnTimer += 1.f;  // Increment the timer
	}

	for (int i = 0; i < this->enemies.size(); i++)
	{
		// Move the enemies
		this->enemies[i].move(0.f, 5.f);

		// Check if enemy is below the window
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);  // Delete the enemy
			this->health--;
		}
	}

	// Check if the enemy is clicked
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld==false)
		{
			this->mouseHeld = true;
			for (size_t i = 0; i < this->enemies.size(); i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					// Add points
					if(this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points+=10;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 7;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 3;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 1;

					// Delete the enemy
					this->enemies.erase(this->enemies.begin() + i);

					break;
				}
			}
		}
	}
	else  this->mouseHeld = false; 
}
void Game::update()
{
	this->pollEvents();

	if (!this->endGame)
	{
		this->updateMousePositions();
		this->updateText();
		this->updateEnemies();
	}

	// End game
	if (this->health <= 0)
		this->endGame = true;
}
void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}
void Game::renderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}
void Game::render()
{
	this->window->clear();

	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}