
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Animation.h"
#include "Collision.h"


using namespace sf;
using namespace std;
class Player {
public:
	Player(Texture* texture, Vector2u imageCount, Vector2f AtkHBSize, float switchTime, float speed, float jumpHeight);
	~Player();

	void Update(float deltaTime);
	void UpdateSprite(float deltaTime);
	void Draw(RenderWindow& window);

	void OnCollision(Vector2f direction);
	void OnHitCol(Vector2f direction);
	bool getIsDead() {
		return isDead;
	}
 


	//getter for player position for camera positioning!
	Vector2f GetterPos() {
		return body.getPosition();
	}
	Collision GetCollider() {
		return Collision(hitBox, body);
	}
	Collision GetAtkCol() {
		return Collision(AtkHitBox, body);
	}

	//bool isCollidingWithCoin(Coin* body) {
	//	if (Player::body.getGlobalBounds().intersects(body->getGlobalBounds())) {
	//		return true;
	//	}
	//	return false;
	//}

	void setPos(Vector2f newPos) {
		body.setPosition(newPos);
	}
	bool getIsAtk() {
		return isAttack;
	}
	bool getOldIsAtk() {
		return oldIsAttack;
	}

	void setAnimation(Texture* texture, Vector2u imageCount, float switchTime);
	void setAnimation(Texture* texture, Vector2u imageCount, float switchTime, bool repeat);

private:
	RectangleShape body;
	RectangleShape hitBox;
	RectangleShape AtkHitBox;

	Animation animation;
	float speed;
	bool faceRight;

	bool isInJump;
	bool isTakeHit;
	bool isDead;

	int hp;

	bool isFalling;
	bool isAttack;
	bool oldIsAttack;

	Vector2f velocity;


	Texture* texture;

	string textureName;
	string texturePath;
	Vector2u frameCount;
	float switchTime;

	float jumpHeight;

};