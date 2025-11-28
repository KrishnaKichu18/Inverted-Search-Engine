/*******************************************************************************************************************************************************************
 * Function Name    : Read_and_Validate
 *
 * Description      :
 *      Validates all command-line input files before adding them to the active file list.
 *      Each file must pass the following checks in order:
 *
 *          1. Check_Ext()      → Validates that the file has a ".txt" extension (case-sensitive)
 *          2. Check_Avail()    → Ensures the file exists and is readable
 *          3. Check_Content()  → Confirms the file is not empty
 *          4. No_Duplicate()   → Prevents adding a file already present in the list
 *
 *      If a file clears all checks, it is appended to the LIST using Add_To_List(),
 *      and its file pointer remains open for later database creation.
 *
 * Prototype        : Status Read_and_Validate( int argc, char *argv[], LIST **head );
 *
 * Input Parameters : argc → Number of command-line arguments.
 *                    argv → List of argument strings (filenames begin from index 1).
 *                    head → Pointer to the head pointer of the file-list structure.
 *
 * Return Value     : SUCCESS → Returned after validating all files (valid or invalid).
 *                    FAILURE → Never returned for file validation errors; only insufficient
 *                              arguments trigger program exit.
 *
 * Features         :
 *                    • Performs modular validation for each file.
 *                    • Adds only valid, non-duplicate, non-empty files to the active list.
 *                    • Prints descriptive messages for each validation outcome.
 *                    • Automatically closes file pointers for invalid files, preventing leaks.
 *
 * Special Cases    :
 *                    • If fewer than 2 arguments → Program prints error and exits immediately.
 *                    • If all files fail validation → *head remains NULL*, but SUCCESS still returned.
 *
 *******************************************************************************************************************************************************************/


#include "Types.h"
#include "Validate.h"
#include "Inverted_Search.h"
#include <string.h>
#include <stdlib.h>

Status Read_and_Validate( int argc, char* argv[], LIST **head )
{
    *head = NULL;

    if( argc < 2 )
    {
        printf("[INFO]: Error in number of files\n");
        exit(1);
    }

    for( int i = 1; i < argc; i++ )
    {
        FILE *fptr;

        if( Check_Ext( argv[i] ) == SUCCESS )
        {
            if( Check_Avail( argv[i], &fptr ) == SUCCESS )
            {
                if( Check_Content( fptr ) == SUCCESS )
                {
                    if( No_Duplicate( argv[i], *head ) == SUCCESS )
                    {
                        if( Add_To_List( head, argv[i], fptr ) == SUCCESS )
                        {
                            printf("[INFO]: %s added to list\n", argv[i] );
                        }
                        else
                        {
                            printf("[INFO]: %s failed to be listed\n", argv[i] );
                            fclose( fptr );

                        }
                    }
                    else
                    {
                        printf("[INFO]: %s is Duplicate file\n", argv[i] );
                        fclose( fptr );
                    }
                }
                else
                {
                    printf("[INFO]: %s is Empty file\n", argv[i] );
                    fclose( fptr );

                }
            }
            else
            {
                printf("[INFO]: %s File not available\n", argv[i] );
            }
        }
        else
        {
            printf("[INFO]: %s File wrong extension\n", argv[i] );
        }
    }

    return SUCCESS;
}

/* Validates the Extension of all files */
Status Check_Ext( char* filename )
{
    char* ext = strstr( filename, ".txt" );
    if( ext == NULL || strcmp( ext, ".txt" ) != 0 )
        return FAILURE;

    return SUCCESS;
}

/* Validate if the file is available */
Status Check_Avail( char* filename, FILE **fptr )
{
    *fptr = fopen( filename, "r" );
    if( *fptr == NULL )
    {
        return FAILURE;
    }

    return SUCCESS;
}

/* Check if file is empty or not */
Status Check_Content( FILE *fptr )
{
    fseek( fptr, 0, SEEK_END );

    if( ftell( fptr ) != 0 )
    {
        fseek( fptr, 0, SEEK_SET );
        return SUCCESS;
    }

    return FAILURE;
}

/* Check if any file is duplicate */
Status No_Duplicate( char* filename, LIST *head )
{
    if( head == NULL )
        return SUCCESS;
        
    LIST *temp = head;
    while( temp != NULL )
    {
        if( strcmp( filename, temp -> FILENAME ) == 0 )
        {
            return DUPLICATE;
        }

        temp = temp -> link;
    }

    return SUCCESS;
}