#include "Player.h"

//do not move this around
Player::Player(Texture* texture, Vector2u imageCount, Vector2f AttackSize, float switchTime, float speed, float jumpHeight) :
	animation(texture, imageCount, switchTime, true) {
	this->texture = texture;
	this->speed = speed;
	this->jumpHeight = jumpHeight;
	this->switchTime = switchTime;
	this->hp = 3;
	faceRight = true;
	body.setSize(Vector2f(180.0f, 180.0f));
	body.setPosition(0.0f, 300.0f); //where player is initially! (sonic in this case)
	body.setTexture(texture);
	body.setOrigin(body.getSize() / 2.0f);
	hitBox.setSize(Vector2f(47.0f, 50.0f));
	hitBox.setOrigin(hitBox.getSize() / 2.0f);
	hitBox.setPosition(body.getPosition());
	AtkHitBox.setSize(AttackSize);
	AtkHitBox.setOrigin(AttackSize.x - AttackSize.x * 0.8f, AttackSize.y);
	AtkHitBox.setPosition(body.getPosition());
}

void Player::setAnimation(Texture* texture, Vector2u imageCount, float switchTime) {
	animation = Animation(texture, imageCount, switchTime, true);
	body.setTexture(texture);	
}

void Player::setAnimation(Texture* texture, Vector2u imageCount, float switchTime, bool repeat) {
	animation = Animation(texture, imageCount, switchTime, repeat);
	body.setTexture(texture);
}

Player::~Player() {}

void Player::Update(float deltaTime) {
	oldIsAttack = isAttack;
	if (isDead == false) {
		if (isInJump == false)
			velocity.x *= 0.00f;
		if (Keyboard::isKeyPressed(Keyboard::A) && isInJump == false && isAttack == false && isTakeHit == false) {
			velocity.x -= speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::D) && isInJump == false && isAttack == false && isTakeHit == false) {
			velocity.x += speed;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space) && isInJump == false && isFalling == false && isAttack == false && isTakeHit == false) {
			isInJump = true;
			velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
		}
		if (Mouse::isButtonPressed(Mouse::Left) && isInJump == false && isFalling == false && isAttack == false && isTakeHit == false) {
			isAttack = true;
		}
	}

	velocity.y += 981.0f * deltaTime;
	isFalling = true;

	body.setTextureRect(animation.uvRect);
	body.move(velocity * deltaTime);
	hitBox.setPosition(body.getPosition());
	if (isDead == false) {		
		AtkHitBox.setPosition(body.getPosition());
	}
}

void Player::UpdateSprite(float deltaTime) {
	if (isDead == false) {
		string textureName = "";
		if (velocity.y < 0) {
			if (isInJump == true) {
				textureName = "Jump";
				texturePath = "assets/Sprites/Player/Jump.png";
				frameCount = Vector2u(3, 1);
			}
		}
		else { //velocity.y >= 0
			isInJump = false;
			if (isFalling) {
				textureName = "Fall";
				texturePath = "assets/Sprites/Player/Fall.png";
				frameCount = Vector2u(3, 1);
				if (velocity.x > 0.0f) {
					faceRight = true;
					AtkHitBox.setOrigin(AtkHitBox.getSize().x - AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
				}
				else if (velocity.x < 0.0f) {
					faceRight = false;
					AtkHitBox.setOrigin(AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
				}
			}

			if (hp <= 0) {
				AtkHitBox.setPosition(AtkHitBox.getPosition().x, -1000);
				textureName = "Death";
				texturePath = "assets/Sprites/Player/Death.png";
				frameCount = Vector2u(11, 1);
			}
			else if (isTakeHit) {
				textureName = "Hit";
				texturePath = "assets/Sprites/Player/Hit.png";
				frameCount = Vector2u(4, 1);
			}
			else if (isAttack) {
				textureName = "Atk";
				texturePath = "assets/Sprites/Player/Attack1.png";
				frameCount = Vector2u(7, 1);
			}
			else if (velocity.x == 0.00f && isFalling == false && isAttack == false && isTakeHit == false) {
				textureName = "Idle";
				texturePath = "assets/Sprites/Player/Idle.png";
				frameCount = Vector2u(11, 1);
			}
			else if (velocity.x != 0.00f && isFalling == false && isAttack == false && isTakeHit == false) {
				textureName = "Run";
				texturePath = "assets/Sprites/Player/Run.png";
				frameCount = Vector2u(8, 1);
				if (velocity.x > 0.0f) {
					faceRight = true;
					AtkHitBox.setOrigin(AtkHitBox.getSize().x - AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
				}
				else {
					faceRight = false;
					AtkHitBox.setOrigin(AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
				}
			}
		}
		if (textureName != "") {
			if (this->textureName != textureName) {
				this->textureName = textureName;
				texture->loadFromFile(texturePath);
				if (textureName == "Atk")
					setAnimation(texture, frameCount, 0.15f, false);
				else if (textureName == "Hit")
					setAnimation(texture, frameCount, 0.2f, false);
				else if (textureName == "Death")
					setAnimation(texture, frameCount, 0.1f, false);
				else
					setAnimation(texture, frameCount, this->switchTime);
			}
		}
		else {
			cout << "textureName is empty!" << endl;
			cout << velocity.x << velocity.y << isInJump << isFalling << endl << endl;
		}
		bool EndAnimation = animation.Update(deltaTime, faceRight);
		if (isAttack && EndAnimation) {
			isAttack = false;
		}
		if (isTakeHit && EndAnimation) {
			isTakeHit = false;
		}
		if (hp <= 0 && EndAnimation) {
			isDead = true;
		}
	}
}

void Player::Draw(RenderWindow& window) {
	window.draw(body);
	//window.draw(AtkHitBox);
}

void Player::OnCollision(Vector2f direction) {
	if (direction.x < 0.0f) {
		//collision of left
		velocity.x = 0.0f;
	}
	else if (direction.x > 0.0f) {
		//collision of right
		velocity.x = 0.0f;
	}

	if (direction.y < 0.0f) {
		//collision of ground (bottom)
		velocity.y = 0.0f;
		isFalling = false;
	}
}

void Player::OnHitCol(Vector2f direction) {
	isTakeHit = true;
	hp--;
	if (direction.x < 0.0f) {
		faceRight = true;
		AtkHitBox.setOrigin(AtkHitBox.getSize().x - AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
	}
	else if (direction.x > 0.0f) {
		faceRight = false;
		AtkHitBox.setOrigin(AtkHitBox.getSize().x * 0.8f, AtkHitBox.getSize().y);
	}
}