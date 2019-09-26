//---------------------------------------------------------------------------------------
//  NAME:: in_list										        
//                                                              
//  DESCRIPTION:: determins if a value is in a list of a given length			
// 	returns the position in the list if found otherwise	returns a zero value			
//---------------------------------------------------------------------------------------
int	in_list( int inx, int *list, int num_pit) 
{
	int i, fnd;

	fnd = 0;
	for (i=1; i <= num_pit; i++) {
		if ( inx == list[i] )
			fnd = i;
	}
	
	return(fnd);
} 
	



