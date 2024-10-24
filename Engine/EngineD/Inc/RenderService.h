#pragma once

#include "Service.h"

namespace EngineD
{
	class CameraService;
	class RenderObjectComponent;
	class TransformComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;
		void Update(float deltaTime);
		void Render() override;
		void DebugUI() override;

		void Register(const RenderObjectComponent* roc);
		void Unregister(const RenderObjectComponent* roc);

	private:
		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		struct Entry
		{
			const RenderObjectComponent* renderComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		using RenderEntries = std::vector<Entry>;
		RenderEntries mRenderEntries;

		float mFPS;
	};
}