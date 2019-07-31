#ifndef SHIP_HPP
#define SHIP_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

const int SCREEN_SIZE = 600;

const float MAX_SPEED = 250.0;
const float MAX_SPEED_2 = MAX_SPEED*MAX_SPEED;

const float LASER_SPEED = 500.0;
const int MAX_LASERS = 3;
const int LASER_DELAY_MS = 250;

const float SHIP_RAD = 25.0;

float r_2(const sf::Vector2f& vec);
float r(const sf::Vector2f& vec);
float dist_2(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
float dist(const sf::Vector2f& vec1, const sf::Vector2f& vec2);

enum screen_pos
{
	POS_TOP,
	POS_BOTTOM
};

class Laser : public sf::Drawable
{
public:
	Laser(const sf::Vector2f& pos, screen_pos side);

	const sf::Vector2f& get_pos() const { return pos_; }
	const sf::Vector2f& get_vel() const { return vel_; }

	void update(const sf::Time& del);
	
	bool is_ded() { return ded_; }

private:
	void set_pos(const sf::Vector2f& pos);
	void set_vel(const sf::Vector2f& vel);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f pos_;
	sf::Vector2f vel_;
	sf::CircleShape point_;

	bool ded_;

	screen_pos side_;
};

class Ship : public sf::Drawable
{
public:
	Ship(const char file[], screen_pos side);

	const sf::Vector2f& get_pos() const { return pos_; }
	const sf::Vector2f& get_vel() const { return vel_; }
	const std::vector<Laser>& get_lasers() { return bolts_; }
	const sf::IntRect& get_bounds() { return bounds_; }
	
	void set_vel(const sf::Vector2f& vel);
	void set_vel(float x, float y);

	void fire();

	void update(const sf::Time& del);
	void reset();

	void set_enemy(Ship& enemy) { enemy_ = &enemy; }
	const sf::Vector2f get_enemy_pos() const;
	const sf::Vector2f get_enemy_vel() const;
	const std::vector<sf::Vector2f> get_enemy_lasers() const; 

	bool is_ded() { return ded_; }
	bool is_inv() { return inv_; }

private:
	void set_pos(const sf::Vector2f& pos);

	void init_spr(const char file[]);
	bool collides(const sf::Vector2f& pt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Vector2f pos_;
	sf::Vector2f vel_;
	float rad_;

	sf::IntRect bounds_;
	screen_pos side_;

	sf::Texture tx_;
	sf::Sprite spr_;

	std::vector<Laser> bolts_;
	sf::Clock laser_clock;
	sf::Clock inv_clock;
	
	Ship* enemy_;

	bool ded_;
	bool inv_;
};

#endif //SHIP_HPP