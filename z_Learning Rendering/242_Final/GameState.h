#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public EngineD::AppState
{
	struct PlanetInfo
	{
		Vector3 position{};
		float orbitRadius{};
		float orbitTime{};
		float orbitMultiplier{};
		float orbitAngle{};
		float dayTime{};
		float dayMultiplier{};
		float rotationAngle{};
	};

public:
	virtual void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

protected:
	void CreateSkySphere(std::filesystem::path texturePath);
	void CreatePlanet(float planetRadius, float orbitRadius, float orbitTime, float dayTime, std::filesystem::path texturePath);
	void RenderPlanet(int i);
	void RenderPlanetImage(int i);

	EngineD::Graphics::Camera mCamera;
	EngineD::Graphics::Camera mCamera2;


	std::vector<EngineD::Graphics::RenderTarget*> mRenderTargets;
	std::vector<EngineD::Graphics::Texture*> mTextures;
	std::vector<PlanetInfo*> mInfo;
	EngineD::Graphics::Sampler mSampler;

	size_t planetCount;
};