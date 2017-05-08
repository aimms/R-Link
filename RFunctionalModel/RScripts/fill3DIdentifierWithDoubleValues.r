k=1; 
for(i in 1:(length(doubles)) ){
	for(j in 1:(length(cities)) ) {
		for(l in 1:(length(doubles))){
			values[k] = (length(doubles) * ( length(cities)*i + j ) + l) + .1234; 
			k = k + 1;
		}  
	}
}