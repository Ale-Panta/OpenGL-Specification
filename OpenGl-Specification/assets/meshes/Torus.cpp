#include "Torus.h"

#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"

namespace OpenGL
{
	Torus::Torus(float innerRadius, float outerRadius, int precision)
	{
		for (int i = 0; i < (precision + 1) * (precision + 1); i++) 
		{ 
			Vertices.push_back({ 
				glm::vec3(), // Postiion
				glm::vec2(), // TexCoord or UV
				glm::vec3(), // Normal
				glm::vec3(), // S tangent
				glm::vec3()	 // T tangent
				}); 
		}

		for (int i = 0; i < precision * precision * 6; i++) 
		{ 
			VertexIndices.push_back(0); 
		}

		// calculate first ring   
		for (int i = 0; i < precision + 1; i++)
		{
			float amt = toRadians(i * 360.0f / precision);
		
			// build the ring by rotating points around the origin, then moving them outward
			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::vec3 initPos(rMat * glm::vec4(innerRadius, 0.0f, 0.0f, 1.0f));
			Vertices[i].Position = glm::vec3(initPos + glm::vec3(outerRadius, 0.0f, 0.0f));
			
			// compute texture coordinates for each vertex on the ring
			Vertices[i].TexCoords = glm::vec2(0.0f, ((float)i / (float)precision));
			
			// compute tangents and normals -- first tangent is Y-axis rotated around Z
			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
			Vertices[i].TTangent = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
			Vertices[i].STangent = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f)); // second tangent is -Z.
			Vertices[i].Normal = glm::cross(Vertices[i].TTangent, Vertices[i].STangent); // their X-product is the normal.     
		}

		// rotate the first ring about Y to get the other rings   
		for (int ring = 1; ring < precision + 1; ring++) 
		{
			for (int vert = 0; vert < precision + 1; vert++) 
			{
				// rotate the vertex positions of the original ring around the Y axis
				float amt = (float)( toRadians(ring * 360.0f / precision));
				glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
				Vertices[ring*(precision + 1) + vert].Position = glm::vec3(rMat * glm::vec4(Vertices[vert].Position, 1.0f));
				
				// compute the texture coordinates for the vertices in the new rings
				Vertices[ring*(precision + 1) + vert].TexCoords = glm::vec2((float)ring*2.0f / (float)precision, Vertices[vert].TexCoords.t);
				
				if (Vertices[ring*(precision + 1) + vert].TexCoords.s > 1.0) 
					Vertices[ring*(precision +1) + vert].TexCoords.s -= 1.0f;
				
				// rotate the tangent and bitangent vectors around the Y axis
				rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
				Vertices[ring*(precision + 1) + vert].STangent = glm::vec3(rMat * glm::vec4(Vertices[vert].STangent, 1.0f));
				rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
				Vertices[ring*(precision + 1) + vert].TTangent = glm::vec3(rMat * glm::vec4(Vertices[vert].TTangent, 1.0f));
				
				// rotate the normal vector around the Y axis
				rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
				Vertices[ring*(precision + 1) + vert].Normal = glm::vec3(rMat * glm::vec4(Vertices[vert].Normal, 1.0f));
			}    
		}

		// calculate triangle indices corresponding to the two triangles built per vertex   
		for (int ring = 0; ring < precision; ring++) 
		{
			for (int vert = 0; vert < precision; vert++) 
			{
				VertexIndices[((ring* precision + vert) * 2) * 3 + 0] = ring*(precision + 1) + vert;
				VertexIndices[((ring* precision + vert) * 2) * 3 + 1] = (ring + 1)*(precision + 1) + vert;
				VertexIndices[((ring* precision + vert) * 2) * 3 + 2] = ring*(precision + 1) + vert + 1;
				VertexIndices[((ring* precision + vert) * 2 + 1) * 3 + 0] = ring*(precision + 1) + vert + 1;
				VertexIndices[((ring* precision + vert) * 2 + 1) * 3 + 1] = (ring + 1)*(precision + 1) + vert;
				VertexIndices[((ring* precision + vert) * 2 + 1) * 3 + 2] = (ring + 1)*(precision + 1) + vert + 1;
			}    
		}

		SetupMesh();
	}

	float Torus::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

}