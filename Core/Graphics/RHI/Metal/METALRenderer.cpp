#include "Core/Graphics/RHI/Metal/METALRendererImpl.h"
#include "Core/Graphics/Renderer.h"
#include "Metal/MTLRenderPass.hpp"

namespace CGL::Graphics
{
    void Renderer::Constructor_METAL(SDL_Window* window)
    {
        this->m_impl = new METALRendererImpl(window);

        CGL_LOG(Renderer, Info, "Metal Renderer Initialized");
    }

    void Renderer::Destructor_METAL()
    {
        delete static_cast<METALRendererImpl*>(m_impl);
        m_impl = nullptr;

        CGL_LOG(Renderer, Info, "Metal Renderer Destroyed");
    }

    void Renderer::BeginFrame_METAL()
    {
        GetImpl()->GetPoolRef()->alloc()->init();
        GetImpl()->SetDrawable(GetImpl()->GetMetalLayer()->nextDrawable());

        GetImpl()->SetCmdBuffer(GetImpl()->GetQueue()->commandBuffer());

        const f32 r = m_clearColor[0];
        const f32 g = m_clearColor[1];
        const f32 b = m_clearColor[2];
        const f32 a = m_clearColor[3];

        auto rpDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        rpDescriptor->colorAttachments()->object(0)->setTexture(GetImpl()->GetDrawable()->texture());
        rpDescriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        rpDescriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor{ r, g, b, a });
        rpDescriptor->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);

        auto rCmdEncoder = GetImpl()->GetCmdBuffer()->renderCommandEncoder(rpDescriptor);

        rCmdEncoder->endEncoding();
        rpDescriptor->release();
    }

    void Renderer::EndFrame_METAL()
    {
        GetImpl()->GetCmdBuffer()->presentDrawable(GetImpl()->GetDrawable());
        GetImpl()->GetCmdBuffer()->commit();

        GetImpl()->GetPoolRef()->release();
    }

    void Renderer::Resize_METAL(u32 width, u32 height)
    {
        GetImpl()->GetMetalLayer()->setDrawableSize(CGSize{ (f32)width, (f32)height });
    }

    METALRendererImpl* Renderer::GetImpl() const
    {
        assert(GetAPI() == RHIType::Metal);
        return static_cast<METALRendererImpl*>(m_impl);
    }
}  // namespace CGL::Graphics
