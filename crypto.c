/******************************************************************************\												                           
|	Program : crypto.c						                           
|	Problem : An encryption algorithm.
|	Method  : user's gives key. (xor operation w/ pseudo-random bytes from key)	                               
|	Author  : Moaan Ahmed							                         
|	Date    : 09/06/2014		                   										                               
\******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#define bufferSize 600

char key[90], *inFileName, *outFileName;
FILE *inFile, *outFile;

unsigned char readBuffer[bufferSize];
int bytesCopied, i, j;
unsigned char c, r;
unsigned int seed;

unsigned char xor(unsigned char a, unsigned char r)
{
	unsigned char c = 0;
	for(j = 0;j < 8; j++)
	{	
		c*=2;
		if((a > 127) != (r > 127)) c++;
		r*=2; 
		a*=2;
	}
	return c;
}

int main(int argc, char**argv)
{
	if(argc < 3) {
		printf("Incorrect Usage. Include parameters: [input file] [output file]\n");
		return -1;	
	}

	printf("Enter Key:");
	fgets(key, 90, stdin);
	
	//sets the seed based on the key.
	for(i=0; key[i]; i++) seed = (seed + 2)*key[i];
	srand(seed);
	
	//open both files
	inFileName = argv[1];
	outFileName = argv[2];	
	
	//open input file
	inFile = fopen(inFileName, "r");
	if(inFile == NULL) 
	{
		printf("\nFile %s Open Error.\n", inFileName);
		return 1;
	}
	
	//open output file	
	outFile = fopen(outFileName, "w");
	if(outFile == NULL) 
	{
		printf("\nFile %s Open Error.\n", outFileName);
		return 1;
	}

	//read from input file to char array. encrypt char array and write it to output file
	while(1) 
	{
		bytesCopied = fread(readBuffer, sizeof(char), bufferSize, inFile);
		for(i = 0;i < bytesCopied; i++) 
		{
			c = readBuffer[i];
			r = rand();
			readBuffer[i] = xor(c, r);
		}
		fwrite(readBuffer, sizeof(char), bytesCopied, outFile);
		if(bytesCopied < bufferSize) break;
	}
	
	
	//read one char. encrypt/decrypt it. write it.
	/*
	c = fgetc(inFile);
	while(c != EOF)
	{
		r = rand();
		fputc(xor(c, r), outFile);
		c = fgetc(inFile);
	}*/
	
	fclose(outFile);
	fclose(inFile);

	return 0;
}