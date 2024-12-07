#pragma once
#include <EngineD/Inc/EngineD.h>

class GameState : public EngineD::AppState
{
public:
	virtual void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateBuffer() = 0;
	virtual void CreateShader() = 0;

	EngineD::Graphics::Camera mCamera;

	EngineD::Graphics::Texture mTexture;
	EngineD::Graphics::Sampler mSampler;
};

class CubeState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPC mMesh;
};

class RectState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPC mMesh;
};

class PlaneState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPC mMesh;
};

class SphereState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPC mMesh;
};

class CylinderState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPC mMesh;
};

class SkyBoxState : public GameState
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPX mMesh;
};															   
															   
class SkySphereState : public GameState						   
{
public:
	void Update(float deltaTime) override;

protected:
	void CreateBuffer() override;
	void CreateShader() override;

private:
	EngineD::Graphics::MeshPX mMesh;
};