/*Author: Alex Orozco
* Date: 1/25/16
* Description: Write a program that reads in 10 annual reports for the specified years and generates a single summary file called
"summary.csv". Each line of the summary file should contain a single name followed by its popularity rank in each of the
10 annual reports. The name and all the ranks should be separated by commas. The file extension "csv" stands for
comma separated values and Excel will automatically separate the values into columns when you view such a file as a
spreadsheet. The names in the summary file must appear in alphabetical order. For any given name, the ranks should
appear in chronological order (first the rank in 1920, then 1930, and so on until 2010). A name will appear in the
summary report if it was one of the 100 most popular names at least once. A name that appears in the top 100 for one
year may be missing from the top 100 in other years. A name not included in the top 100 has no rank at all for that year.
You can indicate this as a missing value to Excel by placing two adjacent commas in the summary file.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LENGTH_BUFFER 15
#define NUM_NAME_BUFFER 1000
#define LINE_BUFFER_SIZE 25 //NAME_LENGTH_BUFFER + 10 for data

void sortArray(char theNames[NUM_NAME_BUFFER][NAME_LENGTH_BUFFER], int numbers[NUM_NAME_BUFFER][10], int numberOfNames) //bubble sort
{
  char temp[NAME_LENGTH_BUFFER];
  int numTemp;
  int i, j, k, l;
  for ( i = 0; i < numberOfNames; i++)
  {
    for (j = 0; j < numberOfNames - 1; j++)
    {
      l = strcmp(theNames[j], theNames[j + 1]);
      if (l == 1) //found swap targets
      {
        strcpy(temp, theNames[j]);
        strcpy(theNames[j], theNames[j + 1]);
        strcpy(theNames[j + 1], temp);
        for (k = 0; k < 10; k++)  //swap the number elements
        {
          numTemp = numbers[j][k]; //store value
          numbers[j][k] = numbers[j + 1][k]; //swap the ahead array value to current
          numbers[j + 1][k] = numTemp; //change ahead to the stored value;
        }
      }
    }
  } 
}

void createOutputFile(char names[NUM_NAME_BUFFER][NAME_LENGTH_BUFFER], int yearNames[NUM_NAME_BUFFER][10], int numberOfNames)
{
  FILE *output;
  output = fopen("summary.csv", "w");
  char header[56] = "Name,1920,1930,1940,1950,1960,1970,1980,1990,2000,2010,\n";
  fwrite(header, sizeof(char), sizeof(header) / sizeof(char), output); //address point
  int i, j;
  char newLine = '\n';
  for (i = 0 ; i < numberOfNames; i++)
  {
    j = 0;
    while ((names[i][j] >= 65 && names[i][j] <= 90) || (names[i][j] >= 97 && names[i][j] <= 122)) //while char is in ascii range.
    {
      fwrite(&names[i][j], 1, 1, output); //WRITE THE NAME 1 char at a time.
      j++;
    }
    fwrite(",", 1, 1, output); //write comma to file
    for ( j = 0; j < 10; j++) //FILL IN THE YEARS
    {
      if (yearNames[i][j] != 0) //non empty location
        fprintf(output, "%d", yearNames[i][j]); //write the int
      fwrite(",", 1, 1, output); //write comma to file
    }
    fwrite(&newLine, sizeof(char), sizeof(newLine), output);
  }
  fclose(output);
}
int processName(char name[LINE_BUFFER_SIZE], char theNames[NUM_NAME_BUFFER][NAME_LENGTH_BUFFER], int numbers[NUM_NAME_BUFFER][10], int numberOfNames, int powerOfNums, int yearIndex)
{
  char nameTempStorage[NAME_LENGTH_BUFFER] = "";
  int i, j, k, numberFound, multipleOf10;
  i = 0;
  numberFound = 0;
  while (1) //parse the name our of the char array
  {
    if (name[i] == ',') //looking for the comma
    {
      i += 3;
      break;
    }
    if (((int)name[i] >= 65 && name[i] <= 90) || ((int)name[i] >= 97 && name[i] <= 122)) // as long as our value is in the ascii table and we haven't found a comma
      nameTempStorage[i] = name[i];
    i++;
  } //i is at the index for getting the number.
  multipleOf10 = 1;
  while (powerOfNums > 0)
  {
    for (k = 0; k < powerOfNums - 1; k++) //power of 10, easier that using the pow() from math.h
      multipleOf10 *= 10;
    numberFound += (48 - (int)name[i]) * multipleOf10; //"343" will turn into 300+ 40+3 ///// name[i] - '0' == the raw value of the ascii value
    i++; //increment
    multipleOf10 = 1; //reset power
    powerOfNums--; //decrement power Control
  }
  numberFound *= -1; //comes out negative, must inverse it.
  for (i = 0; i < numberOfNames; i++)
  {
    k = strcmp(nameTempStorage, theNames[i]);
    if( k == 0) //we found a copy at index i, dont add name but add only year.
    {
      numbers[i][yearIndex] = numberFound;
      return 0;
    }
  }
  strcpy(theNames[numberOfNames], nameTempStorage); 
  numbers[numberOfNames][yearIndex] =  numberFound;
  return 1;
}

int readSingleFile(char theNames[NUM_NAME_BUFFER][NAME_LENGTH_BUFFER], int nameNumberRep[NUM_NAME_BUFFER][10], int yearIndex, char fileName[],int totalNames)
{
  int i, j, powerIndex, namesAdded;
  char ch;
  FILE *theFile = fopen(fileName, "r");
  i = 0;
  j = 0;
  powerIndex = 0;
  namesAdded = 0;
  char lineBuffer[LINE_BUFFER_SIZE];
  while (j < 100 && theFile != NULL)
  {
    ch = fgetc(theFile);
    if (ch <= 57 && ch >= 48) //we found an ascii number
      powerIndex++;
    if (ch == '\n')
    {
      j++; //controls the number of names we found.
      ch = fgetc(theFile); // need to do one more read for the new tab
      namesAdded += processName(lineBuffer, theNames, nameNumberRep, namesAdded+totalNames, powerIndex, yearIndex); 
      powerIndex = 0; //reset vars
      i = 0; //reset vars
      memset(lineBuffer, 0, sizeof(lineBuffer)); //reset Array
    }
    lineBuffer[i] = ch;
    i++;
  }
  return namesAdded;
}
void read10Files()
{
  int i;
  int totalNames = 0; //keeps track of the total number of names
  char names[NUM_NAME_BUFFER][NAME_LENGTH_BUFFER] = {"",""}; //array for names
  int yearNums[NUM_NAME_BUFFER][10] = {0}; //array for numbers of names in years
  char fileNames[12] = "yob1920.txt"; //file names
  int indexChange = 5; //controls the year we are accessing
  for (i = 0; i < 10; i++)
  {
    if(fileNames[indexChange] == ':') //ascii table incrementation
    {
      fileNames[indexChange] = '0';
      fileNames[indexChange - 1] = '0'; // change 9 to a 0
      fileNames[indexChange - 2]++; //change the 1 to a 2
    }
    totalNames += readSingleFile(names, yearNums, i, fileNames, totalNames); //returns 1 if a new name is added. 0 if only a new year is added.
    fileNames[indexChange]++; //ascii table incrementation
  }
  sortArray(names, yearNums, totalNames);
  createOutputFile(names, yearNums, totalNames);
}
int main(void)
{
  read10Files();
}
