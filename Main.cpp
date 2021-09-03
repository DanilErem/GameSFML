// AnimationSFML.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Player.h"
#include "Ground.h"
#include "Box.h"
#include "Goblin.h"

using namespace std;
using namespace sf;

static const float CAMERA_HEIGHT = 568.0f;

void ResizeCamera(const RenderWindow& window, View& view) {
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);

	//set size
	view.setSize(CAMERA_HEIGHT * aspectRatio, CAMERA_HEIGHT);
}

int main()
{
	Clock clock;
	float deltaTime = 0.0f;
	RenderWindow window(VideoMode(980, 700), "Animation", Style::Close | Style::Titlebar);
	window.setFramerateLimit(60);

	View view(Vector2f(0.0f, 0.0f), Vector2f(CAMERA_HEIGHT, CAMERA_HEIGHT * 0.7f));

	Texture playerTexture;
	playerTexture.loadFromFile("assets/Sprites/Idle.png");
	Player player(&playerTexture, Vector2u(11, 1), Vector2f(57, 10), 0.1f, 180.0f, 70.0f);

	Texture boxTexture;
	boxTexture.loadFromFile("assets/Sprites/Box.png");
	vector < Box* > boxes;
	Box box1(&boxTexture, Vector2f(52, 52), Vector2f(100, 315));
	Box box2(&boxTexture, Vector2f(52, 52), Vector2f(-500, 315));
	
	boxes.push_back(&box1);
	boxes.push_back(&box2);

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("assets/Sprites/Background1.png");
	RectangleShape background(Vector2f(4640, 793));
	background.setTexture(&backgroundTexture);
	background.setOrigin(background.getSize() / 2.0f);

	Texture forwardgroundTexture;
	forwardgroundTexture.loadFromFile("assets/Sprites/Background2.png");
	RectangleShape forwardground(Vector2f(4640, 799));
	forwardground.setTexture(&forwardgroundTexture);
	forwardground.setOrigin(forwardground.getSize() / 2.0f);

	Texture ground;
	ground.loadFromFile("assets/Sprites/Ground.png");

	vector < Ground > groundTiles;
	groundTiles.push_back(Ground(&ground, Vector2f(928.0f, 72.0f), Vector2f(0.0f, 360.0f)));
	groundTiles.push_back(Ground(&ground, Vector2f(928.0f, 72.0f), Vector2f(928.0f, 360.0f)));
	groundTiles.push_back(Ground(&ground, Vector2f(928.0f, 72.0f), Vector2f(1856.0f, 360.0f)));
	groundTiles.push_back(Ground(&ground, Vector2f(928.0f, 72.0f), Vector2f(-1856.0f, 360.0f)));
	groundTiles.push_back(Ground(&ground, Vector2f(928.0f, 72.0f), Vector2f(-928.0f, 360.0f)));

	Texture goblinTexture1;
	Texture goblinTexture2;
	goblinTexture1.loadFromFile("assets/Sprites/Goblin/Idle.png");
	goblinTexture2.loadFromFile("assets/Sprites/Goblin/Run.png");
	vector < Goblin* > goblins;
	Goblin goblin1(Goblin(&goblinTexture1, Vector2u(4, 1), Vector2f(300.0f, 200.0f), Vector2f(39.0f, 5.0f), 0.1f, 0.0f));
	Goblin goblin2(Goblin(&goblinTexture2, Vector2u(8, 1), Vector2f(300.0f, 200.0f), Vector2f(39.0f, 5.0f), 0.1f, 70.0f));
	//^^Enemy(Texture* texture, Vector2u imageCount, float switchTime, float speed)
	goblin1.setPos({
		200,
		300
	});
	goblin2.setPos({
		-200,
		300
	});

	goblins.push_back(&goblin1);
	goblins.push_back(&goblin2);

	while (window.isOpen()) {
		Event event;

		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1.0f / 20.0f)
			deltaTime = 1.0f / 20.0f; //if you run at 10 fps it'll go at 20fps
										//this is to stop glitching of a movement bug when you fling the window around

		while (window.pollEvent(event)) {
			//if window closed below
			if (event.type == Event::Closed)
				window.close();
			//if it's resized below, it will resize on aspect ratio rather than dragging out the picture
			if (event.type == Event::Resized)
				ResizeCamera(window, view);
		}

		player.Update(deltaTime);

		Vector2f direction; //direction into player on collision func

		for (Ground& ground : groundTiles) {
			Collision col = player.GetCollider();
			if (ground.GetCollider().CheckCollision(col, direction, 1.0f)) {
				player.OnCollision(direction);
			}
		}

		for (Goblin* goblin : goblins) {
			//update enemies once per frame
			goblin->Update(deltaTime);

			Vector2f direction;

			for (Ground& ground : groundTiles) {
				Collision col1 = goblin->GetCollider();
				if (ground.GetCollider().CheckCollision(col1, direction, 1.0f)) {
					goblin->OnCollision(direction);
				}
			}

			Collision col2 = goblin->GetCollider();
			if (player.getIsAtk() && player.getOldIsAtk() == false) {
				if (player.GetAtkCol().CheckCollision(col2, direction)) {
					goblin->OnHitCol(direction);
				}
			}
			
			if (player.getIsDead() == false) {
				Collision col3 = player.GetCollider();
				if (goblin->GetAtkCol().CheckCollision(col3, direction)) {
					goblin->OnAtkCol(direction);
					if (goblin->getIsAttack() && goblin->getOldIsAttack() == false)
						player.OnHitCol(direction);
				}
			}

			for (Box* box : boxes) {
				Collision col4 = goblin->GetCollider();
				if (box->GetCollider().CheckCollision(col4, direction, 1.0f)) {
					goblin->OnCollision(direction);
				}
			}

			goblin->UpdateSprite(deltaTime);
		}
		
		for (Box* box : boxes) {
			Collision col = player.GetCollider();
			if (box->GetCollider().CheckCollision(col, direction, 1.0f)) {
				player.OnCollision(direction);
			}
		}
		player.UpdateSprite(deltaTime);

		window.clear(Color::White);

		window.draw(background);

		for (Ground& ground : groundTiles) {
			ground.Draw(window);	
		}
		
		for (Box* box : boxes) {
			box->Draw(window);
		}

		player.Draw(window);

		for (Goblin* goblin : goblins) {
			goblin->Draw(window);
		}

		window.draw(forwardground);

		//must be after player updates ex: OBJECT_NAME.Update(deltaTime);
		view.setCenter(player.GetterPos().x, 180);
		/*view.setCenter(player.GetterPos().x, player.GetterPos().y - 150);*/
		window.setView(view);
		
		//window.draw(rectangle);

		window.display();
	}	
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
