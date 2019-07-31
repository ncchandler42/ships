#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>

#include "ship.hpp"
#include "ai1.hpp"
#include "ai2.hpp"

class Game: public sf::Drawable
{
public:
    Game(): s1("./ship1.png", POS_TOP), s2("./ship2.png", POS_BOTTOM), s1_score(0), s2_score(0)
    {
        s1.set_enemy(s2);
        s2.set_enemy(s1);

        font.loadFromFile("./ComicSans.ttf");
    }

    void update()
    {
        sf::Time del = clock.restart();

        ai1(s1, del);
        s1.update(del);

        ai2(s2, del);
        s2.update(del);

        if (s1.is_ded())
        {
            s2_score++;
            s1.reset();
        }
        
        if (s2.is_ded())
        {
            s1_score++;
            s2.reset();
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::Text score;
        score.setFont(font);
        score.setString("P1: " + std::to_string(s1_score) + "\nP2: " + std::to_string(s2_score));
        score.setPosition(0, SCREEN_SIZE/2-40);
        //score.setColor(sf::Color::Black);
        target.draw(score);
        
        target.draw(s1);
        target.draw(s2);
    }

private:
    Ship s1, s2;
    unsigned int s1_score, s2_score;

    sf::Clock clock;
    sf::Font font;
};

#endif // GAME_HPP
