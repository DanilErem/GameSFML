#pragma once
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <SFML\Graphics.hpp>

using namespace sf;
class Collision
{
	public:
		Collision();
		Collision(RectangleShape& hitBox, RectangleShape& body);
		~Collision();
		bool CheckCollision(Collision& other, Vector2f& direction, float push); //wall is 0, a light object would be 1
		bool CheckCollision(Collision& other, Vector2f& direction);
		void Move(float dx, float dy) {
			body.move(dx, dy);
		}

		Vector2f GetPositionColl() {
			return hitBox.getPosition();
		} //get for pos with colliding into stuff!
		Vector2f GetHalfSize() {
			return hitBox.getSize() / 2.0f;
		}
	private:
		RectangleShape& hitBox;
		RectangleShape& body;
};

