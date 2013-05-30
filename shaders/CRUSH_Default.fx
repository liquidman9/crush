float4x4 World;
float4x4 View;
float4x4 Projection;

float4 AmbientColor = float4(1, 1, 1, 1);
float AmbientIntensity = 0.75;

float4x4 WorldInverseTranspose;

float3 DiffuseLightDirection = normalize(float3(1, 1, 1));
float4 DiffuseColor = float4(1, 1, 1, 1);
float DiffuseIntensity = 1.0;

float Shininess = 200;
float4 SpecularColor = float4(1, 1, 1, 1);    
float SpecularIntensity = 1;
float3 ViewVector = float3(1, 0, 0);

float4 ShieldColor = float4(1, 0, 0, 0.5);

texture ModelTexture;
sampler2D textureSampler = sampler_state {
    Texture = (ModelTexture);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VertexShaderInput
{
    float4 Position : POSITION0;
    float4 Normal : NORMAL0;
    float2 TextureCoordinate : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float4 Color : COLOR0; 
    float3 Normal : TEXCOORD0;
    float2 TextureCoordinate : TEXCOORD1;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

    float4 normal = mul(input.Normal, WorldInverseTranspose);
    float lightIntensity = dot(normal, DiffuseLightDirection);
    output.Color = saturate(DiffuseColor * DiffuseIntensity * lightIntensity);

    output.Normal = normal;
    output.TextureCoordinate = input.TextureCoordinate;

    return output;
}

float4 PixelShaderFunctionShiny(VertexShaderOutput input) : COLOR0
{
    float3 light = normalize(DiffuseLightDirection);
    float3 normal = normalize(input.Normal);
    float3 r = normalize(2 * dot(light, normal) * normal - light);
    float3 v = normalize(mul(normalize(ViewVector), World));

    float dotProduct = dot(r, v);
    float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0) * length(input.Color);

    float4 textureColor = tex2D(textureSampler, input.TextureCoordinate);
    textureColor.a = 1;
    
    return saturate(textureColor * (input.Color + AmbientColor * AmbientIntensity) + specular);
}

float4 PixelShaderFunctionShinyShield(VertexShaderOutput input) : COLOR0
{
	float4 color;
    float3 light = normalize(DiffuseLightDirection);
    float3 normal = normalize(input.Normal);
    float3 r = normalize(2 * dot(light, normal) * normal - light);
    float3 v = normalize(mul(normalize(ViewVector), World));

    float dotProduct = dot(r, v);
    float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0) * length(input.Color);
    
    color = saturate(ShieldColor * (input.Color + AmbientColor * AmbientIntensity) + specular);
	color.a = ShieldColor.a;
	return color;
}

float4 PixelShaderFunctionDull(VertexShaderOutput input) : COLOR0
{
    float4 textureColor = tex2D(textureSampler, input.TextureCoordinate);
    textureColor.a = 1;
    
    return saturate(textureColor * (input.Color + AmbientColor * AmbientIntensity));
}

technique Shield
{
    pass Pass1
    {	
		AlphaBlendEnable	= true;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunctionShinyShield();
    }
}

technique Shiny
{
    pass Pass1
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunctionShiny();
    }
}

technique Dull
{
    pass Pass1
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunctionDull();
    }
}