int ordinalSuffix_index(int n){
	if(n<0){
		n*=-1;
	}
	if(n==11 || n==12){
		return 3;
	}
	if(n%10==1){
		return 0;
	}
	if(n%10==2){
		return 1;
	}
	if(n%10==3){
		return 2;
	}
	return 3;
}
