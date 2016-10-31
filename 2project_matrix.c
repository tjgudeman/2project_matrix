/*
 Name: TJ Gudeman
Date: 10/24/16
Assignment: Matrix multiplication.
Purpose: This program recieves user input which will then open 2 files
		 which are read into 2-D arrays. They then are multiplied together
		 using threads. The answer is stored in an array and then written to
		 the file "output.txt".
*/


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


int file1Array[200][200];
int file2Array[200][200];
int resultsArray[200][200];
int column1;
int row2;
struct rowsAndColumns // So we can pass rows and columns to thread
{
	int i;
	int j;
	
};
void *multiply( void *rowsAndColumns); //Define thread



int main(int argc, char const *argv[]){
	int xx = 0;
	FILE *file1; //File pointer
	FILE *file2;
	char file1Name[50]="";
	char file2Name[50]="";
	// int file1Array[50][50];
	int row1=0;
	int column2=0;
	char tempCharHolder;	

	/****************************************************
	Test to see if file1 opens
	****************************************************/
	printf("Please enter the first file name: ");
	scanf("%s", file1Name);

	file1=fopen(file1Name, "r");
	if(file1 == NULL){
		printf("File is not valid\n");
		exit(1);
	} else{
	printf("File was found\n");
	}

	/****************************************************
	Go through file to find number of rows and columns
	****************************************************/
	tempCharHolder = getc(file1);
	
	while(tempCharHolder != EOF){
		while(tempCharHolder != '\n' &&  row1< 1){
			if(tempCharHolder == ' '){
				column1++;
			}
			 tempCharHolder= getc(file1);
		}
		if(tempCharHolder == '\n'){
			row1++;
		}
		tempCharHolder = getc(file1);
	}
	column1++;
	row1++;

	printf("Rows: %i\n", row1);
	printf("Columns: %i\n", column1);


	/****************************************************
	Go through file and fill file1Array[][]
	****************************************************/
	file1Array[row1][column1];
	rewind(file1); // Resets file pointer
	for(int i = 0; i < column1; i++){
		for(int j = 0; j < row1; j++){
			fscanf(file1, "%d", &file1Array[j][i]); 
		}
	}
	//Print array 
	//*** Delete after testing ***********
	for(int i = 0; i < column1; i++){
		for(int j = 0; j < row1; j++){
			printf("%d, ", file1Array[j][i]); 
		}
		printf("\n");
	}




	/****************************************************
	Test to see if file2 opens
	****************************************************/
	printf("Please enter the second file name: ");
	scanf("%s", file2Name);
	file2=fopen(file2Name, "r");
	if(file2 == NULL){
		printf("File is not valid\n");
		exit(1);
	} else{
	printf("File was found\n");
	}
	

	/****************************************************
	Go through file to find number of rows and columns
	****************************************************/
	tempCharHolder = getc(file2);
	while(tempCharHolder != EOF){
		while(tempCharHolder != '\n' &&  row2< 1){
			if(tempCharHolder == ' '){
				column2++;
			}
			tempCharHolder = getc(file2);
		}
		if(tempCharHolder == '\n'){
			row2++;
		}
		tempCharHolder = getc(file2);
	}
	column2++;
	printf("Rows: %i\n", row2);
	printf("Columns: %i\n", column2);


	/****************************************************
	Go through file and fill file1Array[][]
	****************************************************/
	file2Array[row2][column2];
	rewind(file2); // Resets file pointer
	for(int i = 0; i < row2; i++){
		for(int j = 0; j < column2; j++){
			fscanf(file2, "%d", &file2Array[i][j]); 
		}
	}
	//Print array 
	//*** Delete after testing ***********
	for(int i = 0; i < column2; i++){
		for(int j = 0; j < row2; j++){
			printf("%d, ", file2Array[j][i]); 
		}
		printf("\n" );
	}


	/****************************************************
	Test array to see if it is compatable for multiplication
	****************************************************/
	if( column1 != row2){
		printf("These matricies are not compatable\n");
		printf("%d %d\n", column1, row2);
		exit(1);
	} else{
		resultsArray[row1][column2];
	}



	/****************************************************
	Multiply with threads
	****************************************************/ 
	for(int i = 0; i <row1; i++){
		for(int j = 0; j <column2; j++){ // ^^ These 2 lines traverse arrays
			// A way of passing the rows and columns (so we can fill array and not worry about order of threads)
			struct rowsAndColumns *data = (struct rowsAndColumns *) malloc(sizeof(struct rowsAndColumns));
			data->i=i;
			data->j=j;

			//Get thread ready
			pthread_t threadID;
			pthread_attr_t attributes;
			pthread_attr_init(&attributes);

			//Create thread
			pthread_create(&threadID, &attributes, multiply, data);
			pthread_join(threadID, NULL);
		}
	}
	

	/****************************************************
	Store array with answer
	****************************************************/
	FILE *outPutFile;
	char output[]="output.txt";
	outPutFile=fopen(output, "w");
	for(int i =0; i <row1; i++){
		for(int j =0; j<column2; j++){
			fprintf(outPutFile, "%d ", resultsArray[i][j]);
		}
		fprintf(outPutFile, "\n");
	}

	fclose(outPutFile);


 	/****************************************************
		Exit
	****************************************************/
	return 0;
}



	/****************************************************
		Function used in threading; Note that a struct
		is passed in because a thread funciton can
		only recieve one argument, but 2 need to be passed
	****************************************************/
void *multiply(void *passedStruct){
	struct rowsAndColumns *data = passedStruct; //Using a struct allows row1 and column2 to be passed
	int answer = 0;

	for (int i = 0; i < column1; ++i){
		answer += file1Array[data->i][i] * file2Array[i][data->j];
	}
	resultsArray[data->i][data->j] = answer; //Saving to array

}
	
