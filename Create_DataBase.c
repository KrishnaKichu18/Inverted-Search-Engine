/*******************************************************************************************************************************************************************
 * File        : Display_and_Search.c
 * Project     : Inverted Search Engine (Project-2)
 * Developer   : Krishna M
 * Date        : 13/11/2025
 *
 * Summary     :
 *      Handles all user-visible output and lookup operations for the inverted index.
 *      This module prints the database in a compact tabular layout and performs fast,
 *      index-based word searches using the pre-built hash table.
 *
 * Function Overview:
 *      → Display_DataBase()
 *            Prints all words and their file-occurrence details in a clean table.
 *            Blank cells are used for continued SUB_NODE entries to keep rows aligned.
 *
 *      → Search_DataBase()
 *            Looks up a word using its bucket index, scans the MAIN_NODE chain,
 *            and prints occurrence counts and file details if found.
 *
 *      → Display_Menu()
 *            Shows the main interactive menu for the entire project.
 *
 * Notes:
 *      • Display and search use only the hash table; no file I/O here.
 *      • Designed for readability — compact rows, clean continuation lines.
 *      • Safe against empty buckets and empty database conditions.
 *******************************************************************************************************************************************************************/


#include "Inverted_Search.h"
#include "Types.h"
#include "Validate.h"


Status Create_DataBase( HASH_T *Hash_T, LIST **head )
{

	if( head == NULL )
	{
		printf("[INFO]: No File to add to the List.\n");
		return FAILURE;
	}

	LIST *Ltemp = *head;

	while( Ltemp != NULL )
	{
		char str[MAX_WORD_LENGTH];

		while( fscanf( Ltemp -> fptr, "%s", str ) != EOF )
		{
			int index = Find_Index( str[0] );
			Insert_To_Hash_Table( index, str, Ltemp -> FILENAME, Hash_T );
		}

		Ltemp = Ltemp -> link;
	}

	return SUCCESS;
}


/**/
void Initialise_Hash_Table( HASH_T *Hash_T )
{
	for( int i = 0; i < 27; i++ )
	{
		Hash_T[i].index = i;
		Hash_T[i].link = NULL;
	}
}


/**/
INDEX Find_Index( char chr )
{
	chr = tolower( chr );
	if( chr >= 'a' && chr <= 'z' )
		return chr % 97;
	else
		return 26;
}


/**/
MAIN_NODE* Create_Main_Node( char* word, char* filename )
{
	MAIN_NODE* New_main = malloc( sizeof( MAIN_NODE ) );
	if( New_main == NULL )
	{
		perror("Malloc failed for MAIN_NODE");
		return NULL;
	}

	strcpy( New_main -> word, word );
	New_main -> file_count = 1;
	New_main -> Next_Main_node = NULL;

	SUB_NODE* First_sub = Create_Sub_Node( filename );
	if( First_sub == NULL )
	{
		free( New_main );
		return NULL;
	}

	New_main -> Next_Sub_node = First_sub;

	return New_main;
	
}


/**/
SUB_NODE* Create_Sub_Node( char* filename )
{
	SUB_NODE* new_sub = malloc( sizeof( SUB_NODE ) );
	if( new_sub == NULL )
	{
		perror("Malloc failed for SUB_NODE");
		return NULL;
	}

	strcpy( new_sub -> File_name, filename );
	new_sub -> link = NULL;
	new_sub -> word_count = 1;

	return new_sub;

}


/**/
Status Insert_To_Hash_Table( int index, char* word, char* filename, HASH_T *Hash_T )
{

	MAIN_NODE *main_temp = Hash_T[index].link;
	MAIN_NODE *Prev_main = NULL;

	// Case 1: Search if word exists
	while( main_temp != NULL )
	{
		if( strcmp( main_temp -> word, word ) == 0 )
			break;

		Prev_main = main_temp;
		main_temp = main_temp -> Next_Main_node;
	}

	// Case 2: Word not found, create a new main node
	if( main_temp == NULL )
	{
		MAIN_NODE *new_main = Create_Main_Node( word, filename );
		if( new_main == NULL )
		{
			return FAILURE;
		}

		if( Prev_main == NULL )
			Hash_T[index].link = new_main;
		else
			Prev_main -> Next_Main_node = new_main;

		return SUCCESS;

	}

	// Case 3: Word exists - check if file already has the word
	SUB_NODE *Sub_temp = main_temp -> Next_Sub_node;
	SUB_NODE *Prev_sub = NULL;

	while( Sub_temp != NULL )
	{
		if( strcmp( Sub_temp -> File_name, filename ) == 0 )
		{
			Sub_temp -> word_count++;

			return SUCCESS;
		}


		Prev_sub = Sub_temp;
		Sub_temp = Sub_temp -> link;
	}

	// Case 4: Word Exists - file not found with this word
	SUB_NODE *New_sub = Create_Sub_Node( filename );
	if( New_sub == NULL )
		return FAILURE;

	if( Prev_sub == NULL )
		main_temp -> Next_Sub_node = New_sub;
	else
		Prev_sub -> link = New_sub;

	main_temp -> file_count++;

	return SUCCESS;
	
}
