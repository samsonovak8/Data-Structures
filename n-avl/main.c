#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

typedef struct avl
{
    char* value;
    char* key;
    struct avl* right_ptr;
    struct avl* left_ptr;
    size_t height;
    size_t elem_number;

} avl;

avl* create_tree(char* key, char* value)
{
    avl* tree = (avl*) calloc(1, sizeof(avl));
    tree->key = key;
    tree->value= value;
    tree->height = tree->elem_number = 0;
    tree->left_ptr = tree->right_ptr = NULL;
    return tree;
}

void delete_tree(avl* tree, int elem_number)
{
    if (tree != NULL)
    {
        if(elem_number)
        {
            delete_tree(tree->left_ptr, elem_number);
            delete_tree(tree->right_ptr, elem_number);
        }

        if (tree->key != NULL)
            free(tree->key);
        if (tree->value != NULL)
            free(tree->value);

        free(tree);
    }
    return;
}

int height(avl* tree)
{
    size_t answer = 0;
    if (tree != NULL)
    {
        size_t left_height = 0;
        size_t right_height = 0;

        if (tree->left_ptr != NULL)
            left_height = tree->left_ptr->height;
        if(tree->right_ptr != NULL)
            right_height = tree->right_ptr->height;

        answer = left_height;
        if (answer < right_height)
            answer = right_height;

        ++answer;
    }
    return answer;
}

avl* left_rotation(avl* tree)
{
    if (tree == NULL)
        return NULL;

    avl* tmp = tree->right_ptr;
    tree->right_ptr = tmp ->left_ptr;
    tmp ->left_ptr = tree;
    tree->height = height(tree);
    tmp ->height = height(tmp);
    return tmp;
}

avl* right_rotation(avl* tree)
{
    if (tree == NULL)
        return NULL;

    avl* tmp = tree->left_ptr;
    tree->left_ptr = tmp ->right_ptr;
    tmp ->right_ptr = tree;
    tree->height = height(tree);
    tmp ->height = height(tmp);
    return tmp;
}

avl* getbalance_tree(avl* tree)
{
    if (tree == NULL)
        return NULL;

    int lchild_height = height(tree->left_ptr);
    int rchild_height = height(tree->right_ptr);

    if (lchild_height - rchild_height == 2)
    {
        int llchild_height = height(tree->left_ptr->left_ptr);
        int lrchild_height = height(tree->left_ptr->right_ptr);
        if (llchild_height - lrchild_height < 0)                            // the subtree of the left child is smaller than the subtree of the right child
                                                                            // need to balance with left rotation
        {
            tree -> left_ptr = left_rotation(tree->left_ptr);
        }
        
        return right_rotation(tree);
    }

    if (rchild_height - lchild_height == 2)
    {
        int rrchild_height = height(tree->right_ptr->right_ptr);
        int rlchild_height = height(tree->right_ptr->left_ptr);

        if (rrchild_height - rlchild_height < 0)
        {
            tree->right_ptr = right_rotation(tree->right_ptr);
        }
        return left_rotation(tree);
    }

    return tree;
}

int compare(char* s1, char* s2)
{
    return strcmp(s1, s2);
}

avl* insert_tree(avl* tree, char* key, char* value)
{
    int flag = compare(key, tree->key);
    if (!flag)
    {
        free(tree->value);
        free(key);
        tree->value = value;
    }
    else 
    {
        if (flag < 0)
        {
            if (tree->left_ptr != NULL)
                tree->left_ptr = insert_tree(tree->left_ptr, key, value);
            else 
                tree->left_ptr = create_tree(key, value);
        }
        else 
        {
            if (tree->right_ptr != NULL)
                tree->right_ptr = insert_tree(tree->right_ptr, key, value);
            else 
                tree->right_ptr = create_tree(key, value);
        }
    }
    tree->height = height(tree);
    tree->elem_number += 1;
    tree = getbalance_tree(tree);
    return tree;
}

