#include "pch.h"
#include "CEngine.h"
#include "CTimer.h"

D3D_FEATURE_LEVEL featureLevels[] =
{
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1
};

DWORD WINAPI thDXThread(void * p)
{
    CTimer timer;
    while (true)
    {
        timer.Start();
        for (auto it : CEngine::GameEngine->_renderList)
        {
            it->OnInitialize();
        }
        CEngine::GameEngine->_initTime = timer.Stop();
        timer.Start();
        CEngine::GameEngine->OnUpdate(CEngine::GameEngine->_totaTime);
        CEngine::GameEngine->_updtTime = timer.Stop();
        timer.Start();
        CEngine::GameEngine->OnFrame();
        CEngine::GameEngine->_framTime = timer.Stop();
        CEngine::GameEngine->_totaTime = CEngine::GameEngine->_initTime + CEngine::GameEngine->_updtTime + CEngine::GameEngine->_framTime;
        CEngine::GameEngine->_run_Time += CEngine::GameEngine->_totaTime;
    }
}

CEngine::CEngine()
{
    GameEngine = this;
    m_d3dFeatureLevel = D3D_FEATURE_LEVEL_11_1;
    m_displayModes = nullptr;
    m_displayModesCount = 0;
    m_displayMode = nullptr;

    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    m_Width = desktop.right;
    m_Height = desktop.bottom;
}

void CEngine::OnInitialize()
{
    if (Initialized)
        return;


    DWORD creationFlags = 0;

    com_ptr<IDXGIFactory> factory;
    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.put());

    std::list<IDXGIAdapter*> adapters;
    IDXGIAdapter* pAdapter;
    IDXGIOutput* output;

    for (UINT i = 0;
        factory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
        ++i)
    {
        adapters.push_back(pAdapter);
    }

    CResult hr = adapters.front()->EnumOutputs(0, &output);

    DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    hr = output->GetDisplayModeList(format, 0, &m_displayModesCount, NULL);
    m_displayModes = new DXGI_MODE_DESC[m_displayModesCount];
    ZeroMemory(m_displayModes, sizeof(DXGI_MODE_DESC) * (m_displayModesCount));
    hr = output->GetDisplayModeList(format, 0, &m_displayModesCount, m_displayModes);

    int la = 1000000;
    for (UINT i = 0;
        i < m_displayModesCount;
        i++)
    {
        int a = m_displayModes[i].Width - m_Width;
        a += m_displayModes[i].Height - m_Height;
        a = abs(a);

        if (a < la)
        {
            la = a;
            m_displayMode = &m_displayModes[i];
        }
    }

    DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
    swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
    swapChainDesc.BufferDesc.Width = m_displayMode->Width;
    swapChainDesc.BufferDesc.Height = m_displayMode->Height;
    swapChainDesc.BufferDesc.RefreshRate = m_displayMode->RefreshRate;
    swapChainDesc.BufferDesc.Format = m_displayMode->Format;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // All Windows Store apps must use this SwapEffect.
    swapChainDesc.Flags = 0;
    swapChainDesc.OutputWindow = _hWnd;

    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // Specify nullptr to use the default adapter.
        D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
        0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        creationFlags,              // Set debug and Direct2D compatibility flags.
        featureLevels,              // List of feature levels this app can support.
        ARRAYSIZE(featureLevels),   // Size of the list above.
        D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
        &swapChainDesc,
        _dxgiSwapChain.put(),
        _device.put(),              // Returns the Direct3D device created.
        &m_d3dFeatureLevel,         // Returns feature level of device created.
        _context.put()              // Returns the device immediate context.
    );

    ID3D11RenderTargetView* nullViews[] = { nullptr };
    _context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
    _context->Flush();

    com_ptr<ID3D11Texture2D> backBuffer = winrt::capture<ID3D11Texture2D>(_dxgiSwapChain, &IDXGISwapChain::GetBuffer, 0);
    hr = _device->CreateRenderTargetView(backBuffer.get(), nullptr, _renderTargetView.put());

    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        static_cast<UINT>(m_displayMode->Width),
        static_cast<UINT>(m_displayMode->Height),
        1, // This depth stencil view has only one texture.
        1, // Use a single mipmap level.
        D3D11_BIND_DEPTH_STENCIL
    );

    winrt::com_ptr<ID3D11Texture2D> depthStencil;
    hr =
        _device->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            depthStencil.put()
        );


    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    hr =
        _device->CreateDepthStencilView(
            depthStencil.get(),
            &depthStencilViewDesc,
            _depthStencilTargetView.put()
        );

    // Set the 3D rendering viewport to target the entire window.
    m_screenViewport = CD3D11_VIEWPORT(
        0.0f,
        0.0f,
        m_displayMode->Width,
        m_displayMode->Height
    );

    hThread = CreateThread(nullptr, 0, thDXThread, nullptr, 0, nullptr);

    Initialized = true;
}

void CEngine::OnUpdate(double elapsed)
{
    MainCamera.OnUpdate(elapsed);
    _renderList.sort(CRender::Sorter);
    for(auto it : GameEngine->_renderList)
    {
        it->OnUpdate(GameEngine->_totaTime);
    }
}

void CEngine::OnFrame()
{
    static float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    _context->ClearRenderTargetView(_renderTargetView.get(), color);
    _context->ClearDepthStencilView(_depthStencilTargetView.get(), 0, 1.0f, 0);

    // Draw 3D
    for (auto it : this->_renderList)
    {
        if(!it->is2D)
            it->OnFrame();
    }

    // Draw 2D
    for (auto it : this->_renderList)
    {
        if (it->is2D)
            it->OnFrame();
    }

    _dxgiSwapChain->Present(1, 0);
}

void CEngine::OnDispose()
{
    TerminateThread(hThread, 0);
}

void CEngine::Initialize(HWND hWnd)
{
    _hWnd = hWnd;
    OnInitialize();
}
