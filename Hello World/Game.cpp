#include "Game.h"

Game::Game() :
	view(sf::View(sf::FloatRect(640, 360, 256, 144))), walls{ wall }, zombieShot(false), gameOver(false), score(0)
{
	if (!cursorText.loadFromFile("Textures/pointer.png"))
		std::cout << "ERROR LOADING CURSOR TEXTURE" << std::endl;

	sf::Vector2u textSize = cursorText.getSize();
	textSize.x /= 2;
	textSize.y /= 2;
	cursor.setTexture(cursorText);
	cursor.setTextureRect(sf::IntRect(textSize.x * 0, textSize.y * 0, textSize.x, textSize.y));
	cursor.setOrigin(textSize.x / 2.0f, textSize.y / 2.0f);
	cursor.setScale(0.25, 0.25);

	if (!bgSoundBuffer.loadFromFile("Sound/bgsound.ogg"))
		std::cout << "ERROR LOADING BACKGROUND SOUND" << std::endl;

	bgSound.setBuffer(bgSoundBuffer);

	if (!ricochetBuffer.loadFromFile("Sound/ricochet.ogg"))
		std::cout << "ERROR LOADING RICOCHET SOUND" << std::endl;

	ricochetSfx.setBuffer(ricochetBuffer);

	if (!attackedBuffer.loadFromFile("Sound/attacked.ogg"))
		std::cout << "ERROR LOADING BITE SOUND" << std::endl;

	attacked.setBuffer(attackedBuffer);

	//size							position
	walls[0].initWall(sf::Vector2f(64.0f, 96.0f), sf::Vector2f(416.0f, 656.0f));
	walls[1].initWall(sf::Vector2f(64.0f, 64.0f), sf::Vector2f(32.0f, 672.0f));
	//dinding sebelah kiri 96 pixel dari dinding kanan
	walls[2].initWall(sf::Vector2f(128.0f, 480.0f), sf::Vector2f(192.0f, 464.0f));
	//box in the middle
	walls[3].initWall(sf::Vector2f(32.0f, 32.0f), sf::Vector2f(624.0f, 400.0f));
	//dinding kiri 160 pixel dari tengah
	walls[4].initWall(sf::Vector2f(32.0f, 192.0f), sf::Vector2f(464.0f, 608.0f));
	walls[5].initWall(sf::Vector2f(96.0f, 288.0f), sf::Vector2f(432.0f, 368.0f));
	walls[6].initWall(sf::Vector2f(160.0f, 64.0f), sf::Vector2f(400.0f, 192.0f));
	walls[7].initWall(sf::Vector2f(96.0f, 64.0f), sf::Vector2f(368.0f, 128.0f));
	walls[8].initWall(sf::Vector2f(160.0f, 32.0f), sf::Vector2f(432.0f, 80.0f));
	//dinding pojok kiri
	walls[9].initWall(sf::Vector2f(32.0f, 416.0f), sf::Vector2f(16.0f, 208.0f));
	walls[10].initWall(sf::Vector2f(192.0f, 64.0f), sf::Vector2f(128.0f, 128.0f));
	walls[11].initWall(sf::Vector2f(32.0f, 64.0f), sf::Vector2f(16.0f, 608.0f));
	//labirin kanan
	walls[12].initWall(sf::Vector2f(96.0f, 96.0f), sf::Vector2f(752.0f, 112.0f));
	walls[13].initWall(sf::Vector2f(288.0f, 32.0f), sf::Vector2f(848.0f, 16.0f));
	walls[14].initWall(sf::Vector2f(384.0f, 96.0f), sf::Vector2f(1056.0f, 112.0f));
	walls[15].initWall(sf::Vector2f(64.0f, 64.0f), sf::Vector2f(1248.0f, 32.0f));
	walls[16].initWall(sf::Vector2f(32.0f, 64.0f), sf::Vector2f(1232.0f, 192.0f));
	walls[17].initWall(sf::Vector2f(96.0f, 448.0f), sf::Vector2f(1200.0f, 448.0f));
	walls[18].initWall(sf::Vector2f(32.0f, 32.0f), sf::Vector2f(1264.0f, 528.0f));
	walls[19].initWall(sf::Vector2f(96.0f, 448.0f), sf::Vector2f(1040.0f, 448.0f));
	walls[20].initWall(sf::Vector2f(192.0f, 64.0f), sf::Vector2f(896.0f, 640.0f));
	walls[21].initWall(sf::Vector2f(96.0f, 288.0f), sf::Vector2f(752.0f, 528.0f));
	walls[22].initWall(sf::Vector2f(64.0f, 64.0f), sf::Vector2f(736.0f, 256.0f));
	walls[23].initWall(sf::Vector2f(160.0f, 96.0f), sf::Vector2f(848.0f, 272.0f));
	walls[24].initWall(sf::Vector2f(64.0f, 224.0f), sf::Vector2f(898.0f, 432.0f));

	const int tiles[] =
	{	//1 tile = 32 * 32 pixel
			//	1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40
		/*1*/	21, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 66, 67, 66, 67, 66, 67, 66, 67, 20, 16, 8 , 10, 9 , 10, 8 , 16, 61, 62,
		/*2*/	21, 8 , 8 , 8 , 9 , 9 , 10, 25, 25, 26, 26, 27, 27, 27, 27, 27, 25, 25, 35, 36, 36, 36, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 66, 67,
		/*3*/	21, 16, 8 , 8 , 10, 25, 26, 27, 25, 14, 13, 4 , 4 , 6 , 3 , 7 , 8 , 9 , 8 , 9 , 8 , 9 , 4 , 7 , 1 , 51, 9 , 6 , 5 , 1 , 1 , 6 , 1 , 1 , 3 , 1 , 6 , 2 , 1 , 78,
		/*4*/	22, 1 , 4 , 4 , 2 , 75, 3 , 24, 9 , 12, 4 , 6 , 7 , 27, 27, 25, 8 , 9 , 8 , 9 , 9 , 8 , 6 , 1 , 1 , 9 , 10, 3 , 1 , 1 , 1 , 1 , 7 , 1 , 5 , 1 , 1 , 1 , 1 , 77,
		/*5*/	2 , 1 , 1 , 2 , 3 , 3 , 3 , 10, 10, 9 , 7 , 1 , 2 , 16, 9 , 9 , 8 , 8 , 9 , 9 , 10, 10, 4 , 6 , 7 , 16, 8 , 3 , 2 , 1 , 5 , 1 , 4 , 3 , 7 , 4 , 7 , 2 , 5 , 78,
		/*6*/	21, 9 , 16, 10, 10, 16, 10, 9 , 10, 10, 1 , 5 , 4 , 7 , 4 , 10, 10, 10, 10, 10, 10, 10, 16, 16, 16, 52, 16, 8 , 15, 15, 9 , 24, 53, 53, 16, 16, 9 , 10, 3 , 78,
		/*7*/	21, 26, 25, 10, 9 , 8 , 24, 24, 8 , 8 , 2 , 3 , 6 , 2 , 1 , 9 , 10, 10, 10, 10, 10, 10, 16, 16, 8 , 16, 51, 8 , 9 , 9 , 8 , 24, 52, 52, 8 , 10, 10, 14, 4 , 78,
		/*8*/	21, 25, 25, 10, 3 , 1 , 2 , 5 , 8 , 8 , 10, 8 , 1 , 3 , 1 , 9 , 10, 10, 10, 15, 8 , 8 , 1 , 6 , 1 , 7 , 3 , 4 , 3 , 10, 9 , 1 , 61, 62, 8 , 16, 5 , 3 , 1 , 76,
		/*9*/	76, 27, 26, 16, 7 , 1 , 7 , 6 , 16, 8 , 9 , 8 , 5 , 1 , 5 , 10, 9 , 9 , 10, 51, 52, 9 , 1 , 2 , 3 , 5 , 3 , 1 , 2 , 10, 8 , 1 , 66, 67, 8 , 54, 2 , 1 , 6 , 75,
		/*10*/	74, 25, 27, 49, 3 , 1 , 1 , 1 , 16, 8 , 10, 8 , 2 , 7 , 6 , 16, 8 , 9 , 10, 24, 24, 8 , 8 , 8 , 4 , 4 , 1 , 7 , 1 , 16, 16, 1 , 1 , 1 , 16, 52, 1 , 1 , 1 , 79,
		/*11*/	75, 31, 27, 49, 4 , 5 , 5 , 1 , 16, 8 , 10, 8 , 3 , 5 , 6 , 16, 8 , 9 , 10, 28, 29, 24, 16, 16, 16, 16, 8 , 1 , 74, 10, 10, 1 , 1 , 1 , 16, 16, 4 , 5 , 7 , 21,
		/*12*/	21, 31, 48, 9 , 1 , 2 , 1 , 4 , 16, 8 , 10, 8 , 1 , 1 , 2 , 9 , 8 , 9 , 10, 30, 31, 8 , 16, 16, 16, 16, 8 , 7 , 3 , 24, 54, 1 , 1 , 1 , 10, 8 , 1 , 4 , 1 , 21,
		/*13*/	21, 24, 49, 10, 4 , 4 , 1 , 5 , 16, 8 , 10, 8 , 4 , 1 , 1 , 8 , 9 , 8 , 10, 20, 11, 16, 5 , 1 , 5 , 16, 16, 3 , 1 , 9 , 10, 1 , 1 , 1 , 16, 10, 3 , 5 , 1 , 21,
		/*14*/	11, 24, 49, 10, 5 , 7 , 4 , 5 , 16, 8 , 10, 8 , 2 , 1 , 2 , 10, 8 , 25, 10, 8 , 53, 52, 4 , 1 , 3 , 16, 16, 1 , 5 , 9 , 10, 1 , 1 , 1 , 10, 9 , 3 , 2 , 1 , 21,
		/*15*/	9 , 24, 49, 10, 4 , 7 , 3 , 2 , 16, 8 , 10, 8 , 3 , 4 , 1 , 8 , 8 , 25, 10, 16, 16, 16, 4 , 5 , 1 , 16, 16, 2 , 6 , 10, 10, 1 , 1 , 1 , 10, 9 , 1 , 1 , 4 , 21,
		/*16*/	8 , 24, 49, 10, 7 , 6 , 1 , 3 , 25, 8 , 10, 8 , 6 , 5 , 4 , 8 , 8 , 24, 10, 16, 16, 16, 1 , 1 , 1 , 16, 16, 1 , 6 , 9 , 10, 76, 1 , 1 , 8 , 10, 7 , 3 , 1 , 21,
		/*17*/	8 , 24, 49, 10, 6 , 1 , 6 , 1 , 26, 8 , 10, 8 , 10, 10, 4 , 9 , 24, 24, 10, 8 , 16, 16, 4 , 6 , 4 , 16, 16, 4 , 20, 15, 15, 1 , 1 , 1 , 9 , 16, 5 , 1 , 7 , 21,
		/*18*/	16, 24, 49, 10, 6 , 3 , 3 , 7 , 9 , 9 , 8 , 9 , 10, 10, 1 , 24, 14, 13, 10, 27, 16, 16, 1 , 7 , 1 , 8 , 8 , 51, 57, 24, 25, 1 , 1 , 1 , 8 , 16, 7 , 7 , 1 , 16,
		/*19*/	62, 54, 49, 10, 6 , 5 , 2 , 6 , 8 , 8 , 9 , 8 , 9 , 9 , 2 , 24, 12, 11, 24, 10, 25, 16, 1 , 1 , 1 , 52, 51, 56, 52, 25, 24, 1 , 1 , 1 , 9 , 8 , 1 , 1 , 1 , 16,
		/*20*/	67, 54, 49, 12, 7 , 1 , 2 , 7 , 8 , 9 , 8 , 8 , 22, 22, 7 , 24, 25, 25, 24, 10, 25, 16, 7 , 5 , 3 , 2 , 5 , 6 , 2 , 3 , 2 , 1 , 1 , 1 , 9 , 8 , 2 , 1 , 5 , 16,
		/*21*/	61, 62, 49, 12, 5 , 1 , 4 , 4 , 9 , 9 , 9 , 9 , 23, 23, 1 , 24, 24, 24, 25, 10, 25, 16, 3 , 3 , 1 , 1 , 2 , 4 , 1 , 7 , 1 , 1 , 1 , 1 , 10, 16, 1 , 3 , 4 , 16,
		/*22*/	66, 67, 49, 12, 3 , 4 , 2 , 1 , 8 , 8 , 8 , 9 , 22, 23, 5 , 25, 25, 26, 27, 10, 26, 26, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
		/*23*/	47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47,

	};
	map.load(tiles);
	srand(static_cast<unsigned int>(time(NULL)));
}

