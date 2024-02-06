#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public EngineD::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateShape();

	using Vertices = std::vector<EngineD::Graphics::Vertex>;
	Vertices mVertices;
};

class TriangleState : public GameState
{
private:
	void CreateShape() override;
};

class TriforceState : public GameState
{
private:
	void CreateShape() override;
};

class DiamondState : public GameState
{
private:
	void CreateShape() override;
};

class HeartState : public GameState
{
private:
	void CreateShape() override;
};