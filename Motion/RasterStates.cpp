#include "stdafx.h"
#include "RasterStates.h"

D3D11_RASTERIZER_DESC RasterStates::defaultRasterDesc;
D3D11_RASTERIZER_DESC RasterStates::wireframeRasterDesc;

RasterStates::RasterStates()
{
}

RasterStates::~RasterStates()
{
}

void RasterStates::Init()
{
	wireframeRasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeRasterDesc.CullMode = D3D11_CULL_NONE;
	defaultRasterDesc.FillMode = D3D11_FILL_SOLID;
	defaultRasterDesc.CullMode = D3D11_CULL_NONE;
}