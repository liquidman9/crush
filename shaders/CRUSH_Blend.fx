texture ModelTexture1;
texture ModelTexture2;
sampler2D textureSampler1 = sampler_state {
    Texture = (ModelTexture1);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler2D textureSampler2 = sampler_state {
    Texture = (ModelTexture2);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

float4 AdditiveBlendPS(float2 textureCoordinate : TEXCOORD0) : COLOR0
{
    float4 textureColor1 = tex2D(textureSampler1, textureCoordinate);
    float4 textureColor2 = tex2D(textureSampler2, textureCoordinate);
    textureColor1 = saturate(textureColor1 + textureColor2);
    textureColor1.a = 1;
    return textureColor1;
}

float4 CopyAlphaPS(float2 textureCoordinate : TEXCOORD0) : COLOR0
{
    float4 textureColor1 = tex2D(textureSampler1, textureCoordinate);
    float4 textureColor2 = tex2D(textureSampler2, textureCoordinate);
    textureColor1.a = textureColor2.a;
    return textureColor1;
}

technique AdditiveBlend
{
    pass Pass1
    {
        PixelShader = compile ps_2_0 AdditiveBlendPS();
    }
}

technique CopyAlpha
{
    pass Pass1
    {
        PixelShader = compile ps_2_0 CopyAlphaPS();
    }
}
