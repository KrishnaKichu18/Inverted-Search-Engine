/*******************************************************************************************************************************************************************
 * File        : Create_and_Insert.c
 * Project     : Inverted Search Engine (Project-2)
 * Developer   : Krishna M
 * Date        : 13/11/2025
 *
 * Description :
 *      Core logic for building and maintaining the inverted index. This module:
 *        • Creates hash table and initializes all bucket heads
 *        • Reads tokens from validated files and inserts them into the index
 *        • Creates and links MAIN_NODE and SUB_NODE dynamically as needed
 *        • Prevents duplicate file-indexing to avoid double counting
 *
 * Function Overview :
 *
 *      → Create_DataBase( HASH_T *Hash_T, LIST **head )
 *            • Scans each validated file from the file list
 *            • Tokenizes every word and inserts into hash table using hashing
 *            • Skips files that are already indexed earlier
 *
 *      → Initialise_Hash_Table( HASH_T *Hash_T )
 *            • Sets index value and resets link pointer for all 27 buckets
 *
 *      → Find_Index( char chr )
 *            • Maps first character of word into bucket index
 *            • 'a'–'z' → 0–25
 *            • Others → 26
 *
 *      → Create_Main_Node( char* word, char* filename )
 *            • Allocates + initializes a new MAIN_NODE
 *            • Automatically creates its first SUB_NODE entry
 *
 *      → Create_Sub_Node( char* filename )
 *            • Allocates + initializes a new SUB_NODE entry for filename
 *
 *      → Insert_To_Hash_Table( int index, char* word, char* filename, HASH_T *Hash_T )
 *            • Handles all insertion cases:
 *                 1. Brand-new word → new MAIN_NODE
 *                 2. Word exists in DB → update existing structure
 *                 3. File has word already → just increment count
 *                 4. File is new for this word → attach new SUB_NODE
 *
 *      → File_Already_Indexed( const char *fname, HASH_T *Hash_T )
 *            • Prevents duplicate re-indexing of already processed files
 *
 * Data Structure :
 *      HASH_T
 *         ↳ MAIN_NODE (unique words)
 *               ↳ SUB_NODE (filename, count)
 *
 * Return Behavior :
 *      • All creation/insert functions → SUCCESS on proper insertion
 *      • Memory allocation failures → FAILURE
 *      • Duplicate indexing detection → EXISTS / NOT_EXISTS
 *
 * Notes :
 *      • fscanf() tokenization uses whitespace as separator by default
 *      • Word matching is case-sensitive (storage-exact)
 *      • fptr must already be open when passed to Create_DataBase()
 *      • Hash insertion always maintains forward traversal order
 *
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

		// After update, check if any file is already indexed to ensure no duplicate it found
		if ( File_Already_Indexed( Ltemp -> FILENAME, Hash_T ) == EXISTS )
        {
            printf("[INFO]: '%s' already present in database. Skipping...\n",
                   Ltemp -> FILENAME );

            Ltemp = Ltemp -> link;
            continue;
        }


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


/**/
Status File_Already_Indexed (const char *fname, HASH_T *Hash_T )
{
    for ( int i = 0; i < 27; i++ )
    {
        MAIN_NODE *main = Hash_T[i].link;

        while ( main )
        {
            SUB_NODE *sub = main -> Next_Sub_node;

            while ( sub )
            {
                if (strcmp(sub -> File_name, fname ) == 0 )
                    return EXISTS;  // Exists in DB

                sub = sub -> link;
            }
            
			main = main -> Next_Main_node;
        }
    }
    return NOT_EXISTS;  // Not found
}