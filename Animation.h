#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>

using namespace sf;

class Animation {
public:
	Animation(Texture* texture, Vector2u imageCount, float switchTime, bool repeat);
	~Animation();

	bool Update(float deltaTime, bool faceRight);

	//variables
	IntRect uvRect;

private:
	Vector2u imageCount; //how many images are there?
	Vector2u currentImage;

	//priv variables
	float totalTime;
	float switchTime; //amount of time til next image
	bool repeat;
};