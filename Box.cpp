#include "Box.h"

Box::Box(Texture* texture, Vector2f size, Vector2f position)
{
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	//body.setFillColor(Color::Black);
	body.setTexture(texture);
	body.setPosition(position);
	hitBox.setSize(Vector2f(size.x, size.y));
	hitBox.setOrigin(size / 2.0f);
	hitBox.setPosition(body.getPosition());
}


Box::~Box()
{
}

void Box::Draw(RenderWindow& window) {
	window.draw(body);
}
