// Mandelbrot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ComplexPlane.h"


int main()
{
	double width = VideoMode::getDesktopMode().width /2;
	double height = VideoMode::getDesktopMode().height /2;
	VideoMode vm(width, height);
	RenderWindow window(vm, "Mandelbrot", Style::Default);
	
	ComplexPlane ComplexPlane(width, height);

	Font font;
	font.loadFromFile("fonts/ARIAL.TTF");

	Text text;
	text.setFont(font);


	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				Vector2i mousePixel(Mouse::getPosition(window));

				if (event.mouseButton.button == Mouse::Left)
				{
					ComplexPlane.zoomIn();
					ComplexPlane.setCenter(mousePixel);
				}

				if (event.mouseButton.button == Mouse::Right)
				{
					ComplexPlane.zoomOut();
					ComplexPlane.setCenter(mousePixel);
				}
			}

			if (event.type == Event::MouseMoved) 
			{
				Vector2i mousePixel(event.mouseMove.x, event.mouseMove.y);
				ComplexPlane.setMouseLocation(mousePixel);
			}

			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}
		}
		ComplexPlane.updateRender();
		ComplexPlane.loadText(text);

		window.clear();

		window.draw(ComplexPlane);
		window.draw(text);

		window.display();
	}
}
