/*******************************************************************************************************************************************************************
 * File        : Display_and_Search.c
 * Project     : Inverted Search Engine (Project-2)
 * Developer   : Krishna M
 * Date        : 13/11/2025
 *
 * Summary     :
 *      Handles all user-facing output for the Inverted Search project. This includes:
 *          • Rendering the full database in a clean tabular format
 *          • Searching for a single word and showing all file occurrences
 *          • Printing the interactive menu used by the main driver
 *
 * Function Overview:
 *      → Display_DataBase()
 *              Prints all 27 hash buckets in a condensed table layout.
 *              Words appear once, followed by aligned file/occurrence rows.
 *
 *      → Search_DataBase()
 *              Looks up a word by its hash index and prints every file in which it occurs.
 *              Also reports total file count and occurrence count per file.
 *
 *      → Display_Menu()
 *              Shows the main program options with neat formatting.
 *
 * Notes:
 *      • Optimized for readability; avoids repetitive word/index printing.
 *      • Works directly on the in-memory hash table created or loaded earlier.
 *      • All emoji and formatting are intentional, not bugs.
 *******************************************************************************************************************************************************************/


#include "Inverted_Search.h"
#include "Types.h"


DISPLAY Display_DataBase( HASH_T* H_Table )
{
	printf("\n======================================================================\n");
    printf(" 📊  INVERTED SEARCH DATABASE\n");
    printf("======================================================================\n");
    printf("| %-3s | %-15s | %-8s | %-20s | %-8s |\n", 
           "Idx", "Word", "Files", "File Name", "Count");
    printf("|-----|-----------------|----------|----------------------|----------|\n");


	int is_empty = 1;
	for( int i = 0; i < 27; i++ )
	{
		if( H_Table[i].link != NULL )
		{
			MAIN_NODE* main_node = H_Table[i].link;

			while( main_node != NULL )
			{

				is_empty = 0;

				SUB_NODE* sub_node = main_node -> Next_Sub_node;

				// print first line with full info
				if (sub_node != NULL)
				{
					printf("| %-3d | %-15s | %-8ld | %-20s | %-8ld |\n",
						i,
						main_node->word,
						main_node->file_count,
						sub_node->File_name,
						sub_node->word_count);

					sub_node = sub_node->link;
				}

				while( sub_node != NULL )
				{
					printf("| %-3s | %-15s | %-8s | %-20s | %-8ld |\n",
                       "",
                       "",
                       "",
                       sub_node->File_name,
                       sub_node->word_count);

					sub_node = sub_node -> link;
		
				}

				main_node = main_node -> Next_Main_node;
				printf("\n");
			}
		}
	}

	if( is_empty )
		printf("| %-68s |\n", "[INFO]: Database is empty. Nothing to display.");

    printf("|-----|-----------------|----------|----------------------|----------|\n");
    printf("======================================================================\n\n");

}


Status Search_DataBase( HASH_T* H_Table, char* word )
{
	int index = Find_Index( word[0] );

	MAIN_NODE* main_node = H_Table[index].link;

	while( main_node != NULL )
	{
		if( strcmp( main_node -> word, word ) == 0 )
		{
			printf("\n============================================================\n");
			printf(" 🔍  Word: %-20s | Found in %ld file%s\n",
						 main_node -> word,
						 main_node -> file_count,
						(main_node -> file_count > 1 ? "s" : ""));
			printf("------------------------------------------------------------\n");
		
			SUB_NODE* sub_node = main_node -> Next_Sub_node;
			No_Of_Files file_no = 1;

			while( sub_node != NULL )
			{
				 printf(" [%02ld] %-25s → %3ld occurrence%s\n",
                       file_no++,
                       sub_node->File_name,
                       sub_node->word_count,
                       (sub_node->word_count > 1 ? "s" : ""));

                sub_node = sub_node->link;
			}

			printf("============================================================\n\n");
			printf("[INFO]: Search Successful\n");
            return SUCCESS;
		}
		
		main_node = main_node -> Next_Main_node;
	}

	printf("\n[INFO]: Word '%s' not found in the database.\n", word);
    return FAILURE;

}


/**/
void Display_Menu()
{
    printf("\n============================================================\n");
    printf("\t     🔍  INVERTED SEARCH - MAIN MENU\n");
    printf("============================================================\n\n");

    printf("  1️⃣  Create Database\n");
    printf("  2️⃣  Display Database\n");
    printf("  3️⃣  Search Database\n");
    printf("  4️⃣  Save Database\n");
    printf("  5️⃣  Update Database\n");
    printf("  6️⃣  Exit\n");

	printf("\n------------------------------------------------------------\n");

}