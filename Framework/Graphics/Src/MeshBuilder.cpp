#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace EngineD;
using namespace EngineD::Graphics;

namespace
{
	Color GetNextColour(int& index)
	{
		constexpr Color colorTable[] = {
			Colors::Red,
			Colors::Yellow,
			Colors::Pink,
			Colors::Green,
			Colors::Purple,
			Colors::Orange,
			Colors::Blue,
			Colors::Indigo,
			Colors::Beige,
			Colors::DarkBlue
		};

		index = (index + 1) % std::size(colorTable);
		return colorTable[index];
	}

	void CreateCubeIndices(std::vector<uint32_t>& indices)
	{
		indices =
		{
			//front
			0, 1, 2,
			0, 2, 3,
			//back
			7, 5, 4,
			7, 6, 5,
			//right
			3, 2, 6,
			3, 6, 7,
			//left
			4, 5, 1,
			4, 1, 0,
			//top
			1, 5, 6,
			1, 6, 2,
			//bottom
			0, 3, 7,
			0, 7, 4
		};
	}

	void CreatePlaneIndices(std::vector<uint32_t>& indices, uint32_t numRows, uint32_t numCols)
	{
		for (uint32_t r = 0; r < numRows; ++r)
		{
			for (uint32_t c = 0; c < numCols; ++c)
			{
				uint32_t i = (r * (numCols + 1)) + c;

				//triangle 1
				indices.push_back(i);
				indices.push_back(i + numCols + 2);
				indices.push_back(i + 1);

				//triangle 2
				indices.push_back(i);
				indices.push_back(i + numCols + 1);
				indices.push_back(i + numCols + 2);
			}
		}
	}
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
	MeshPC mesh;
	const float hs = size * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	mesh.vertices.push_back({ {  -hs, -hs, -hs }, GetNextColour(index) });
	mesh.vertices.push_back({ { 0.0f,  hs, 0.0f }, GetNextColour(index) });
	mesh.vertices.push_back({ {   hs, -hs, -hs }, GetNextColour(index) });

	mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColour(index) });
	mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColour(index) });

	mesh.indices = {
		//front
		0, 1, 2,
		//right
		2, 1, 4,
		//back
		4, 1, 3,
		//left
		0, 3, 1,
		//bottom
		0, 2, 4,
		0, 4, 3
	};

	return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
	MeshPC mesh;
	const float hs = size * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	mesh.vertices.push_back({ {-hs, -hs, -hs}, GetNextColour(index) });
	mesh.vertices.push_back({ {-hs,  hs, -hs}, GetNextColour(index) });
	mesh.vertices.push_back({ { hs,  hs, -hs}, GetNextColour(index) });
	mesh.vertices.push_back({ { hs, -hs, -hs}, GetNextColour(index) });

	mesh.vertices.push_back({ {-hs, -hs, hs}, GetNextColour(index) });
	mesh.vertices.push_back({ {-hs,  hs, hs}, GetNextColour(index) });
	mesh.vertices.push_back({ { hs,  hs, hs}, GetNextColour(index) });
	mesh.vertices.push_back({ { hs, -hs, hs}, GetNextColour(index) });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC EngineD::Graphics::MeshBuilder::CreateRectPC(float width, float height, float depth)
{
	MeshPC mesh;
	const float hw = width * 0.5f;
	const float hh = height * 0.5f;
	const float hd = depth * 0.5f;

	srand(time(nullptr));
	int index = rand() % 100;

	mesh.vertices.push_back({ {-hw, -hh, -hd}, Colors::Red });
	mesh.vertices.push_back({ {-hw,  hh, -hd}, Colors::Blue });
	mesh.vertices.push_back({ { hw,  hh, -hd}, Colors::Green });
	mesh.vertices.push_back({ { hw, -hh, -hd}, Colors::Yellow });

	mesh.vertices.push_back({ {-hw, -hh, hd}, Colors::Red });
	mesh.vertices.push_back({ {-hw,  hh, hd}, Colors::Blue });
	mesh.vertices.push_back({ { hw,  hh, hd}, Colors::Green });
	mesh.vertices.push_back({ { hw, -hh, hd}, Colors::Yellow });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC MeshBuilder::CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
	srand(time(nullptr));
	int index = rand() % 100;

	MeshPC mesh;

	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;

	float x = -hpw;
	float y = -hph;

	for (uint32_t r = 0; r <= numRows; ++r)
	{
		for (uint32_t c = 0; c <= numCols; ++c)
		{
			mesh.vertices.push_back({ {x, y, 0.0f}, GetNextColour(index) });
			x += spacing;
		}
		x = -hpw;
		y += spacing;
	}

	CreatePlaneIndices(mesh.indices, numRows, numCols);

	return mesh;
}

MeshPC EngineD::Graphics::MeshBuilder::CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
	MeshPC mesh;

	srand(time(nullptr));
	int index = rand() % 100;

	const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5f;

	float x = -hpw;
	float z = -hph;

	for (uint32_t r = 0; r <= numRows; ++r)
	{
		for (uint32_t c = 0; c <= numCols; ++c)
		{
			mesh.vertices.push_back({ {x, 0.0f, z}, GetNextColour(index) });
			x += spacing;
		}
		x = -hpw;
		z += spacing;
	}

	CreatePlaneIndices(mesh.indices, numRows, numCols);

	return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(uint32_t slices, uint32_t rings)
{
	srand(time(nullptr));
	int index = rand() % 100;

	MeshPC mesh;
	const float hh = static_cast<float>(rings) * 0.5f;

	for (uint32_t r = 0; r <= rings; ++r)
	{
		float ringPos = static_cast<float>(r);
		for (uint32_t s = 0; s <= slices; ++s)
		{
			float slicePos = static_cast<float>(s);
			float rotation = (slicePos / static_cast<float>(slices) * Math::Constants::TwoPi);

			mesh.vertices.push_back({ {
					cos(rotation),
					ringPos - hh,
					sin(rotation)},
					GetNextColour(index) });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}
