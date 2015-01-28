struct VOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4x4 matWVP[3];

VOut vsmain(float4 position : POSITION, float2 texcoord : TEXCOORD) //float4 color : COLOR
{
    VOut output;


    float4x4 mat = matWVP[0];
    mat = mul(matWVP[1], mat);
    mat = mul(matWVP[2], mat);
    
    output.position = mul(mat, position);
    //output.position = mul(output.position, matWVP[2]);
    output.texcoord = texcoord;
    
    return output;
}

sampler2D tex1;
sampler2D tex2;

float4 psmain(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    return tex2D(tex1, texcoord) * tex2D(tex2, texcoord);
}
