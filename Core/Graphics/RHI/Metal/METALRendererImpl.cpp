#include "METALRendererImpl.h"

namespace CGL::Graphics
{
    CGL_DEFINE_LOG_CATEGORY(METALRendererImpl);

    METALRendererImpl::METALRendererImpl(SDL_Window* window)
    {
        m_view = SDL_Metal_CreateView(window);

        m_device = MTL::CreateSystemDefaultDevice();
        m_cmdQueue = m_device->newCommandQueue();

        m_layer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(m_view));
        m_layer->setDevice(m_device);
        m_layer->setPixelFormat(MTL::PixelFormatRGBA8Unorm_sRGB);

        CGL_LOG(METALRendererImpl, Info, m_device->name()->cString(NS::ASCIIStringEncoding));
        m_pipeline = std::make_unique<METALPipelineHandler>();

        m_arpool = nullptr;
    }

    void METALRendererImpl::startEncoding(const std::array<f32, 4>& clearcolor)
    {
        auto iRenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();

        const auto& [ r, g, b, a ] = clearcolor;

        iRenderPassDescriptor->colorAttachments()->object(0)->setTexture(m_context->texture());
        iRenderPassDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        iRenderPassDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor{ r, g, b, a });
        iRenderPassDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);

        this->m_rcmdEncoder = m_cmdBuffer->renderCommandEncoder(iRenderPassDescriptor);

        iRenderPassDescriptor->release();
    }

    METALRendererImpl::~METALRendererImpl()
    {
        m_device->release();
        m_device = nullptr;
    }
}
