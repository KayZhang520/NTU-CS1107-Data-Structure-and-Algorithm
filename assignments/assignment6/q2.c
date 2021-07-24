#include <stdio.h>
#include <stdlib.h>

#define TABLESIZE 7
#define PRIME     3

enum Marker {EMPTY,USED,DELETED};

typedef struct _slot{
    int key;
    enum Marker indicator;
} HashSlot;

int HashInsert(int key, HashSlot hashTable[]);
int HashDelete(int key, HashSlot hashTable[]);


int hash1(int key);
int hash2(int key);

int main()
{
    int opt;
    int i;
    int key;
    int comparison;
    HashSlot hashTable[TABLESIZE];

    for(i=0;i<TABLESIZE;i++){
        hashTable[i].indicator = EMPTY;
        hashTable[i].key = 0;
    }

    printf("============= Hash Table ============\n");
    printf("|1. Insert a key to the hash table  |\n");
    printf("|2. Delete a key from the hash table|\n");
    printf("|3. Print the hash table            |\n");
    printf("|4. Quit                            |\n");
    printf("=====================================\n");
    printf("Enter selection: ");
    scanf("%d",&opt);
    while(opt>=1 && opt <=3){
        switch(opt){
        case 1:
            printf("Enter a key to be inserted:\n");
            scanf("%d",&key);
            comparison = HashInsert(key,hashTable);
            if(comparison <  0)
                printf("Duplicate key\n");
            else if(comparison < TABLESIZE)
                printf("Insert: %d Key Comparisons: %d\n",key, comparison);
            else
                printf("Key Comparisons: %d. Table is full.\n",comparison);
            break;
        case 2:
            printf("Enter a key to be deleted:\n");
            scanf("%d",&key);
            comparison = HashDelete(key,hashTable);
            if(comparison <0)
                printf("%d does not exist.\n", key);
            else if(comparison <= TABLESIZE)
                printf("Delete: %d Key Comparisons: %d\n",key, comparison);
            else
                printf("Error\n");//happens when comparison > tablesize
            break;
        case 3:
            for(i=0;i<TABLESIZE;i++) printf("%d: %d %c\n",i, hashTable[i].key,hashTable[i].indicator==DELETED?'*':' ');
            break;
        }
        printf("Enter selection: ");
        scanf("%d",&opt);
    }
    return 0;
}

int hash1(int key)
{
    return (key % TABLESIZE);
}

int hash2(int key)
{
    return (key % PRIME) + 1;
}

int HashInsert(int key, HashSlot hashTable[])
{
   //Write your code here
   int hcode, comparisons = 0, ghostindex = -1, j;
   for(j = 0; j < TABLESIZE; j++){//loop for exactly 37 times, from i=0 to i=36
		hcode = hash1(key + j*hash2(key));
		if(hashTable[hcode].indicator == EMPTY){//if encounter empty slot insert into the slot
			hashTable[hcode].indicator = USED;
			hashTable[hcode].key = key;
			return comparisons;
		}
		else if(hashTable[hcode].indicator == DELETED&&ghostindex == -1){//if encounter deleted item, save it and continue first
			ghostindex = hcode;
		}
		else if(hashTable[hcode].indicator == USED){//if slot is "USED"
			if(hashTable[hcode].key == key)return -1;//if encounter duplicate key terminate
			comparisons++;//keeps track of how many comparisons are done
		}
   }
   if(ghostindex != -1){
		hashTable[ghostindex].key = key;
		hashTable[ghostindex].indicator = USED;
		return comparisons;
   }
   return TABLESIZE;
}

int HashDelete(int key, HashSlot hashTable[])
{
   //Write your code here
   int i = 0, comparisons = 0;
   HashSlot *ptr;

   while(i<TABLESIZE){//if the table is full, there is no need to do all the work
		ptr = &hashTable[hash1(key + i*hash2(key))];
		if(ptr->indicator == EMPTY)
			return -1;//first hslot being empty means key is definitely not in
		else if(ptr->indicator == USED){
			comparisons++;
			if(ptr->key == key){//compare key with slot's key
				ptr->indicator = DELETED;
				return comparisons;
			}
		}
		i++;
   }
   return -1;//key is not in all 36 hslots
}

