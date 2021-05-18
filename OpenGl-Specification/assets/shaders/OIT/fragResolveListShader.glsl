#version 440 core

// The per-pixel image containing the head pointers
layout (binding = 0, r32ui) uniform uimage2D head_pointer_image;

// Buffer containing linked lists of fragments
layout (binding = 1, rgba32ui) uniform uimageBuffer list_buffer;

// This is the output color
layout (location = 0) out vec4 color;

// This is the maximum number of overlapping fragments allowed
#define MAX_FRAGMENTS 15

// Temporary array used for sorting fragments
uvec4 fragment_list[MAX_FRAGMENTS];

void main(void)
{
    uint current_index;
    uint fragment_count = 0;

    // Get the initial head pointer from the header_pointer_image.
    current_index = imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x;

    // Iterate until we haven't reached the end of the list or exhausted the storage available in fragments[]...
    while (current_index != 0 && fragment_count < MAX_FRAGMENTS)
    {
        // Read the item from the linked list.
        uvec4 fragment = imageLoad(list_buffer, int(current_index));

        // Check the covera value.
        uint coverage = fragment.w;

        /*
         * gl_SampleID is in the range [0, gl_NumSamples).
         * Check if the texel is covered or not. Basically it's doing a 4x multisapling.
         */
        if ((coverage &(1 << gl_SampleID)) != 0)
            fragment_list[fragment_count++] = fragment; // Note, fragment_count is first assigned and then incremented.

        // Assign the next pointer.
        current_index = fragment.x;
    }

    uint i, j;

    // Sorting...
    if (fragment_count > 1)
    {
        for (i = 0; i < fragment_count - 1; i++)
        {
            for (j = i + 1; j < fragment_count; j++)
            {
                uvec4 fragment1 = fragment_list[i];
                uvec4 fragment2 = fragment_list[j];

                float depth1 = uintBitsToFloat(fragment1.z);
                float depth2 = uintBitsToFloat(fragment2.z);

                if (depth1 < depth2)
                {
                    fragment_list[i] = fragment2;
                    fragment_list[j] = fragment1;
                }
            }
        }
    }

    vec4 final_color = vec4(0.0);

    // Blending...
    for (i = 0; i < fragment_count; i++)
    {
        vec4 modulator = unpackUnorm4x8(fragment_list[i].y);
        final_color = mix(final_color, modulator, modulator.a);
    }

    // Output final color to the Plane mesh attached to vieport.
    color = final_color;
}