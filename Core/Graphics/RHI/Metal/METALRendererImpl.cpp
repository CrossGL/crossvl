#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "METALRendererImpl.h"
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace CGL::Graphics
{
    METALRendererImpl::METALRendererImpl(SDL_Window* window)
    {
        mView = SDL_Metal_CreateView(window);

        mLayer = static_cast<CA::MetalLayer*>(SDL_Metal_GetLayer(mView));
        mLayer->setDevice(MTL::CreateSystemDefaultDevice());
        mLayer->setPixelFormat(MTL::PixelFormatRGBA8Unorm_sRGB);

        mDevice = mLayer->device();

        mQueue = mDevice->newCommandQueue();

        arPool = nullptr;
    }

    METALRendererImpl::~METALRendererImpl()
    {
        mDevice->release();
        SDL_Metal_DestroyView(mView);
    }

}  // namespace CGL::Graphics
