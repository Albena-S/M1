// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle () {}
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres indices des sommets du triangle:
    unsigned int v[3];
};


struct Mesh {
    std::vector< Vec3 > vertices; //array of mesh vertices positions
    std::vector< Vec3 > normals; //array of vertices normals useful for the display
    std::vector< Triangle > triangles; //array of mesh triangles
    std::vector< Vec3 > triangle_normals; //triangle normals to display face normals

    //Compute face normals for the display
    void computeTrianglesNormals(){

        //A faire : implémenter le calcul des normales par face
        //Attention commencer la fonction par triangle_normals.clear();
        //Iterer sur les triangles

        //La normal du triangle i est le resultat du produit vectoriel de deux ses arêtes e_10 et e_20 normalisé (e_10^e_20)
        //L'arete e_10 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 1 (triangles[i][1])
        //L'arete e_20 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 2 (triangles[i][2])

        //Normaliser et ajouter dans triangle_normales
    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals(  ){
        //Utiliser weight_type : 0 uniforme, 1 aire des triangles, 2 angle du triangle

        //A faire : implémenter le calcul des normales par sommet comme la moyenne des normales des triangles incidents
        //Attention commencer la fonction par normals.clear();
        //Initializer le vecteur normals taille vertices.size() avec Vec3(0., 0., 0.)
        //Iterer sur les triangles

        //Pour chaque triangle i
        //Ajouter la normal au triangle à celle de chacun des sommets en utilisant des poids
        //0 uniforme, 1 aire du triangle, 2 angle du triangle

        //Iterer sur les normales et les normaliser


    }

    void computeNormals(){
        computeTrianglesNormals();
        computeVerticesNormals();
    }
};

//Transformation made of a rotation and translation
struct Transformation {
    Mat3 rotation;
    Vec3 translation;
};

//Basis ( origin, i, j ,k )
struct Basis {
    inline Basis ( Vec3 const & i_origin,  Vec3 const & i_i, Vec3 const & i_j, Vec3 const & i_k) {
        origin = i_origin; i = i_i ; j = i_j ; k = i_k;
    }

    inline Basis ( ) {
        origin = Vec3(0., 0., 0.);
        i = Vec3(1., 0., 0.) ; j = Vec3(0., 1., 0.) ; k = Vec3(0., 0., 1.);
    }
    Vec3 operator [] (unsigned int ib) {
        if(ib==0) return i;
        if(ib==1) return j;
        return k;}

    Vec3 origin;
    Vec3 i;
    Vec3 j;
    Vec3 k;
};

//Fonction à completer
void collect_one_ring (std::vector<Vec3> const & i_vertices,
                       std::vector< Triangle > const & i_triangles,
                       std::vector<std::vector<unsigned int> > & o_one_ring) {//one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    //Attention verifier que l'indice n'est pas deja present
    //Tous les points opposés dans le triangle sont reliés

}

//Fonction à completer
void compute_vertex_valences (const std::vector<Vec3> & i_vertices,
                              const std::vector< Triangle > & i_triangles,
                              std::vector<unsigned int> & o_valences ) {
    //Utiliser la fonction collect_one_ring pour récuperer le 1-voisinage
}

//Input mesh loaded at the launch of the application
Mesh mesh;
std::vector< float > mesh_valence_field; //normalized valence of each vertex

Basis basis;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;
DisplayMode displayMode;
int weight_type;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

// ------------------------------------
// File I/O
// ------------------------------------
bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > const & i_vertices ,
              std::vector< Vec3 > const & i_normals ,
              std::vector< Triangle > const & i_triangles,
              std::vector< Vec3 > const & i_triangle_normals ,
              bool save_normals = true ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl ;

    unsigned int n_vertices = i_vertices.size() , n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals) myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else myfile << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2]<< " ";
        if (save_normals) myfile << i_triangle_normals[f][0] << " " << i_triangle_normals[f][1] << " " << i_triangle_normals[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              std::vector< Vec3 > & o_triangle_normals,
              bool load_normals = true )
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z ;

        myfile >> x >> y >> z ;
        o_vertices.push_back( Vec3( x , y , z ) );

        if( load_normals ) {
            myfile >> x >> y >> z;
            o_normals.push_back( Vec3( x , y , z ) );
        }
    }

    o_triangles.clear();
    o_triangle_normals.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));

            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }

        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}

