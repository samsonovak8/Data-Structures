#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <immintrin.h>
#include <inttypes.h>

struct item
{
    char key[32];
    char value[32];
};

typedef struct hash_table
{
    struct item* data;
    int* used;
    size_t size;
} hash_table;

enum element_state {EMPTY, BUSY, DELETED, NOT_FOUND};

hash_table* create_table(int capacity)
{
    hash_table* table = (hash_table*) calloc(1, sizeof(hash_table));
    table->data  =	(struct item*) calloc(capacity, sizeof(struct item));
    table->used = (int*) calloc(capacity, sizeof(int));
    table->size  = capacity;
    return table;
}

hash_table* delete_table(hash_table* table)
{
	if (table != NULL)
	{
		free(table->used);
		free(table->data);
		free(table);
	}
    return NULL;
}

void free_all(hash_table** tables, char* key, char* value)
{
	if (tables != NULL)
	{
		for(size_t i = 0; i < 10; ++i)
			delete_table(tables[i]);

		free(tables);
	}
	
	if(key != NULL)
		free(key);

	if (value != NULL)
		free(value);

	return;
}

#pragma GCC target "avx2"
uint32_t hash(struct item* item, int size)
{
    uint32_t hash_value = _mm_crc32_u32(0, *(uint32_t*)item -> key);

	return hash_value % size;
}

int insert_table(hash_table* table, struct item* item)
{
	if (table == NULL)
		return EMPTY;

	size_t cnt = 0;
	unsigned start = hash(item, table->size);

    for (unsigned i = start; cnt < table->size; ++i)
    {
		++cnt;
		i %= table -> size;
        if (table->used[i] != BUSY)
		{
            table->used[i]  = BUSY;
            table->data[i]  = *item;
            return i;
        }
    }
    return NOT_FOUND;
}

int find_table(hash_table* table, struct item* item)
{
	if (table == NULL)
		return NOT_FOUND;

	size_t cnt = 0;
	unsigned start = hash(item, table->size);
	
    for (unsigned i = start; cnt < table->size; ++i) // (i = (i + 1) % table->size)
    {
		++cnt;
		i %= table -> size;
        if (table->used[i] == EMPTY)
			return NOT_FOUND;
        if (table->used[i] == BUSY)
		{
			int flag = strcmp(item -> key, (table -> data + i) -> key);
			if (!flag)
				return i;
		}
    }
    return NOT_FOUND;
}

int erase_table(hash_table* table, struct item* item)
{
	if (table == NULL)
		return 0;

    int index = find_table(table, item);

    if (index == NOT_FOUND)
		return 0;
        
    table->used[index] = DELETED;

    return 1;
}

int main()
{
    hash_table** tables = (hash_table**) calloc(10, sizeof(hash_table*));
	int  table_index;	
	if (tables == NULL)
		return 1;

	char command_type;

	while(1)
	{
		command_type = getchar();
		if (command_type == '!' || command_type == EOF)
			break;

		char* key = (char*) calloc(32, sizeof(char));
    	char* value = (char*) calloc(32, sizeof(char));
		struct item item;

		if (key == NULL || value == NULL)
		{
			free_all(tables, key, value);
			return 1;
		}

		if (command_type == 'c')
		{
			scanf("reate %d", &table_index);
			tables[table_index] = create_table(100000);
		}

		if (command_type == 'i')
		{
			scanf("nsert %d %s %s", &table_index, key, value);

            memcpy(item.key, key, 32);
            memcpy(item.value, value, 32);
            int res = find_table(tables[table_index], &item);
            if (res != NOT_FOUND)
				memcpy(tables[table_index]->data[res].value, value, 32);
            else
				insert_table(tables[table_index], &item);
		}

		if (command_type == 'e')
		{
			scanf("rase %d %s", &table_index, key);
            memcpy(item.key, key, 32);
			erase_table(tables[table_index], &item);
		}

		if(command_type == 'f')
		{
			scanf("ind %d %s", &table_index, key);
            memcpy(item.key, key, 32);
			int res = find_table(tables[table_index], &item);
			if (res == NOT_FOUND)
				printf("MISSING\n");
            else 
				printf("%s\n", tables[table_index]->data[res].value);
		}

		if (command_type == 'd')
		{
			scanf("estroy %d", &table_index);
			tables[table_index] = delete_table(tables[table_index]);
		}

        while (getchar() != '\n')
			continue;
    }

	for(size_t i = 0; i < 10; ++i)
		delete_table(tables[i]);
	free(tables);

    return 0;
}
