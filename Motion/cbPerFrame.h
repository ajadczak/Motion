#pragma once
#ifndef cbPerFrame_h__
#define cbPerFrame_h__
#include <DirectXMath.h>

struct cbPerFrame
{
	DirectX::XMFLOAT3 mLightDirection;
	DirectX::XMFLOAT3 mLightPosition;
	DirectX::XMFLOAT4 mLightColor;
	

	cbPerFrame(){};
	//cbPerFrame(DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT3 lightPosition, DirectX::XMFLOAT4 lightColor) : mLightColor(lightColor), mLightDirection(lightDirection), mLightPosition(lightPosition){};
	~cbPerFrame(){};
};

#endif // cbPerFrame_h__