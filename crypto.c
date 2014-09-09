#include <stdio.h>
#include <stdlib.h>
#define bufferSize 600

char key[90], *inFileName, *outFileName;
FILE *inFile, *outFile;

unsigned char readBuffer[bufferSize];
int bytesCopied, i, j;
unsigned char c, r;
unsigned int seed;
char *mode; //encryption or decryption mode. Needed for swapping only.

//the xor function does the confusion
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

//the swap function does the diffusion
void swap(int copiedBytes) 
{
	int midpoint = copiedBytes/2;
	char tempC;
	int i, j;
	//swap first and last chars of buffer until you reach the middle.
	for(i = 0, j = copiedBytes -1; i < midpoint; i++,j--)
	{
		tempC = readBuffer[i];
		readBuffer[i] = readBuffer[j];
		readBuffer[j] = tempC;
	}	
}

int main(int argc, char**argv)
{
	if(argc < 4) {
		printf("Incorrect Usage. \nInclude params: [\"-e\" | \"-d\"] [input file] [output file]\n");
		return -1;	
	}

	printf("Enter Key:");
	fgets(key, 90, stdin);
	
	//sets the seed based on the key.
	int seedAddition = key[0]; //Maybe no affect but it could make it encryption weaker or stronger. ....analyze.
	for(i=0; key[i]; i++) seed = (seed + seedAddition)*key[i];
	srand(seed);
	
	//determine if we're trying to encrypt or decrypt
	mode = argv[1];
	
	//open both files
	inFileName = argv[2];
	outFileName = argv[3];	
	
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

	/*read from input file to char array. encrypt char array and write it to output file.
	 *The array's contents will be swapped such that the last element swapped with element 0,
	 *second-to-last element is swapped with element 1, etc.
	 *If encrypting, swapping will be done before substitution, If decrypting, it will be done after.
	 */
	while(1) 
	{
		bytesCopied = fread(readBuffer, sizeof(char), bufferSize, inFile);
		
		if(mode == "-e") swap(bytesCopied);
		
		for(i = 0;i < bytesCopied; i++) 
		{
			c = readBuffer[i];
			r = rand();
			readBuffer[i] = xor(c, r);
		}
		
		if(mode == "-d") swap(bytesCopied);
		
		fwrite(readBuffer, sizeof(char), bytesCopied, outFile);
		if(bytesCopied < bufferSize) break;
	}
	
	fclose(outFile);
	fclose(inFile);

	return 0;
}