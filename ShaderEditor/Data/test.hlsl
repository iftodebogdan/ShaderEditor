struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

float3x3 matTest33;
float2x2 matTest22;
float3x2 matTest32;
float4x4 matWVP[3];

bool bArr[3];
bool b1 : register(b0);
bool2 b2 : register(b1);
bool3 b3;
bool4 b4;

int iArr[3];
int i1;// : register(i0);
int2 i2;
int3 i3;
int4 i4;

float3 fArr[3];
float f1;
float2 f2;
float3 f3;
float4 f4;

VOut vsmain(float4 position : POSITION, float2 texcoord : TEXCOORD) //float4 color : COLOR
{
    VOut output;

    int i;
    int value = 1.f;
    output.color = 0.f;
    bool boolTest = true;
    int intTest = 1;
    float floatTest = 1.f;
    
    for(i = 0; i < 3; i++)
    {
	boolTest = boolTest * bArr[i];
    }
    
    if(b1)
	boolTest = boolTest * (b3.x * b3.y * b3.z * b4.x * b4.y * b4.z * b4.w);
    else
	boolTest = false;
    if(b2.x)
	boolTest = boolTest * (b3.x * b3.y * b3.z * b4.x * b4.y * b4.z * b4.w);
   else
	boolTest = false;
    if(b2.y)
	boolTest = boolTest * (b3.x * b3.y * b3.z * b4.x * b4.y * b4.z * b4.w);
    else
	boolTest = false;
	
    output.color.r = boolTest;

    [loop] for(i = 0; i < i1; i++)
    {
	intTest = intTest * iArr[i];
    }
    for(i = 0; i < 3; i++)
    {
	intTest = intTest * iArr[i];
    }
    intTest *= (i2.x * i2.y * i3.x * i3.y * i3.z * i4.x * i4.y * i4.z * i4.w);
    
    output.color.g = intTest;

    for(i = 0; i < 3; i++)
    {
	floatTest = floatTest * fArr[i].x * fArr[i].y * fArr[i].z;
    }
    floatTest *= ( f1.x * f2.x * f2.y * f3.x * f3.y * f3.z * f4.x * f4.y * f4.z * f4.w);
    
    output.color.b = floatTest;

    float4x4 mat = matWVP[0];
    mat = mul(mat, matWVP[1]);
    mat = mul(mat, matWVP[2]);
    output.position = mul(position, mat);
    output.position.xyz = mul(output.position.xyz, matTest33);
    output.position.xy = mul(output.position.xy, matTest22);
    //output.position.xyz = mul(matTest32, output.position.xy);
    saturate(output.color);
    //saturate(output.color.y);
    //saturate(output.color.z);
    output.texcoord = texcoord;
    return output;
}

sampler2D s2D;
samplerCUBE sCube;

float4 psmain(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET //uniform float4x4 modelViewProjection
{
    return texCUBE(sCube, float3(texcoord, 1.f)) * tex2D(s2D, texcoord);
}
