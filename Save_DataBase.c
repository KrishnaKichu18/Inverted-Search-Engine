/*******************************************************************************************************************************************************************
 * Function Name    : Save_DataBase
 *
 * Description      : Saves the current inverted search database into a formatted text file. The function supports
 *                    three user-selected save modes:
 *
 *                        1. Overwrite the existing save file.
 *                        2. Append new entries to the existing save file.
 *                        3. Save the database to a new, user-specified file.
 *
 *                    The database is serialized in the format:
 *
 *                        #index; word; file_count; filename1; count1; filename2; count2; ... #
 *
 *                    where:
 *                        • index      → Hash table index (0–26)
 *                        • word       → Unique word stored in MAIN_NODE
 *                        • file_count → Number of files containing the word
 *                        • filename   → File name from SUB_NODE
 *                        • count      → Occurrence count of the word in that file
 *
 * Prototype        : Status Save_DataBase( HASH_T *H_Table );
 *
 * Input Parameters : H_Table → Pointer to the 27-element hash table whose contents are to be saved.
 *
 * Return Value     : SUCCESS → Database saved or appended successfully.
 *                    FAILURE → If the file cannot be created or opened.
 *
 * Features         :
 *                    • Automatically handles file existence checks.
 *                    • Lets the user decide overwrite / append / save-as modes.
 *                    • Persists the complete multi-level linked list structure.
 *                    • Maintains formatting consistency for easier loading.
 *                    • Ensures empty databases do not write invalid or partial entries.
 *
 * Special Cases    :
 *                    • If the database is empty → Nothing is written; user is notified.
 *                    • If append mode is chosen → Duplicate data may appear in the save file.
 *                    • If a new filename is entered → A completely new save file is created.
 *
 * Algorithm        :
 *                    1. Check if the default save file exists.
 *                    2. Ask the user whether to overwrite, append, or save to a new file.
 *                    3. Open the file in the selected mode.
 *                    4. For each index in the hash table:
 *                         a. For each MAIN_NODE:
 *                               - Write index, word, and file_count.
 *                               - For each SUB_NODE:
 *                                     * Write filename and word_count.
 *                               - End record with " #\n".
 *                    5. If no data was present → Print info message.
 *                    6. Close the file and report success.
 *
 * Notes            :
 *                    • Insert_To_Hash_Table() is not used here since this function only serializes data.
 *                    • The output format is intentionally consistent to ensure clean parsing during loading.
 *                    • The user prompt helps avoid accidental overwrites of important saved data.
 *
 *******************************************************************************************************************************************************************/


#include "Types.h"
#include "Inverted_Search.h"

Status Save_DataBase( HASH_T* H_Table )
{
    char filename[256] = "Saved_DataBase.txt";
    FILE* check = fopen( filename, "r" );

    int append_mode = 0;

    // Ask user if they wish to overwrite or append or create new save file

    if( check )
    {
        fclose( check );
        
        int choice;
        printf("\n============================================================\n");
        printf(" 💾  Save file '%s' already exists.\n", filename);
        printf("============================================================\n");
        printf("  1️⃣  Overwrite existing file\n");
        printf("  2️⃣  Append new data( may contain duplicates)\n");
        printf("  3️⃣  Save as a new file\n");
        printf("------------------------------------------------------------\n");
        printf("  Enter your choice: ");

        scanf("%d", &choice );
        fflush( stdin );
    
        switch( choice )
        {
            case 2: 
                append_mode = 1;
                break;

            case 3:
                printf("\n[INFO]: Enter new filename: ");
                scanf( "%255s", filename );
                break;

            default:
                append_mode = 0; // Overwrite
        }
    }

    else
    {
        printf("\n[INFO]: Save file not found. Creating new '%s'\n", filename);
    }


    // Open file based on append mode
    FILE* fptr = fopen( filename, append_mode ? "a" : "w" );
    if( fptr == NULL )
    {
        perror("[INFO]: Could not open file to save database");
        return FAILURE;
    }

    //  Write data from database to save file
    int is_empty = 1;

    for( int i = 0; i < 27; i++ )
    {
        MAIN_NODE* main_node = H_Table[i].link;

        while( main_node )
        {
            is_empty = 0;
            fprintf(    fptr, "#%d; %s; %ld;",
                        i,
                        main_node -> word,
                        main_node -> file_count );

            SUB_NODE* sub_node = main_node -> Next_Sub_node;
            while( sub_node )
            {
                fprintf( fptr, " %s; %ld;", sub_node -> File_name, sub_node -> word_count );
                
                sub_node = sub_node -> link;
            }

            fprintf( fptr, " #\n" );
            main_node = main_node -> Next_Main_node;
        }
    }

    if( is_empty )
        printf("[INFO]: No DataBase data to save\n");

    fclose( fptr );

    printf("\n[INFO]: Database successfully %ssaved to '%s'\n",
           append_mode ? "appended and " : "",
           filename);

    return SUCCESS;
}