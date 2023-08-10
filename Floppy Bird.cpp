#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <deque>
#include <cstdlib>

using namespace std;

const int screenWidth = 1800;
const int screenHeight = 1400;

class Ball {
public: 

	Vector2 ballPosition = { (float)screenWidth / 4, (float)screenHeight / 2 };

	const float jumpSpeed = -420.0f;
	const float fallSpeed = 4000.0f;
	float currentSpeed = 0.0f;
	const float rate = 15.0f;
	float currentGravity = 1.0f;

	void Update(float delta) {
		if (IsKeyDown(KEY_SPACE)) {
			currentSpeed = jumpSpeed;
			currentGravity = -rate;
		}
		else {
			currentGravity = rate;
		}

		if (currentSpeed < fallSpeed) {
			currentSpeed += currentGravity;
		}

		// Acceleration
		ballPosition.y += currentSpeed * delta;

		if (ballPosition.y >= screenHeight) {
			ballPosition.y = screenHeight;
			currentSpeed = 0.0f;
			currentGravity = rate;
		}
	}

	void Draw() {
		DrawCircleV(ballPosition, 50, MAROON);
	}

	void Reset() {
		ballPosition = { (float)screenWidth / 3, (float)screenHeight / 2 };
	}

};

class Tubes {
public:
	const float speed = -3.0f;

	float spaceStartMin = screenWidth / 6;
	float spaceStartMax = screenWidth / 6 * 3;
	float randomTopHeight = 0.0f;
	const float space = 350.0f; //between top and bottom

	float tubeWidth = screenWidth / 10;
	float spaceBetweenPairs = (screenWidth - (tubeWidth * 4)) / 4;

	deque<Vector2> tubesTopPos = { { (float)(screenWidth / 4) * 3, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	deque<Vector2> tubesBottomPos = {{(float)(screenWidth / 4) * 3, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	deque<float> tubesTopLength = { 0.0f, 0.0f, 0.0f, 0.0f }; 

	Tubes() {
		randomHeightTop();
		horizontalSpacing();
	}

	void randomHeightTop() {
		for (unsigned int i = 0; i < tubesTopPos.size(); i++) {
			float randomTopHeight = GetRandomValue(spaceStartMin, spaceStartMax);
			tubesTopLength[i] = randomTopHeight;

			tubesTopPos[i].y = 0.0f;
			tubesBottomPos[i].y = tubesTopLength[i] + space;
		}
	}

	void horizontalSpacing() {
		for (unsigned int i = 0; i < tubesTopPos.size(); i++) {
			tubesTopPos[i].x = (float)(screenWidth / 4) * 3 + (tubeWidth + spaceBetweenPairs) * i;
			tubesBottomPos[i].x = tubesTopPos[i].x;
		}
	}

	void Draw() {
		for (unsigned int i = 0; i < tubesTopPos.size(); i++) {

			float x = tubesTopPos[i].x;

			float yTop = 0.0f; 

			float yBottom = tubesBottomPos[i].y;

			Rectangle recTop = Rectangle{ x, yTop, tubeWidth, tubesTopLength[i]};
			DrawRectangleRec(recTop, DARKGRAY);

			Rectangle recBottom = Rectangle{ x, yBottom, tubeWidth, screenHeight };
			DrawRectangleRec(recBottom, DARKGRAY);

		}

	}

	void Update() {
		for (unsigned int i = 0; i < tubesTopPos.size(); i++) {
			tubesTopPos[i].x += speed;
			tubesBottomPos[i].x += speed;

			if (tubesTopPos[i].x == 0) {
				float randomTopHeight = GetRandomValue(spaceStartMin, spaceStartMax);
				tubesTopLength.push_back(randomTopHeight);
				tubesTopPos.push_back({ screenWidth, 0.0f });
				tubesBottomPos.push_back({ screenWidth, randomTopHeight + space });

				if (tubesTopPos[i].x + tubeWidth < 0) {
					tubesTopPos.pop_front();
					tubesBottomPos.pop_front();
					tubesTopLength.pop_front();
				}
			}

		}
	}
};


class Game {
public:
	Ball ball;
	Tubes tubes;

	bool boolgameOver = false;

	void Update() {
		float deltaTime = GetFrameTime();
		ball.Update(deltaTime); 
		tubes.Update();
		collisionWithGround();
	}

	void Draw() {
		ball.Draw();
		tubes.Draw();
	}

	void collisionWithGround() {
		if (ball.ballPosition.y >= screenHeight) {
			gameOver();
		}
	}

	void gameOver() {
		ball.Reset();
		boolgameOver = false;
	}
};

int main(void) {
	
	InitWindow(screenWidth, screenHeight, "Floppy Bird");
	
	SetTargetFPS(60);

	Game game;

	while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime();
		
		BeginDrawing();

		ClearBackground(RAYWHITE);
		
		game.Draw();
		game.Update();
		
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
