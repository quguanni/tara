
/* 
machine name 
openfile Sucess_formmahicne
"this is output form"
append to file Successs

 */


#include	<stdio.h>

#define	LINELEN	600		/* Max chars in a line. */

main(int argc, char *argv[])
{
	char	* r_machine = argv[1];
	
	int k;	
	char	r_machine_file[LINELEN];	
	char 	tempbuf[LINELEN];
	FILE	* Success_fp;
	FILE	* Success_from_fp;


	if (argc < 2)  
	{
		fprintf(stderr, "process_success_file: missing arguments\n");
		exit(1);
	}
		
	strcpy(r_machine_file,"");
	strcat(r_machine_file, "Success_from_");

	strcat(r_machine_file, r_machine);		/* r_machine_file = temp_dir_/r_machine */	
	
	Success_fp = fopen("Success", "a");
	Success_from_fp = fopen(r_machine_file, "r");


	if (Success_fp == NULL)  
	{
		fprintf(stderr, "addu: can't open file Success.\n");
		exit(1);
	}

	if (Success_from_fp == NULL)  
	{
		fprintf(stderr, "addu: can't open file %s.\n",r_machine_file);
		exit(1);
	}

	fprintf(Success_fp,"====================\n");
	fprintf(Success_fp,"\nSuccess from %s\n",argv[1]);
	while (fgets(tempbuf,LINELEN,Success_from_fp) != NULL)
	{
		fprintf(Success_fp, tempbuf);
	}
	fprintf(Success_fp,"====================\n");

	fclose(Success_fp);
	fclose(Success_from_fp);

	exit(0);
}



