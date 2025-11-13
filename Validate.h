#ifndef VALIDATE_H
#define VALIDATE_H

#include "Types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Read and Validate
Status Read_and_Validate( int argc, char* argv[], LIST **head );

// To check extension is .txt
Status Check_Ext( char* filename );

// To check if file is present
Status Check_Avail( char* filename, FILE **fptr );

// To check if file empty
Status Check_Content( FILE *fptr );

// To check duplicate file
Status No_Duplicate( char* filename, LIST *head );

#endif