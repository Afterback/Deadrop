#pragma once
#include "D3D11Common.h"

namespace deadrop
{
    namespace render
    {
        class D3D11Device
        {
        public:
            static void Init(ComPtr<ID3D11Device> d3dDevice, 
                ComPtr<ID3D11DeviceContext> d3dDeviceContext,
                ComPtr<ID3D11DeviceContext> d3dDeviceContextDeferred)
            {
                m_device = d3dDevice;
                m_deviceContext = d3dDeviceContext;
                m_deviceContextDeferred = d3dDeviceContextDeferred;
            }
            inline static ID3D11Device* GetDevice() { return m_device.Get(); };
            inline static ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext.Get(); };
            inline static ID3D11DeviceContext* GetDeviceContextDeferred(){ return m_deviceContextDeferred.Get(); };

        private:
            static ComPtr<ID3D11Device> m_device;
            static ComPtr<ID3D11DeviceContext> m_deviceContext;
            static ComPtr<ID3D11DeviceContext> m_deviceContextDeferred;
        };
    }
}
