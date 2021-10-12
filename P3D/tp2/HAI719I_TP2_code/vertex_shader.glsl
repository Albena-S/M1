#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
uniform float scale ;

// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle
uniform vec3 translate ;
// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader
layout(location = 1) in vec3 i_color;
out vec3 o_color;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
//------

void main(){

    //Mettre à jour ce code pour appliquer la translation et la mise à l'échelle
    gl_Position =  vec4(vertexPosition_modelspace,1);
    gl_Position.xy = vec2(vertexPosition_modelspace.x * scale, vertexPosition_modelspace.y * scale);
    gl_Position += vec4(translate,1);
    o_color = i_color;
    //Assigner la normale à la variable interpolée color
    //ajouter ici
}

