#ifndef INVERTEDSEARCH_H
#define INVERTEDSEARCH_H


#include "Types.h"
#include <ctype.h>

INDEX Find_Index( char chr );

SUB_NODE* Create_Sub_Node( char* filename );

void Initialise_Hash_Table( HASH_T *Hash_T );

Status Create_DataBase( HASH_T *Hash_T, LIST **head );

MAIN_NODE* Create_Main_Node( char* word, char* filename );

Status Insert_To_Hash_Table( int index, char* word, char* filename, HASH_T *Hash_T );

DISPLAY Display_DataBase( HASH_T* H_Table );

Status Search_DataBase( HASH_T* H_Table, char* word );

void Display_Menu();

Status Print_List( LIST *head );

Status Add_To_List( LIST **head, char *filename, FILE *fptr );

Status Save_DataBase( HASH_T* H_Table );

Status  Update_DataBase( HASH_T* H_Table, LIST **head );

Status File_Already_Indexed (const char *fname, HASH_T *Hash_T );


#endif