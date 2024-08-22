#pragma once

#include "Metal/MTLLibrary.hpp"
#include <Metal/Metal.hpp>
#include <memory>

struct METALCompileObjects
{
    std::unique_ptr<MTL::Library*>& library;
    MTL::Device* device;
};
