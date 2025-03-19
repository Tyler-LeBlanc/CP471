#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LexicalAnalysis.h"
#include "AST.h"

/*
 *
 *                                   Phase 1 Code
 *
 */

#define BUFFERSIZE 1024
#define INITIAL_CAPACITY 100

// Mapping of acceptance states to token types
StateTokenMap tokenMap[] = {
    {1, "IDENTIFIER"},
    {2, "KEYWORD"},
    {3, "INTEGER"},
    {5, "DOUBLE"},
    {8, "EXPONENT"},
    {9, "COMPARISON_OPERATOR"},
    {10, "COMPARISON_OPERATOR"},
    {11, "COMPARISON_OPERATOR"},
    {12, "COMPARISON_OPERATOR"},
    {14, "COMPARISON_OPERATOR"},
    {13, "ASSIGNMENT_OPERATOR"},
    {15, "MATH_OPERATOR"},
    {16, "DELIMITER"}};

// Array of all keywords
const char *keywords[] = {"and", "def", "do", "double", "else", "fed", "fi", "if", "int", "not", "od", "or", "print", "return", "then", "while"};

// Transition table for states based on character read
int transition_table[128][17] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1. - 1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {13, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 12, 0, 14, 0, 0, 0},
    {9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 6, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

/*
/ Implementation of the Double Buffer methods
/ allowing us to read the files faster
/
*/

// Function to initialize the double buffer
DOUBLE_BUFFER *initializeBuffer(const char *filename)
{
    // Allocate memory for DOUBLEBUFFER
    DOUBLE_BUFFER *db = (DOUBLE_BUFFER *)malloc(sizeof(DOUBLE_BUFFER));

    // check if memory was properly allocated
    // if memory was not properly allocated return
    if (!db)
    {
        printf("Failed to allocate buffer.");
        return NULL;

        // If memory has been allocated for DOUBLEBUFFER alocate memory for inner buffers
    }
    else
    {
        // Allocate memory for inner buffers
        db->buffer1 = (char *)malloc(BUFFERSIZE);
        db->buffer2 = (char *)malloc(BUFFERSIZE);

        // Verify if memory was properly allocated
        // If not, free memory allocated and return
        if (!db->buffer1 | !db->buffer2)
        {
            free(db->buffer1);
            free(db->buffer2);
            free(db);
            return NULL;

            // If memory has been allocated, open file for reading
        }
        else
        {
            // Open file for reading
            db->file = fopen(filename, "r");

            // Verify if file was properly opened
            // If not, free memory and return
            if (!db->file)
            {
                printf("Error opening file.");
                free(db->buffer1);
                free(db->buffer2);
                free(db);
                return NULL;

                // If memory has been allocated, set the rest of the variables and load first buffer
            }
            else
            {
                // Set values
                db->bufferSize = BUFFERSIZE;
                db->currentBuffer = db->buffer1;
                db->nextBuffer = db->buffer2;
                db->currentPos = 0;

                // Read bufferSize - 1 character into current buffer and store characters read
                size_t cRead = fread(db->currentBuffer, 1, db->bufferSize - 1, db->file);
                // Add null value to end of buffer to help iterations
                db->currentBuffer[cRead] = '\0';

                return db;
            }
        }
    }
}

// Function to switch buffers when one is empty and refill with text
void switchBuffer(DOUBLE_BUFFER *db)
{
    // Swap current and next buffer
    char *temp = db->currentBuffer;
    db->currentBuffer = db->nextBuffer;
    db->nextBuffer = temp;

    // Load next(current) buffer
    size_t cRead = fread(db->currentBuffer, 1, db->bufferSize - 1, db->file);
    // Add null value to end of buffer for iteration
    db->currentBuffer[cRead] = '\0';
    // Reset current position
    db->currentPos = 0;
}

// Function to read characters from the double buffer
char getNextChar(DOUBLE_BUFFER *db, int *line)
{
    // Checks if Buffer has reached the end
    if (db->currentBuffer[db->currentPos] == '\0')
    {
        switchBuffer(db); // Swtich to the other buffer
    }
    // Check for new lines
    if (db->currentBuffer[db->currentPos] == '\n')
    {
        (*line)++;
    }

    return db->currentBuffer[db->currentPos++]; // Return current character
}

void freeDB(DOUBLE_BUFFER *db)
{
    fclose(db->file);
    free(db->buffer1);
    free(db->buffer2);
    free(db);
}

/*
/ Implement token and string methods to store and compare values
/
*/

void initializeArray(TOKEN_ARRAY *tokenArray)
{
    // Allocate space for the array
    tokenArray->array = (TOKEN *)malloc(INITIAL_CAPACITY * sizeof(TOKEN));
    // Verify space was correctly allocated
    if (!tokenArray->array)
    {
        printf("Error allocating memory for array.");
        return;
    }

    // Set the size and capacity of array
    tokenArray->size = 0;
    tokenArray->capacity = INITIAL_CAPACITY;
}

void addToken(TOKEN_ARRAY *tokenArray, TOKEN token)
{
    // Check if array is full, add more space if array is full
    if (tokenArray->size == tokenArray->capacity)
    {
        tokenArray->capacity *= 2;
        tokenArray->array = (TOKEN *)realloc(tokenArray->array, tokenArray->capacity * sizeof(TOKEN));

        // Check if memory was reallocated correctly
        if (!tokenArray->array)
        {
            printf("Failed to reallocate memory for array.");
        }
    }

    // Add the token to the array
    tokenArray->array[tokenArray->size++] = token;
}

void freeTK(TOKEN_ARRAY *tk)
{
    free(tk->array);
    free(tk);
}

/*
/ Implementation of the Token Map methods
/ to see which states correspond to which types
/
*/

// Find the zie of the token map
int tokenMapSize = sizeof(tokenMap) / sizeof(tokenMap[0]);

// Function to get the token type for a given state
const char *getTokenType(int state)
{
    for (int i = 0; i < tokenMapSize; i++)
    {
        if (tokenMap[i].state == state)
        {
            return tokenMap[i].type;
        }
    }
    return "UNKNOWN"; // Default for unmapped states
}

// Function to copy strings without using string library
void copyString(char *dest, const char *src, int maxLen)
{
    int i;
    // Loop through characters in the source until terminator is reached
    for (i = 0; i < maxLen - 1 && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0'; // Terminate the destination string
}

// Function to compare strings
int compareStrings(const char *str1, const char *str2)
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0')
    {
        if (str1[i] != str2[i])
        {
            return 0; // Strings are not equal
        }
        i++;
    }
    // If both strings reach their end simultaneously, they are equal
    return (str1[i] == '\0' && str2[i] == '\0');
}

// Function to check if a token is a keyword
int isKeyword(const char *token)
{
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (compareStrings(token, keywords[i]))
        {
            return 1; // It is a keyword
        }
    }
    return 0; // Not a keyword
}

