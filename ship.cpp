#include <iostream>
#include <cmath>
#include "ship.hpp"

float r_2(const sf::Vector2f& vec)
{
	return vec.x*vec.x + vec.y*vec.y;
}

float r(const sf::Vector2f& vec)
{
	return std::sqrt(r_2(vec));
}

float dist_2(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	return (vec2.x - vec1.x)*(vec2.x - vec1.x) + (vec2.y - vec1.y)*(vec2.y - vec1.y);
}

float dist(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	return std::sqrt(dist_2(vec1, vec2));
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ship::Ship(const char file[], screen_pos side)
{
	side_ = side;

	init_spr(file);

	rad_ = SHIP_RAD;
	bounds_ = sf::IntRect(0, 0, SCREEN_SIZE, SCREEN_SIZE/2-50);

	set_pos(sf::Vector2f(bounds_.width/2, bounds_.height/2));
	set_vel(sf::Vector2f(0,0));

	ded_ = false;
	inv_ = false;

	laser_clock.restart();
	inv_clock.restart();
}

void Ship::set_vel(const sf::Vector2f& vel)
{
	sf::Vector2f new_vel = vel;

	if (r_2(new_vel) > MAX_SPEED_2)
		new_vel = new_vel / r(new_vel) * MAX_SPEED;

	vel_ = new_vel;
}

void Ship::set_vel(float x, float y)
{
	set_vel(sf::Vector2f(x, y));
}

void Ship::set_pos(const sf::Vector2f& pos)
{
	pos_ = pos;
}

void Ship::fire()
{
	if (laser_clock.getElapsedTime().asMilliseconds() > LASER_DELAY_MS && bolts_.size() < MAX_LASERS)
	{
		bolts_.emplace_back( Laser(get_pos(), side_) );
		laser_clock.restart();
	}
}

void Ship::update(const sf::Time& del)
{
	sf::Vector2f new_pos = get_pos() + get_vel()*del.asSeconds();

	if (new_pos.x-rad_ < bounds_.left)
		new_pos.x = bounds_.left+rad_;
	if (new_pos.x+rad_ > bounds_.width)
		new_pos.x = bounds_.width-rad_;

	if (new_pos.y-rad_ < bounds_.top)
		new_pos.y = bounds_.top+rad_;
	if (new_pos.y+rad_ > bounds_.height)
		new_pos.y = bounds_.height-rad_;

	set_pos(new_pos);

	switch (side_)
	{
		case POS_TOP:
			spr_.setPosition(new_pos);
			break;
		case POS_BOTTOM:
			spr_.setPosition(sf::Vector2f(new_pos.x, SCREEN_SIZE - new_pos.y));
			break;
	}

	for(auto bolt = std::begin(bolts_); bolt != std::end(bolts_);)
	{
		bolt->update(del);

		if (bolt->is_ded())
			bolts_.erase(bolt); // erase increments the iterator
		else
			++bolt;
	}

	for (const sf::Vector2f& bolt: get_enemy_lasers())
	{
		if (collides(bolt))
		{
			bolts_.clear();
			ded_ = true;
		}
	}

	if (is_inv() && inv_clock.getElapsedTime().asSeconds() > 0.75)
		inv_ = false;

	if (is_inv())
		spr_.setColor(sf::Color::Green);
	else
		spr_.setColor(sf::Color::White);
}

void Ship::reset()
{
	set_pos(sf::Vector2f(bounds_.width/2, bounds_.height/2));
	set_vel(sf::Vector2f(0,0));

	ded_ = false;
	inv_ = true;
	inv_clock.restart();
	laser_clock.restart();

	bolts_.clear();
}

bool Ship::collides(const sf::Vector2f& pt)
{
	return !is_inv() && dist(get_pos(), pt) < rad_;
}

void Ship::init_spr(const char file[])
{
	if(!tx_.loadFromFile(file))
		std::cerr << "Failed to open file " << file << '\n';//do something?
	spr_.setTexture(tx_);
	spr_.setOrigin(SHIP_RAD, SHIP_RAD);
}

void Ship::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Laser& bolt: bolts_)
		target.draw(bolt);

	target.draw(spr_);
}

const sf::Vector2f Ship::get_enemy_pos() const 
{
	return sf::Vector2f(enemy_->get_pos().x, SCREEN_SIZE-enemy_->get_pos().y); 
}

const sf::Vector2f Ship::get_enemy_vel() const
{
	return sf::Vector2f(enemy_->get_vel().x, -enemy_->get_vel().y); 
}

const std::vector<sf::Vector2f> Ship::get_enemy_lasers() const 
{
	std::vector<sf::Vector2f> ene_bolts;

	for (const Laser& bolt: enemy_->get_lasers())
		ene_bolts.emplace_back(sf::Vector2f(bolt.get_pos().x, SCREEN_SIZE-bolt.get_pos().y));

	return ene_bolts;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Laser::Laser(const sf::Vector2f& pos, screen_pos side)
{
	set_pos(pos);
	set_vel(sf::Vector2f(0, LASER_SPEED));

	point_.setRadius(2);

	if (side == POS_BOTTOM)
		point_.setFillColor(sf::Color::Red);
	else
		point_.setFillColor(sf::Color::Blue);

	side_ = side;
	ded_ = false;
}

void Laser::set_vel(const sf::Vector2f& vel)
{
	vel_ = vel;
}

void Laser::set_pos(const sf::Vector2f& pos)
{
	pos_ = pos;
}

void Laser::update(const sf::Time& del)
{
	sf::Vector2f new_pos = get_pos() + get_vel()*del.asSeconds();

	set_pos(new_pos);
	switch (side_)
	{
		case POS_TOP:
			point_.setPosition(new_pos);
			break;
		case POS_BOTTOM:
			point_.setPosition(sf::Vector2f(new_pos.x, SCREEN_SIZE-new_pos.y));
			break;
	}

	if (new_pos.y > SCREEN_SIZE)
		ded_ = true;
}

void Laser::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(point_);
}
