float4x4 World;
float4x4 View;
float4x4 Projection;

float4 EmissiveColor = float4(1, 1, 1, 1);
float4 BlackColor = float4(0, 0, 0, 1);

/*
//texture ModelTexture;
sampler2D textureSampler = sampler_state {
    Texture = (ModelTexture);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};
*/

struct VertexShaderInput
{
    float4 Position : POSITION0;
    //float2 TextureCoordinate : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    //float2 TextureCoordinate : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

    //output.TextureCoordinate = input.TextureCoordinate;

    return output;
}

float4 PS_Black(VertexShaderOutput input) : COLOR0
{
    return BlackColor;
}

float4 PS_Emissive(VertexShaderOutput input) : COLOR0
{
//    float4 textureColor = tex2D(textureSampler, input.TextureCoordinate);
//    textureColor.a = 1;
    return EmissiveColor;
}

technique Black
{
    pass Pass1
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PS_Black();
    }
}

technique Emissive
{
    pass Pass1
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PS_Emissive();
    }
}