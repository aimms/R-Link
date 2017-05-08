k=1; 
for(i in 1:(length(doubles)) ){
	for(j in 1:(length(cities)) ) {
		for(l in 1:(length(doubles))){
			if( (i + j + l ) %% 2 == 0 ){
			    values[k] = "Amsterdam";
		    }
            else{
			    values[k] = "Haarlem";
		    }
			k = k + 1;
		}  
	}
}