// Function to print out all tokens stored
void printTokens(TOKEN_ARRAY *tokenArray)
{
    printf("Tokens Found:\n");
    FILE *tokenFile = fopen("./tokens.txt", "w");
    for (int i = 0; i < tokenArray->size; i++)
    {
        fprintf(tokenFile, "Lexeme:%s, Type: %s\n", tokenArray->array[i].lexeme, tokenArray->array[i].type);
    }
    fclose(tokenFile);
}

/*
/ Phase 1 main Function
/ Runs the lexical analyzer
/
*/

TOKEN_ARRAY* LexicalAnalysis(char* filename)
{
    char c;
    int cline = 1;
    int state = 0;
    int previous_state = 0;

    // Initialize the buffer
    DOUBLE_BUFFER *db = initializeBuffer(filename);
    if (!db)
    {
        printf("Error initializing Double Buffer");
        return NULL;
    }

    // Initialize the token array
    TOKEN_ARRAY *tk = (TOKEN_ARRAY *)malloc(sizeof(TOKEN_ARRAY));
    initializeArray(tk);

    char tempBuffer[250]; // Temporary buffer to store token characters
    int tempPos = 0;      // Track position in the temp buffer

    // Loop to read character and store tokens
    while ((c = getNextChar(db, &cline)) != '\0')
    {
        int ascii = c;
        previous_state = state;
        state = transition_table[ascii][state];

        // Store current token characters
        tempBuffer[tempPos++] = c;
        // Handle invalid characters
        if (state == -1)
        {
            FILE *errorFile = fopen("./tokenError.txt", "w");
            fprintf(errorFile, "Invalid character:%c, on line %d\n", c, cline);
            fclose(errorFile);
            previous_state = state;
            state = 0;

            // Handle end of token
        }
        if (state == 0)
        {
            if (previous_state != -1)
            {
                db->currentPos--;
                tempBuffer[tempPos - 1] = '\0';
            }

            const char *tokenType = getTokenType(previous_state);

            // Check for unkown tokens
            if (compareStrings(tokenType, "UNKNOWN"))
            {
                printf("Unknown Token Type found on line %d\n", cline);
            }

            // Create new token and add it to the token array
            TOKEN token;
            copyString(token.lexeme, tempBuffer, sizeof(token.lexeme));

            // Verify keywords are valid, if not, set to id
            if (compareStrings(tokenType, "KEYWORD") && !isKeyword(tempBuffer))
            {
                // Change type to IDENTIFIER if not an actual keyword
                copyString(token.type, "IDENTIFIER", sizeof(token.type));
            }
            else
            {
                // Add token type to token
                copyString(token.type, tokenType, sizeof(token.type));
            }

            // Add token to array
            addToken(tk, token);
            tempPos = 0; // Reset the buffer after reading a token
        }
    }

    // Free memory
    freeDB(db);

    return tk;
}

