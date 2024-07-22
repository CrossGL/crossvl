#include "renderer.hpp"

#include "SDL2/SDL_log.h"

#include <fstream>
#include <string>

namespace {
    std::string ShaderContent{};

    void read_contents(std::string name) {
        std::ifstream filename{ name };

        if (!filename.is_open()) {
            SDL_Log("Failed to open file");
            return;
        }

        std::string line;
        while (std::getline(filename, line)) {
            ShaderContent += line + "\n";
        }

        filename.close();
    }
}

void Pipeline::LoadShader(MTL::Device* gpu_device)
{
    NS::Error* error{};

    read_contents("shader.metal");

    MTL::Library* shaders = gpu_device->newLibrary(
        NS::String::string(ShaderContent.c_str(), NS::StringEncoding::UTF8StringEncoding),
        nullptr, &error
    );

    if (!shaders) {
        SDL_Log("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    // vertex shader
    MTL::Function* vertexFunc = shaders->newFunction(
        NS::String::string("VertexFunc", NS::UTF8StringEncoding)
    );

    // fragment shader
    MTL::Function* fragmentFunc = shaders->newFunction(
        NS::String::string("FragmentFunc", NS::UTF8StringEncoding)
    );

    MTL::RenderPipelineDescriptor* rPipelineDescriptor{ MTL::RenderPipelineDescriptor::alloc()->init() };

    rPipelineDescriptor->setLabel(NS::String::string("test", NS::UTF8StringEncoding));
    rPipelineDescriptor->setVertexFunction(vertexFunc);
    rPipelineDescriptor->setFragmentFunction(fragmentFunc);
    rPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(
        MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB
    );

    this->custom_pipeline = gpu_device->newRenderPipelineState(rPipelineDescriptor, &error);

    if (!this->custom_pipeline) {
        SDL_Log("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    shaders->release();
    vertexFunc->release();
    fragmentFunc->retain();

    rPipelineDescriptor->release();

}

typedef struct
{
    float x, y, z;
} float3;

void Pipeline::LoadBuffers(MTL::Device* gpu_device)
{
    int num_of_vertices = 3;

    const float position[] = {
         0.0f,  0.25f, 0.0f, 0.0f,
        -0.25f, -0.25f, 0.0f, 0.0f,
         0.25f, -0.25f, 0.0f, 0.0f
    };

    const float colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    uint possize = sizeof(position);
    uint colsize = sizeof(colors);

    MTL::Buffer* BufferOne = gpu_device->newBuffer(possize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* BufferTwo = gpu_device->newBuffer(colsize, MTL::ResourceStorageModeManaged);

    memcpy(BufferOne->contents(), position, possize);
    memcpy(BufferTwo->contents(), colors, colsize);    

    BufferOne->didModifyRange(NS::Range::Make(0, BufferOne->length()));
    BufferTwo->didModifyRange(NS::Range::Make(0, BufferTwo->length()));

    this->Position = BufferOne;
    this->Color = BufferTwo;

    this->tVertices = num_of_vertices;


}
