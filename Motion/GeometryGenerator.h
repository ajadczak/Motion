#ifndef GeometryGenerator_h__
#define GeometryGenerator_h__
#include "MeshData.h"
#include <d3d.h>

class GeometryGenerator
{
public:
	GeometryGenerator();
	~GeometryGenerator();
	MeshData CreateGrid(float width, float depth, unsigned int m, unsigned int n);
	float GetHeight(float x, float z, float mult);
};

float GeometryGenerator::GetHeight(float x, float z, float mult)
{
	return mult * (z*sinf(0.1f*x) + x*cosf(0.1f * z));
}

MeshData GeometryGenerator::CreateGrid(float width, float depth, unsigned int m, unsigned int n)
{
	MeshData data;
	unsigned int vertexCount = m * n;
	unsigned int faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	float r = 0;
	data.Vertices.resize(vertexCount);
	for (unsigned int i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (unsigned int j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;
			data.Vertices[i * n + j].Pos = DirectX::XMFLOAT3(x, GetHeight(x, z, 0.3f), z);
			data.Vertices[i * n + j].Color = DirectX::XMFLOAT4(r, 0.0f, 0.5f, 1.0f);
			if ((int)r == 1)
				r = 0.0f;

			r += 0.04f;
		}
	}

	data.Indices.resize(faceCount * 3);
	unsigned int k = 0;
	for (unsigned int i = 0; i < m - 1; ++i)
	{
		for (unsigned int j = 0; j < n - 1; ++j)
		{
			data.Indices[k]		= i * n + j;
			data.Indices[k + 1] = i * n + j + 1;
			data.Indices[k + 2] = (i + 1) * n + j;
			data.Indices[k + 3] = (i + 1) * n + j;
			data.Indices[k + 4] = i * n + j + 1;
			data.Indices[k + 5] = (i + 1) * n + j + 1;
			k += 6;
		}
	}

	return data;
}

GeometryGenerator::GeometryGenerator(){}
GeometryGenerator::~GeometryGenerator() {}

#endif // GeometryGenerator_h__