/*
 * HW1 : Weather Data interp.
 *
 *  Created on: Jan 15, 2016
 *      Author: ao1013
 Parses data from weather records
 */

#include <stdio.h>
#define BUFFERSIZE 100

int main(int argc, char* argv[])
{
	int numberOfInputs = 0;
	char inputString[BUFFERSIZE];
  int counter; //variable used for logic flow of the program.
  int i;
	printf("Monthly Totals\n");
	scanf("%d", &numberOfInputs); //controls the looping
	while(numberOfInputs > 0)
	{
    numberOfInputs--;
    counter = 0;
		scanf("%s",inputString); //get input string
		for(i = 0; i < BUFFERSIZE; i++)
		{
			if(counter == 1)
			{
				if(inputString[i+1] == '-' && inputString[i-1] == '-')
				{
					printf(" "); //print a space, we, we only have 1 char to print for this month.
				}
				printf("%c",inputString[i]); //prints the month
			}
			if(inputString[i] == ',') //date has be found, exit loop.
			{
				counter = 0;
				break;
			}
			else if(inputString[i] == '-')
			{
				counter++;
			}
		}
		while(counter < BUFFERSIZE)
		{
			if(inputString[counter] == '-')
			{
				counter = 0;
				break;
			}
			printf("%c",inputString[counter]); //prints the year (always 4 chars long).
			counter++;
		}
		printf(" Precip = ");
		for(i = 0; i < BUFFERSIZE; i++)
		{
			if(counter > 19)
			{
				break;
			}
			else if(inputString[i] == ',')
			{
				counter++;
			}
			else if(counter == 19)
			{
				printf("%c",inputString[i]); //prints the year (always 4 chars long).
			}
		}
   printf("\n");
	}
	return 0;
}
