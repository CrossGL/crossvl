#include <metal_stdlib>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
    float4 color;
};

fragment float4 SpinningCubePS(VertexOut in [[stage_in]]) 
{
    return in.color;
}
