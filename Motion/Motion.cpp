// Motion.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "d3dxGlobal.h"
#include "D3Dcompiler.h"
#include "GameTimer.h"
#include "ObjectManager.h"
#include "RasterStates.h"
#include "ConstantBuffer.h"
#include "cbPerFrame.h"
#include "cbPerObject.h"
#include "ShaderHelper.h"
#include "GeometryGenerator.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
GameTimer* mTimer;

class Motion : public D3DApp
{
public:
	Motion(HINSTANCE instance);
	~Motion();
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseWheel(int x);
	void OnKeyDown(WPARAM keyCode, LPARAM flags);

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();	
	void BuildGeometry(); 
	void LoadShaders();
	void BuildVertexLayout();
	void BuildRasterState();
	float Normalize(float min, float max, float val);
	ID3D11Buffer* CreateVertexBuffer(std::vector<Vertex> verts);
	ID3D11Buffer* CreateIndexBuffer(std::vector<unsigned int> indices);

	float mTheta;
	float mPhi;
	float mRadius;

	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	POINT mLastMousePos;

	float minY, maxY;
	bool isWireframeEnabled = false;

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mVertexLayout;
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3DBlob* vertexShaderBlob;
	ID3DBlob* pixelShaderBlob;
	ConstantBuffer<cbPerFrame> framecBuffer;
	ConstantBuffer<cbPerObject> objectcBuffer;
	ID3D11RasterizerState* defaultRasterState;
	ID3D11RasterizerState* wireframeRasterState;

	MeshData mMeshData;
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Motion motion(hInstance);
	if (!motion.Init())
		return 0;

	return motion.Run();
}

Motion::Motion(HINSTANCE instance) : D3DApp(instance), mRadius(155.0f), mPhi(0.25f*MathHelper::Pi), mTheta(2.5f*MathHelper::Pi)
{

}

Motion::~Motion()
{
	ReleaseCOM(mVertexShader);
	ReleaseCOM(mPixelShader);
	ReleaseCOM(mVertexLayout);
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(vertexShaderBlob);
	ReleaseCOM(pixelShaderBlob);
}

float Motion::Normalize(float min, float max, float val)
{
	return (val - min) / (max - min);
}

void Motion::BuildGeometry()
{
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), Colors::Red },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), Colors::Green },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), Colors::Blue },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), Colors::Cyan },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), Colors::Magenta },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), Colors::Silver },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), Colors::Yellow },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), Colors::Black }
	};

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3,

		4, 6, 5,
		4, 7, 6,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
	};

	std::vector<Vertex> vertexData(std::begin(vertices), std::end(vertices));
	mVertexBuffer = CreateVertexBuffer(vertexData);

	std::vector<unsigned int> indexData(std::begin(indices), std::end(indices));
	mIndexBuffer = CreateIndexBuffer(indexData);
}

ID3D11Buffer* Motion::CreateVertexBuffer(std::vector<Vertex> vertices)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* (UINT)vertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	ID3D11Buffer* buffer;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = &vertices[0];
	md3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer);
	return buffer;
}

ID3D11Buffer* Motion::CreateIndexBuffer(std::vector<unsigned int> indices)
{
	// Setup index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(UINT)* (UINT)indices.size();
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

	ID3D11Buffer* buffer;
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &buffer));
	return buffer;
}

void Motion::BuildVertexLayout()
{
	/*
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};*/
	// Create input layout
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &mVertexLayout));
}

void Motion::LoadShaders()
{
	HR(ShaderHelper::LoadCompiledShader("SimpleVertexShader.cso", &vertexShaderBlob));
	HR(ShaderHelper::LoadCompiledShader("SimplePixelShader.cso", &pixelShaderBlob));
	HR(md3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &mVertexShader));
	HR(md3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &mPixelShader));
	md3dImmediateContext->VSSetShader(mVertexShader, NULL, 0);
	md3dImmediateContext->PSSetShader(mPixelShader, NULL, 0);
}

void Motion::BuildRasterState()
{
	// Set raster state
	HR(md3dDevice->CreateRasterizerState(&RasterStates::defaultRasterDesc, &defaultRasterState));
	HR(md3dDevice->CreateRasterizerState(&RasterStates::wireframeRasterDesc, &wireframeRasterState));
}

