#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int	id;
	char name;
	int	order;
	struct RecordType* next;
};

// Fill out this structure
struct HashType {
	struct RecordType* head;
};

// Compute the hash function
int hash(int x) {
	//Folding Hash Function
	int sum = 0;
	while (x) {
		sum += x % 10;
		x /= 10;
	}
	return sum;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType* pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
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
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
	int i;
	for (i = 0; i < hashSz; ++i)
	{
		if ((pHashArray + i)->head != NULL) {
			printf("Index %d -> %d, %c, %d ", i, (pHashArray + i)->head->id, (pHashArray + i)->head->name, (pHashArray + i)->head->order);
			struct RecordType* record = (pHashArray + i)->head;
			while (record->next != NULL) {
				printf("-> %d, %c, %d ", record->next->id, record->next->name, record->next->order);
				record = record->next;
			}
			puts("");
		}

	}
}

void insertHash(struct HashType* ptrHT, struct RecordType* ptrRT);
struct HashType* createHashTable();

int main(void)
{
	struct RecordType* pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType* hashTable = NULL;
	hashTable = createHashTable();

	int hashSz = 50;

	for (int i = 0; i < recordSz; i++)
		insertHash(hashTable, (pRecords + i));

	displayRecordsInHash(hashTable, hashSz);
}

struct HashType* createHashTable() {
	struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * 50);

	for (int i = 0; i < 50; i++)
		(hashTable + i)->head = NULL;


	return hashTable;
}

void insertHash(struct HashType* ptrHT, struct RecordType* ptrRT) {
	int index = hash(ptrRT->id);

	if ((ptrHT + index)->head == NULL) {
		(ptrHT + index)->head = ptrRT;
		(ptrHT + index)->head->next = NULL;
		return;
	}

	struct RecordType* current = (ptrHT + index)->head;
	while (current->next != NULL) {
		current = current->next;
	}

	current->next = ptrRT;
	current->next->next = NULL;
}