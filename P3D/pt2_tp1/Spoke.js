// Définition de la classe Triangle


class Spoke
{
    /** constructeur */
    constructor()
    {
        /** shader */

        let srcVertexShader = dedent
            `#version 300 es
            in vec2 glVertex;
            void main()
            {
                gl_Position = vec4(glVertex, 0.0, 1.0);
            }`;

        let srcFragmentShader = dedent
            `#version 300 es
            precision mediump float;
            out vec4 glFragColor;
            void main()
            {
                glFragColor = vec4(0.9, 0.9, 0.9, 1.0);
            }`;

        // compiler le shader de dessin
        this.m_ShaderId = Utils.makeShaderProgram(srcVertexShader, srcFragmentShader, "Spoke");

        // déterminer où sont les variables attribute
        this.m_VertexLoc = gl.getAttribLocation(this.m_ShaderId, "glVertex");

        /** VBOs */

        // créer et remplir le buffer des coordonnées
        this.vertices = [
0.0, 0.0109,
0.2174, 0.0109,
0.2174, 0.0109,
0.2826, 0.1196,
0.2826, 0.1196,
0.2994, 0.1104,
0.2994, 0.1104,
0.2391, 0.0109,
0.2391, 0.0109,
0.3261, 0.0109,
0.3261, 0.0109,
0.3696, 0.0761,
0.3696, 0.0761,
0.386, 0.0667,
0.386, 0.0667,
0.3478, 0.0109,
0.3478, 0.0109,
0.4783, 0.0109        
];
        
        

        this.m_VertexBufferId = Utils.makeFloatVBO(this.vertices, gl.ARRAY_BUFFER, gl.STATIC_DRAW);
    }


    /*public Point lerp(Point P0, Point P1, Point P) 
        {
            double y1 = P0.Y + (P1.Y - P0.Y) * ((P.X - P0.X) / (P1.X - P0.X));
            double x1 = P.X;

            double y2 = P.Y;
            double x2 = P0.X + (P1.X - P0.X) * ((P.Y - P0.Y) / (P1.Y - P0.Y));

            return new Point((x1 + x2) / 2, (y1 + y2) / 2);
    }

*/
    /** dessiner l'objet */
    onDraw()
    {
        // activer le shader
        gl.useProgram(this.m_ShaderId);

        // activer et lier le buffer contenant les coordonnées
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId);
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC2, gl.FLOAT, gl.FALSE, 0, 0);

        // dessiner un triangle avec les trois vertices
        gl.drawArrays(gl.LINES, 0, this.vertices.length/2);

        // désactiver le buffer
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // désactiver le shader
        gl.useProgram(null);
    }


    /** destructeur */
    destroy()
    {
        // supprimer le shader et le VBO
        Utils.deleteShaderProgram(this.m_ShaderId);
        Utils.deleteVBO(this.m_VertexBufferId);
    }
}