bool Motion::Init()
{
	bool ret = D3DApp::Init();
	RasterStates::Init();
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//BuildGeometry();
	LoadShaders();
	BuildVertexLayout();
	BuildRasterState();
	GeometryGenerator geomGen;
	mMeshData = geomGen.CreateGrid(150.0f, 150.0f, 50, 50);

	for (auto &val : mMeshData.Vertices)
	{
		if (val.Pos.y < minY)
			minY = val.Pos.y;
		if (val.Pos.y > maxY)
			maxY = val.Pos.y;
	}
	

	mVertexBuffer = CreateVertexBuffer(mMeshData.Vertices);
	mIndexBuffer = CreateIndexBuffer(mMeshData.Indices);

	XMMATRIX I = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	// Setup constant buffers
	HR(framecBuffer.Initialize(md3dDevice));
	HR(objectcBuffer.Initialize(md3dDevice));

	XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, p);

	return ret;
}

void Motion::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, p);
}

void Motion::UpdateScene(float dt)
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = DirectX::XMVectorZero();
	XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
}

void Motion::DrawScene()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	md3dImmediateContext->IASetInputLayout(mVertexLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (isWireframeEnabled)
		md3dImmediateContext->RSSetState(wireframeRasterState);
	else
		md3dImmediateContext->RSSetState(defaultRasterState);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	
	XMMATRIX wvp = world*view*proj;

	wvp = XMMatrixTranspose(wvp);
	//cbPerFrame mPerFrameCB;
	//mPerFrameCB.mLightColor = XMFLOAT4(1.0f, 0.25f, 0.25f, 0.0f);
	//mPerFrameCB.mLightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	//mPerFrameCB.mLightPosition = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	//framecBuffer.Data = mPerFrameCB;
	//framecBuffer.ApplyChanges(md3dImmediateContext);

	cbPerObject mPerObjectCB;
	XMStoreFloat4x4(&mPerObjectCB.mWorldViewProj, wvp);
	objectcBuffer.Data = mPerObjectCB;
	objectcBuffer.ApplyChanges(md3dImmediateContext);

	ID3D11Buffer* cbuffers[2] = { framecBuffer.Buffer(), objectcBuffer.Buffer() };
	md3dImmediateContext->VSSetConstantBuffers(0, 2, cbuffers);	
	md3dImmediateContext->PSSetConstantBuffers(0, 2, cbuffers);
	
	GeometryGenerator geomGen;
	
	auto time = mTimer.TotalTime();
	auto displacement = sinf(time);
	for (auto &vector : mMeshData.Vertices)
	{
		// update displacement
		vector.Pos.y = geomGen.GetHeight(vector.Pos.x, vector.Pos.z, displacement);

		// update colors
		if (vector.Pos.y > 6.0f)
		{
			auto x = MathHelper::Clamp(0.50f + Normalize(6.0f, maxY, vector.Pos.y), 0.0f, 1.0f);
			auto val = XMFLOAT4(x, x, x, 1.0f);
			vector.Color = val; 
		}
		else if (vector.Pos.y < 6.0f)
		{
			auto norm = Normalize(minY, 6, vector.Pos.y);
			auto val = XMFLOAT4(0.00f, MathHelper::Clamp(0.0f + norm, 0.0f, 1.0f), 0.0f, 1.0f);
			vector.Color = val;
		}
	}

	D3D11_MAPPED_SUBRESOURCE vbuf;
	md3dImmediateContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vbuf);
	memcpy(vbuf.pData, &mMeshData.Vertices[0], sizeof(Vertex) * mMeshData.Vertices.size());
	md3dImmediateContext->Unmap(mVertexBuffer, 0);

	md3dImmediateContext->DrawIndexed((UINT)mMeshData.Indices.size(), 0, 0);
	HR(mSwapChain->Present(0, 0));
}

void Motion::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Motion::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Motion::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.025f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.025f * static_cast<float>(y - mLastMousePos.y));

		mTheta += dx;
		mPhi += dy;
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.01f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.01f * static_cast<float>(y - mLastMousePos.y));

		mRadius += dx - dy;
		//mRadius = MathHelper::Clamp(mRadius, 3.0f, 50.0f);
	}
}

void Motion::OnKeyDown(WPARAM keyCode, LPARAM flags)
{
	if (keyCode == VK_F1)
	{
		isWireframeEnabled = !isWireframeEnabled;
	}
	if (keyCode == VK_ESCAPE)
	{
		SendMessage(mhMainWnd, WM_CLOSE, 0, 0);
	}
}

void Motion::OnMouseWheel(int x)
{
	if (x > 0)
	{
		mRadius -= 0.4f;
	}
	else
	{
		mRadius += 0.4f;
	}
}
