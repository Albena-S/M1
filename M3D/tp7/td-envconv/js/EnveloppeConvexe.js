const algoEnvConv = {demiPlan: 0, jarvis:1, graham:2};

class EnveloppeConvexe{
	constructor(lesPoints, a){
		this.algo = a ?? algoEnvConv.jarvis;
		this.points = lesPoints;
		this.setAlgo(this.algo);
	}

	getElements (){
		let ec = new Array();
		for (let i=0; i < this.envconv.length; i++){
			ec.push(this.points[this.envconv[i]]);
		}
		return ec;
	}

	setAlgo(idAlgo){
		this.algo = idAlgo;
		switch (this.algo){
			case algoEnvConv.demiPlan:
				this.envconv = this.algoDemiPlan(this.points);
				break;
			case algoEnvConv.jarvis:
				this.envconv = this.algoJarvis(this.points);
				break;
			case algoEnvConv.graham:
				this.envconv = this.algoGraham(this.points);
				break;	
			default:
				console.log("algo non défini => algo jarvis utilisé")
				this.envconv = this.algoDemiPlan(this.points);
				break;
		}
	}

	findMinIdx(points){
		let imin = 0;
		for(let i = 1; i < points.length; i++){
			if (points[i].y < points[imin].y)
				imin = i;
		}
		return imin;
	}

	determinant(v1,v2){
		return v1.x * v2.y - v1.y * v2.x;
	}

	detSign(v1,v2){
		let d = this.determinant(v1,v2);
		if (d > 0) return 1;
		if (d == 0) return 0;
		return -1;
	}

	vect(a,b){
		return{x:b.x-a.x,y:b.y - a.y}
	}

	// return 0 si les points sont alignés, -1 pour un tour horaire (droit=horaire), +1 pour un tour gauche (=direct=trigo=antihoraire)
	tour(a, b, c){
		return  this.detSign( this.vect(a,b), this.vect(a,c));
	}

	findNext(previous, min){
		let V = this.points;
		let gauche = min;
		for (int i = 0; i < n; i++){
		if (tour(V[previous], V[gauche], V[i]) == -1)
			gauche = i;
		}
	}

	algoDemiPlan(V){
		console.log("algo d'intersection des demi-plans")
		//initialisations
		let envconv = new Array();
		let n = V.length;
		let current, previous, i, j,k;

		for (i = 0; i < n; i++){
			for (j = i+1; j < n; j++){
				  let mc = true;
				  k = 0;
				  while ( k < n && mc){
				  	if (k == 0){
				  		let first =  i > 0 ? 0 : (j == 1 ? 2 : 1 );
				  		previous = this.tour(V[i], V[j], V[first]);
				  	}
				  	if (k != i && k != j){
				  		current =  this.tour(V[i], V[j], V[k] );
				  		if (current == 0) // exception points alignés
				  			console.log(( "alignement", [V[i], V[j], V[k]]));
				  		else if (current != previous)
				  			mc = false;
				  	}
				  	k++;
				  }

				  if (k ==n && previous == current){
				  	if (current > 0) {
				  		envconv.push(i);
				  		envconv.push(j);
				  	}
				  	if (current < 0){
				  		envconv.push(j);
				  		envconv.push(i);
				  	}
				  	else
				  		console.log(( "alignement", [V[i], V[j], V[k]]));
				  }

			}
		}

		console.log(envconv.length);

		return envconv;
	}

	 algoJarvis(V){
		console.log("algo Jarvis")
		//initialisations
		let min = findMinIdx(V);
		let envconv = [min];
		let current, previous = min; 
		//début
		while(current != min){
			current = findNext(previous, min);
			envconv.push(current);
			previous = current;
		}
		return envconv;
	}

	algoGraham(points){
		let imin = this.findMinIdx(points);
		let ec = new Array();
		ec[0] = imin;

		//todo

		return ec;
	}
}