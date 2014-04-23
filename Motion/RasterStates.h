#ifndef RasterStates_h__
#define RasterStates_h__
#include <d3d11.h>

struct RasterStates
{
	static D3D11_RASTERIZER_DESC defaultRasterDesc;
	static D3D11_RASTERIZER_DESC wireframeRasterDesc;
	static void Init();
	RasterStates();
	~RasterStates();
};

#endif // RasterStates_h__