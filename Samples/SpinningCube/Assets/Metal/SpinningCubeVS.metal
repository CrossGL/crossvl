#include <metal_stdlib>
using namespace metal;

struct TransformationData
{
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 perspectiveMatrix;
};

struct VertexOut
{
    float4 position [[position]];
    float4 color;
};

vertex VertexOut SpinningCubeVS(uint vertexID [[vertex_id]], constant VertexOut* vertexData, constant TransformationData* transformationData)
{
    VertexOut out;

    float4 vertexPosition = vertexData[vertexID].position;

    float4x4 ModelView = transformationData->viewMatrix * transformationData->modelMatrix;
    float4x4 ModelViewProjection = transformationData->perspectiveMatrix * ModelView;

    out.position = ModelViewProjection * vertexPosition;
    out.color = vertexData[vertexID].color;

    return out;
}
