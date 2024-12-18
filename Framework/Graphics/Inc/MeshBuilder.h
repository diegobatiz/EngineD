#pragma once

#include "MeshTypes.h"

namespace EngineD::Graphics
{
	class MeshBuilder
	{
	public:
		//Pyramid
		static MeshPC CreatePyramidPC(float size);

		//Cube
		static MeshPC CreateCubePC(float size);
		static Mesh CreateCube(float size);

		//Rectangle
		static MeshPC CreateRectPC(float width, float height, float depth);

		//Plane
		static MeshPC CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshPC CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing, Color color);
		static MeshPX CreateVerticalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshPX CreateHorizontalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);
		static Mesh CreateHorizontalPlane(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshD CreatePlane(uint32_t numRows, uint32_t numCols, float spacing, EngineD::Color color);

		//Cylinder
		static MeshPC CreateCylinderPC(uint32_t slices, uint32_t rings);

		//Sphere
		static MeshPC CreateSpherePC(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateSpherePX(uint32_t slices, uint32_t rings, float radius);
		static Mesh CreateSphere(uint32_t slices, uint32_t rings, float radius);

		//Sky Stuff
		static MeshPX CreateSkySpherePX(uint32_t slices, uint32_t rings, float radius);
		static Mesh CreateSkySphere(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateSkyBoxPX(float size);

		//screen stuff
		static MeshPX CreateScreenQuad();
		
		//Sprite Quad
		static Mesh CreateSpriteQuad(float width, float height);
		static MeshP CreateSpriteQuadP(float width, float height);
	};
}