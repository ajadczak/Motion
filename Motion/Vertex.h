#ifndef Vertex_h__
#define Vertex_h__
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
	//DirectX::XMFLOAT2 Tex0;
	//DirectX::XMFLOAT2 Tex1;
	Vertex() {};
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) : Pos(pos), Color(color) {}
	Vertex(DirectX::XMFLOAT3 pos, DirectX::FXMVECTOR color) : Pos(pos)
	{
		XMStoreFloat4(&Color, color);
	}
};

#endif // Vertex_h__