Game::~Game() {
	zombies = std::vector<Zombie>();
	bullets = std::vector<Bullet>();
}

sf::Vector2f Game::checkViewCenter() {
	float deltaViewX = 0;
	float deltaViewY = 0;
	if (player.getCharCoord().x - 128 < 0)
		deltaViewX = player.getCharCoord().x - 128;

	else if (player.getCharCoord().x + 128 > 1280)
		deltaViewX = (player.getCharCoord().x + 128) - 1280;

	if (player.getCharCoord().y - 72 < 0)
		deltaViewY = player.getCharCoord().y - 72;

	else if (player.getCharCoord().y + 72 > 720)
		deltaViewY = (player.getCharCoord().y + 72) - 720;

	return sf::Vector2f(player.getCharCoord().x - deltaViewX, player.getCharCoord().y - deltaViewY);
}

void Game::updatePlayer() {
	if (player.healthPoints <= 0.f) {
		gameOver = true;
		gOver.setScore(score);
	}
	sf::RenderWindow* win = window.getWindow();
	sf::Vector2f playerPos = player.getCharCoord();
	player.updatePlayer(win);

	GUI.setHealthAndScore(player.healthPoints, score, checkViewCenter());

	//fire
	if (player.fire && !player.isFiring) {
		b1.fire(win, playerPos);
		bullets.push_back(Bullet(b1));
	}

	for (size_t i = 0; i < bullets.size(); ++i) {
		bullets[i].projectile.move(bullets[i].currVelocity);
		if (bullets[i].projectile.getPosition().x < 0 || bullets[i].projectile.getPosition().x > 1280
			|| bullets[i].projectile.getPosition().y < 0 || bullets[i].projectile.getPosition().y > 720) {
			//removing bullets out of the map
			bullets.erase(bullets.begin() + i);
			bullets.shrink_to_fit();
		}
	}
}

