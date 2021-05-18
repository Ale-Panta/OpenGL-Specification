#version 440 core

/*
 * Turn on early fragment testing. This ensure that fragments that are obscured by previously 
 * opaque geometry are not added to the linked list.
 */
layout (early_fragment_tests) in;

// Linked list 1D buffer.
layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;

layout (binding = 1, rgba32ui) uniform writeonly uimageBuffer list_buffer;

// This is the atomic counter used to allocate items in the linked list.
layout (binding = 0, offset = 0) uniform atomic_uint list_counter;

layout (location = 0) out vec4 color;

in vec3 frag_position;
in vec3 frag_normal;
in vec4 surface_color;
in int gl_SampleMaskIn[];

// --- Begin layout uniform blocks ------------------------------------------------------------------------------------

layout (std140, binding = 25) uniform LightProperties
{
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// --- End layout uniform blocks --------------------------------------------------------------------------------------

void main()
{
	uint index;

	/*
	 * Allocate an index in the linked list buffer. This function increments the atomic counter and returns the
	 * _old_ value of the counter. Thus, the first fragment to execute this code will receive the value 0, the next
	 * will receive 1, and so on.
	 */
	index = atomicCounterIncrement(list_counter);

	uint old_head;

	/*
	 * Now insert the fragment into the list. To do this, we atomically exchange our newly allocated index with the 
	 * current content of the head pointer image. This function writes our new value to memory and returns the _old_ value.
	 */
	old_head = imageAtomicExchange(head_pointer_image, ivec2(gl_FragCoord.xy), index);

	vec4 modulator = vec4(surface_color.rgb, surface_color.a);

	// Now assamble the fragment into the buffer. This will be the item...
	uvec4 item;

	item.x = old_head;							// item.x = next pointer.
	item.y = packUnorm4x8(modulator);			// item.y = color.
	item.z = floatBitsToUint(gl_FragCoord.z);	// item.z = depth.
	item.w = int(gl_SampleMaskIn[0]);			// item.w = set the pixel to be considered as covered.

	// Write the data into the buffer at the right location.
	imageStore(list_buffer, int(index), item);
}