// ------------------------------------
// Application initialization
// ------------------------------------
void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glDisable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    display_normals = false;
    display_mesh = true;
    display_smooth_normals = true;
    displayMode = LIGHTED;
    display_basis = false;
}


// ------------------------------------
// Rendering.
// ------------------------------------

void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void drawAxis( Vec3 const & i_origin, Vec3 const & i_direction ) {

    glLineWidth(4); // for example...
    drawVector(i_origin, i_origin + i_direction);
}

void drawReferenceFrame( Vec3 const & origin, Vec3 const & i, Vec3 const & j, Vec3 const & k ) {

    glDisable(GL_LIGHTING);
    glColor3f( 0.8, 0.2, 0.2 );
    drawAxis( origin, i );
    glColor3f( 0.2, 0.8, 0.2 );
    drawAxis( origin, j );
    glColor3f( 0.2, 0.2, 0.8 );
    drawAxis( origin, k );
    glEnable(GL_LIGHTING);

}

void drawReferenceFrame( Basis & i_basis ) {
    drawReferenceFrame( i_basis.origin, i_basis.i, i_basis.j, i_basis.k );
}

typedef struct {
    float r;       // ∈ [0, 1]
    float g;       // ∈ [0, 1]
    float b;       // ∈ [0, 1]
} RGB;



RGB scalarToRGB( float scalar_value ) //Scalar_value ∈ [0, 1]
{
    RGB rgb;
    float H = scalar_value*360., S = 1., V = 0.85,
            P, Q, T,
            fract;

    (H == 360.)?(H = 0.):(H /= 60.);
    fract = H - floor(H);

    P = V*(1. - S);
    Q = V*(1. - S*fract);
    T = V*(1. - S*(1. - fract));

    if      (0. <= H && H < 1.)
        rgb = (RGB){.r = V, .g = T, .b = P};
    else if (1. <= H && H < 2.)
        rgb = (RGB){.r = Q, .g = V, .b = P};
    else if (2. <= H && H < 3.)
        rgb = (RGB){.r = P, .g = V, .b = T};
    else if (3. <= H && H < 4.)
        rgb = (RGB){.r = P, .g = Q, .b = V};
    else if (4. <= H && H < 5.)
        rgb = (RGB){.r = T, .g = P, .b = V};
    else if (5. <= H && H < 6.)
        rgb = (RGB){.r = V, .g = P, .b = Q};
    else
        rgb = (RGB){.r = 0., .g = 0., .b = 0.};

    return rgb;
}

