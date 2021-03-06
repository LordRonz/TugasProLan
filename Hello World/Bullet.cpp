#include "Bullet.h"

Bullet::Bullet() :projectile(sf::CircleShape(.8f)), currVelocity(0.0f, 0.0f), maxSpeed(35.f)
{
	projectile.setFillColor(sf::Color::Black);
	projectile.setOrigin(projectile.getRadius(), projectile.getRadius());
}

void Bullet::fire(sf::RenderWindow *window, const sf::Vector2f &playerCenter)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	sf::Vector2f worldPosition = window->mapPixelToCoords(mousePosition);
	aimDir = sf::Vector2f(worldPosition) - playerCenter;
	aimDirNorm = aimDir / static_cast<float>(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));
	projectile.setPosition(playerCenter);
	currVelocity = aimDirNorm * maxSpeed;
}

sf::CircleShape &Bullet::getBullet()
{
	return projectile;
}