/*
 *                                       Phase 2 Code
 *
 *
 */

#define STACK_MAX 100
#define MAX_RULES 189

// So the heap has the form non-terminal, terminal and production. It's a little confusion and awful to read
// If there is no terminal and it is just a non-terminal, then the terminal slot is the first char of the non-terminal it leads to.
// If there is a terminal then the terminal is just the terminal and the non terminal is epsilon.

ParsingRule parsing_table[MAX_RULES] = {
    {"program", "f", "fdecls decs st_seq ."},
    {"fdecls", "f", "fdec fdecls_"},
    {"fdecls_", "double", "ε"},
    {"fdecls_", ";", "; fdec fdecls_"},
    {"fdecls_", "int", "ε"},
    {"fdec", "d", "def type fname ( params ) decs st_seq fed"},
    {"params", "t", "type var params_"},
    {"params_", ",", ", type var params_"},
    {"params_", ")", "ε"},
    {"fname", "i", "id"},
    {"decs", "d", "decl decs_"},
    {"decs_", "print", "ε"},
    {"decs_", "while", "ε"},
    {"decs_", ";", "ε"},
    {"decs_", "if", "ε"},
    {"decs_", "fed", "ε"},
    {"decs_", ".", "ε"},
    {"decs_", "char", "ε"},
    {"decs_", "return", "ε"},
    {"decl", "t", "type varlist"},
    {"type", "i", "int"},
    {"type", "d", "double"},
    {"varlist", "v", "var , varlist_"},
    {"varlist_", "print", "ε"},
    {"varlist_", "while", "ε"},
    {"varlist_", ";", "ε"},
    {"varlist_", "if", "ε"},
    {"varlist_", "fed", "ε"},
    {"varlist_", ".", "ε"},
    {"varlist_", "char", "ε"},
    {"varlist_", ",", ", var varlist_"},
    {"varlist_", "return", "ε"},
    {"st_seq", "s", "statement ; st_seq"},
    {"statement", ";", "ε"},
    {"statement", "else", "ε"},
    {"statement", "od", "ε"},
    {"statement", "fed", "ε"},
    {"statement", ".", "ε"},
    {"statement", "fi", "ε"},
    {"statement", "v", "var = expr"},
    {"statement", "i", "if bexpr then st_seq st_seq_ fi"},
    {"statement", "w", "while bexpr do st_seq od"},
    {"statement", "p", "print expr"},
    {"statement", "r", "return expr"},
    {"st_seq_", "e", "else st_seq"},
    {"st_seq_", "fi", "ε"},
    {"expr", "t", "term expr_"},
    {"expr_", "and", "ε"},
    {"expr_", "then", "ε"},
    {"expr_", ";", "ε"},
    {"expr_", "else", "ε"},
    {"expr_", "+", "+ term expr_"},
    {"expr_", "-", "- term expr_"},
    {"expr_", ">=", "ε"},
    {"expr_", "<", "ε"},
    {"expr_", ">", "ε"},
    {"expr_", "==", "ε"},
    {"expr_", "od", "ε"},
    {"expr_", "do", "ε"},
    {"expr_", "fed", "ε"},
    {"expr_", ".", "ε"},
    {"expr_", "fi", "ε"},
    {"expr_", ",", "ε"},
    {"expr_", ")", "ε"},
    {"expr_", "or", "ε"},
    {"expr_", "<>", "ε"},
    {"expr_", "<=", "ε"},
    {"expr_", "]", "ε"},
    {"term", "f", "factor term_"},
    {"term_", "*", "* factor term_"},
    {"term_", "and", "ε"},
    {"term_", "then", "ε"},
    {"term_", ";", "ε"},
    {"term_", "else", "ε"},
    {"term_", '%', "% factor term_"},
    {"term_", "+", "ε"},
    {"term_", "-", "ε"},
    {"term_", "/", "/ factor term_"},
    {"term_", ">=", "ε"},
    {"term_", "<", "ε"},
    {"term_", ">", "ε"},
    {"term_", "==", "ε"},
    {"term_", "od", "ε"},
    {"term_", "do", "ε"},
    {"term_", "fed", "ε"},
    {"term_", ".", "ε"},
    {"term_", "fi", "ε"},
    {"term_", ",", "ε"},
    {"term_", ")", "ε"},
    {"term_", "or", "ε"},
    {"term_", "<>", "ε"},
    {"term_", "<=", "ε"},
    {"term_", "]", "ε"},
    {"exprseq", "e", "expr , exprseq_"},
    {"exprseq_", ",", ", expr exprseq_"},
    {"exprseq_", ")", "ε"},
    {"bexpr", "b", "bterm bexpr_"},
    {"bexpr_", "then", "ε"},
    {"bexpr_", "do", "ε"},
    {"bexpr_", ")", "ε"},
    {"bexpr_", "o", "or bfactor bterm_"},
    {"bterm", "b", "bfactor bterm_"},
    {"bterm_", "then", "ε"},
    {"bterm_", "do", "ε"},
    {"bterm_", ")", "ε"},
    {"bterm_", "or", "ε"},
    {"bterm_", "a", "and bfactor bterm_"},
    {"bfactor", "(", "( bexpr )"},
    {"bfactor", "e", "expr comp expr"},
    {"bfactor", "n", "not bfactor"},
    {"comp", "<", "<>"},
    {"comp", ">", ">="},
    {"comp", "=", "=="},
    {"var", "i", "id [ expr ]"},
    {"id", "l", "letter id_"},
    {"id_", "print", "ε"},
    {"id_", "(", "ε"},
    {"id_", "*", "ε"},
    {"id_", "and", "ε"},
    {"id_", "double", "ε"},
    {"id_", "while", "ε"},
    {"id_", "then", "ε"},
    {"id_", ";", "ε"},
    {"id_", "else", "ε"},
    {"id_", "if", "ε"},
    {"id_", '%', "ε"},
    {"id_", "+", "ε"},
    {"id_", "-", "ε"},
    {"id_", "/", "ε"},
    {"id_", "[", "ε"},
    {"id_", ">=", "ε"},
    {"id_", "int", "ε"},
    {"id_", "<", "ε"},
    {"id_", ">", "ε"},
    {"id_", "od", "ε"},
    {"id_", "do", "ε"},
    {"id_", "fed", "ε"},
    {"id_", ".", "ε"},
    {"id_", "char", "ε"},
    {"id_", "fi", "ε"},
    {"id_", ",", "ε"},
    {"id_", ")", "ε"},
    {"id_", "return", "ε"},
    {"id_", "or", "ε"},
    {"id_", "<>", "ε"},
    {"id_", "=", "ε"},
    {"id_", "<=", "ε"},
    {"id_", "]", "ε"},
    {"id_", "l", "letter id_"},
    {"id_", "d", "digit id_"},
    {"number", "i", "integer"},
    {"number", "D", "Double"},
    {"Double", "i", "integer . integer"},
    {"integer", "d", "digit integer_"},
    {"integer_", "*", "ε"},
    {"integer_", "and", "ε"},
    {"integer_", "then", "ε"},
    {"integer_", ";", "ε"},
    {"integer_", '%', "ε"},
    {"integer_", "+", "ε"},
    {"integer_", "-", "ε"},
    {"integer_", "/", "ε"},
    {"integer_", ">=", "ε"},
    {"integer_", "<", "ε"},
    {"integer_", ">", "ε"},
    {"integer_", "od", "ε"},
    {"integer_", "do", "ε"},
    {"integer_", "fed", "ε"},
    {"integer_", ".", "ε"},
    {"integer_", "fi", "ε"},
    {"integer_", ",", "ε"},
    {"integer_", ")", "ε"},
    {"integer_", "or", "ε"},
    {"integer_", "<>", "ε"},
    {"integer_", "<=", "ε"},
    {"integer_", "]", "ε"},
    {"integer_", "d", "digit integer_"},
    {"factor", "(", "( expr )"},
    {"factor", "v", "var"},
    {"factor", "n", "number"},
    {"factor", "f", "fname ( exprseq )"},
    {"letter", "c", "char"},
    {"digit", "n", "num"},
    {"digits_", "d", "digits_"},
    {"exponent", "E", "E sign digit digits_"},
    {"exponent", "e", "e sign digit digits_"},
    {"sign", "num", "ε"},
    {"sign", "+", "+"},
    {"sign", "-", "-"},
};







