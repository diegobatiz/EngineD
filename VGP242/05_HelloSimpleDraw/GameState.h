#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public EngineD::AppState
{
public:
	virtual void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	virtual void Render() override;

protected:
	EngineD::Graphics::Camera mCamera;
	std::string mNextStateName;
};

class TransformState : public GameState
{
	void Initialize() override
	{
		GameState::Initialize();
		mNextStateName = "SphereState";
	}
	void Render() override;
};

class SphereState : public GameState
{
	void Initialize() override
	{
		GameState::Initialize();
		mNextStateName = "AABBState";
	}
	void Render() override;
};

class AABBState : public GameState
{
	void Initialize() override
	{
		GameState::Initialize();
		mNextStateName = "FilledAABBState";
	}
	void Render() override;
};

class FilledAABBState : public GameState
{
	void Initialize() override
	{
		GameState::Initialize();
		mNextStateName = "DrawingState";
	}
	void Render() override;
};

class DrawingState : public GameState
{
	void Initialize() override
	{
		GameState::Initialize();
		mNextStateName = "TransformState";
	}
	void Render() override;
};