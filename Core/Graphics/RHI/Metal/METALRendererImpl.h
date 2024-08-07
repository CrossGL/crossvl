#pragma once

#include "Core/Logging/Log.h"
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "SDL2/SDL_metal.h"
#include "SDL2/SDL_video.h"

namespace CGL::Graphics
{
    CGL_DECLARE_LOG_CATEGORY(METALRendererImpl);

    class METALRendererImpl
    {
    public:
        METALRendererImpl(SDL_Window* window);
        ~METALRendererImpl();

        inline CA::MetalLayer* GetMetalLayer() const { return mLayer; }

        inline MTL::Device* GetDevice() const { return mDevice; }
        inline MTL::CommandQueue* GetQueue() const { return mQueue; }

        inline void SetCmdBuffer(MTL::CommandBuffer* cBuffer) { cmdBuffer = cBuffer; }
        inline MTL::CommandBuffer* GetCmdBuffer() { return cmdBuffer; }

        inline NS::AutoreleasePool* GetPoolRef() const { return arPool; }

        inline void SetDrawable(CA::MetalDrawable* drawable) { mDrawable = drawable; }
        inline CA::MetalDrawable* GetDrawable() const { return mDrawable; }

        METALRendererImpl() = delete;

    private:
        SDL_MetalView mView;
        CA::MetalLayer* mLayer;
        CA::MetalDrawable* mDrawable;

        MTL::Device* mDevice;
        MTL::CommandQueue* mQueue;
        MTL::CommandBuffer* cmdBuffer;

        NS::AutoreleasePool* arPool;
    };
}  // namespace CGL::Graphics