void Game::updateWalls() {
	//Wall Collision
	for (size_t i = 0; i < walls.size(); ++i) {
		walls[i].GetCollider().checkCollision(player.GetCollider());
		for (size_t j = 0; j < zombies.size(); ++j)
			walls[i].GetCollider().checkCollision(zombies[j].GetCollider());
		for (size_t j = 0; j < bullets.size(); ++j) {
			if (bullets[j].projectile.getGlobalBounds().intersects(walls[i].body.getGlobalBounds())) {
				float distance = sqrt(pow((player.getCharCoord().x - bullets[j].projectile.getPosition().x), 2) + pow((player.getCharCoord().y - bullets[j].projectile.getPosition().y), 2));
				ricochetSfx.setVolume(std::min(std::max((100.f - distance / 5.f), 1.f), 100.f));
				bullets.erase(bullets.begin() + j);
				bullets.shrink_to_fit();
				ricochetSfx.play();
			}
		}
	}
}

void Game::updateZombie(sf::Vector2f playerPos)
{
	if (zombies.size() < 6) {
		zombies.push_back(zombie);
		zombies[zombies.size() - 1].setLocation();
	}

	for (size_t i = 0; i < zombies.size(); ++i) {
		zombieShot = false;
		if (sqrt(pow((zombies[i].zombiePosition.x - playerPos.x), 2) + pow((zombies[i].zombiePosition.y - playerPos.y), 2)) > 20.f)
			zombies[i].Move(playerPos);
		else {
			if (zombies[i].attack()) {
				player.healthPoints = player.isMidNight ? player.healthPoints -= 35 : player.healthPoints -= 20;
				attacked.play();
			}
		}
		for (size_t j = 0; j < bullets.size(); ++j) {
			if (bullets[j].projectile.getGlobalBounds().intersects(zombies[i].zombieSprite.getGlobalBounds())) {
				bullets.erase(bullets.begin() + j);
				bullets.shrink_to_fit();
				zombieShot = true;
			}
		}
		zombies[i].update(zombieShot, playerPos, player.isMidNight);
		if (zombies[i].reallyDead) {
			zombies.erase(zombies.begin() + i);
			score += 10;
			--i;
		}
	}
}

