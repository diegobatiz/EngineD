//simple shader that only uses position and color and applies transform

cbuffer ConstantBuffer : register(b0)
{
    matrix wvp;
}

Texture2D grassTexture : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3 id : INSTANCEPOS;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

////NOISE

#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : (((int)x)-1) )

namespace Details
{
	/*
	 * Permutation table. This is just a random jumble of all numbers 0-255,
	 * repeated twice to avoid wrapping the index at 255 for each lookup.
	 * This needs to be exactly the same for all instances on all platforms,
	 * so it's easiest to just keep it as static explicit data.
	 * This also removes the need for any initialisation of this class.
	 *
	 * Note that making this an int[] instead of a char[] might make the
	 * code run faster on platforms with a high penalty for unaligned single
	 * byte addressing. Intel x86 is generally single-byte-friendly, but
	 * some other CPUs are faster with 4-aligned reads.
	 * However, a char[] is smaller, which avoids cache trashing, and that
	 * is probably the most important aspect on most architectures.
	 * This array is accessed a *lot* by the noise functions.
	 * A vector-valued noise over 3D accesses it 96 times, and a
	 * float-valued 4D noise 64 times. We want this to fit in the cache!
	 */
// #ifdef SIMPLEX_INTEGER_LUTS
// 	typedef int LutType;
// #else
// 	typedef unsigned char LutType;
// #endif
	
    static int perm[512] =
    {
        151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
		151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };
	
    inline float grad(int hash, float x, float y)
    {
        int h = hash & 7; // Convert low 3 bits of hash code
        float u = h < 4 ? x : y; // into 8 simple gradient directions,
        float v = h < 4 ? y : x; // and compute the dot product with (x,y).
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
    }
	
}

/* Skewing factors for 2D simplex grid:
 * F2 = 0.5*(sqrt(3.0)-1.0)
 * G2 = (3.0-Math.sqrt(3.0))/6.0
 */
#define F2 0.366025403
#define G2 0.211324865

// 2D simplex noise
float NoiseSimplex(float2 v)
{
    float n0, n1, n2; // Noise contributions from the three corners
	
	// Skew the input space to determine which simplex cell we're in
    float s = (v.x + v.y) * F2; // Hairy factor for 2D
    float xs = v.x + s;
    float ys = v.y + s;
    int i = FASTFLOOR(xs);
    int j = FASTFLOOR(ys);
	
    float t = (float) (i + j) * G2;
    float X0 = i - t; // Unskew the cell origin back to (x,y) space
    float Y0 = j - t;
    float x0 = v.x - X0; // The x,y distances from the cell origin
    float y0 = v.y - Y0;
	
	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0)
    {
        i1 = 1;
        j1 = 0;
    } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else
    {
        i1 = 0;
        j1 = 1;
    } // upper triangle, YX order: (0,0)->(0,1)->(1,1)
	
	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6
	
    float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
    float y2 = y0 - 1.0f + 2.0f * G2;
	
	// Wrap the integer indices at 256, to avoid indexing Details::perm[] out of bounds
    int ii = i & 0xff;
    int jj = j & 0xff;
	
	// Calculate the contribution from the three corners
    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 < 0.0f)
        n0 = 0.0f;
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * Details::grad(Details::perm[ii + Details::perm[jj]], x0, y0);
    }
	
    float t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 < 0.0f)
        n1 = 0.0f;
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * Details::grad(Details::perm[ii + i1 + Details::perm[jj + j1]], x1, y1);
    }
	
    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 < 0.0f)
        n2 = 0.0f;
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * Details::grad(Details::perm[ii + 1 + Details::perm[jj + 1]], x2, y2);
    }
	
	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
    return 40.0f * (n0 + n1 + n2); // TODO: The scale factor is preliminary!
}

//END OF NOISE


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    float noiseX = NoiseSimplex(input.id.xz * 0.3);
    float noiseZ = NoiseSimplex(-input.id.xz * 0.3);
    float noiseY = (noiseX + 1.0f) * 0.7f;
    
    noiseX *= 0.5f;
    noiseZ *= 0.5f;
    
    input.position.xz += input.id.xz; 
    input.position.x += noiseX;
    input.position.z += noiseZ;
    input.position.y += (input.texCoord.y - 1.0f) * -1.0f * noiseY;
    
    output.position = mul(float4(input.position, 1.0f), wvp);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 texColour = grassTexture.Sample(textureSampler, input.texCoord);
    

    return texColour;
}
