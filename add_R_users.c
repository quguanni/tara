
/* 
 Add a user to a file of the same machine name

	addu buim 169.237.71.246
	addu zen 169.237.71.246
	addu ben 169.237.71.246

 */


#include	<stdio.h>

#define	LINELEN	80		/* Max chars in a line. */

main(int argc, char *argv[])
{
	char	* r_user    = argv[1];  
	char	* r_machine = argv[2];
	
	int k;	
	char	r_machine_file[LINELEN];	

	FILE	* tmpfile;


	if (argc < 3)  
	{
		fprintf(stderr, "addu: missing arguments\n");
		exit(1);
	}
		
	tmpfile = NULL;

	if ( (k = system("/usr/bin/test -d temp_dir_") ) != 0)		
	{
		system("mkdir temp_dir_"); 
	}
	
	strcpy(r_machine_file,"");
	strcat(r_machine_file, "temp_dir_/");


	strcat(r_machine_file, r_machine);		/* r_machine_file = temp_dir_/r_machine */	
	


	tmpfile = fopen(r_machine_file, "a");

	if (tmpfile == NULL)  
	{
		fprintf(stderr, "addu: can't open %s.\n", r_machine_file);
		exit(1);
	}

	strcat(r_user,"\n");

	fprintf(tmpfile, r_user);

	fclose(tmpfile);

	exit(0);
}



