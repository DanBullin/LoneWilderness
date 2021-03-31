#version 450 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Interface block for the outputs of the vertex shader
in TES_OUT {
	vec3 Normals;
	vec2 TexCoords;
	flat int TexUnit;
	vec4 Tint;
	vec3 ViewPos;
	vec3 FragPos;
} gs_in[];

out GS_OUT {
	vec3 Normals;
	vec2 TexCoords;
	flat int TexUnit;
	vec4 Tint;
	vec3 ViewPos;
	vec3 FragPos;
} gs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
	vec3 u_viewPos;
};

vec3 getNormal();

void main()
{
    for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		gs_out.FragPos = gs_in[i].FragPos;
	    gs_out.Normals = gs_in[i].Normals;
		gs_out.TexCoords = gs_in[i].TexCoords;
		gs_out.TexUnit = gs_in[i].TexUnit;
		gs_out.Tint = gs_in[i].Tint;
		gs_out.ViewPos = gs_in[i].ViewPos;
		EmitVertex();
	}
	EndPrimitive();
}

vec3 getNormal()
{
	vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	return normalize(cross(a, b));
}