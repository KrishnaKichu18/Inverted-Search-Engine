#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILENAME_MAX 4096
#define MAX_WORD_LENGTH 100

typedef enum{
    FAILURE,
    SUCCESS,
    FILENOTFOUND,
    DUPLICATE,
    EMPTY,
    NOT_EXISTS,
    EXISTS

} Status;

typedef char WORD[MAX_WORD_LENGTH];
typedef int INDEX;
typedef void DISPLAY;
typedef char FILE_NAME[FILENAME_MAX];
typedef long int No_Of_Files;
typedef long int Word_Count;


typedef struct node{
    char FILENAME[FILENAME_MAX];
    FILE *fptr;
    struct node *link;

} LIST;


typedef struct Sub_Node{
    FILE_NAME File_name;
    Word_Count word_count;
    struct Sub_Node *link;

} SUB_NODE;


typedef struct Main_Node{
    WORD word;
    No_Of_Files file_count;
    struct Sub_Node *Next_Sub_node;
    struct Main_Node *Next_Main_node;

} MAIN_NODE;


typedef struct Hash_Table
{
    int index;
    struct Main_Node *link;

} HASH_T;


#endif