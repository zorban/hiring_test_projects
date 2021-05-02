//--------------------------------------------------------------------------------------
// bioritam.fx
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 g_mWorldViewProjection;	// World * View * Projection matrix

texture g_tDiffuse;

sampler LinearSampler =
sampler_state
{
    Texture = <g_tDiffuse>;
    MinFilter = Linear;
    MagFilter = Linear;
};


//--------------------------------------------------------------------------------------
// Vertex shader
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : POSITION;
    float4 Diffuse : COLOR0;
};

VS_OUTPUT ColorVS( float3 Position : POSITION,
                   float4 Diffuse : COLOR0 )
{
    VS_OUTPUT Output;
    
    Output.Position = mul( float4(Position, 1), g_mWorldViewProjection );
    Output.Diffuse = Diffuse;
    
    return Output;
}

//--------------------------------------------------------------------------------------
// Color fill
//--------------------------------------------------------------------------------------
float4 ColorPS( float4 vDiffuse : COLOR0 ) : COLOR0
{
    return vDiffuse;
}


technique Color
{
    pass P0
    {
        CullMode = NONE;
        
        VertexShader = compile vs_2_0 ColorVS();
      //  PixelShader = compile ps_2_0 ColorPS();          
    }
}
