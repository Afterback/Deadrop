#include "D3D11Device.h"
using namespace deadrop::render;

// required to not cause linker errors
ComPtr<ID3D11Device> D3D11Device::m_device = nullptr;
ComPtr<ID3D11DeviceContext> D3D11Device::m_deviceContext = nullptr;
ComPtr<ID3D11DeviceContext> D3D11Device::m_deviceContextDeferred = nullptr;
