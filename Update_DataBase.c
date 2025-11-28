/*******************************************************************************************************************************************************************
 * Function Name    : Update_DataBase
 *
 * Description      :
 *      Loads a previously saved inverted search database from a serialized text file and rebuilds
 *      the in-memory hash table structure. File format expected:
 *
 *          #index; word; file_count; filename1; count1; filename2; count2; ... #
 *
 *      For each entry, the function restores all MAIN_NODE and SUB_NODE relationships by inserting
 *      each occurrence into the hash table using Insert_To_Hash_Table().
 *
 * Prototype        : Status Update_DataBase( HASH_T *H_Table, LIST **head );
 *
 * Input Parameters :
 *      H_Table  → Pointer to the hash table to be reinitialized and reconstructed
 *      head     → Currently not used for reconstruction (maintained for future expansion)
 *
 * Return Value     :
 *      SUCCESS → If the file exists, has content, and at least partial data is parsed
 *      FAILURE → If the file is missing or completely empty
 *
 * Special Notes    :
 *      • Hash table is always reset using Initialise_Hash_Table() before loading.
 *      • Duplicate prevention is not required because table is fresh on every load.
 *      • Partial or malformed lines are ignored without stopping overall reconstruction.
 *      • Function does not rebuild original file list (`head`) since SUB_NODEs store names.
 *
 * Features         :
 *      • Allows choosing between default save file and custom filename.
 *      • Preserves file occurrence counts using repeated Insert_To_Hash_Table() calls.
 *      • Restores the inverted index into a usable state even with minor formatting issues.
 *
 * Limitations      :
 *      • Database is restored only from valid parsed tokens; no strict corruption detection.
 *      • Spacing and formatting must closely match Save_DataBase() output for proper parsing.
 *
 *******************************************************************************************************************************************************************/


#include "Types.h"
#include "Inverted_Search.h"


Status  Update_DataBase( HASH_T* H_Table, LIST **head )
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
    getchar();

    if( choice == 2 )
    {
        printf("\n[INFO]: Enter filename to load from: ");
        scanf("%255s", filename );
        getchar();

    }

    FILE *fptr = fopen( filename, "r" );
    if( fptr == NULL )
    {
        printf("\n[INFO]: Could not open '%s'. File not Found\n", filename );
        return FAILURE;
    }

    fseek( fptr, 0, SEEK_END );
    if( ftell( fptr ) == 0 )
    {
        printf("\n[INFO]: '%s' is Empty. Nothing to load.\n", filename );
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