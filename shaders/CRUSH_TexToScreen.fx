texture ModelTexture;
sampler2D textureSampler = sampler_state {
    Texture = (ModelTexture);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

float4 PixelShaderFunction(float2 textureCoordinate : TEXCOORD0) : COLOR0
{
    float4 textureColor = tex2D(textureSampler, textureCoordinate);
    textureColor.a = 1;
    return textureColor;
}

technique Default
{
    pass Pass1
    {
        PixelShader = compile ps_2_0 PixelShaderFunction();
    }
}