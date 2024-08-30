#pragma once

#include "Metal/MTLDevice.hpp"
#include "Metal/MTLRenderPipeline.hpp"

#include <Core/Graphics/Buffer.h>
#include <Core/Logging/Log.h>

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(METALPipelineHandler);

    class METALPipelineHandler
    {
    public:
        METALPipelineHandler();

        inline MTL::RenderPipelineDescriptor* GetRenderPipelineDescriptor() const { return m_rpDescriptor; }
        inline MTL::RenderPipelineState* GetRenderPipelineState() const { return m_rpState; }

        inline MTL::DepthStencilState* GetDepthStencilState() const { return m_depthStencilState; }

        inline void SetIndexBuffer(IndexBuffer* indexBuffer) { m_indexBuffer = indexBuffer; }
        inline METALIndexBuffer* GetIndexBuffer() const {  return m_indexBuffer; }

        void CreateRenderPipelineState(MTL::Device* gpu_device);
    private:
        MTL::RenderPipelineDescriptor* m_rpDescriptor;
        MTL::RenderPipelineState* m_rpState;

        MTL::DepthStencilState* m_depthStencilState;

        METALIndexBuffer* m_indexBuffer;
    };
}
