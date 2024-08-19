#include <metal_stdlib>
using namespace metal;

struct VertexData
{
    float4 Position [[position]];
    float4 Color;
};

fragment float4 HelloTrianglePS(VertexData in [[stage_in]])
{
    return in.Color;
}
