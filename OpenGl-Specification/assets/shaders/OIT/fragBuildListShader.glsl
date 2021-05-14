#version 440 core

layout (early_fragment_tests) in;

layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;
layout (binding = 1, rgba32ui) uniform writeonly uimageBuffer list_buffer;
layout (binding = 0, offset = 0) uniform atomic_uint list_counter;

layout (location = 0) out vec4 color;

in vec3 frag_position;
in vec3 frag_normal;
in vec4 surface_color;
in int gl_SampleMaskIn[];

uniform vec3 light_position = vec3(40.0, 20.0, 100.0);

void main()
{
	uint index;
	uint old_head;
	uvec4 item;
	vec4 frag_color;

	index = atomicCounterIncrement(list_counter);
	old_head = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), index);

	vec3 L = normalize(light_position - frag_position);
	vec3 V = normalize(-frag_position);
	vec3 N = normalize(frag_normal);
	vec3 H = normalize(L + V);

	float NdotL = dot(N, L);
	float NdotH = dot(N, H);

	vec4 modulator = vec4(surface_color.rgb * NdotL, surface_color.a);

	item.x = old_head;
	item.y = packUnorm4x8(modulator);
	item.z = floatBitsToUint(gl_FragCoord.z);
	item.w = int(gl_SampleMaskIn[0]);

	imageStore(list_buffer, int(index), item);

	frag_color = modulator;

	// color = frag_color;
}