#include "Goblin.h"
#include <SFML\Graphics.hpp>

#include "Animation.h"

Goblin::Goblin(Texture* texture, Vector2u imageCount, Vector2f VisionSize, Vector2f AttackSize, float switchTime, float speed) :
	animation(texture, imageCount, switchTime, true) {
	this->speed = speed;
	this->texture = texture;
	this->switchTime = switchTime;
	hp = 3;
	faceRight = true;
	body.setSize(Vector2f(150.0f, 150.0f));
	//body.setPosition(600.0f, 300.0f);
	body.setTexture(texture);
	body.setOrigin(body.getSize() / 2.0f);
	hitBox.setSize(Vector2f(50.0f, 50.0f));
	hitBox.setOrigin(hitBox.getSize() / 2.0f);
	hitBox.setPosition(body.getPosition());
	Vision.setSize(VisionSize);
	Vision.setOrigin(VisionSize.x - VisionSize.x, VisionSize.y);
	Vision.setPosition(body.getPosition());
	AtkHitBox.setSize(AttackSize);
	AtkHitBox.setOrigin(AttackSize.x - AttackSize.x * 0.8f, AttackSize.y);
	AtkHitBox.setPosition(body.getPosition());
}

Goblin::~Goblin() {}

void Goblin::Draw(RenderWindow& window) {
	window.draw(body);
	//window.draw(AtkHitBox);
}

void Goblin::Update(float deltaTime) {
	oldIsAttack = isAttack;
	if (isHit || isDead || isAttack) {
		velocity.x = 0; //slowly stop moving higher is slower time to stop (aka sonics physics engine kinda)
	}
	else {
		velocity.x = speed;
	}
	velocity.y += 981.0f * deltaTime;

	body.setTextureRect(animation.uvRect);
	body.move(velocity * deltaTime); //not frame specific 
	hitBox.setPosition(body.getPosition());
	if (isDead == false) {
		Vision.setPosition(body.getPosition());
		AtkHitBox.setPosition(body.getPosition());
	}
}

void Goblin::UpdateSprite(float deltaTime) {
	if (isDead == false) {
		string textureName = "";
		if (hp <= 0) {
			textureName = "Death";
			texturePath = "assets/Sprites/Goblin/Death.png";
			frameCount = Vector2u(4, 1);
		}
		else if (isHit) {
			textureName = "Hit";
			texturePath = "assets/Sprites/Goblin/Hit.png";
			frameCount = Vector2u(4, 1);
		}
		else if (isAttack) {
			textureName = "Atk";
			texturePath = "assets/Sprites/Goblin/Attack.png";
			frameCount = Vector2u(8, 1);
		}
		else {
			if (velocity.x == 0) {
				textureName = "Idle";
				texturePath = "assets/Sprites/Goblin/Idle.png";
				frameCount = Vector2u(4, 1);
			}
			else if (velocity.x != 0) {
				textureName = "Run";
				texturePath = "assets/Sprites/Goblin/Run.png";
				frameCount = Vector2u(8, 1);
				if (velocity.x >= 0.0f) {
					faceRight = true; // if he is facing right, face right
					Turn(faceRight);
				}
				else {
					faceRight = false; //if he is NOT facing right, face left
					Turn(faceRight);
				}
			}
		}
		if (textureName != "") {
			if (this->textureName != textureName) {
				this->textureName = textureName;
				texture->loadFromFile(texturePath);
				if (textureName == "Hit")
					setAnimation(texture, frameCount, 0.2f, false);
				else if (textureName == "Death")
					setAnimation(texture, frameCount, 0.2f, false);
				else if (textureName == "Atk")
					setAnimation(texture, frameCount, this->switchTime, false);
				else
					setAnimation(texture, frameCount, this->switchTime, true);
			}
		}
		else {
			cout << "textureName is empty!" << endl;
			cout << velocity.x << velocity.y << endl;
		}

		bool EndAnimation = animation.Update(deltaTime, faceRight);
		if (isAttack && EndAnimation) {
			isAttack = false;
		}
		if (isHit && EndAnimation) {
			isHit = false;
		}
		if (hp <= 0 && EndAnimation) {
			Dead();
		}
	}
}

void Goblin::Dead() {
	isDead = true;
	AtkHitBox.setPosition(AtkHitBox.getPosition().x, -1000);
}

void Goblin::setAnimation(Texture* texture, Vector2u imageCount, float switchTime, bool repeat) {
	animation = Animation(texture, imageCount, switchTime, repeat);
	body.setTexture(texture);
}

void Goblin::Turn(bool faceright) {
	if (faceright) {
		Vision.setOrigin(Vision.getSize().x - Vision.getSize().x, Vision.getSize().y);
		AtkHitBox.setOrigin(AtkHitBox.getSize().x - AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
	}
	else {
		Vision.setOrigin(Vision.getSize().x, Vision.getSize().y);
		AtkHitBox.setOrigin(AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
	}
}

void Goblin::OnAtkCol(Vector2f direction) 
{
	if (isHit == false)
		this->isAttack = true;
	if (direction.x < 0.0f) {
		faceRight = false;
		Turn(faceRight);
		speed = speed * -1;
	}
	else if (direction.x > 0.0f) {
		faceRight = true;
		Turn(faceRight);
		speed = speed * -1;
	}
}

void Goblin::OnHitCol(Vector2f direction) {
	hp--;
	this->isHit = true;
	if (direction.x < 0.0f) {
		faceRight = true;
		Turn(faceRight);
		speed = speed * -1;
	}
	else if (direction.x > 0.0f) {
		faceRight = false;
		Turn(faceRight);
		speed = speed * -1;
	}
}

void Goblin::OnCollision(Vector2f direction) 
{
	if (direction.x < 0.0f) {
		//collision of left
		speed = speed * -1; // moves according to time
	}
	else if (direction.x > 0.0f) {
		//collision of right
		speed = speed * -1;
	}
	if (direction.y < 0.0f) {
		//collision of ground (bottom)
		velocity.y = 0.0f;
	}
	else if (direction.y > 0.0f) {
		//colliding on the top of sonics head
		velocity.y = 0.0f;
	}
}
