float4x4 World;
float4x4 View;
float4x4 Projection;

float4 EmissiveColor = float4(1, 1, 1, 1);
float4 BlackColor = float4(0, 0, 0, 1);

float4x4 WorldInverseTranspose;

float3 DiffuseLightDirection = normalize(float3(10, 3.5, 10));
float Shininess = 200;
float4 SpecularColor = float4(1, 1, 1, 1);    
float SpecularIntensity = 0.50;
float3 ViewVector = float3(1, 0, 0);

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
    float4 Normal : NORMAL0;
    //float2 TextureCoordinate : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float3 Normal : TEXCOORD0;
    //float2 TextureCoordinate : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);
    float4 normal = mul(input.Normal, WorldInverseTranspose);
    output.Normal = normal;

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

float4 PS_Specular(VertexShaderOutput input) : COLOR0
{
    float3 light = normalize(DiffuseLightDirection);
    float3 normal = normalize(input.Normal);
    float3 r = normalize(2 * dot(light, normal) * normal - light);
    float3 v = normalize(mul(normalize(ViewVector), World));

    float dotProduct = dot(r, v);
    float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0);
    specular.a = 1;
    return saturate(specular);
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

technique Specular
{
    pass Pass1
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PS_Specular();
    }
}