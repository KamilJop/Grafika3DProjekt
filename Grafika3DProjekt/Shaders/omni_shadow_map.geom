/**
 * @file point_shadow.geom
 * @brief Geometry shader for point light shadow mapping
 */

#version 330

/** Input primitive layout: triangles */
layout (triangles) in;

/** Output primitive layout: triangle strip with max 18 vertices */
layout (triangle_strip, max_vertices = 18) out;

/** Light space transformation matrices for the 6 faces of a cubemap */
uniform mat4 lightMatrices[6];

/** Fragment position to pass to the fragment shader */
out vec4 FragPos;

/**
 * @brief Main geometry shader function
 *
 * Transforms each triangle to all 6 faces of the cubemap for point light shadow mapping.
 */
void main()
{
    for (int face = 0; face < 6; face++)
    {
        gl_Layer = face;
        for (int i = 0; i < 3; i++)
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
