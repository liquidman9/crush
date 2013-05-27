float2 TexelSize;
//int BlurAmount;
#define BlurAmount 16
#define BlurScale 1.0
#define BlurStrength 0.3

texture ModelTexture;
sampler2D textureSampler = sampler_state {
    Texture = (ModelTexture);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

float Gaussian(float x, float deviation)
{
    return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));
}

float4 HorizontalPS(float2 vUv : TEXCOORD0) : COLOR0
{
   // Locals
    float halfBlur = float(BlurAmount) * 0.5;
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    float4 texColor = float4(0.0, 0.0, 0.0, 0.0);
    
    // Gaussian deviation
    float deviation = halfBlur * 0.35;
    deviation *= deviation;
    float strength = 1.0 - BlurStrength;
    
    // Horizontal blur
    for (int i = 0; i < 16; ++i)
    {
        if ( i >= BlurAmount )
            break;
        
        float offset = float(i) - halfBlur;
        texColor = tex2D(textureSampler, vUv + float2(offset * TexelSize.x * BlurScale, 0.0)) * Gaussian(offset * strength, deviation);
        color += texColor;
    }
    
    // Apply color
    float4 pixelColor = saturate(color);
    pixelColor.w = 1.0;
    return pixelColor;
}

float4 VerticalPS(float2 vUv : TEXCOORD0) : COLOR0
{
   // Locals
    float halfBlur = float(BlurAmount) * 0.5;
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    float4 texColor = float4(0.0, 0.0, 0.0, 0.0);
    
    // Gaussian deviation
    float deviation = halfBlur * 0.35;
    deviation *= deviation;
    float strength = 1.0 - BlurStrength;
    
    // Vertical blur
    for (int i = 0; i < 16; ++i)
    {
        if ( i >= BlurAmount )
            break;
        
        float offset = float(i) - halfBlur;
        texColor = tex2D(textureSampler, vUv + float2(0.0, offset * TexelSize.y * BlurScale)) * Gaussian(offset * strength, deviation);
        color += texColor;
    }
    
    // Apply color
    float4 pixelColor = saturate(color);
    pixelColor.w = 1.0;
    return pixelColor;
}


technique Horizontal
{
    pass Pass1
    {
        PixelShader = compile ps_2_0 HorizontalPS();
    }

}

technique Vertical
{
    pass Pass1
    {
        PixelShader = compile ps_2_0 VerticalPS();
    }

}