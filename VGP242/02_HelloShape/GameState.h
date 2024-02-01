#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public AppState
{
public:
	void Initialize();

	void Terminate();

	void Update(float deltaTime);

private:
	float mLifeTime = 0.0f;
};