# Andrew Towers
# Program 5
# This program parses a text file with fields separated whitespace in the order: first name, last name,
# number of plate appearances, number of at bats, number of singles, number of doubles, number of triples,
# number of home runs, number of walks or hit by pitch.  It then calculates the batting average, slugging
# percentage, and on-base percentage and prints the formated results to the screen

BEGIN {
	printf("---- BEGIN STATISTICS REPORT ----\n\n"); # print heading
	printf("LASTNAME, FIRSTNAME     AVG   SLG   OBP\n");
}
	
{
	if ( NF != 9) # Check if wrong number of fields entered 
	{
		printf("Error. Wrong number of fields in input\n");
		next; # End processing for this line and go to next line
	}
	printname = $2", "$1; # Store name in "last, first" format
	printf("   %-18s: ", printname); # Print formatted name
	for (field = 3; field <= 9; field++) # For each numeric field
	{
		if ($field !~ /^[0-9]+$/) # If the field is not entirely digits
		{
			print "Unable to compute result. Invalid numeric data. ***"; # Print error message
			next; # End processing for this line and go to next line
		}
		
	}
	
	at_bats = $4 
	plate_appearances = $3
	bb_plus_hbp = $9
	total_hits = 0; # Initialize total hits counter to 0
	weighted_hits = 0; # Initialize weighted hits counter to 0
	
	hit_type = 1; # Initialize hit type to 1 for single
	for (field = 5; field <= 8; field++) # Increment through hit types, starting at singles field
	{
		hit_array[hit_type++] = $field; # Store in hit_array with index as the hit type's corresponding number of bases
	}
	
	for (hit_type = 1; hit_type <= 4; hit_type++) # For each hit type in hit array
	{
		total_hits += hit_array[hit_type]; # Increment total hits to be used for average and obp;
		weighted_hits += hit_array[hit_type] * hit_type; # Multiply number of hits by corresponding number of bases, increment weighted_hits, to be used for SLG
	}
	
	batting_average = total_hits / at_bats; # Compute batting average
	slugging_percentage = weighted_hits / at_bats; # Compute slugging percentage
	obp = (total_hits + bb_plus_hbp) / plate_appearances; # Computer OBP
	
	printf("%4.3f %4.3f %4.3f\n", batting_average, slugging_percentage, obp); # Format AVG, SLG, and OBP and print
}
END {
	printf("\n---- END STATISTICS REPORT ----\n") # Print closing line
}