#pragma once
#include "Collision.h"

#include <SFML\Graphics.hpp>

#include "Animation.h"

using namespace sf;
using namespace std;

class Goblin
{
public:
	class Goblin(Texture* texture, Vector2u imageCount, Vector2f Vision, Vector2f AttackSize, float switchTime, float speed);
	~Goblin();
	void Update(float deltaTime);
	void Draw(RenderWindow& window);
	void setAnimation(Texture* texture, Vector2u imageCount, float switchTime, bool repeat);
	void UpdateSprite(float deltaTime);
	Collision GetCollider() {
		return Collision(hitBox, body);
	}
	Collision GetAtkCol() {
		return Collision(AtkHitBox, body);
	}


	void OnCollision(Vector2f direction);
	void OnHitCol(Vector2f direction);
	void OnAtkCol(Vector2f direction);
	void Turn(bool faceright);
	void Dead();
	FloatRect getGlobalBounds() {
		return body.getGlobalBounds(); //gets bounds for detection inside Player.h
	}
	void setPos(sf::Vector2f newPos) {
		body.setPosition(newPos);
	}
	Vector2f GetterPos() {
		return body.getPosition();
	}
	bool getIsAttack() {
		return isAttack;
	}
	bool getOldIsAttack() {
		return oldIsAttack;
	}
private:
	Texture* texture;

	RectangleShape body;
	RectangleShape hitBox;
	RectangleShape AtkHitBox;
	RectangleShape Vision;

	Animation animation;
	float speed;
	bool faceRight;

	bool isAttack;
	bool oldIsAttack;

	int hp;

	bool isDead;
	bool isHit;

	string textureName;
	string texturePath;
	Vector2u frameCount;
	float switchTime;

	Vector2f velocity;
};

