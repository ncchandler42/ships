#ifndef AI2_HPP
#define AI2_HPP

#include <SFML/Window.hpp> // for keyboard inputs
#include "ship.hpp"

void ai2(Ship& my_ship, const sf::Time& del)
{
	//my_ship.get_enemy_pos()
	//my_ship.get_enemy_vel()
	//my_ship.get_enemy_lasers()

	float x_vel = 0;
	float y_vel = 0;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		x_vel -= MAX_SPEED;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		x_vel += MAX_SPEED;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		y_vel += MAX_SPEED;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		y_vel -= MAX_SPEED;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		my_ship.fire();
	}
	
	my_ship.set_vel(x_vel, y_vel);
}

#endif // AI2_HPP
