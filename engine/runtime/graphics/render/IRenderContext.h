#pragma once
#include "engine/core/memory/memory.h"
#include "ITexture2D.h"
#include "IRenderTarget.h"
#include "IViewport.h"
#include "IShader.h"
#include "IRasterizerState.h"
#include "IBuffer.h"
#include "IPipelineState.h"
#include "IDepthStencilState.h"

#include <vector>
#include <array>

namespace deadrop 
{
    namespace render 
    {
        // the default color that will be used to clear the screen
        const float defaultClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        enum class RendererAPI
        {
            D3D11,
        };

        // a structure that defines a rectangle
        struct Rect
        {
            unsigned int left   = 0;
            unsigned int right  = 0;
            unsigned int top    = 0;
            unsigned int bottom = 0;
        };

        // defines the multiple shader stages
        // that resource can be bound to
        enum BIND_STAGE
        {
            BIND_STAGE_VERTEX,
            BIND_STAGE_PIXEL,
            BIND_STAGE_GEOMETRY,
        };

        // a structure that defines resolution and
        // bit-depth
        struct VideoMode
        {
            unsigned int width = 0;
            unsigned int height = 0;
            int8_t		 bitsPerPixel = 0;
        };

        // a descriptor that defines resolution and presentation formate
        struct DeviceDesc
        {
            unsigned int width = 0;
            unsigned int height = 0;
            TEXTURE2D_FORMAT format = TEXTURE2D_FORMAT::RGBA_8UN;
        };

        // a descriptor that defines the internal swapchain textures
        // use in rendering and presentation
        struct SwapchainDesc
        {
            unsigned int width;
            unsigned int height;
            TEXTURE2D_FORMAT format = TEXTURE2D_FORMAT::RGBA_8UN;
            bool windowed;
            unsigned short refreshRateNom;
            unsigned short refreshRateDenom;
            void* windowHandle;
        };

        // an interface to expose the functionality of the gpu pipeline
        class IRenderContext
        {
        public:
            virtual ~IRenderContext(){}

            // graphics pipleline object creation functions
            virtual bool CreateDevice(const DeviceDesc& deviceDesc) = 0;
            virtual bool CreateSwapchain(const SwapchainDesc& swapchain) = 0;
            virtual uptr<ITexture2D> CreateTexture2D(const Texture2DDesc& desc,
                const memory::MemoryBlock& data = { 0, nullptr },
                const std::vector<memory::MemoryBlock>& dataArray = std::vector<memory::MemoryBlock>{} ) = 0;

            virtual uptr<IBuffer>				CreateBuffer(BufferDesc& desc, 
                const memory::MemoryBlock& data = memory::MemoryBlock{ 0, nullptr }) = 0;
            virtual uptr<IShader>               CreateShader(const ShaderDesc& desc) = 0;
            virtual uptr<ITexture2D>            CreateDepth2D(const Texture2DDesc& desc) = 0;
            virtual uptr<IViewport>             CreateViewport(const ViewportDesc& desc) = 0;
            virtual uptr<IRenderTarget>         CreateRenderTarget(ITexture2D* texture) = 0;
            virtual uptr<IPipelineState>        CreatePipelineState(const PipelineStateDesc& pipelineState) = 0;
            virtual uptr<IRasterizerState>      CreateRasterizerState(const RasterizerStateDesc& desc) = 0;
            virtual uptr<IDepthStencilState>    CreateDepthStencilState(const DepthStencilStateDesc& desc) = 0;

            // graphics pipeline changing functions
            virtual void BindShader(IShader* shader) = 0;
            virtual void BindState(IRasterizerState* state) = 0;
            virtual void BindState(IDepthStencilState* state) = 0;
            virtual void BindState(IPipelineState* pipelineState) = 0;
            virtual void BindTexture(unsigned int slot, ITexture2D* texture) = 0;
            virtual void BindBuffer(IBuffer* buffer, unsigned int slot, const BIND_STAGE& stage) = 0;
            virtual void BindUniformBuffer(IUniformBuffer* uniformBuffer, const BIND_STAGE& stage = BIND_STAGE::BIND_STAGE_VERTEX) = 0;
            virtual void BindRenderTarget(IRenderTarget* renderTarget, ITexture2D* depthStencil) = 0;
            virtual void Clear(IRenderTarget* renderTarget, const float color[4] = defaultClearColor) = 0;
            virtual void Clear(ITexture2D* depthTexture) = 0;
            virtual sptr<ITexture2D> GetBackBuffer() = 0;
            virtual void Draw(unsigned int vertexCount, unsigned int startOffset = 0) = 0;
            virtual void DrawIndexed(unsigned int indexCount, unsigned int startOffset = 0) = 0;
            virtual void Present(bool vsync = false) = 0;

            virtual void SetViewport(IViewport* viewport) = 0;
            virtual void SetScissor(const Rect& rect)  = 0;

            virtual const DeviceDesc& GetDeviceDesc() = 0;
        };
    }
}
