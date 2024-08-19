#pragma once

#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "METALPipelineHandler.h"

#include "SDL2/SDL_metal.h"
#include "SDL2/SDL_video.h"

#include "Core/Logging/Log.h"

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(METALRendererImpl);

    class METALRendererImpl
    {
    public:
        explicit METALRendererImpl(SDL_Window* window);
        ~METALRendererImpl();

        inline CA::MetalLayer* GetMetalLayer() const { return m_layer; }
        inline CA::MetalDrawable* GetMetalContext() const { return m_context; }

        inline MTL::Device* GetDevice() const { return m_device; }
        inline MTL::CommandQueue* GetCommandQueue() const { return m_cmdQueue; }
        inline MTL::CommandBuffer* GetCommandBuffer() const { return m_cmdBuffer; }

        inline MTL::RenderCommandEncoder* GetRenderCommandEncoder() const { return m_rcmdEncoder; }
        inline MTL::PrimitiveType GetPrimitiveType() const { return m_primitiveType; }

        inline NS::AutoreleasePool* GetAutoReleasePool() const { return m_arpool; }
        inline METALPipelineHandler* GetRenderPipelineHandler() const { return m_pipeline.get(); }

        inline void InitAutoReleasePool() { m_arpool = NS::AutoreleasePool::alloc()->init(); }
        inline void SetPrimitiveType(MTL::PrimitiveType pType) { m_primitiveType = pType; }
        inline void SetNextDrawable() { m_context = m_layer->nextDrawable(); }
        inline void SetCommandBuffer() { m_cmdBuffer = m_cmdQueue->commandBuffer(); }

        void startEncoding(const std::array<f32, 4>& clearcolor = { 0.0f, 0.0f, 0.0f, 1.0f });

    private:
        SDL_MetalView           m_view;
        CA::MetalLayer*         m_layer;
        CA::MetalDrawable*      m_context;

        MTL::Device*            m_device;
        MTL::CommandQueue*      m_cmdQueue;
        MTL::CommandBuffer*     m_cmdBuffer;

        MTL::RenderCommandEncoder*      m_rcmdEncoder;
        MTL::PrimitiveType              m_primitiveType;

        NS::AutoreleasePool*            m_arpool;

        std::unique_ptr<METALPipelineHandler> m_pipeline;
    };
}
