// Définition de la classe GridXZ

// classes et modules nécessaires
Requires("libs/Mesh/Mesh.js");


/**
 * Cette classe construit un maillage en forme de grille horizontale pour visualiser le plan XZ
 */
class GridXZ
{
    /**
     * Cette classe construit un maillage en forme de grille horizontale pour visualiser le plan XZ
     * @param count : nombre de graduations de part et d'autre de zéro
     * @param step : écart entre deux lignes
     * @param alpha : transparence de la grille
     * @param width : largeur des lignes
     */
    constructor(count=5, step=1.0, alpha=1.0, width=1.0)
    {
        // paramètres
        this.m_Alpha = alpha;
        this.m_Width = width;

        // plan de coupe par défaut à l'infini
        this.m_ClipPlaneOn = false;
        this.m_ClipPlane = vec4.fromValues(0,0,1,-1e38);

        // compiler le shader
        this.m_ShaderId = null;
        this.compileShader();

        // créer et remplir les buffers des coordonnées et des couleurs
        let vertices = [];
        let colors = [];
        for (let i=-count; i<=count; i++) {
            // lignes selon X
            vertices.push(-count);
            vertices.push(0.0);
            vertices.push(i*step);

            colors.push(0.0);
            colors.push(0.0);
            colors.push((i+count)/(2.0*count));

            vertices.push(+count);
            vertices.push(0.0);
            vertices.push(i*step);

            colors.push(1.0);
            colors.push(0.0);
            colors.push((i+count)/(2.0*count));

            // lignes selon Z
            vertices.push(i*step);
            vertices.push(0.0);
            vertices.push(-count);

            colors.push((i+count)/(2.0*count));
            colors.push(0.0);
            colors.push(0.0);

            vertices.push(i*step);
            vertices.push(0.0);
            vertices.push(+count);

            colors.push((i+count)/(2.0*count));
            colors.push(0.0);
            colors.push(1.0);
        }

        // créer les VBOs des coordonnées et des couleurs
        this.m_VertexBufferId = Utils.makeFloatVBO(vertices, gl.ARRAY_BUFFER, gl.STATIC_DRAW);
        this.m_ColorBufferId  = Utils.makeFloatVBO(colors, gl.ARRAY_BUFFER, gl.STATIC_DRAW);

        // nombre de sommets
        this.m_VertexCount = vertices.length / 3;
    }


    /**
     * retourne le source du Vertex Shader
     */
    getVertexShader()
    {
        let srcVertexShader = "";
        srcVertexShader += "#version 300 es\n";
        srcVertexShader += "in vec3 glVertex;\n";
        srcVertexShader += "in vec3 glColor;\n";
        srcVertexShader += "out vec3 frgColor;\n";
        srcVertexShader += "out vec4 frgPosition;\n";
        srcVertexShader += "uniform mat4 mat4ModelView;\n";
        srcVertexShader += "uniform mat4 mat4Projection;\n";
        srcVertexShader += "void main()\n";
        srcVertexShader += "{\n";
        srcVertexShader += "    frgPosition = mat4ModelView * vec4(glVertex, 1.0);\n";
        srcVertexShader += "    gl_Position = mat4Projection * frgPosition;\n";
        srcVertexShader += "    frgColor = glColor;\n";
        srcVertexShader += "}\n";
        return srcVertexShader;
    }


    /**
     * retourne le source du Fragment Shader
     */
    getFragmentShader()
    {
        let srcFragmentShader = "";
        srcFragmentShader += "#version 300 es\n";
        srcFragmentShader += "precision mediump float;\n";
        srcFragmentShader += "in vec4 frgPosition;\n";
        if (this.m_ClipPlaneOn) {
            srcFragmentShader += "uniform vec4 ClipPlane;\n";
        }
        srcFragmentShader += "in vec3 frgColor;\n";
        srcFragmentShader += "out vec4 glFragData[4];\n";
        srcFragmentShader += "void main()\n";
        srcFragmentShader += "{\n";
        if (this.m_ClipPlaneOn) {
            srcFragmentShader += "    if (dot(frgPosition, ClipPlane) < 0.0) discard;\n";
        }
        srcFragmentShader += "    glFragData[0] = vec4(frgColor, "+this.m_Alpha.toFixed(2)+");\n";
        srcFragmentShader += "    glFragData[1] = vec4(0.0);\n";
        srcFragmentShader += "    glFragData[2] = vec4(frgPosition.xyz, 1.0);\n";
        srcFragmentShader += "    glFragData[3] = vec4(1.0, 1.0, 1.0, 0.0);\n";
        srcFragmentShader += "}\n";
        return srcFragmentShader;
    }


