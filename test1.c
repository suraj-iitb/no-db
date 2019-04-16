#include<stdio.h>
#include<conio.h>
#include<String.h>
void getData(char *buff);
int main()
{
	printf("Enter the schema of the table\n");
	char schema_definition[MAX_LIMIT]; //define your schema
	/*
	CREATE FOREIGN TABLE zip_codes_t (Zipcode varchar, ZipCodeType varchar, City varchar, State varchar,
	LATITUDE varchar, LONGITUDE varchar) SERVER pglog OPTIONS ( 
	filename '/home/shreya/Downloads/free-zipcode-database-Primary.csv', format 'csv' );
	*/
	/*
	CREATE FOREIGN TABLE <table_name> (<attribute1_name> <attribute1_type>, <attribute2_name> <attribute2_type>,) 
	<SERVER pglog>* OPTIONS (filename <absolute filepath>, format <e.g 'csv'> );
	<>* irrelevant to us for now
	*/
   	fgets(schema, MAX_LIMIT, stdin); 

   	

	FILE *fp = fopen("","r"); //D:\\Test.csv
}
void queryParser() //handle selects
{

}
void getData(char buff[])
{
   char *token = strtok(buff,",");
   int counter=0;
 
   while( token != NULL ) 
   {
 	counter++; 
	printf( " %s\n",token);
      	token = strtok(NULL,",");
   }	  
}
