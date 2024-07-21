#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#ifdef __WIN32__
// FIXME: OpenGL Headers

constexpr Uint32 window_flags = SDL_WINDOW_OPENGL;

#elif __APPLE__
#include "SDL2/SDL_metal.h"

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "Foundation/Foundation.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "Metal/Metal.hpp"

#include "metal/renderer.hpp"

constexpr Uint32 window_flags = SDL_WINDOW_METAL;

#endif

int main()
{
    if (SDL_InitSubSystem(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("%s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Visualizer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        window_flags
    );

    if (!window)
    {
        SDL_Log("%s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

#ifdef __WIN32__
    // FIXME: OpenGL Context 

#elif __APPLE__
    
    SDL_MetalView metal_view = SDL_Metal_CreateView(window);

    CA::MetalLayer* Layer = (CA::MetalLayer*)SDL_Metal_GetLayer(metal_view);

    Layer->setDevice(MTL::CreateSystemDefaultDevice());

    int w, h;
    SDL_Metal_GetDrawableSize(window, &w, &h);
    

    MTL::Device* GPU = Layer->device();

    MTL::CommandQueue* Queue = GPU->newCommandQueue();

    CA::MetalDrawable* drawable{};

    MTL::RenderPassDescriptor* rpDescriptor{ MTL::RenderPassDescriptor::alloc()->init() };

    Pipeline pipeline;
    
    pipeline.LoadShader(GPU);
    pipeline.LoadBuffers(GPU);

#endif
    
    SDL_bool state{ SDL_TRUE };

    SDL_Event evnt{};
    while (state) {
        while (SDL_PollEvent(&evnt))
        {
            if (evnt.type == SDL_QUIT || evnt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                state = SDL_FALSE;
            
        }
#ifdef __WIN32__
        // Rendering
    }
#elif __APPLE__

        drawable = Layer->nextDrawable();

        rpDescriptor->colorAttachments()->object(0)->setTexture(drawable->texture());

        MTL::CommandBuffer* cmdBuffer = Queue->commandBuffer();

        MTL::RenderCommandEncoder* cmdEncoder = cmdBuffer->renderCommandEncoder(rpDescriptor);

        cmdEncoder->setRenderPipelineState(pipeline.GetRenderPipeline());

        cmdEncoder->setVertexBuffer(pipeline.Position, 0, 0);
        cmdEncoder->setVertexBuffer(pipeline.Color, 0, 1);

        cmdEncoder->drawPrimitives(
            MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3)
        );

        cmdEncoder->endEncoding();
        cmdBuffer->presentDrawable(drawable);
        cmdBuffer->commit();


    }
    SDL_Metal_DestroyView(metal_view);

#endif

    SDL_DestroyWindow(window);
    SDL_Quit();
}
