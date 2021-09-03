#include "Ground.h"


Ground::Ground(Texture* texture, Vector2f size, Vector2f position) {
	body.setSize(size);
	body.setOrigin(size / 2.0f);
	body.setTexture(texture);
	body.setPosition(position);
	hitBox.setSize(Vector2f(size.x, size.y - 35));
	hitBox.setOrigin(size);
	hitBox.setPosition(body.getPosition());
}

Ground::~Ground() {}

void Ground::Draw(RenderWindow& window) {
	window.draw(body);
}