#pragma once
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <SFML\Graphics.hpp>
#include "Collision.h"

using namespace sf;
class Box
{
public:
	Box(Texture* texture, Vector2f size, Vector2f position);
	~Box();

	void Draw(RenderWindow& window);
	Collision GetCollider() {
		return Collision(hitBox, body);
	}

private:
	RectangleShape body;
	RectangleShape hitBox;
};

