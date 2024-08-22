#include "METALPipelineHandler.h"
#include "Foundation/NSError.hpp"
#include "Foundation/NSString.hpp"

namespace CGL::Graphics {

    CGL_DEFINE_LOG_CATEGORY(METALPipelineHandler);

    METALPipelineHandler::METALPipelineHandler() : m_rpDescriptor { nullptr }, m_rpState { nullptr }
    {
        m_rpDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();

        m_rpDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatRGBA8Unorm_sRGB);

        CGL_LOG(METALPipelineHandler, Info, "RenderPipelineDescriptor Initialized");
    }

    void METALPipelineHandler::CreateRenderPipelineState(MTL::Device* gpu_device)
    {
        if(!m_rpDescriptor) return;

        NS::Error* ns_error{};

        m_rpState = gpu_device->newRenderPipelineState(m_rpDescriptor, &ns_error);

        if(!m_rpState) 
	{
            CGL_LOG(METALPipelineHandler, Error, ns_error->localizedDescription()->utf8String());
            return;
        }

        CGL_LOG(METALPipelineHandler, Info, "RenderPipelineState Created");
        m_rpDescriptor->release();
        m_rpDescriptor = nullptr;
    }
}
