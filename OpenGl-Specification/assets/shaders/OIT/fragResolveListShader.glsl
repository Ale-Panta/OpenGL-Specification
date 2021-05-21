#version 440 core

// The per-pixel image containing the head pointers
layout (binding = 0, r32ui) uniform uimage2D aHeadPointerImage;

// Buffer containing linked lists of fragments
layout (binding = 1, rgba32ui) uniform uimageBuffer aListBuffer;

// This is the maximum number of overlapping fragments allowed
#define MAX_FRAGMENTS 15

// Temporary array used for sorting fragments
uvec4 FragmentList[MAX_FRAGMENTS];

out vec4 Color;

void main(void)
{
    uint currentIndex;
    uint fragmentCount = 0;

    // Get the initial head pointer from the header_pointer_image.
    currentIndex = imageLoad(aHeadPointerImage, ivec2(gl_FragCoord).xy).x;

    // Iterate until we haven't reached the end of the list or exhausted the storage available in fragments[]...
    while (currentIndex != 0 && fragmentCount < MAX_FRAGMENTS)
    {
        // Read the item from the linked list.
        uvec4 fragment = imageLoad(aListBuffer, int(currentIndex));

        // Check the covera value.
        uint coverage = fragment.w;

        
        // gl_SampleID is in the range [0, gl_NumSamples).
        // Check if the texel is covered or not. Basically it's doing a 4x multisapling.
        if ((coverage &(1 << gl_SampleID)) != 0)
            FragmentList[fragmentCount++] = fragment; // Note, fragment_count is first assigned and then incremented.

        // Assign the next pointer.
        currentIndex = fragment.x;
    }

    uint i, j;

    // Sorting...
    if (fragmentCount > 1)
    {
        for (i = 0; i < fragmentCount - 1; i++)
        {
            for (j = i + 1; j < fragmentCount; j++)
            {
                uvec4 fragment1 = FragmentList[i];
                uvec4 fragment2 = FragmentList[j];

                float depth1 = uintBitsToFloat(fragment1.z);
                float depth2 = uintBitsToFloat(fragment2.z);

                if (depth1 < depth2)
                {
                    FragmentList[i] = fragment2;
                    FragmentList[j] = fragment1;
                }
            }
        }
    }

    vec4 finalColor = vec4(0.0);

    // Blending...
    for (i = 0; i < fragmentCount; i++)
    {
        vec4 modulator = unpackUnorm4x8(FragmentList[i].y);
        finalColor = mix(finalColor, modulator, modulator.a);
    }

    // Output final color to the Plane mesh attached to vieport.
    Color = finalColor;
}