#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include <ctype.h>

/*
 *
 *                                   Phase 1 Code
 *
 */

#define BUFFERSIZE 1024
#define INITIAL_CAPACITY 2000
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
int transition_table[128][18] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 7, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {3, 0, 0, 3, 5, 5, 3, 3, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {13, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 12, 0, 14, 0, 0, 0, 0},
    {9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 6, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
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
    // printf("adding token %s to array\n", token.lexeme);
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

TOKEN_ARRAY *LexicalAnalysis(char *filename)
{
    char c;
    int cline = 1;
    int state = 0;
    int previous_state = 0;
    int error = 0; // 0 = false 1 = true

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
            FILE *errorFile = fopen("./errors.txt", "a");
            fprintf(errorFile, "Invalid character:%c, on line %d\n", c, cline);
            error = 1;
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
                error = 1;
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
            if (error == 0) // Do not add to token array if there is a invalid character of any kind
            {
                addToken(tk, token);
                // printf("Adding token to array: %s\n", token.lexeme);
                // for (int j = 0; j < 100; j++)
                // {
                //     printf("{%c | %d} ", tempBuffer[j], tempBuffer[j]);
                // }
                // printf("\n");
            }
            else
            {
                error = 0; // reset errror detection
            }
            tempPos = 0; // Reset the buffer after reading a token
        }
    }

    if (tempPos > 0)
    {
        tempBuffer[tempPos] = '\0';
        TOKEN token;
        copyString(token.lexeme, tempBuffer, sizeof(token.lexeme));
        copyString(token.type, getTokenType(state), sizeof(token.type));
        addToken(tk, token);
        printf("At the end adding: %s\n", token.lexeme);
    }

    printTokens(tk);
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
#define CHILD_ARRAY_SIZE 35

// So the heap has the form non-terminal, terminal and production. It's a little confusion and awful to read
// If there is no terminal and it is just a non-terminal, then the terminal slot is the first char of the non-terminal it leads to.
// If there is a terminal then the terminal is just the terminal and the non terminal is epsilon.

ParseTableEntry ll1_table[NUM_NONTERMINALS][NUM_TERMINALS] = {
    /* NT_PROGRAM: <program> → <fdecls_list> <decl_list> <stmt_seq> . */
    [NT_PROGRAM] = {
        [TERM_DEF] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_INT] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_DOUBLE] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_ID] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_IF] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_WHILE] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_PRINT] = {"<fdecls_list> <decl_list> <stmt_seq> ."},
        [TERM_RETURN] = {"<fdecls_list> <decl_list> <stmt_seq> ."}},
    /* NT_FDECLS_LIST: → <fdec> ; <fdecls_list> | ε */
    [NT_FDECLS_LIST] = {[TERM_DEF] = {"<fdec> ; <fdecls_list>"}, [TERM_INT] = {"ε"}, [TERM_DOUBLE] = {"ε"}, [TERM_ID] = {"ε"}, [TERM_IF] = {"ε"}, [TERM_WHILE] = {"ε"}, [TERM_PRINT] = {"ε"}, [TERM_RETURN] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_FDEC: def <type> <id> ( <params> ) <decl_list> <stmt_seq> fed */
    [NT_FDEC] = {[TERM_DEF] = {"def <type> <id> ( <params> ) <decl_list> <stmt_seq> fed"}},
    /* NT_PARAMS: → <type> <id> <param_list> | ε */
    [NT_PARAMS] = {[TERM_INT] = {"<type> <id> <param_list>"}, [TERM_DOUBLE] = {"<type> <id> <param_list>"}, [TERM_RPAREN] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_PARAM_LIST: → , <type> <id> <param_list> | ε */
    [NT_PARAM_LIST] = {[TERM_COMMA] = {", <type> <id> <param_list>"}, [TERM_RPAREN] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_DECL_LIST: → <decl> ; <decl_list> | ε */
    [NT_DECL_LIST] = {[TERM_INT] = {"<decl> ; <decl_list>"}, [TERM_DOUBLE] = {"<decl> ; <decl_list>"}, [TERM_IF] = {"ε"}, [TERM_WHILE] = {"ε"}, [TERM_PRINT] = {"ε"}, [TERM_RETURN] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_ID] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_DECL: → <type> <varlist> */
    [NT_DECL] = {[TERM_INT] = {"<type> <varlist>"}, [TERM_DOUBLE] = {"<type> <varlist>"}},
    /* NT_TYPE: → int | double */
    [NT_TYPE] = {[TERM_INT] = {"int"}, [TERM_DOUBLE] = {"double"}},
    /* NT_VARLIST: → <id> <varlist_tail> */
    [NT_VARLIST] = {[TERM_ID] = {"<id> <varlist_tail>"}},
    /* NT_VARLIST_TAIL: → , <id> <varlist_tail> | ε */
    [NT_VARLIST_TAIL] = {[TERM_COMMA] = {", <id> <varlist_tail>"}, [TERM_SEMICOLON] = {"ε"}, [TERM_RPAREN] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_STMT_SEQ: → <statement> <stmt_seq_tail> */
    [NT_STMT_SEQ] = {[TERM_ID] = {"<statement> <stmt_seq_tail>"}, [TERM_IF] = {"<statement> <stmt_seq_tail>"}, [TERM_WHILE] = {"<statement> <stmt_seq_tail>"}, [TERM_PRINT] = {"<statement> <stmt_seq_tail>"}, [TERM_RETURN] = {"<statement> <stmt_seq_tail>"}, [TERM_FI] = {"ε"}, [TERM_OD] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_STMT_SEQ_TAIL: → ; <statement> <stmt_seq_tail> | ε */
    [NT_STMT_SEQ_TAIL] = {[TERM_SEMICOLON] = {"; <statement> <stmt_seq_tail>"}, [TERM_FI] = {"ε"}, [TERM_OD] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_STATEMENT:
         → <id> <var_tail> = <expr>
         | if <bexpr> then <stmt_seq> <if_tail>
         | while <bexpr> do <stmt_seq> od
         | print <expr>
         | return <expr>
         | ε
    */
    [NT_STATEMENT] = {[TERM_ID] = {"<id> <var_tail> = <expr>"}, [TERM_IF] = {"if <bexpr> then <stmt_seq> <if_tail>"}, [TERM_WHILE] = {"while <bexpr> do <stmt_seq> od"}, [TERM_PRINT] = {"print <expr>"}, [TERM_RETURN] = {"return <expr>"}, [TERM_SEMICOLON] = {"ε"}, [TERM_FI] = {"ε"}, [TERM_OD] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_IF_TAIL: → fi | else <stmt_seq> fi */
    [NT_IF_TAIL] = {[TERM_FI] = {"fi"}, [TERM_ELSE] = {"else <stmt_seq> fi"}},
    /* NT_EXPR: → <term> <expr_prime> */
    [NT_EXPR] = {[TERM_ID] = {"<term> <expr_prime>"}, [TERM_NUMBER] = {"<term> <expr_prime>"}, [TERM_LPAREN] = {"<term> <expr_prime>"}},
    /* NT_EXPR_PRIME: → + <term> <expr_prime> | - <term> <expr_prime> | ε */
    [NT_EXPR_PRIME] = {[TERM_PLUS] = {"+ <term> <expr_prime>"}, [TERM_MINUS] = {"- <term> <expr_prime>"}, [TERM_RPAREN] = {"ε"}, [TERM_COMMA] = {"ε"}, [TERM_SEMICOLON] = {"ε"}, [TERM_FI] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_LT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_TERM: → <factor> <term_prime> */
    [NT_TERM] = {[TERM_ID] = {"<factor> <term_prime>"}, [TERM_NUMBER] = {"<factor> <term_prime>"}, [TERM_LPAREN] = {"<factor> <term_prime>"}},
    /* NT_TERM_PRIME: → * <factor> <term_prime>
                        | / <factor> <term_prime>
                        | % <factor> <term_prime>
                        | ε
    */
    [NT_TERM_PRIME] = {[TERM_MULT] = {"* <factor> <term_prime>"}, [TERM_DIV] = {"/ <factor> <term_prime>"}, [TERM_MOD] = {"% <factor> <term_prime>"}, [TERM_PLUS] = {"ε"}, [TERM_MINUS] = {"ε"}, [TERM_RPAREN] = {"ε"}, [TERM_COMMA] = {"ε"}, [TERM_SEMICOLON] = {"ε"}, [TERM_FI] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_LT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_FACTOR:
         → <id> <factor_tail> | <number> | ( <expr> )
    */
    [NT_FACTOR] = {[TERM_ID] = {"<id> <factor_tail>"}, [TERM_NUMBER] = {"number"}, [TERM_LPAREN] = {"( <expr> )"}},
    /* NT_FACTOR_TAIL:
         → ( <exprseq> ) | <var_tail>
         (If a '(' follows an identifier then it’s a function call;
          otherwise use the variable’s tail production.)
    */
    [NT_FACTOR_TAIL] = {[TERM_LPAREN] = {"( <exprseq> )"}, [TERM_LBRACKET] = {"<var_tail>"}, [TERM_PLUS] = {"<var_tail>"}, [TERM_MINUS] = {"<var_tail>"}, [TERM_MULT] = {"<var_tail>"}, [TERM_DIV] = {"<var_tail>"}, [TERM_MOD] = {"<var_tail>"}, [TERM_RPAREN] = {"ε"}, [TERM_COMMA] = {"ε"}, [TERM_SEMICOLON] = {"ε"}, [TERM_FI] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_LT] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_EXPRSEQ: → <expr> <exprseq_tail> | ε */
    [NT_EXPRSEQ] = {[TERM_ID] = {"<expr> <exprseq_tail>"}, [TERM_NUMBER] = {"<expr> <exprseq_tail>"}, [TERM_LPAREN] = {"<expr> <exprseq_tail>"}, [TERM_RPAREN] = {"ε"}},
    /* NT_EXPRSEQ_TAIL: → , <expr> <exprseq_tail> | ε */
    [NT_EXPRSEQ_TAIL] = {[TERM_COMMA] = {", <expr> <exprseq_tail>"}, [TERM_RPAREN] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_BEXPR: → <bterm> <bexpr_prime> */
    [NT_BEXPR] = {[TERM_LPAREN] = {"<bterm> <bexpr_prime>"}, [TERM_NOT] = {"<bterm> <bexpr_prime>"}},
    /* NT_BEXPR_PRIME: → or <bterm> <bexpr_prime> | ε */
    [NT_BEXPR_PRIME] = {[TERM_OR] = {"or <bterm> <bexpr_prime>"}, [TERM_THEN] = {"ε"}, [TERM_RPAREN] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_BTERM: → <bfactor> <bterm_prime> */
    [NT_BTERM] = {[TERM_LPAREN] = {"<bfactor> <bterm_prime>"}, [TERM_NOT] = {"<bfactor> <bterm_prime>"}},
    /* NT_BTERM_PRIME: → and <bfactor> <bterm_prime> | ε */
    [NT_BTERM_PRIME] = {[TERM_AND] = {"and <bfactor> <bterm_prime>"}, [TERM_OR] = {"ε"}, [TERM_THEN] = {"ε"}, [TERM_RPAREN] = {"ε"}, [TERM_EP] = {"ε"}},
    /* NT_BFACTOR: → ( <bfactor_inner> ) | not <bfactor> */
    [NT_BFACTOR] = {
        [TERM_LPAREN] = {"( <bfactor_inner> )"},
        [TERM_NOT] = {"not <bfactor>"},
    },
    /* NT_BFACTOR_INNER: → <expr> <comp> <expr> */
    [NT_BFACTOR_INNER] = {[TERM_LPAREN] = {"<expr> <comp> <expr>"}, [TERM_ID] = {"<expr> <comp> <expr>"}, [TERM_NUMBER] = {"<expr> <comp> <expr>"}},
    /* NT_COMP: → < | > | == | <= | >= | <> */
    [NT_COMP] = {[TERM_LT] = {"<"}, [TERM_GT] = {">"}, [TERM_EQEQ] = {"=="}, [TERM_LE] = {"<="}, [TERM_GE] = {">="}, [TERM_NE] = {"<>"}},
    /* NT_VAR_TAIL: → [ <expr> ] | ε */
    [NT_VAR_TAIL] = {[TERM_LBRACKET] = {"[ <expr> ]"}, [TERM_PLUS] = {"ε"}, [TERM_MINUS] = {"ε"}, [TERM_MULT] = {"ε"}, [TERM_DIV] = {"ε"}, [TERM_MOD] = {"ε"}, [TERM_RPAREN] = {"ε"}, [TERM_COMMA] = {"ε"}, [TERM_SEMICOLON] = {"ε"}, [TERM_FI] = {"ε"}, [TERM_DOT] = {"ε"}, [TERM_ASSIGN] = {"ε"}, [TERM_EP] = {"ε"}}};

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
void push(STACK *s, Symbol sym)
{
    if (s->top >= STACK_MAX - 1)
    {
        fprintf(stderr, "Stack overflow!\n");
        exit(1);
    }
    s->array[++(s->top)] = sym;
}

// Function to remove value from the stack
Symbol pop(STACK *s)
{
    if (s->top < 0)
    {
        fprintf(stderr, "Stack underflow!\n");
        exit(1);
    }
    return s->array[(s->top)--];
}

// Function to check what the top value of the stack is
Symbol peek(STACK *s)
{
    if (s->top < 0)
    {
        fprintf(stderr, "Stack empty!\n");
        exit(1);
    }
    return s->array[s->top];
}

// Function to create AST node
NODE *createNode(char *data)
{
    // make memory for new node
    NODE *new_node = (NODE *)malloc(sizeof(NODE));

    new_node->value = strdup(data);
    new_node->child_count = 0;
    new_node->capacity = CHILD_ARRAY_SIZE;

    new_node->children = (NODE **)malloc(new_node->capacity * sizeof(NODE *));

    // return new node
    return new_node;
}

// Function to insert new node into tree
void add_child(NODE *parent, NODE *child)
{
    parent->children[parent->child_count] = child;
    parent->child_count++;
}

/* Convert a TOKEN (from your lexical analyzer) into our Terminal enum.
   We assume that TOKEN.type holds a string such as "def", "int", etc. */
Terminal convertToken(TOKEN token)
{
    if (strcmp(token.lexeme, "def") == 0)
        return TERM_DEF;
    else if (strcmp(token.lexeme, "fed") == 0)
        return TERM_FED;
    else if (strcmp(token.lexeme, "int") == 0)
        return TERM_INT;
    else if (strcmp(token.lexeme, "double") == 0)
        return TERM_DOUBLE;
    else if (strcmp(token.lexeme, "if") == 0)
        return TERM_IF;
    else if (strcmp(token.lexeme, "then") == 0)
        return TERM_THEN;
    else if (strcmp(token.lexeme, "else") == 0)
        return TERM_ELSE;
    else if (strcmp(token.lexeme, "fi") == 0)
        return TERM_FI;
    else if (strcmp(token.lexeme, "while") == 0)
        return TERM_WHILE;
    else if (strcmp(token.lexeme, "do") == 0)
        return TERM_DO;
    else if (strcmp(token.lexeme, "od") == 0)
        return TERM_OD;
    else if (strcmp(token.lexeme, "print") == 0)
        return TERM_PRINT;
    else if (strcmp(token.lexeme, "return") == 0)
        return TERM_RETURN;
    else if (strcmp(token.lexeme, "+") == 0)
        return TERM_PLUS;
    else if (strcmp(token.lexeme, "-") == 0)
        return TERM_MINUS;
    else if (strcmp(token.lexeme, "*") == 0)
        return TERM_MULT;
    else if (strcmp(token.lexeme, "/") == 0)
        return TERM_DIV;
    else if (strcmp(token.lexeme, "%") == 0)
        return TERM_MOD;
    else if (strcmp(token.lexeme, "or") == 0)
        return TERM_OR;
    else if (strcmp(token.lexeme, "and") == 0)
        return TERM_AND;
    else if (strcmp(token.lexeme, "not") == 0)
        return TERM_NOT;
    else if (strcmp(token.lexeme, "(") == 0)
        return TERM_LPAREN;
    else if (strcmp(token.lexeme, ")") == 0)
        return TERM_RPAREN;
    else if (strcmp(token.lexeme, ",") == 0)
        return TERM_COMMA;
    else if (strcmp(token.lexeme, ";") == 0)
        return TERM_SEMICOLON;
    else if (strcmp(token.lexeme, "[") == 0)
        return TERM_LBRACKET;
    else if (strcmp(token.lexeme, "]") == 0)
        return TERM_RBRACKET;
    else if (strcmp(token.lexeme, "<") == 0)
        return TERM_LT;
    else if (strcmp(token.lexeme, ">") == 0)
        return TERM_GT;
    else if (strcmp(token.lexeme, "==") == 0)
        return TERM_EQEQ;
    else if (strcmp(token.lexeme, "<=") == 0)
        return TERM_LE;
    else if (strcmp(token.lexeme, ">=") == 0)
        return TERM_GE;
    else if (strcmp(token.lexeme, "<>") == 0)
        return TERM_NE;
    else if (strcmp(token.lexeme, "=") == 0)
        return TERM_ASSIGN;
    else if (strcmp(token.lexeme, ".") == 0)
        return TERM_DOT;
    else if (strcmp(token.type, "IDENTIFIER") == 0)
        return TERM_ID;
    else if (strcmp(token.type, "INTEGER") == 0)
        return TERM_NUMBER;
    else if (strcmp(token.type, "DOUBLE") == 0)
        return TERM_NUMBER;
    else if (strcmp(token.lexeme, "$") == 0)
        return TERM_EOF;
    else
    {
        fprintf(stderr, "Unknown token: %s\n", token.lexeme);
        exit(1);
    }
}

/*
 * Convert a token from a production string into a Symbol.
 * Nonterminals are assumed to be enclosed in angle brackets (e.g., <expr>).
 */
Symbol getSymbolFromTokenString(const char *tokenStr)
{
    Symbol sym;
    int len = strlen(tokenStr);
    // Special case: treat "<id>" as a terminal, not a nonterminal.
    if (strcmp(tokenStr, "<id>") == 0)
    {
        sym.type = SYMBOL_TERMINAL;
        sym.value = TERM_ID;
        return sym;
    }

    if ((tokenStr[0] == '<' && tokenStr[len - 1] == '>') && !(tokenStr[0] == '<' && tokenStr[1] == '>'))
    {
        sym.type = SYMBOL_NONTERMINAL;
        if (strcmp(tokenStr, "<program>") == 0)
            sym.value = NT_PROGRAM;
        else if (strcmp(tokenStr, "<fdecls_list>") == 0)
            sym.value = NT_FDECLS_LIST;
        else if (strcmp(tokenStr, "<fdec>") == 0)
            sym.value = NT_FDEC;
        else if (strcmp(tokenStr, "<params>") == 0)
            sym.value = NT_PARAMS;
        else if (strcmp(tokenStr, "<param_list>") == 0)
            sym.value = NT_PARAM_LIST;
        else if (strcmp(tokenStr, "<decl_list>") == 0)
            sym.value = NT_DECL_LIST;
        else if (strcmp(tokenStr, "<decl>") == 0)
            sym.value = NT_DECL;
        else if (strcmp(tokenStr, "<type>") == 0)
            sym.value = NT_TYPE;
        else if (strcmp(tokenStr, "<varlist>") == 0)
            sym.value = NT_VARLIST;
        else if (strcmp(tokenStr, "<varlist_tail>") == 0)
            sym.value = NT_VARLIST_TAIL;
        else if (strcmp(tokenStr, "<stmt_seq>") == 0)
            sym.value = NT_STMT_SEQ;
        else if (strcmp(tokenStr, "<stmt_seq_tail>") == 0)
            sym.value = NT_STMT_SEQ_TAIL;
        else if (strcmp(tokenStr, "<statement>") == 0)
            sym.value = NT_STATEMENT;
        else if (strcmp(tokenStr, "<if_tail>") == 0)
            sym.value = NT_IF_TAIL;
        else if (strcmp(tokenStr, "<expr>") == 0)
            sym.value = NT_EXPR;
        else if (strcmp(tokenStr, "<expr_prime>") == 0)
            sym.value = NT_EXPR_PRIME;
        else if (strcmp(tokenStr, "<term>") == 0)
            sym.value = NT_TERM;
        else if (strcmp(tokenStr, "<term_prime>") == 0)
            sym.value = NT_TERM_PRIME;
        else if (strcmp(tokenStr, "<factor>") == 0)
            sym.value = NT_FACTOR;
        else if (strcmp(tokenStr, "<factor_tail>") == 0)
            sym.value = NT_FACTOR_TAIL;
        else if (strcmp(tokenStr, "<exprseq>") == 0)
            sym.value = NT_EXPRSEQ;
        else if (strcmp(tokenStr, "<exprseq_tail>") == 0)
            sym.value = NT_EXPRSEQ_TAIL;
        else if (strcmp(tokenStr, "<bexpr>") == 0)
            sym.value = NT_BEXPR;
        else if (strcmp(tokenStr, "<bexpr_prime>") == 0)
            sym.value = NT_BEXPR_PRIME;
        else if (strcmp(tokenStr, "<bterm>") == 0)
            sym.value = NT_BTERM;
        else if (strcmp(tokenStr, "<bterm_prime>") == 0)
            sym.value = NT_BTERM_PRIME;
        else if (strcmp(tokenStr, "<bfactor>") == 0)
            sym.value = NT_BFACTOR;
        else if (strcmp(tokenStr, "<bfactor_inner>") == 0)
            sym.value = NT_BFACTOR_INNER;
        else if (strcmp(tokenStr, "<comp>") == 0)
            sym.value = NT_COMP;
        else if (strcmp(tokenStr, "<var_tail>") == 0)
            sym.value = NT_VAR_TAIL;
        else
        {
            fprintf(stderr, "Unknown nonterminal: %s\n", tokenStr);
            exit(1);
        }
    }
    else
    {
        sym.type = SYMBOL_TERMINAL;
        if (strcmp(tokenStr, "def") == 0)
            sym.value = TERM_DEF;
        else if (strcmp(tokenStr, "fed") == 0)
            sym.value = TERM_FED;
        else if (strcmp(tokenStr, "int") == 0)
            sym.value = TERM_INT;
        else if (strcmp(tokenStr, "double") == 0)
            sym.value = TERM_DOUBLE;
        else if (strcmp(tokenStr, "if") == 0)
            sym.value = TERM_IF;
        else if (strcmp(tokenStr, "then") == 0)
            sym.value = TERM_THEN;
        else if (strcmp(tokenStr, "else") == 0)
            sym.value = TERM_ELSE;
        else if (strcmp(tokenStr, "fi") == 0)
            sym.value = TERM_FI;
        else if (strcmp(tokenStr, "while") == 0)
            sym.value = TERM_WHILE;
        else if (strcmp(tokenStr, "do") == 0)
            sym.value = TERM_DO;
        else if (strcmp(tokenStr, "od") == 0)
            sym.value = TERM_OD;
        else if (strcmp(tokenStr, "print") == 0)
            sym.value = TERM_PRINT;
        else if (strcmp(tokenStr, "return") == 0)
            sym.value = TERM_RETURN;
        else if (strcmp(tokenStr, "+") == 0)
            sym.value = TERM_PLUS;
        else if (strcmp(tokenStr, "-") == 0)
            sym.value = TERM_MINUS;
        else if (strcmp(tokenStr, "*") == 0)
            sym.value = TERM_MULT;
        else if (strcmp(tokenStr, "/") == 0)
            sym.value = TERM_DIV;
        else if (strcmp(tokenStr, "%") == 0)
            sym.value = TERM_MOD;
        else if (strcmp(tokenStr, "or") == 0)
            sym.value = TERM_OR;
        else if (strcmp(tokenStr, "and") == 0)
            sym.value = TERM_AND;
        else if (strcmp(tokenStr, "not") == 0)
            sym.value = TERM_NOT;
        else if (strcmp(tokenStr, "(") == 0)
            sym.value = TERM_LPAREN;
        else if (strcmp(tokenStr, ")") == 0)
            sym.value = TERM_RPAREN;
        else if (strcmp(tokenStr, ",") == 0)
            sym.value = TERM_COMMA;
        else if (strcmp(tokenStr, ";") == 0)
            sym.value = TERM_SEMICOLON;
        else if (strcmp(tokenStr, "[") == 0)
            sym.value = TERM_LBRACKET;
        else if (strcmp(tokenStr, "]") == 0)
            sym.value = TERM_RBRACKET;
        else if (strcmp(tokenStr, "<") == 0)
            sym.value = TERM_LT;
        else if (strcmp(tokenStr, ">") == 0)
            sym.value = TERM_GT;
        else if (strcmp(tokenStr, "==") == 0)
            sym.value = TERM_EQEQ;
        else if (strcmp(tokenStr, "<=") == 0)
            sym.value = TERM_LE;
        else if (strcmp(tokenStr, ">=") == 0)
            sym.value = TERM_GE;
        else if (strcmp(tokenStr, "<>") == 0)
            sym.value = TERM_NE;
        else if (strcmp(tokenStr, "=") == 0)
            sym.value = TERM_ASSIGN;
        else if (strcmp(tokenStr, ".") == 0)
            sym.value = TERM_DOT;
        else if (strcmp(tokenStr, "number") == 0)
            sym.value = TERM_NUMBER;
        else if (strcmp(tokenStr, "$") == 0)
            sym.value = TERM_EOF;
        else if (strcmp(tokenStr, "<id>") == 0)
            sym.value = TERM_ID;
        else
        {
            fprintf(stderr, "Unknown terminal: %s\n", tokenStr);
            exit(1);
        }
    }
    return sym;
}

/* Helper to trim whitespace from a string */
char *trim_whitespace(char *str)
{
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return str;
}

/*
 * Given a production string (with tokens separated by spaces), break it into an array of Symbols.
 */
#define MAX_PROD_TOKENS 50
int parse_production(const char *production, Symbol symbols[], int max_symbols)
{
    char prod_copy[256];
    strncpy(prod_copy, production, sizeof(prod_copy));
    prod_copy[sizeof(prod_copy) - 1] = '\0';
    int count = 0;
    char *token = strtok(prod_copy, " ");
    while (token != NULL && count < max_symbols)
    {
        token = trim_whitespace(token);
        if (strlen(token) > 0)
        {
            symbols[count++] = getSymbolFromTokenString(token);
        }
        token = strtok(NULL, " ");
    }
    return count;
}

/* Helper function to skip whitespace tokens */
void skipWhitespace(TOKEN_ARRAY *tokenArray, int *ip)
{
    while (*ip < tokenArray->size &&
           (strcmp(tokenArray->array[*ip].lexeme, " ") == 0 ||
            strcmp(tokenArray->array[*ip].lexeme, "\t") == 0 ||
            strcmp(tokenArray->array[*ip].lexeme, "\n") == 0 ||
            strcmp(tokenArray->array[*ip].lexeme, "\0") == 0))
    {
        (*ip)++;
    }
}

void syntax_error(int expected, char *token_found, int current_terminal, int token_index)
{
    FILE *errorFile = fopen("./errors.txt", "a");
    fprintf(errorFile, "Syntax Error: expected token '%d' but found '%s' Current Terminal '%d', TOKEN INDEX: '%d'\n", expected, token_found, current_terminal, token_index);
    fclose(errorFile);
}

/*
 *   Phase 2 main function
 *   Runs the syntax analyzer
 */

void *SyntaxAnalysis(TOKEN_ARRAY *tk, STACK *stack)
{
    // Push end-of-input marker and the start symbol (<program>) onto the stack
    Symbol eof_sym = {SYMBOL_TERMINAL, TERM_EOF};
    push(stack, eof_sym);
    Symbol start_sym = {SYMBOL_NONTERMINAL, NT_PROGRAM};
    push(stack, start_sym);

    int ip = 0;
    TOKEN current_token = tk->array[ip];
    Terminal current_terminal = convertToken(current_token);
    Symbol top = peek(stack);

    while (!isEmpty(stack) && top.value != 37 && ip != tk->size)
    {
        top = peek(stack);
        if (top.value != 37)
        {
            // Found a terminal
            if (top.type == SYMBOL_TERMINAL)
            {
                // check if terminal found matches current token
                if (top.value == current_terminal)
                {
                    pop(stack);
                    printf("Matches Terminal: %s\n", current_token.lexeme);
                    ip++;
                    skipWhitespace(tk, &ip);
                    // printf("1: Ip: %d Size: %d Current token changed to: %s\n", ip, tk->size, current_token.lexeme);
                    if (ip < tk->size)
                    {
                        current_token = tk->array[ip];
                        current_terminal = convertToken(current_token);
                        // printf("New token: %s", current_token.lexeme);
                    }
                    else
                    {
                        current_terminal = TERM_EOF;
                    }
                    // terminal value does not match current token (syntax error)
                }
                else
                {
                    // printf("Syntax Error: expected token '%d' but found '%s Current Terminal %d'\n", top.value, current_token.lexeme, current_terminal);
                    syntax_error(top.value, current_token.lexeme, current_terminal, ip);
                    if (ip < tk->size)
                    {
                        ip = ip + 1;
                        skipWhitespace(tk, &ip);
                        // printf("Ip incremented\n");
                        current_token = tk->array[ip];
                        // printf("1: Ip: %d Size: %d Current token changed to: %s\n", ip, tk->size, current_token.lexeme);
                        current_terminal = convertToken(current_token);
                        // printf("New token: %s", current_token.lexeme);
                    }
                }
                // found a non-terminal
            }
            else
            {
                int nt = top.value;
                int t = current_terminal;
                const char *prod = ll1_table[nt][t].production;
                // printf("Production: %s\n", prod);
                if (prod == NULL)
                {
                    if (ll1_table[nt][TERM_EP].production == "ε")
                    {
                        pop(stack);
                    }
                    else
                    {
                        // printf("Syntax Error: expected token '%d' but found '%s Current Terminal %d'\n", top.value, current_token.lexeme, current_terminal);
                        syntax_error(top.value, current_token.lexeme, current_terminal, ip);
                        if (ip < tk->size)
                        {
                            ip = ip + 1;
                            skipWhitespace(tk, &ip);
                            // printf("Ip incremented\n");
                            current_token = tk->array[ip];
                            // printf("1: Ip: %d Size: %d Current token changed to: %s\n", ip, tk->size, current_token.lexeme);
                            current_terminal = convertToken(current_token);
                            // printf("New token: %s", current_token.lexeme);
                        }
                    }
                }
                else
                {
                    // printf("Applying production: %s\n", prod);
                    pop(stack);
                    if (strcmp(prod, "ε") != 0)
                    {
                        Symbol prod_symbols[MAX_PROD_TOKENS];
                        int count = parse_production(prod, prod_symbols, MAX_PROD_TOKENS);
                        for (int i = count - 1; i >= 0; i--)
                        {
                            push(stack, prod_symbols[i]);
                        }
                    }
                }
            }
        }
    }
    if (current_terminal == TERM_EOF && ip == tk->size)
        printf("Input successfully parsed.\n");
    else
    {
        FILE *errorFile = fopen("./errors.txt", "a");
        fprintf(errorFile, "Syntax Error: input not fully consumed");
        fclose(errorFile);
    }
}

/*
 *   Main Function
 */
void printTokenArray(TOKEN_ARRAY *tk)
{
    printf("Printing Token Array:\n");
    for (int i = 0; i < tk->size; i++)
    {
        printf("Token: %s\n", tk->array[i].lexeme);
    }
}

int main()
{
    FILE *errorFile = fopen("./errors.txt", "w");
    fprintf(errorFile, ""); // Wipe the error File
    fclose(errorFile);
    printf("Lexical Analysis\n-----------------------------------------------------\n");
    TOKEN_ARRAY *tk = LexicalAnalysis("Test6.cp");
    STACK *stack = malloc(sizeof(STACK));
    printf("init stack\n");
    initializeStack(stack);
    // printTokenArray(tk);
    printf("Syntax Analysis\n------------------------------------------------------- %d\n", tk->size);
    // printf("End of token stream: %s", tk->array[tk->size].lexeme);
    SyntaxAnalysis(tk, stack);
}