#include <raylib.h>
#include <iostream>
#include <raymath.h>
#include <deque>
#include <cstdlib>

using namespace std;

const int screenWidth = 1800;
const int screenHeight = 1400;

Color MYPINK = { 250, 135, 187, 255 };
Color MYYELLOW = { 255, 255, 250, 255 };
Color MYPURPLE = { 119, 111, 227, 255 };

class Ball {
public:

	Vector2 ballPosition = { (float)screenWidth / 4, (float)screenHeight / 2 };

	const float jumpSpeed = -420.0f;
	const float fallSpeed = 4000.0f;
	float currentSpeed = 0.0f;
	const float rate = 20.0f;
	float currentGravity = 1.0f;

	float radius = 50.0f;

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
		DrawCircleV(ballPosition, radius, MYPINK);
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
	deque<Vector2> tubesBottomPos = { {(float)(screenWidth / 4) * 3, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	deque<float> tubesTopLength = { 0.0f, 0.0f, 0.0f, 0.0f };

	Tubes() {
		randomTopHeight = GetRandomValue(spaceStartMin, spaceStartMax);

		horizontalSpacing();
		randomHeightTop();
	}

	void randomHeightTop() {
		for (unsigned int i = 0; i < tubesTopPos.size(); i++) {
			tubesTopLength[i] = GetRandomValue(spaceStartMin, spaceStartMax);
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

			Rectangle recTop = Rectangle{ x, yTop, tubeWidth, tubesTopLength[i] };
			DrawRectangleRec(recTop, MYPURPLE);

			Rectangle recBottom = Rectangle{ x, yBottom, tubeWidth, screenHeight };
			DrawRectangleRec(recBottom, MYPURPLE);

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

	void Reset() {
		tubesTopPos.clear();
		tubesBottomPos.clear();
		tubesTopLength.clear();

		for (unsigned int i = 0; i < 4; i++) {
        float randomTopHeight = GetRandomValue(spaceStartMin, spaceStartMax);
        tubesTopLength.push_back(randomTopHeight);
        tubesTopPos.push_back({ (float)(screenWidth / 4) * 3 + (tubeWidth + spaceBetweenPairs) * i, 0.0f });
        tubesBottomPos.push_back({ (float)(screenWidth / 4) * 3 + (tubeWidth + spaceBetweenPairs) * i, randomTopHeight + space });
    }
	}
};


class Game {
public:
	Ball ball;
	Tubes tubes;
	bool paused = false;
	int score = 0;
	double lastUpdateTime = 0;
	double scoreInterval = 1.5;

	void Update() {
		float deltaTime = GetFrameTime();
		if (!paused) {
			ball.Update(deltaTime);
			tubes.Update();
			collisionWithGround();
			collisionWithTube();
			Score();
		}

		if (IsKeyPressed(KEY_ENTER)) { 
			if (paused) {
				ball.Reset();
				tubes.Reset();
				paused = false;
			}
			else {
				paused = true;
			}
		}
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

	void collisionWithTube() {
		for (unsigned int i = 0; i < tubes.tubesTopPos.size(); i++) {
			float tubeLeft = tubes.tubesTopPos[i].x;
			float tubeRight = tubeLeft + tubes.tubeWidth; 

			if (ball.ballPosition.x + ball.radius > tubeLeft && ball.ballPosition.x - ball.radius < tubeRight) {
				if (ball.ballPosition.y - ball.radius < tubes.tubesTopPos[i].y + tubes.tubesTopLength[i] ||
					ball.ballPosition.y + ball.radius > tubes.tubesBottomPos[i].y) {
					gameOver();
				}
			}
		}
	}

	void gameOver() {
		paused = true;
		lastUpdateTime = GetTime();
		score = 0;
	} 

	void Score() {
		double scoreTimer = GetTime();
		if (scoreTimer - lastUpdateTime >= scoreInterval) {
			lastUpdateTime = scoreTimer;
			score++;
		}
	}
};

int main(void) {

	InitWindow(screenWidth, screenHeight, "Floppy Bird");

	SetTargetFPS(60);

	Game game;

	while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime();

		BeginDrawing();

		ClearBackground(MYYELLOW);

		game.Draw();
		game.Update();

		DrawText(TextFormat("Score: %i", game.score), 20, 20, 40, MYPINK);

		if (game.paused) {
			DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 60) / 2, screenHeight / 2 - 60, 60, MYPINK);
			DrawText("Press Enter to Reset", screenWidth / 2 - MeasureText("Press Enter to Reset", 40) / 2, screenHeight / 2 + 40, 40, MYPINK);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