// Function to Initialize the stack
void initializeStack(STACK *stack)
{
    stack->top = -1;
}

// Function to check if the stack is empty (0 is False)
int isEmpty(STACK *stack)
{
    return stack->top == -1;
}

// Function to check if the stack is full
int isFull(STACK *stack)
{
    return stack->top == STACK_MAX - 1;
}

// Function to add value onto the stack
//use: push(&stack, char)
void push(STACK *stack, const char *value)
{
    if (isFull(stack))
    {
        printf("Stack is full.\n");
        return;
    }
    // Allocate and copy the string to store on the stack
    stack->array[++stack->top] = strdup(value);
}

// Function to remove value from the stack
//use: pop(&stack, char)
char* pop(STACK *stack)
{
    if (isEmpty(stack)){
        printf("Stack is empty.");
        return NULL;
    }

    return stack->array[stack->top--];
}

// Function to check what the top value of the stack is
char* peek(STACK *stack)
{
    if (!isEmpty(stack))
    {
        return stack->array[stack->top];
    }

    return NULL;
}


// Function to create AST node
NODE *createNode(char* data){
    //make memory for new node
    NODE *new_node = (NODE*) malloc(sizeof(NODE));

    //initialize values
    new_node->value = data;
    new_node->left = NULL;
    new_node->right = NULL;

    //return new node
    return new_node;
}


