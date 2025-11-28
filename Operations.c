/*******************************************************************************************************************************************************************
 * File Name       : List_Operations.c
 * Module Purpose  : Handles file list construction and display utilities for the
 *                   Inverted Search Engine project.
 *
 * Function Overview:
 *
 *      → Print_List( LIST *head )
 *              Prints all file names currently stored in the linked list.
 *              Files are displayed in insertion order, space-separated.
 *              Returns EMPTY if no files exist in the list.
 *
 *      → Add_To_List( LIST **head, char *filename, FILE *fptr )
 *              Creates a new LIST node and appends it to the end of the linked list.
 *              Stores filename and the already-open file pointer for later processing.
 *              Ensures dynamic memory allocation for each incoming file node.
 *
 * Input Details :
 *      • head     → Pointer to the head pointer of the file list
 *      • filename → Filename to store in the new node
 *      • fptr     → Valid file pointer associated with filename
 *
 * Return Values :
 *      Print_List : SUCCESS → List printed successfully
 *                   EMPTY   → No files in the list
 *
 *      Add_To_List: SUCCESS → Node insertion successful
 *                   FAILURE → Memory allocation failed
 *
 * Notes :
 *      • Maintains insertion order – new files always added at the end.
 *      • File pointer remains open for later use in building the inverted index.
 *      • Safe dynamic allocation ensures flexible file handling.
 *
 *******************************************************************************************************************************************************************/


#include "Types.h"

/**/
Status Print_List( LIST *head )
{
    if( head == NULL )
    {
        return EMPTY;
    }
    
    LIST *temp = head;
    
    while( temp )
    {
        printf( "%s ", temp -> FILENAME );
        temp = temp -> link;
    }

    return SUCCESS;

}

/* Insert files at last */
Status Add_To_List( LIST **head, char *filename, FILE *fptr )
{
    LIST *new = malloc( sizeof( LIST ) );
    if( new == NULL )
        return FAILURE;

    new -> fptr = fptr;
    new -> link = NULL;
    strcpy( new -> FILENAME, filename );

    if( *head == NULL )
    {
        *head = new;
        return SUCCESS;
    }

    LIST *temp = *head;
    while( temp -> link != NULL )
        temp = temp -> link;

    temp -> link = new;
    return SUCCESS;

}