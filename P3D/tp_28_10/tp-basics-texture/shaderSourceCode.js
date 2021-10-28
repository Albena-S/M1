let vsc = `#version 300 es

//an attribute is input (in) to a vertex shader
in vec3 vertices;

//texture coordinates in the simplest case:
//in the vertex shader straight through the fragment shader
in vec3 vst_coords; 
out vec3 fst_coords;

uniform mat4 modelview;
uniform mat4 projection;

void main(){

	vec4 objectCoords = vec4(vertices,1.0, 1.0);
	vec4 eyeCoords = modelview * objectCoords;
	gl_Position = projection * eyeCoords;
	fst_coords = vst_coords;
}`;

let fsc = `#version 300 es

// fragment shaders need a default precision 
// highp is reserved word for high precision
precision highp float;
in vec3 fst_coords;

// a uniform (global variable set before the shader runs)
// to reference the texture
uniform sampler2D u_texture;

//fragment shader declare output for the fragment shader
out vec4 color;

void main(){
	color = texture(u_texture, fst_coords);
}`;