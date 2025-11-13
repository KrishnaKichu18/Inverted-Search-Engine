/*******************************************************************************************************************************************************************
 * Title        : Krishna_M_M_Project2_Inverted_Search_25017E_031
 * Project      : Project 2 – Inverted Search Engine
 * Developer    : Krishna M
 * Date         : 13/11/2025
 *
 * Description  : Main driver program for the Inverted Search Engine. This module acts as the controller for the entire project.
 *                It validates input files, constructs the inverted index, and provides a menu-driven interface for operations such as:
 *
 *                  • Creating the database from multiple text files
 *                  • Displaying the index in a compact tabular form
 *                  • Searching for any word across all indexed files
 *                  • Saving the database to a persistent file
 *                  • Loading (updating) the database from a saved file
 *                  • Closing all resources and exiting cleanly
 *
 *                Each word is stored in a hash table containing 27 lists:
 *                  - 0 to 25 → 'a' to 'z'
 *                  - 26      → digits and special characters
 *
 *                The data structure hierarchy is:
 *                  HASH_T (27 elements)
 *                      ↳ MAIN_NODE (unique words)
 *                           ↳ SUB_NODE (file occurrences of each word)
 *
 *                MAIN_NODE stores:
 *                     - The word
 *                     - Number of files containing that word
 *                     - Linked list of SUB_NODE entries
 *
 *                SUB_NODE stores:
 *                     - File name where the word appears
 *                     - Occurrence count in that file
 *
 * Features:
 *                • Handles multiple files with automatic validation
 *                • Tracks word frequency per file
 *                • Fast lookup using hashing (O(1) average case)
 *                • Can save/restore the complete database between runs
 *                • Clean, readable tabular output using formatted printing
 *                • Graceful cleanup with full file-pointer shutdown
 *
 * Key Concepts Demonstrated:
 *                - Hash table implementation with separate chaining
 *                - Multi-level linked list design (main + sublist)
 *                - File handling, validation, and safe parsing
 *                - Modular C programming with proper separation of concerns
 *                - Data persistence using structured save/load format
 *                - Interactive menu-driven application design
 *
 * Workflow Summary:
 *                1. Validate input files
 *                2. Build inverted index (Create Database)
 *                3. Display / Search / Save / Load
 *                4. Clean shutdown (close all file pointers)
 *
 * Save File Format (per entry):
 *                #index; word; file_count; filename1; count; filename2; count; #
 *
 * Example:
 *                #5; file; 1; README.txt; 1; #
 *                #7; header; 1; README.txt; 1; #
 *
 * Usage Example:
 *                ./Inverted file1.txt file2.txt notes.txt
 *
 * Output Example (Search):
 *                🔍 Word: file              | Found in 2 files
 *                -----------------------------------------------
 *                 [01] report.txt            → 3 occurrences
 *                 [02] README.txt            → 1 occurrence
 *
 *******************************************************************************************************************************************************************/


#include "Inverted_Search.h"
#include "Types.h"
#include "Validate.h"


int main(int argc, char *argv[])
{

	LIST *head;
	HASH_T H_Table[27];
	int Created_DataBase = 0;

	Initialise_Hash_Table( H_Table );

	if( Read_and_Validate( argc, argv, &head ) == SUCCESS )
	{
		printf("\n[INFO]: Files in the List are : ");
		Print_List( head );
	}


	while(1)
	{
		Display_Menu();
		printf("\n");

		int choice;
		printf("  Enter your option: ");
		scanf("%d", &choice );

		switch ( choice )
		{
			case 1:
				if( Created_DataBase != 0 )
				{
					printf("\n[INFO]: Database already exists\n");
					break;
				}

				Create_DataBase( H_Table, &head );
				printf("\n[INFO]: DataBase Creation Successful\n");
				Created_DataBase = 1;

				break;

			case 2:
				Display_DataBase( H_Table );
				break;

			case 3:
				char word[50];
				printf("\n[INFO]: Enter the Word you wish to search: ");
				scanf("%s", word);

				Search_DataBase( H_Table, word );
				break;

			case 4:
				if( Created_DataBase == 0 )
				{
					printf("\n[INFO]: No DataBase to Save. Create One First\n");
					break;
				}

				Save_DataBase( H_Table );
				break;

			case 5:
				if( Created_DataBase == 1 )
				{
					printf("\n[INFO]: DataBase already created\n");
					break;
				}
				
				if(	Update_DataBase( H_Table ) == SUCCESS )
					Created_DataBase = 1;
				
				break;

			case 6:
				{
					LIST *temp = head;
					while ( temp != NULL) 
					{
						if ( temp -> fptr != NULL)
							fclose( temp-> fptr );
						temp = temp -> link;
					}

					printf("\n[INFO]: All files closed successfully.\n");
					printf("[INFO]: Exiting Inverted Search. Goodbye!\n");
					exit(0);
				}
				
			default:
				printf("\n[INFO]: Invalid Option\n");
		}
	}
}