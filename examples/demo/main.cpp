#include <sfeGLI/Texture.hpp>
#include <sfeGLI/Sprite.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "sfeGLI demo");
	
	sfe::Texture tex;
	tex.loadFromFile("test.ktx");
	sfe::Sprite spr(tex);

	while (window.isOpen())
	{
		// Event processing
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Request for closing the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear the whole window before rendering a new frame
		window.clear();
		// Draw some graphical entities
		window.draw(spr);
		// End the current frame and display its contents on screen
		window.display();
	}
}