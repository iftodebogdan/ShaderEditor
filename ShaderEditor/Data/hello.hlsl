struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4x4 matWVP[3];

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    float4x4 mat = matWVP[0];
    for(int i = 1; i < 3; i++)
	mat = mul(mat, matWVP[i]);
    output.position = mul(position, mat);
    output.color = mul(position, mat);
    
    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, uniform float4x4 modelViewProjection) : SV_TARGET
{
    return color;
}
