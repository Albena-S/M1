1. Créer (ou lire) l’image qui servira de texture 
2. Donner des coordonnées de texture aux sommets du modèle à texturer
3. Créer un ou plusieurs objets de texture pour stocker les textures
• Et transférer les données de texture de l’image vers l’objet texture (buffer)
4. Paramétrer le texture mapping 
• filtrage, dépassement (pour les coordonnées de texture hors [0,1]), et
5. Activer une ou plusieurs unités de texture 
6. Associer un objet de texture à une unité de texture
Côté GLSL
1. Dans le vertex shader
Faire passer les coordonnées de textures au fragment shader (attributes (in) -> varying (out)
2. Dans le fragment shader
• Déclarer l’ (les) unité(s) de texture utilisée(s) (sampler2D)
• Trouver la couleur d’un texel pour les coordonnées de texture en utilisant un sampler
• Utiliser la couleur du texel pour calculer la couleur du fragment



# Rendu 
## Explications de partie 'EN PRATIQUE' de cours par rapport au code de demo

## Côté javascript

### 1.Créer (ou lire) l’image qui servira de texture 
premiere partie de programme(de draw) qui correspond au 2 premiers shaders