    /**
     * recompile le shader
     */
    compileShader()
    {
        // supprimer l'ancien shader s'il y en avait un
        if (this.m_ShaderId != null) Utils.deleteShaderProgram(this.m_ShaderId);

        // construire le vertex shader
        let srcVertexShader = this.getVertexShader();

        // construire le fragment shader
        let srcFragmentShader = this.getFragmentShader();

        // compiler le shader de dessin
        this.m_ShaderId = Utils.makeShaderProgram(srcVertexShader, srcFragmentShader, "GridXZ");

        // déterminer où sont les variables attribute et uniform
        this.m_VertexLoc     = gl.getAttribLocation(this.m_ShaderId, "glVertex");
        this.m_ColorLoc      = gl.getAttribLocation(this.m_ShaderId, "glColor");
        this.m_MatPloc       = gl.getUniformLocation(this.m_ShaderId, "mat4Projection");
        this.m_MatMVloc      = gl.getUniformLocation(this.m_ShaderId, "mat4ModelView");
        this.m_ClipPlaneLoc  = gl.getUniformLocation(this.m_ShaderId, "ClipPlane");
    }


    /**
     * dessine l'objet
     * @param mat4Projection : matrice de projection
     * @param mat4ModelView : matrice de vue
     */
    onDraw(mat4Projection, mat4ModelView)
    {
        // activer le shader
        gl.useProgram(this.m_ShaderId);

        // fournir les matrices P et MV
        mat4.glUniformMatrix(this.m_MatPloc, mat4Projection);
        mat4.glUniformMatrix(this.m_MatMVloc, mat4ModelView);

        // si le plan de coupe est actif, alors le fournir
        if (this.m_ClipPlaneOn) {
            vec4.glUniform(this.m_ClipPlaneLoc, this.m_ClipPlane);
        }

        // activer et lier le buffer contenant les coordonnées
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC3, gl.FLOAT, gl.FALSE, 0, 0);

        // activer et lier le buffer contenant les couleurs
        gl.enableVertexAttribArray(this.m_ColorLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_ColorBufferId);
        gl.vertexAttribPointer(this.m_ColorLoc, Utils.VEC3, gl.FLOAT, gl.FALSE, 0, 0);

        // dessiner les lignes
        gl.lineWidth(this.m_Width);
        gl.drawArrays(gl.LINES, 0, this.m_VertexCount);

        // désactiver les buffers et le shader
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.disableVertexAttribArray(this.m_ColorLoc);
        gl.useProgram(null);
    }


    /**
     * définit un plan de coupe pour les fragments. Ce plan est en coordonnées caméra
     * @param active : true s'il faut compiler un shader gérant le plan de coupe
     * @param plane : vec4 en coordonnées caméra ou null pour supprimer le plan de coupe
     */
    setClipPlane(active=false, plane=null)
    {
        // il faut recompiler s'il y a un changement d'état
        let recompile = (active != this.m_ClipPlaneOn);

        this.m_ClipPlaneOn = active;
        if (plane == null) {
            this.m_ClipPlane = vec4.fromValues(0,0,1,1e38);
        } else {
            this.m_ClipPlane = plane;
        }

        // recompiler le shader
        if (recompile) this.compileShader();
    }


    /**
     * désactive le plan de coupe
     */
    resetClipPlane()
    {
        let recompile = this.m_ClipPlaneOn;
        this.m_ClipPlaneOn = false;
        this.m_ClipPlane = vec4.fromValues(0,0,1,-1e38);
        if (recompile) this.compileShader();
    }


    /** destructeur */
    destroy()
    {
        Utils.deleteShaderProgram(this.m_ShaderId);
        Utils.deleteVBO(this.m_VertexBufferId);
        Utils.deleteVBO(this.m_ColorBufferId);
    }
}