void drawSmoothTriangleMesh( Mesh const & i_mesh , bool draw_field = false ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {

        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position
            const Vec3 & n = i_mesh.normals[i_mesh.triangles[tIt][i]]; //Vertex normal

            if( draw_field && mesh_valence_field.size() > 0 ){
                RGB color = scalarToRGB( mesh_valence_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawTriangleMesh( Mesh const & i_mesh , bool draw_field = false  ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
        const Vec3 & n = i_mesh.triangle_normals[ tIt ]; //Triangle normal
        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position

            if( draw_field ){
                RGB color = scalarToRGB( mesh_valence_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawMesh( Mesh const & i_mesh , bool draw_field = false ){
    if(display_smooth_normals)
        drawSmoothTriangleMesh(i_mesh, draw_field) ; //Smooth display with vertices normals
    else
        drawTriangleMesh(i_mesh, draw_field) ; //Display with face normals
}

void drawVectorField( std::vector<Vec3> const & i_positions, std::vector<Vec3> const & i_directions ) {
    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_directions.size() ; ++pIt) {
        Vec3 to = i_positions[pIt] + 0.02*i_directions[pIt];
        drawVector(i_positions[pIt], to);
    }
}

void drawNormals(Mesh const& i_mesh){

    if(display_smooth_normals){
        drawVectorField( i_mesh.vertices, i_mesh.normals );
    } else {
        std::vector<Vec3> triangle_baricenters;
        for ( const Triangle& triangle : i_mesh.triangles ){
            Vec3 triangle_baricenter (0.,0.,0.);
            for( unsigned int i = 0 ; i < 3 ; i++ )
                triangle_baricenter += i_mesh.vertices[triangle[i]];
            triangle_baricenter /= 3.;
            triangle_baricenters.push_back(triangle_baricenter);
        }

        drawVectorField( triangle_baricenters, i_mesh.triangle_normals );
    }
}

//exo1
void DrawCurve(std::vector<Vec3> &TabPointsOfCurve, long nbPoints){
    glLineWidth(30);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.58,0.43,0.83);
    for (int i =0; i< nbPoints; i++){
        glVertex3fv(TabPointsOfCurve[i].mVals);
    }
    glEnd();
}

void HermiteCubicCurve(Vec3 P0, Vec3 P1, Vec3 V0, Vec3 V1, long nbU, std::vector<Vec3> &res){
    res.clear();
    res.resize(nbU);
    /*float f1,f2,f3,f4;
    float pow2, pow3,u;
    for (int i = 0; i < nbU; i++){
        u = (float) i/(nbU-1);
        pow2 = u*u;
        pow3 = u*u*u;
        f1 = 2.0*pow3 - 3.*pow2 + 1;
        f2 = -2.*pow2 + 3.*pow2;
        f3 = pow3 - 2.*pow2 + u;
        f4 = pow3 - pow2;
        res[(int)i] = f1 * P0 + f2 * P1 + f3 * V0 + f4* V1;
        std::cout << u<< " " << res[i].mVals[0] << " " << res[i].mVals[1] << " " << res[i].mVals[2] << std::endl; 
    }
    */
    Vec3 a,b,c,d;
    float pow2, pow3,u,x,y,z;
    a = 2 * P0 - 2 * P1 +  V0 + V1;
    b = -3 * P0 + 3 * P1 - 2 * V0 - V1;
    c = V0;
    d = P0;
    for (int i = 0; i < nbU; i++){
        u = (float) i/(nbU-1);
        pow2 = u*u;
        pow3 = u*u*u;
        x = a[0]*pow3 + b[0]*pow2 + c[0]* u +d[0];
        y = a[1]*pow3 + b[1]*pow2 + c[1]*u + d[1];
        z = a[2]*pow3 + b[2]*pow2 + c[2]*u + d[2];
        res[i] = Vec3(x,y,z);
    }
}
int factoriel(int n)
{
    int ret = 1;
    while (n > 1)
        ret *= n--;
    return ret;
}

//exo2
void BezierCurveByBernstein(std::vector<Vec3> TabControlPoint,
                             long nbControlPoint,
                             long nbU, 
                             std::vector<Vec3> &res){
    res.clear();
    res.resize(nbU);
    Vec3 somme;
    float B,u,n = nbControlPoint - 1, fac_n = (float) factoriel(n) ;
    for (int u_ = 0; u_ < nbU; u_++){
        u = (float) u_ / (nbU-1);
        somme = Vec3(0., 0., 0.);
        for (int i = 0; i < nbControlPoint; i ++) {
            B = ((fac_n)/ (factoriel(i)*factoriel(n-i))) * pow(u,i) * pow((1-u), (n-i));
            somme += B *  TabControlPoint[i];
        }
        res[u_] = somme;
    }
} 

//exo3
void BezierCurveByCasteljau(std::vector<Vec3> TabControlPoint, 
                            long nbControlPoint, 
                            long nbU,
                            std::vector<std::vector<Vec3>> &res) {
    res.clear();
    res.resize(nbControlPoint);
    float u = (float) 1/4,cmp=0;
    
    for (int i = 0; i < nbControlPoint; i ++) {
        res[i].push_back(TabControlPoint[i]);
    }
    /* for (int u_ = 1; u_ < nbU; u_++){
        u = (float) u_ / (nbU-1); */
        for (int k = 0; k < nbControlPoint-1 ; k++){
            for (int i = 0; i < nbControlPoint-k-1; i ++){
                    res[i].push_back((1-u)*res[i][k] + u*res[i+1][k]);
                    cmp++;
                }
        }
    /* }  */
} 

void transpose_vector(std::vector<std::vector<Vec3>> &original,
                std::vector<std::vector<Vec3>> &transpose){
    transpose.clear();
    transpose.resize(original[0].size());
    for (int i = 0; i < (int)original.size(); i++){
        for (int k = 0; k < (int)original[i].size(); k++){
            transpose[k].push_back(original[i][k]);
        }
    }
}




//TP4
//exo 1
void DrawPoints(std::vector<Vec3> &TabPoints, long nbPoints){
    //glLineWidth(30);

    glColor3f(0,0.53,0.99);
    glPointSize(8.);
    glBegin(GL_POINTS);
    
    for (int i =0; i< nbPoints; i++){
        glVertex3fv(TabPoints[i].mVals);
    }
    glEnd();
}
void DrawPointsYellow(std::vector<Vec3> &TabPoints, long nbPoints){
    //glLineWidth(30);

    glColor3f(.97,.83,0.09);
    glPointSize(8.);
    glBegin(GL_POINTS);
    
    for (int i =0; i< nbPoints; i++){
        glVertex3fv(TabPoints[i].mVals);
    }
    glEnd();
}

void surfaceCylindrique(std::vector<Vec3> pointsControl,
                         Vec3 droiteFin,
                         int nbU,
                         int nbV,
                         std::vector<Vec3> &res){
    res.clear();
    res.resize(nbU*nbV);
    float pasV;
    //pu - point sur la dorite 
    Vec3 fin, deb;
    int cmp = 0;
    std::vector<Vec3> bezierUPoints;
    BezierCurveByBernstein(pointsControl, pointsControl.size(), nbU, bezierUPoints);
    for (int u = 0; u < nbU; u++){  
        deb = bezierUPoints[u];
        fin = deb + droiteFin;
        for (int v = 0; v < nbV; v++){
            pasV = (float) v/(nbV-1);
            res[cmp++] = (1-pasV)*deb + pasV*fin;        
        }
    }
}

//exo2


void surfaceReglee(std::vector<Vec3> pointsControl1,
                    std::vector<Vec3> pointsControl2,
                    int nbU,
                    int nbV,
                    std::vector<Vec3> &res){
    res.clear();
    res.resize(nbU*nbV);
    float pasV;
    //pu - point sur la dorite 
    Vec3 fin, deb;
    int cmp = 0;
    std::vector<Vec3> bezier1, bezier2;
    BezierCurveByBernstein(pointsControl1, pointsControl1.size(), nbU, bezier1);

    BezierCurveByBernstein(pointsControl2, pointsControl2.size(), nbU, bezier2);
    for (int u = 0; u < nbU; u++){  
        deb = bezier1[u];
        fin = bezier2[u];
        for (int v = 0; v < nbV; v++){
            pasV = (float) v/(nbV-1);
            res[cmp++] = (1-pasV)*deb + pasV*fin;        
        }
    }

}
//exo3
void BezierSurfaceByBernstein(std::vector<Vec3> pointsControl, 
                                long nbControlPointU, 
                                long nbControlPointV,
                                long nbU, 
                                long nbV,
                                std::vector<Vec3> &res){
    res.clear();
    res.resize(nbU*nbV);
    Vec3 somme, dU,gV;
    float Bu,Bm,u,v,n = nbControlPointU - 1,m = nbControlPointV - 1, 
            fac_n = (float) factoriel(n),
            fac_m = (float) factoriel(m)   ;
    for (int u_ = 0; u_ < nbU; u_++){
        u = (float) u_ / (nbU-1);
        somme = Vec3(0., 0., 0.);
        for (int v_ = 0; v_ < nbV; v_++){
            v = (float) v_ / (nbV-1);
            for (int i = 0; i < nbControlPointU; i ++) {
                
                Bu = ((fac_n)/ (factoriel(i)*factoriel(n-i))) * pow(u,i) * pow((1-u), (n-i));
                for (int j = 0; j < nbControlPointV; j ++) {
                    //Bu = ((fac_n)/ (factoriel(i)*factoriel(n-i))) * pow(u,i) * pow((1-u), (n-i));
                    Bm = ((fac_m)/ (factoriel(j)*factoriel(m-j))) * pow(v,j) * pow((1-v), (m-j));

                    somme += (Bu * Bm) *  pointsControl[i*nbControlPointU + j];
                }
            }
            res[u_] = somme;
            
        }
    }
 
} 

//Draw fonctionn
void draw () {



    if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }

    glColor3f(0.8,1,0.8);
    std::vector<Vec3> points;
    int nbPoints = 10;
    int nbPointsControl = 3;
    std::vector<Vec3> pointsControl;
    //exo 1 - execution > decommenter les 3 lignes suivants
/*     Vec3 P0=Vec3(0., 0., 0.), P1=Vec3(2., 0., 0.), V0=Vec3(1., 1., 0.), V1=Vec3(1., -1., 0.);
    HermiteCubicCurve(P0, P1, V0, V1, nbPoints, points);
    DrawCurve(points, nbPoints); */
    
    
    //exo 2 - execution > decommenter les 5 lignes suivants
    //DrawCurve(pointsControl, nbPointsControl);
    //BezierCurveByBernstein(pointsControl, nbPointsControl, nbPoints, points);
    nbPointsControl = 3;
    pointsControl= { Vec3(0., 0., 0.), Vec3(1., 0., 0.),Vec3(1., 1., 0.)};
    //DrawPoints(pointsControl, nbPointsControl); 

    //exo 3 - execution > (de)commenter les lignes suivants
/*    nbPointsControl = 4;
    nbPoints = 4;
    //std::vector<Vec3> pointsControl= { Vec3(0., 0., 0.), Vec3(.1, .2, 0.), Vec3(.2, .2, 0.), Vec3(.3, .1, 0.)};
    //std::vector<Vec3> pointsControl= { Vec3(0., 0., 0.), Vec3(1., 2., 0.), Vec3(2., 2., 0.), Vec3(3., 1., 0.)};
    pointsControl= { Vec3(0., 0., 0.), Vec3(.5, 1., 0.), Vec3(1., 1., 0.), Vec3(1.5, .5, 0.)};

    std::vector<std::vector<Vec3>> tab_points;
    BezierCurveByCasteljau(pointsControl, nbPointsControl, nbPoints, tab_points);

    std::vector<std::vector<Vec3>> transpose;
    transpose_vector(tab_points, transpose);

    for (int i = 0; i < (int)transpose.size(); i++){
        for (int k = 0; k < (int)transpose[i].size(); k++){
             DrawCurve(transpose[i],transpose[i].size());
        }
    }*/

    //TP2
    //exo1
/*    pointsControl= { Vec3(0., 0., 0.),Vec3(.5, .5, 0.), Vec3(1., 0., 0.)};
    std::vector<Vec3> bezierUPoints;
    BezierCurveByBernstein(pointsControl, pointsControl.size(), 3, bezierUPoints);
    //DrawCurve(bezierUPoints,bezierUPoints.size());
    surfaceCylindrique(pointsControl,Vec3(0., .5, .5), 10, 10, points);
    std::cout <<" points.size " << points.size() << std::endl;
    DrawPoints(points, points.size());*/

    //exo2
    /*pointsControl= { Vec3(.25, 0., 0.),Vec3(.5, .25, .25), Vec3(.75, 0., .5)};
    std::vector<Vec3> pointsControl2= { Vec3(0., .5, 0.), Vec3(.25, .75, 0.), Vec3(.75, .75, 0.), Vec3(1., .5, 0.)};    std::vector<Vec3> bezierUPoints;
    std::vector<Vec3> bezierUPoints1, bezierUPoints2;
    BezierCurveByBernstein(pointsControl, pointsControl.size(), 10, bezierUPoints1);
    BezierCurveByBernstein(pointsControl2, pointsControl2.size(), 10, bezierUPoints2);
    DrawCurve(bezierUPoints1,bezierUPoints1.size());
    DrawCurve(bezierUPoints2,bezierUPoints2.size());
    surfaceReglee(pointsControl, pointsControl2, 10, 10,points);
    //std::cout <<" points.size " << points.size() << std::endl;
    DrawPoints(points, points.size());*/


    //exo3
    /* pointsControl= { Vec3(.25, 0., 0.),Vec3(.5, .25, .25), Vec3(.75, 0., .5)};
    std::vector<Vec3> pointsControl2= { Vec3(0., .5, 0.), Vec3(.25, .75, 0.), Vec3(.75, .75, 0.), Vec3(1., .5, 0.)};    std::vector<Vec3> bezierUPoints;
    std::vector<Vec3> bezierUPoints1, bezierUPoints2;
    BezierCurveByBernstein(pointsControl, pointsControl.size(), 10, bezierUPoints1);
    BezierCurveByBernstein(pointsControl2, pointsControl2.size(), 10, bezierUPoints2);
    DrawCurve(bezierUPoints1,bezierUPoints1.size());
    DrawCurve(bezierUPoints2,bezierUPoints2.size());
    int nbU=10, nbV=10;
    surfaceReglee(pointsControl, pointsControl2, nbU, nbV,points);
    //std::cout <<" points.size " << points.size() << std::endl;
    DrawPoints(points, points.size());
    std::vector<Vec3> surfaceBezier;
    BezierSurfaceByBernstein( points, (long) nbU, (long) nbV, (long) nbU, (long) nbV, surfaceBezier);
    DrawPointsYellow(surfaceBezier,surfaceBezier.size()); */
    pointsControl= { Vec3(0., 0., 0.),Vec3(.25, .5, .5), Vec3(.75, .5, .5),Vec3(1., 0., 0.),
                    Vec3(0., 0., 1.),Vec3(.25, .5, 1.5), Vec3(.75, .5, 1.5),Vec3(1., 0., 1.)};
    DrawPoints(pointsControl, pointsControl.size());
    int nbU=4, nbV=2;

    std::vector<Vec3> surfaceBezier;
    BezierSurfaceByBernstein( pointsControl, (long) nbU, (long) nbV, (long) nbU, (long) nbV, surfaceBezier);
    std::cout << "surfaceBezier.size() " << surfaceBezier.size() << std::endl;
    DrawPointsYellow(surfaceBezier,surfaceBezier.size());


    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);

        glColor3f(0.,0.,0.);
        drawMesh(mesh, false);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }



    glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals(mesh);
    }

    if( display_basis ){
        drawReferenceFrame(basis);
    }
    glEnable(GL_LIGHTING);


}

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if(displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if(displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

// ------------------------------------
// User inputs
// ------------------------------------
//Keyboard event
void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;


    case 'w': //Change le mode d'affichage
        changeDisplayMode();
        break;


    case 'b': //Toggle basis display
        display_basis = !display_basis;
        break;

    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_mesh = !display_mesh;
        break;

    case 's': //Switches between face normals and vertices normals
        display_smooth_normals = !display_smooth_normals;
        break;

    case '+': //Changes weight type: 0 uniforme, 1 aire des triangles, 2 angle du triangle
        weight_type ++;
        if(weight_type == 3) weight_type = 0;
        mesh.computeVerticesNormals(); //recalcul des normales avec le type de poids choisi
        break;

    default:
        break;
    }
    idle ();
}

//Mouse events
void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }

    idle ();
}

//Mouse motion, update camera
void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}

// ------------------------------------
// Start of graphical application
// ------------------------------------
int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI702I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Mesh loaded with precomputed normals
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);

    //Completer les fonction de calcul de normals
    mesh.computeNormals();

    basis = Basis();

    // A faire : completer la fonction compute_vertex_valences pour calculer les valences
    //***********************************************//
    std::vector<unsigned int> valences;
    // TODO : Question 1 le calcul des valence pour chaques sommets (vertices) remplir le vector valences
    //          Le nombre de sommets voisin au sommet donné ( partage une arête )
    //          TODO : collect_one_ring() [ Permet de collecter le 1-voisinage ]

    // A faire : normaliser les valences pour avoir une valeur flotante entre 0. et 1. dans mesh_valence_field
    //***********************************************//
    // Utile pour la question 2 permettant d'afficher une couleur dépendant de la valence des sommets
    mesh_valence_field.clear();


    glutMainLoop ();
    return EXIT_SUCCESS;
}

