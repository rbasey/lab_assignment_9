#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	//add pointer
	struct RecordType *next;
};

// Fill out this structure
struct HashType
{
	struct RecordType **arr;
	int num_elements;
	int tableSize;

};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	printf("Hash Table:\n");
	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		//skip if there are no records in the row
		if(pHashArray->arr[i] == NULL) {
			continue;
		} else {
			//prints the linked list of records or single record in the row
			printf("Row %d: ", i+1);
			struct RecordType *temp = pHashArray->arr[i];
			while(temp != NULL) {
				printf("%d %c %d; ", temp->id, temp->name, temp->order);
				temp = temp->next;
			}
		}
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	//initalize the hashtable
	struct HashType* table = malloc(sizeof(struct HashType));
    table->num_elements = recordSz;
    table->tableSize = 23;
    table->arr = malloc(sizeof(struct RecordType*) * table->tableSize);
    for(int i = 0; i < table->tableSize; i++) {
        table->arr[i] = NULL;
    }
	//insert records into hashtable
    for(int i = 0; i < recordSz; i++) {
        int bucketIndex = hash(pRecords[i].id);
        
        if (table->arr[bucketIndex] == NULL) {
            table->arr[bucketIndex] = &(pRecords[i]);
        }
        else {
            pRecords[i].next = table->arr[bucketIndex];
            table->arr[bucketIndex] = &(pRecords[i]);
        }
    }
	//display the records in the hash table
    displayRecordsInHash(table, table->tableSize);
	//free the records in the table
	for(int i = 0; i < table->tableSize; i++){
	if(table->arr[i] == NULL) {
			continue;
		} else {
			struct RecordType *curr = table->arr[i];
			struct RecordType *prev = NULL;
			while(curr != NULL) {
				prev = curr->next;
				free(curr);
				curr = prev;
			}
		}
	}
	//free table
	free(table);
}