#pragma once

#include <Metal/Metal.hpp>

class Pipeline {
public:
    inline MTL::RenderPipelineState* GetRenderPipeline() {
        return this->custom_pipeline;
    }

    void LoadShader(MTL::Device* gpu_device);
    void LoadBuffers(MTL::Device* gpu_device);

    MTL::Buffer* Position{};
    MTL::Buffer* Color{};
    int tVertices{};

private:
    MTL::RenderPipelineState* custom_pipeline;
};

class Renderer
{
    
};