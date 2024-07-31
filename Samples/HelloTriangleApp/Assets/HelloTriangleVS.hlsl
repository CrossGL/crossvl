struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color    : COLOR0;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 Color : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = float4(input.Position, 1.0);
    output.Color = input.Color;
    return output;
}