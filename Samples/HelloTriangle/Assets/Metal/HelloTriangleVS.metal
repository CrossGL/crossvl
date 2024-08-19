#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float3 Position;
    float4 Color;
};

struct VertexData
{
    float4 Position [[position]];
    float4 Color;
};

vertex VertexData HelloTriangleVS(constant VertexIn* inData, uint vertexID [[vertex_id]])
{
    VertexData out;

    out.Position = float4(
        inData[vertexID].Position.x,
        inData[vertexID].Position.y,
        inData[vertexID].Position.z,
        1.0f
    );

    out.Color = float4(inData[vertexID].Color);

    return out;
}
