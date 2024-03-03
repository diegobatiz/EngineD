#include "GameState.h"

using namespace EngineD;
using namespace EngineD::Graphics;
using namespace EngineD::Input;

const char* shapeType[] =
{
	"Sphere",
	"AABB",
	"Circle"
};

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}

	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void GameState::Render()
{
}

float radius = 1.0f;
Color shapeColor = Colors::White;
int currentValue;
bool buttonOn = false;
bool checkOn = false;
void GameState::DebugUI()
{
	DebugUI::SetTheme(DebugUI::Theme::Dark);
	ImGui::Begin("DebugUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::LabelText("Title", "Hello World");

	if (ImGui::Button("Button"))
	{
		buttonOn = !buttonOn;
	}
	if (buttonOn)
	{
		ImGui::LabelText("ButtonOn", "Button Pressed");
	}

	if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Alpha##Circle", &mSphereAlpha, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("TransformPos##Square", &mPosition.x, 0.01f, -2.0f, 2.0f);
	}
	ImGui::Combo("ShapeType", &currentValue, shapeType, 3);
	ImGui::Checkbox("CheckBox", &checkOn);
	ImGui::ColorEdit4("ShapeColor", &shapeColor.r);
	if (currentValue == 0 || currentValue == 2)
	{
		ImGui::DragFloat("Radius", &radius, 0.01f, 0.1f, 5.0f);
	}
	ImGui::End();


	shapeColor.a = mSphereAlpha;
	switch (currentValue)
	{
	case 0:
		SimpleDraw::AddSphere(60, 60, radius, shapeColor); break;
	case 1:
		SimpleDraw::AddAABB(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, shapeColor); break;
	case 2:
		SimpleDraw::AddGroundCircle(60, radius, shapeColor); break;
	}
	if (checkOn)
	{
		SimpleDraw::AddTransform(Math::Matrix4::Identity);
	}
	SimpleDraw::AddTransform(Math::Matrix4::Translation(mPosition));
	SimpleDraw::AddGroundPlane(20, Colors::White);

	SimpleDraw::Render(mCamera);
}