void Game::Update() {

	window.Update(); // Update window events.

	if (window.checkIfBegin() && !gameOver && !window.help)
	{
		cursor.setPosition(player.getMousePos());

		updateZombie(player.getCharCoord());
		updatePlayer();
		updateWalls();

		if (bgSound.getStatus() == 0 && rand() % 500 == 8)
			bgSound.play();

		//set view relative to player
		view.setSize(sf::Vector2f(256, 144));
		view.setCenter(checkViewCenter());
	}

	else if (!window.checkIfBegin() || gameOver || window.help)
	{
		if (window.help)
			gOver.help();
		if (gOver.select(gameOver, window.getWindow()) && gameOver) {
			gameOver = false;
			player.healthPoints = 100;
			window.togglePlay();
			gOver.toggler = false;
			player.resetLocation();
			player.resetTime();
			zombies.clear();
			score = 0;
		}
		else if (gOver.helpSelect(window.help, window.getWindow()) && window.help) {
			window.help = false;
			window.togglePlay();
			gOver.helpToggler = false;
		}
		view.setSize(sf::Vector2f(1280, 720));
		view.setCenter(sf::Vector2f(640, 360));
	}

}

void Game::Render()
{
	window.View(view);
	window.BeginDraw();
	if (window.checkIfBegin() && !gameOver && !window.help) {
		window.Draw(map);
		for (size_t i = 0; i < zombies.size(); ++i) {
			if (zombies[i].zombieSprite.getPosition().x < (checkViewCenter().x + 150) && zombies[i].zombieSprite.getPosition().x >(checkViewCenter().x - 150) && zombies[i].zombieSprite.getPosition().y < (checkViewCenter().y + 90) && zombies[i].zombieSprite.getPosition().y >(checkViewCenter().y - 90)) {
			window.Draw(zombies[i].zombieSprite);
			if (zombies[i].bloodSplattered)
				window.Draw(zombies[i].blood);
			}
		}
		window.Draw(*player.getLegsSprite());
		window.Draw(*player.getPlayerSprite());
		window.Draw(*player.getGunSprite());
		for (size_t i = 0; i < bullets.size(); ++i) {
			if (bullets[i].projectile.getPosition().x < (checkViewCenter().x + 128) && bullets[i].projectile.getPosition().x >(checkViewCenter().x - 128) && bullets[i].projectile.getPosition().y < (checkViewCenter().y + 72) && bullets[i].projectile.getPosition().y >(checkViewCenter().y - 72))
				window.Draw(bullets[i].projectile);
		}
		window.Draw(player.nightBox);
		window.Draw(cursor);
		window.Draw(*GUI.getHealthGUI());
		window.Draw(*GUI.getScoreGUI());
	}
	else if (gameOver)
	{
		for (size_t i = 0; i < 3; ++i)
			window.Draw(gOver.text[i]);
	}
	else if (!window.checkIfBegin() && window.help)
	{
		for (size_t i = 0; i < 2; ++i)
			window.Draw(gOver.text[i]);
	}
	//for drawing in main menu
	else if (!window.checkIfBegin() && !window.help)
	{
		sf::Sprite* temp = nullptr;
		window.Draw(*temp);
	}

	window.EndDraw(gameOver);
}

Window* Game::GetWindow()
{
	return &window;
}

void Game::Run()
{
	while (!window.IsDone())
	{
		Update();
		Render();
	}
}