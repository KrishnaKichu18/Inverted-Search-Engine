/*******************************************************************************************************************************************************************
 * Title        : Krishna_M_M_Project2_Inverted_Search_25017E_031
 * Project      : Project 2 – Inverted Search Engine
 * Developer    : Krishna M
 * Date         : 13/11/2025
 *
 * Description  :
 *      Main driver program for the Inverted Search Engine project. This module:
 *          • Validates input text files and stores them into a file list
 *          • Initializes hash table structure used for building the inverted index
 *          • Provides a menu-driven interface to perform core operations:
 *
 *              1. Create Database (build inverted index)
 *              2. Display Database (tabular format)
 *              3. Search for a word across indexed files
 *              4. Save the database to storage
 *              5. Load/Update the database from existing file
 *              6. Exit cleanly and close all open file pointers
 *
 * Data Structure Layout:
 *      HASH_T H_Table[27]  → Hash buckets
 *          0–25 represent 'a' to 'z'
 *          26 stores numbers, symbols, or others
 *
 *      MAIN_NODE per unique word:
 *          • Stores word string
 *          • Tracks number of files containing the word
 *          • Points to SUB_NODE list
 *
 *      SUB_NODE list per file containing the word:
 *          • Stores filename
 *          • Stores occurrence count in that file
 *
 * Save File Format:
 *      #index; word; file_count; filename1; count1; filename2; count2; #
 *
 * Example:
 *      #5; file; 1; report.txt; 3; #
 *
 * Program Flow Summary:
 *      1. Collect and validate filenames from command line
 *      2. Create inverted index on request (menu)
 *      3. Perform display/search/save/update operations interactively
 *      4. Graceful shutdown with complete file closure
 *
 * Features:
 *      • Fast average lookup using hashing (O(1))
 *      • Dynamic multi-file support with frequency tracking
 *      • Human-friendly UI using formatted printing
 *      • Persistent storage support via Save/Load
 *
 * Special Notes:
 *      • Database creation allowed only once until update operation resets flag
 *      • Update loads database from save file only when fresh database not already created
 *      • Every exit path ensures open file pointers are properly closed
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
	int Updated_DataBase = 0;


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
				Updated_DataBase = 1;

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
				if( Updated_DataBase == 0 && Created_DataBase == 0 )
				{
					printf("\n[INFO]: No DataBase to Save. Create One First\n");
					break;
				}

				Save_DataBase( H_Table );
				break;

			case 5:
				if( Created_DataBase == 1 || Updated_DataBase == 1 )
				{
					printf("\n[INFO]: Update DataBase already done\n");
					break;
				}
				
				if(	Update_DataBase( H_Table, &head ) == SUCCESS )
				{
					Updated_DataBase = 1;
					Created_DataBase = 0;
				}

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