//Function to insert new node into tree
// use: insert(&root, char)
void insert(NODE **root, char* data){
    //create a new node
    NODE *new_node = createNode(data);

    //check if tree is empty
    if(*root == NULL){
        *root = new_node;
    }else{
        //do level order traversal to find where to insert node

        NODE *temp;
        NODE *queue[100];
        int front = -1;
        int rear = -1;
        queue[++rear] = *root;

        while(front != rear){
            //get current node
            temp = queue[++front];

            //check if left is empty, if so add node
            if (temp->left == NULL){
                temp->left = new_node;
                return;
            }else{
                //if left is not empty, add to rear
                queue[++rear] = temp->left;
            }

            //check if right is empty, if so add node
            if(temp->right == NULL){
                temp->right = new_node;
                return;
            }else{
                //if right is not empty, add to rear
                queue[++rear] = temp->right;
            }

        }
    }

}





/*
 *   Phase 2 main function
 *   Runs the syntax analyzer
 */
NODE* SyntaxAnalysis(TOKEN_ARRAY *tk, STACK *stack){

    //initialize stack
    initializeStack(stack);
    push(&stack, "program");

    //create token index to track which token we are matching
    int token_index = 0;
    //get the current token item
    TOKEN current_token = tk->array[token_index];

    //create the root of the AST and add starting production
    NODE* root = createNode("Program");


}

/*
 *   Main Function
 */
int main(){

    NODE *root = NULL;

    insert(&root, "abc");

    printf("%s", root->value);

}