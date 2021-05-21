#version 440 core


// Turn on early fragment testing. This ensure that fragments that are obscured by previously 
// opaque geometry are not added to the linked list.
layout (early_fragment_tests) in;

// Linked list 1D buffer.
layout (binding = 0, r32ui) uniform uimage2D aHeadPointerImage;

layout (binding = 1, rgba32ui) uniform writeonly uimageBuffer aListBuffer;

// This is the atomic counter used to allocate items in the linked list.
layout (binding = 0, offset = 0) uniform atomic_uint aListCounter;

layout (std140, binding = 25) uniform LightProperties
{
	mat4 LightModelMat;
	mat4 LightViewMat;
	mat4 LightProjMat;
	vec4 LightPos;
	vec4 LightDir;
	vec4 LightColor;
	vec4 LightAmbient;
};

in VS_FS_INTERFACE
{
	vec4 SurfaceColor;
	vec3 FragPosition;
	vec3 FragNormal;
} Fragment;

in int gl_SampleMaskIn[];

out vec4 Color;

void main()
{
	uint index;
	
	// Allocate an index in the linked list buffer. This function increments the atomic counter and returns the
	// _old_ value of the counter. Thus, the first fragment to execute this code will receive the value 0, the next
	// will receive 1, and so on.
	index = atomicCounterIncrement(aListCounter);

	uint oldHead = 0;
	
	// Now insert the fragment into the list. To do this, we atomically exchange our newly allocated index with the 
	// current content of the head pointer image. This function writes our new value to memory and returns the _old_ value.
	oldHead = imageAtomicExchange(aHeadPointerImage, ivec2(gl_FragCoord.xy), index);

	vec3 L = normalize(LightDir.xyz);
    vec3 V = normalize(-Fragment.FragPosition);
    vec3 N = normalize(Fragment.FragNormal);
    vec3 H = normalize(L + V);

    float NdotL = dot(N, L);
    float NdotH = dot(N, H);

	//vec4 modulator = vec4(Fragment.SurfaceColor.rgb, Fragment.SurfaceColor.a);
    vec4 modulator = vec4(Fragment.SurfaceColor.rgb * max(abs(NdotL), 0.37888), Fragment.SurfaceColor.a);
	vec4 additive_component = mix(Fragment.SurfaceColor, vec4(1.0), 0.6) * vec4(pow(clamp(NdotH, 0.0, 1.0), 26.0)) * 0.7;
	modulator += additive_component;

	// Now assamble the fragment into the buffer. This will be the item...
	uvec4 item = uvec4(0);

	item.x = oldHead;							// item.x = next pointer.
	item.y = packUnorm4x8(modulator);			// item.y = color.
	item.z = floatBitsToUint(gl_FragCoord.z);	// item.z = depth.
	item.w = int(gl_SampleMaskIn[0]);			// item.w = set the pixel to be considered as covered.

	// Write the data into the buffer at the right location.
	imageStore(aListBuffer, int(index), item);
}