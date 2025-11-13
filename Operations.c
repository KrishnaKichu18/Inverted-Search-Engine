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