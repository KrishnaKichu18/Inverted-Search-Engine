/*******************************************************************************************************************************************************************
 * Function Name    : Read_and_Validate
 *
 * Description      : Validates all command-line input files before they are added to the active file list.
 *                    Each file passes through four layers of validation:
 *
 *                        1. Check_Ext()      → Ensures file extension is ".txt"
 *                        2. Check_Avail()    → Ensures the file exists and is readable
 *                        3. Check_Content()  → Ensures the file is not empty
 *                        4. No_Duplicate()   → Ensures the same file is not added twice
 *
 *                    After successful validation, the file and its pointer are added to the LIST using Add_To_List().
 *
 * Prototype        : Status Read_and_Validate( int argc, char *argv[], LIST **head );
 *
 * Input Parameters : argc → Number of command-line arguments.
 *                    argv → Array of argument strings (filenames start from index 1).
 *                    head → Pointer to the head pointer of the linked list storing all validated files.
 *
 * Return Value     : SUCCESS → After validating all files (even if some are rejected).
 *                    FAILURE → Only if argument count is insufficient (program exits immediately).
 *
 * Features         :
 *                    • Performs all file checks in a structured and modular manner.
 *                    • Adds only clean, valid, non-empty, non-duplicate files to the active list.
 *                    • Prints detailed feedback for each file (extension error, duplicate, empty, etc.).
 *                    • Ensures fptr remains open so database creation can consume it later.
 *
 * Special Cases    :
 *                    • If no files are provided → Program exits immediately.
 *                    • If all files fail validation → head remains NULL, but function still returns SUCCESS.
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
                        }
                    }
                    else
                    {
                        printf("[INFO]: %s is Duplicate file\n", argv[i] ); 
                    }
                }
                else
                {
                    printf("[INFO]: %s is Empty file\n", argv[i] );
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