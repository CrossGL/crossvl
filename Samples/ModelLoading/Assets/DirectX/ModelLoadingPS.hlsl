struct PS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float3 Color    : COLOR0;
};

struct PS_OUTPUT
{
    float4 Color : SV_Target0;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT)0;
    output.Color = float4(input.Normal, 1.0);
    return output;
}