avl* balance_tree(avl** tree)
{
    avl*   tree_copy1  = NULL;
    avl* tree_copy2 = NULL;

    tree_copy1 = *tree;

    if (tree_copy1->left_ptr->left_ptr == NULL)
    {   
        tree_copy2 = tree_copy1->left_ptr;             
        tree_copy1->left_ptr = tree_copy2->right_ptr;
    }
    else
        tree_copy2 = balance_tree(&tree_copy1->left_ptr);   

    tree_copy1->height = height(tree_copy1);
    (*tree)  = getbalance_tree(tree_copy1); 

    return tree_copy2;
}

avl* combine_child(avl* left_ptr, avl* right_ptr)
{
    if (left_ptr == NULL)
    {
        return right_ptr;
    }

    if (right_ptr == NULL)
    {
        return left_ptr;
    }

    if(right_ptr->left_ptr == NULL)
    {
        right_ptr->left_ptr = left_ptr;
        return right_ptr;
    }

    avl* new_tree = balance_tree(&right_ptr);
    new_tree->left_ptr = left_ptr;
    new_tree->right_ptr = right_ptr;
    new_tree->height = height(new_tree);
    new_tree = getbalance_tree(new_tree);
    return new_tree;
}

avl* erase_tree(avl* tree, char* key)
{
    if (tree == NULL)
        return NULL;

    int flag = compare(tree-> key, key);
    if (flag == 0)
    {
        avl* left_ptr = tree->left_ptr;
        avl* right_ptr = tree->right_ptr;
        delete_tree(tree, 0);
        return combine_child(left_ptr, right_ptr);
    }
    if (flag < 0)
    {
        if (tree->right_ptr != NULL)
            tree->right_ptr = erase_tree(tree->right_ptr, key);
    }
    if (flag > 0)
    {
        if (tree->left_ptr != NULL)
            tree->left_ptr = erase_tree(tree->left_ptr, key);
    }
    tree->height = height (tree);
    tree = getbalance_tree(tree);
    return tree;
}

avl* find_tree(avl* tree, char* key)
{
    if(tree == NULL || key == NULL)
        return NULL;

    while (tree != NULL)
    {
        int flag = compare(key, tree->key);
        if (!flag)
        {
            return tree;
        }
        if (flag > 0)
            tree = tree->right_ptr;
        if (flag < 0)
            tree = tree->left_ptr;
    }
    return NULL;
}

void free_all(avl* avl_tree, char* key, char* value)
{
    if (avl_tree != NULL)
        delete_tree(avl_tree, 1);

    if (key != NULL)
        free(key);
    
    if (value != NULL)
        free(value);

    return;
}

int main()
{
    char command_type;
    avl* avl_tree = NULL;

    while(1)
    {
        char* key = (char*) calloc(32, sizeof(char));
        char* value = (char*) calloc(32, sizeof(char));
        if(key == NULL || value == NULL)
        {
            free_all(avl_tree, key, value);
            return 1;
        }

        command_type = getchar();

		if (command_type == '!' || command_type == EOF)
		{
            free(key);
            free(value);
            break;
        }

        if(command_type == 'i')
        {
            scanf("nsert %s %s", key, value);
            if (avl_tree != NULL)  
                avl_tree = insert_tree(avl_tree, key, value); 
            else
                avl_tree = create_tree(key, value);
        }
        else if (command_type == 'e')
        {
            scanf("rase %s", key);
            avl_tree = erase_tree(avl_tree, key);
            free(key);
            free(value);
        }
        else if(command_type == 'f')
        {
            scanf("ind %s", key);
            avl* res = find_tree(avl_tree, key);
            if (res != NULL)
                printf("%s\n", res->value);
            else
                printf("MISSING\n");
            free(key);
            free(value);
        }
        else 
        {
            free(value);
            free(key);
        }
    }
    //printf("%ld\n", avl_tree->elem_number);
    delete_tree(avl_tree, 1);
    return 0;
}