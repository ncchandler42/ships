#ifndef AI1_HPP
#define AI1_HPP

#include <cmath>
#include "ship.hpp"

// F_g = (G m_1 m_2 / |r|^2) r_hat
// a_g = F_g / m_1

const float dodge_rad = SHIP_RAD + 75;
const float Gm_lasers = 1e9;
const float Gm_center = 5e1;

const float ANGLE_DEG = 10;
const float SIN_ANGLE = std::sin(ANGLE_DEG * 3.14159 / 180);

void ai1(Ship& my_ship, const sf::Time& del) {
	std::vector<sf::Vector2f> lasers = my_ship.get_enemy_lasers();
	sf::Vector2f acc = {0, 0};

	sf::Vector2f my_pos = my_ship.get_pos();
	for (auto bolt: lasers) {
		float d = dist(bolt, my_pos);
		if (d < dodge_rad) 
			acc += (my_pos-bolt) * Gm_lasers / (d*d); // laserphobic
	}
	
	auto bounds = static_cast<sf::FloatRect>(my_ship.get_bounds());
	sf::Vector2f center = {my_ship.get_enemy_pos().x, bounds.height/2};

	acc += (center-my_pos) * Gm_center * dist(center, my_pos); // but comfortable in the center

	sf::Vector2f ene_pos = my_ship.get_enemy_pos();
	float tri_base = dist(my_pos, ene_pos)*SIN_ANGLE; // Ship fires if enemy is within a cone of sight

	if (std::abs(my_pos.x - ene_pos.x) < tri_base)
		my_ship.fire();

	my_ship.set_vel(acc*del.asSeconds());
}

#endif // AI1_HPP
