#pragma once
#ifdef DLLCEngine_EXPORT
#define DLLCEngine __declspec(dllexport) 
#else
#define DLLCEngine __declspec(dllimport) 
#endif
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include "CEObject.h"
#include "CResult.h"
#include "CCamera.h"
#include "CRender.h"

#include <list>
using namespace winrt;

class DLLCEngine CEngine : public CEObject
{
private:
	com_ptr<ID3D11Device> _device;
	com_ptr<ID3D11DeviceContext> _context;
	com_ptr<IDXGIDevice> _dxgiDevice;
	com_ptr<IDXGISwapChain> _dxgiSwapChain;
	com_ptr<ID3D11RenderTargetView> _renderTargetView;
	com_ptr<ID3D11DepthStencilView> _depthStencilTargetView;
	CD3D11_VIEWPORT m_screenViewport;

	D3D_FEATURE_LEVEL m_d3dFeatureLevel;
	DXGI_MODE_DESC* m_displayModes;
	UINT m_displayModesCount;
	DXGI_MODE_DESC* m_displayMode;
	UINT m_Width;
	UINT m_Height;

	HANDLE hThread;

	std::list<CRender*> _renderList;

	double _initTime;
	double _updtTime;
	double _framTime;
	double _totaTime;
	double _run_Time;

	HWND _hWnd;

	friend DWORD WINAPI thDXThread(void* p);
	friend class CRender;
public:
	CCamera MainCamera;

	CEngine();

	// Heredado vía CEObject
	virtual void OnInitialize() override;
	virtual void OnUpdate(double elapsed) override;
	virtual void OnFrame() override;
	virtual void OnDispose() override;

	void Initialize(HWND hWnd);
};

