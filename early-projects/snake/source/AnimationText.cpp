#include<SFML/Graphics.hpp>
#include"Animation.hpp"
#include"AnimatedSprite.hpp"

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(600, 800), "Example Animation.");
	Texture textures;
	textures.loadFromFile("data/images/eye.png");

	Animation walkLeft(&textures);
	walkLeft.addFramesLine(4, 2, 0);
	Animation walkRight(&textures);
	walkRight.addFramesLine(4, 2, 1);

	//Creation of the animates sprite 
	AnimatedSprite sprite(&walkLeft, AnimatedSprite::Playing, seconds(0.1));
	//game loop 
	Clock clock; 
	while (window.isOpen())
	{
		Time delta = clock.restart();
		Event event; 
		while (window.pollEvent(event))
		{
			if (event.type = Event::Closed)//close event 
				window. close();
		}
		
		float speed = 50;
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			sprite.setAnimation(&walkLeft);
			sprite.play();
			sprite.move(-speed * delta.asSeconds(), 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			sprite.setAnimation(&walkRight);
			sprite.play();
			sprite.move(speed * delta.asSeconds(), 0);
		}
		window.clear();
		sprite.update(delta);
		window.draw(sprite);
		window.display();


	}
	
}