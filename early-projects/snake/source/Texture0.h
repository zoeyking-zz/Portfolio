#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Texture0
{
public:
	Texture0(sf::Texture* texture = nullptr);
	~Texture0();

	void setTexture(sf::Texture* texture);
	sf::Texture* getTexture()const;
	Texture0& addFrame(const sf::IntRect& rect);
	Texture0& addFramesLine(int number_x, int number_y, int line);
	Texture0& addFramesColumn(int number_x, int number_y, int column);

	size_t size()const;
	const sf::IntRect& getRect(size_t index)const;

private:
	friend class AnimatedSprite;
	std::vector<sf::IntRect> _frames;
	sf::Texture* _texture;
};

