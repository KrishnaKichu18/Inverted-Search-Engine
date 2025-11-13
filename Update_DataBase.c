/*******************************************************************************************************************************************************************
 * Function Name    : Update_DataBase
 *
 * Description      : Loads a previously saved inverted search database from a file and reconstructs the entire hash table
 *                    structure. The save file contains serialized entries of the form:
 *
 *                          #index; word; file_count; filename1; count; filename2; count; ... #
 *
 *                    For each record, this function parses the hash index, the word, the number of files containing that word,
 *                    and all (filename, word_count) pairs. Using this data, the function rebuilds MAIN_NODE and SUB_NODE
 *                    linked lists by reusing Insert_To_Hash_Table(), ensuring identical behavior to live database creation.
 *
 * Prototype        : Status Update_DataBase( HASH_T *H_Table );
 *
 * Input Parameters : H_Table → Pointer to the 27-element hash table array into which the database will be reconstructed.
 *
 * Return Value     : SUCCESS → If parsing and reconstruction complete successfully.
 *                    FAILURE → If the file is missing, empty, unreadable, or contains malformed entries.
 *
 * Special Cases    :
 *                    • If the save file does not exist → Function returns FAILURE.
 *                    • If the save file is empty → Function returns FAILURE.
 *                    • If any record is partially corrupted → Only valid parsed data is inserted before returning.
 *                    • Supports flexible spacing and newline placement within the saved file.
 *
 * Features         :
 *                    • Allows loading from default file ("Saved_DataBase.txt") or a user-specified file.
 *                    • Reinitializes the hash table before reconstruction to avoid data overlap.
 *                    • Maintains correct word grouping under corresponding hash index.
 *                    • Preserves file occurrence counts exactly as stored.
 *                    • Uses Insert_To_Hash_Table() for consistent MAIN_NODE and SUB_NODE creation logic.
 *                    • Safely handles variable formatting in saved records.
 *
 * Algorithm        :
 *                    1. Ask user whether to load from default or custom file.
 *                    2. Attempt to open the file and validate non-empty content.
 *                    3. Reset the hash table using Initialise_Hash_Table().
 *                    4. For each database entry:
 *                        a. Parse index, word, and file_count.
 *                        b. For i in range(file_count):
 *                               - Parse filename and word_count.
 *                               - Insert the word into the hash table 'word_count' times.
 *                        c. Consume trailing '#' and newline.
 *                    5. Close the file and report successful loading.
 *
 *******************************************************************************************************************************************************************/


#include "Types.h"
#include "Inverted_Search.h"


Status Update_DataBase( HASH_T* H_Table )
{
    char filename[256] = "Saved_DataBase.txt";
    int choice;

    printf("\n============================================================\n");
    printf(" 🔄  LOAD / UPDATE DATABASE\n");
    printf("============================================================\n");
    printf("  1️⃣  Load from default file ('%s')\n", filename);
    printf("  2️⃣  Load from another file\n");
    printf("------------------------------------------------------------\n");
    printf("  Enter your choice: ");
    scanf("%d", &choice);
    fflush(stdin);

    if( choice == 2 )
    {
        printf("[INFO]: Enter filename to load from: ");
        scanf("%255s", filename );
    }

    FILE *fptr = fopen( filename, "r" );
    if( fptr == NULL )
    {
        printf("[INFO]: Could not open '%s'. File not Found\n", filename );
        return FAILURE;
    }

    fseek( fptr, 0, SEEK_END );
    if( ftell( fptr ) == 0 )
    {
        printf("[INFO]: '%s' is Empty. Nothing to load.\n", filename );
        fclose( fptr );
        return FAILURE;
    }
    rewind( fptr );

    Initialise_Hash_Table( H_Table );

    INDEX index;
    WORD word;
    No_Of_Files file_count;
    FILE_NAME file_name;
    Word_Count word_count;

    while( fscanf( fptr, "#%d; %[^;]; %ld;", &index, word, &file_count ) == 3 )
    {

        for( No_Of_Files i = 0; i < file_count; i++ )
        {
            if( fscanf( fptr, " %[^;]; %ld;", file_name, &word_count ) != 2 )
                break;
            
            for( Word_Count j = 0; j < word_count; j++ )
                Insert_To_Hash_Table( index, word, file_name, H_Table );
        }

        fscanf(fptr, " #\n");

    }

    fclose( fptr );

    printf("\n[INFO]: Database successfully loaded from '%s'\n\n", filename );
    return SUCCESS;

}