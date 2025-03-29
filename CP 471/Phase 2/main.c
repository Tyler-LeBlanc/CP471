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
Phase 3 Code:
*/

typedef struct
{
    char *name;
    int type;
    int returnType;
    int function;
    int paramCount;
    int *params;
} GLOBAL_ITEM;

typedef struct
{
    GLOBAL_ITEM *decs;
    int size;
    int capacity;
} GLOBAL_SCOPE;

typedef struct
{
    char *varName; // variable name
    int type;      // 0 = int 1 = dub
} LOCAL_ITEM;

typedef struct
{
    LOCAL_ITEM *decs; // type and name of variable
    char *funcName;   // function name this belongs to
    int size;
    int capacity;
} LOCAL_LOCATION;
typedef struct
{
    LOCAL_LOCATION *functions; // array of function variables
    int size;
    int capacity;
} LOCAL_SCOPE;
int lineCounter = 1;
void getNextToken(TOKEN_ARRAY *tk, int *index)
{
    while (strcmp(tk->array[*index].lexeme, "\n") == 0 || strcmp(tk->array[*index].lexeme, " ") == 0 || strcmp(tk->array[*index].lexeme, "\t") == 0)
    {
        if (strcmp(tk->array[*index].lexeme, "\n") == 0)
        {
            lineCounter = lineCounter + 1;
        }
        (*index) = (*index) + 1; // skip over empty tokens
    }
}
void PrintGlobal(GLOBAL_SCOPE *globalTable)
{
    printf("\n%-20s %-10s %-15s %-12s %-13s %-10s\n",
           "Name", "Type", "Return Type", "Function", "Param Count", "Params");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < globalTable->size; i++)
    {
        printf("%-20s %-10d %-15d %-12d %-13d ",
               globalTable->decs[i].name,
               globalTable->decs[i].type,
               globalTable->decs[i].returnType,
               globalTable->decs[i].function,
               globalTable->decs[i].paramCount);

        for (int j = 0; j < globalTable->decs[i].paramCount; j++)
        {
            printf("%d, ", globalTable->decs[i].params[j]);
        }
        printf("\n");
    }
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
}
void addGlobalVariable(char *varName, int type, int returnType, int isFunction, int paramsCount, int *params, GLOBAL_SCOPE *globalTable)
{
    GLOBAL_ITEM *newDec = &globalTable->decs[globalTable->size]; // get next avalible slot in the global table
    newDec->name = varName;
    newDec->type = type;       // we get type at the start
    newDec->returnType = type; // return type for a variable will just be it's type.
    newDec->function = isFunction;
    newDec->paramCount = paramsCount;
    if (paramsCount > 0)
    {
        newDec->params = params; // This will just be an empty array
    }
    else
    {
        newDec->params = NULL;
    }
    if (isFunction == 1) // if this is a function tell me!
    {
        // printf("Added function: %s to global table with parameter count %d\n", newDec->name, newDec->paramCount);
    }
    else
    {
        // printf("Added variable: %s to global table with type %d\n", newDec->name, newDec->type);
    }
    globalTable->size = globalTable->size + 1; // increment the size;
    if (globalTable->size == globalTable->capacity)
    {
        globalTable->capacity = globalTable->capacity * 2;                                           // double the capacity
        globalTable->decs = realloc(globalTable->decs, globalTable->capacity * sizeof(GLOBAL_ITEM)); // re allocate more data doubling the size
        printf("Allocated more size for the global table\n");
    }
}
void PrintLocal(LOCAL_SCOPE *localTable)
{
    for (int a = 0; a < localTable->size; a++)
    {
        printf("%s Table:\n", localTable->functions[a].funcName);
        for (int b = 0; b < localTable->functions[a].size; b++)
        {
            printf("| %s %d ", localTable->functions[a].decs[b].varName, localTable->functions[a].decs[b].type);
        }
        printf("|\n");
    }
}
int FunctionExistInLocal(LOCAL_SCOPE *localTable, char *funcName)
{
    for (int i = 0; i < localTable->size; i++)
    {
        if (strcmp(localTable->functions[i].funcName, funcName) == 0)
        {
            return i; // return the index the function is stored at
        }
    }
    return -1; // doesn't exist
}
void addLocalVariable(char *funcName, char *varName, int type, LOCAL_SCOPE *localTable)
{
    // init new local variable
    int exist = 0;
    LOCAL_LOCATION *location;
    exist = FunctionExistInLocal(localTable, funcName); // Check to see if this function has an entry inside the table
    if (exist == -1)
    {
        location = &localTable->functions[localTable->size]; // next open slot in the table this creates a new local function essentially
        location->capacity = 10;
        location->decs = malloc(location->capacity * sizeof(LOCAL_ITEM));
        location->funcName = strdup(funcName);
        location->size = 0;
        location->capacity = 10;
        localTable->size = localTable->size + 1;
    }
    else
    {
        location = &localTable->functions[exist];
        // printf("Function already defined in local table %s Size %d\n", location->funcName, location->size);
    }
    LOCAL_ITEM *localItem = &location->decs[location->size]; // next open location to declare a new variable for this function name
    location->size = location->size + 1;
    localItem->type = type;
    localItem->varName = strdup(varName);
    // localTable->functions->size = localTable->functions->size + 1;
    if (localTable->size == localTable->capacity)
    {
        localTable->capacity = localTable->capacity * 2;
        localTable->functions = realloc(localTable->functions, localTable->capacity * sizeof(LOCAL_LOCATION));
    }
    if (location->size == location->capacity)
    {
        location->capacity = location->capacity * 2;
        location->decs = realloc(location->decs, location->capacity * sizeof(LOCAL_ITEM)); // alloc more mem for full structure
    }
    // printf("Found local variable: %s Inside function: %s\n", varName, funcName);
}

int SearchGlobalTable(GLOBAL_SCOPE *globaleTable, char *name) // take in table and var or func name I am looking for
{
    for (int i = 0; i < globaleTable->size; i++)
    {
        if (strcmp(globaleTable->decs[i].name, name) == 0) // if we found what we're looking for in the global table
        {
            return globaleTable->decs[i].type; // return that we found it and gimme the type >:|
        }
    }
    return -1; // no we didnt find it.
}
int SearchLocalTable(LOCAL_SCOPE *localTable, char *funcName, char *varName)
{
    // printf("Searching local table %s for %s\n", funcName, varName);
    for (int func = 0; func < localTable->size; func++)
    {
        if (strcmp(localTable->functions[func].funcName, funcName) == 0)
        { // if the function is in the local table
            // printf("Found %s Table\n", funcName);
            for (int dec = 0; dec < localTable->functions[func].size; dec++)
            {                                                                            // iterate over all declarations in this function
                if (strcmp(localTable->functions[func].decs[dec].varName, varName) == 0) // if we find the var name
                {
                    return localTable->functions[func].decs[dec].type; // found it now gimme the type either 0 or 1
                }
            }
        }
    }
    return -1;
}
void GetTokensAndAddGlobal(int *index, TOKEN_ARRAY *tk, int type, int insideFunction, int paramsCount, int *params, GLOBAL_SCOPE *globalTable) // This function is kind of unessesary but cuts down a bit on lines of code it mainly just checks for global variables
{
    char *varName;
    *index = *index + 1;
    getNextToken(tk, index);                               // The next token should be some identifier so I can get it's name
    if (strcmp(tk->array[*index].type, "IDENTIFIER") == 0) // If I did get an identifyer (var name)
    {
        varName = tk->array[*index].lexeme; // Get the variable name. I know this is a variable name as the prev token was a keyword (int||dub) and this is a identifier.
        int searchResult = SearchGlobalTable(globalTable, varName);
        //  printf("Done search for %s result %d\n", varName, searchResult);
        if (searchResult == -1)
        {
            // printf("did not find %s in global \n", varName);
            addGlobalVariable(varName, type, type, insideFunction, paramsCount, params, globalTable);
        }
        else
        {
            printf("Semantic error On line %d declaration %s has already been defined\n", lineCounter, varName);
        }
    }
    *index = *index + 1;
    getNextToken(tk, index);
    if (strcmp(tk->array[*index].lexeme, ",") == 0) // If I find a comma after ward
    {
        while (strcmp(tk->array[*index].lexeme, ",") == 0) // While we see commas
        {
            GetTokensAndAddGlobal(index, tk, type, insideFunction, paramsCount, params, globalTable);
        }
    }
}
void GetTokensAndAddLocal(int *index, TOKEN_ARRAY *tk, char *funcName, int type, LOCAL_SCOPE *localTable)
{
    char *varName = "";
    *index = *index + 1;
    getNextToken(tk, index);
    if (strcmp(tk->array[*index].type, "IDENTIFIER") == 0)
    {
        varName = tk->array[*index].lexeme; // if the next token is a delimeter than it is the variable name
        int searchResult = SearchLocalTable(localTable, funcName, varName);
        //  printf("Done search for %s result %d\n", varName, searchResult);
        if (searchResult == -1)
        {
            // printf("did not find %s in global \n", varName);
            addLocalVariable(funcName, varName, type, localTable);
        }
        else
        {
            printf("Semantic error On line %d declaration %s has already been defined\n", lineCounter, varName);
        }
    }
    *index = *index + 1;
    getNextToken(tk, index);
    if (strcmp(tk->array[*index].lexeme, ",") == 0) // If I find a comma after ward
    {
        while (strcmp(tk->array[*index].lexeme, ",") == 0) // While we see commas
        {
            GetTokensAndAddLocal(index, tk, funcName, type, localTable);
        }
    }
}
int FindDec(int inFunction, char *funcName, int index, TOKEN_ARRAY *tk, GLOBAL_SCOPE *globalTable, LOCAL_SCOPE *localTable, char blockNames[][20], int insideBlock)
{
    int searchResult = SearchGlobalTable(globalTable, tk->array[index].lexeme); // search global
    if (searchResult == -1 && inFunction == 1)
    {                                                                                   // If we didn't find the function and are in a function
        searchResult = SearchLocalTable(localTable, funcName, tk->array[index].lexeme); // check the local table
        if (searchResult == -1 && insideBlock != -1)
        {
            searchResult = SearchLocalTable(localTable, blockNames[insideBlock], tk->array[index].lexeme);
        }
    }
    return searchResult;
}
void AnalyzeType(int inFunction, int index, char *funcName, TOKEN_ARRAY *tk, GLOBAL_SCOPE *globalTable, LOCAL_SCOPE *localTable, char blockNames[][20], int insideBlock)
{
    int tempIndex = index;
    char *verifType = "";
    int typeNum = -1;
    if (strcmp(tk->array[tempIndex].type, "IDENTIFIER") == 0) // if we get an identifier;
    {
        // printf("Found Id %s", tk->array[tempIndex].lexeme);
        int searchResult = FindDec(inFunction, funcName, tempIndex, tk, globalTable, localTable, blockNames, insideBlock);
        if (searchResult != -1)
        {                          // if we found it in the table
            if (searchResult == 0) // search returns the type of the variable so we now know what type we need to ensure is the same.
            {
                verifType = "INTEGER";
                typeNum = 0;
            }
            else
            {
                verifType = "DOUBLE";
                typeNum = 1;
            }
            tempIndex = tempIndex + 1; // now that we have the type lets get the next token
            getNextToken(tk, &tempIndex);
            if (strcmp(tk->array[tempIndex].type, "ASSIGNMENT_OPERATOR") == 0 || strcmp(tk->array[tempIndex].type, "COMPARISON_OPERATOR") == 0)
            {                              // If we get an equal sign (setting var to some value)
                                           // printf(" found =\n");
                tempIndex = tempIndex + 1; // now that we have the type lets get the next token
                getNextToken(tk, &tempIndex);
                while (1)
                {

                    if (strcmp(tk->array[tempIndex].type, "IDENTIFIER") == 0) // if we are setting this var to another var
                    {
                        // printf("var is being set equal to %d", tk->array[tempIndex].type);
                        int searchResult = FindDec(inFunction, funcName, tempIndex, tk, globalTable, localTable, blockNames, insideBlock); // find the identifier
                        if (searchResult != typeNum)                                                                                       // type mismatch
                        {
                            printf("Type mismatch with %s and %s on line %d\n", tk->array[index].lexeme, tk->array[tempIndex].lexeme, lineCounter);
                        }
                    }
                    else if (strcmp(tk->array[tempIndex].type, "INTEGER") == 0) // if we find an integer instead of a identifier
                    {
                        if (typeNum != 0)
                        {
                            printf("Type mismatch with %s and %s on line %d\n", tk->array[index].lexeme, tk->array[tempIndex].lexeme, lineCounter);
                        }
                    }
                    else if (strcmp(tk->array[tempIndex].type, "DOUBLE") == 0)
                    {
                        if (typeNum != 1)
                        {
                            printf("Type mismatch with %s and %s on line %d\n", tk->array[index].lexeme, tk->array[tempIndex].lexeme, lineCounter);
                        }
                    }
                    tempIndex = tempIndex + 1;
                    getNextToken(tk, &tempIndex);
                    // printf("Found a %s type: %s \n", tk->array[tempIndex].lexeme, tk->array[tempIndex].type);
                    if (strcmp(tk->array[tempIndex].type, "MATH_OPERATOR") != 0) // not an operator? Stop the loop
                    {
                        break;
                    }
                    else
                    {
                        tempIndex = tempIndex + 1;
                        getNextToken(tk, &tempIndex);
                    }
                }
            }
            else if (strcmp(tk->array[tempIndex].lexeme, "(") == 0) // if we have dec() then we're calling a function of some kind
            {
                tempIndex = tempIndex + 1;
                getNextToken(tk, &tempIndex); // Let's get the next token (should be an identifier or number)
                int paramCount = 0;
                int countingParams = 0;
                int location = 0;
                for (int i = 0; i < globalTable->size; i++)
                {                                                                        // iterate through, lets get the parameter count for this function
                    if (strcmp(globalTable->decs[i].name, tk->array[index].lexeme) == 0) // if we find the function we're looking for inside the global table
                    {
                        paramCount = globalTable->decs[i].paramCount; // get the parameter count;
                        location = i;
                    }
                }
                while (strcmp(tk->array[tempIndex].lexeme, ")") != 0) // while inside function parameter setting thingy stuff
                {
                    int searchResult = -1;
                    if (strcmp(tk->array[tempIndex].type, "IDENTIFIER") == 0)
                    {                                                                                                                  // if it's an identifier
                        searchResult = FindDec(inFunction, funcName, tempIndex, tk, globalTable, localTable, blockNames, insideBlock); // find it in the table
                    }
                    else
                    { // if it's not a variable but it is a number
                        int checkType = -1;
                        if (strcmp(tk->array[tempIndex].type, "INTEGER") == 0)
                        {
                            searchResult = 0;
                        }
                        else if (strcmp(tk->array[tempIndex].type, "DOUBLE") == 0)
                        {
                            searchResult = 1;
                        }
                    } // then manually check and set the search result

                    if (searchResult != globalTable->decs[location].params[countingParams])
                    { // if the type doesn't match
                        printf("Type mismatch with %s and %s on line %d\n", tk->array[index].lexeme, tk->array[tempIndex].lexeme, lineCounter);
                    }
                    tempIndex = tempIndex + 1;
                    getNextToken(tk, &tempIndex);
                    while (strcmp(tk->array[tempIndex].lexeme, ",") == 0) // skip commas
                    {
                        tempIndex = tempIndex + 1;
                        getNextToken(tk, &tempIndex);
                    }
                    countingParams = countingParams + 1;
                }
                if (countingParams != paramCount)
                {
                    printf("Invalid number of parameters for function %s expected %d found %d on line %d\n", tk->array[index].lexeme, paramCount, countingParams, lineCounter);
                }
            }
        }
    }
    else if (strcmp(tk->array[tempIndex].lexeme, "return") == 0)
    { // check for return statement
        tempIndex = tempIndex + 1;
        getNextToken(tk, &tempIndex); // get next token
        if (strcmp(tk->array[tempIndex].type, "IDENTIFIER") == 0)
        { // if it's an identifier after return
            int varType = FindDec(inFunction, funcName, tempIndex, tk, globalTable, localTable, blockNames, insideBlock);
            int returnType;
            for (int fun = 0; fun < globalTable->size; fun++)
            {
                if (strcmp(globalTable->decs[fun].name, funcName) == 0)
                { // find the function in the global and check the return type
                    returnType = globalTable->decs[fun].returnType;
                    break; // end loop
                }
            }
            if (returnType != varType)
            {
                printf("Type mismatch in %s function return %s line %d\n", funcName, tk->array[index].lexeme, lineCounter);
            }
        }
    }
}

void SemanticAnalysis(TOKEN_ARRAY *tk)
{
    // Initalization for local and global table --
    LOCAL_SCOPE *localTable = malloc(sizeof(GLOBAL_SCOPE));
    localTable->capacity = 10;
    localTable->functions = malloc(localTable->capacity * sizeof(LOCAL_LOCATION)); // init the functions array
    localTable->functions->capacity = 10;
    localTable->functions->size = 0;
    localTable->size = 0;
    GLOBAL_SCOPE *globalTable = malloc(sizeof(GLOBAL_SCOPE)); // Create the global table (array of global items)
    globalTable->capacity = 10;
    globalTable->decs = malloc(globalTable->capacity * sizeof(GLOBAL_ITEM)); // allow for 10 items to be added to the global table
    globalTable->size = 0;
    //-- End of global and local table init
    int index = 0;
    int insideFunction = 0; // False = 0, true = 1
    char *funcName = "";    // Current name of the function we are inside
    int inParams = 0;       // False = 0, true = 1
    int paramsCount = 0;
    int buildingFunction = 0; // used to see if we are constructing a function declaration for global
    int type;
    int ifCount = 0;
    int whileCount = 0;
    int elseCount = 0;
    int returnType;
    int insideBlock = -1;
    char blockNames[100][20]; // Stores block names if0, while0, else0 etc etc.
    printf("Starting semantic analysis\n");
    while (index < tk->size) // while inside the array
    {
        getNextToken(tk, &index); // ensure the start of the array isnt nothing
        AnalyzeType(insideFunction, index, funcName, tk, globalTable, localTable, blockNames, insideBlock);
        TOKEN currentToken = tk->array[index]; // get first token
        // Start of adding functions to global table--
        if (strcmp(currentToken.lexeme, "def") == 0) // if found func dec
        {
            buildingFunction = 1;
            insideFunction = 1; // we are inside of a function
            index = index + 1;  // The next token, assuming proper syntax, should be the function type {def int funcName(params)}
            getNextToken(tk, &index);
            if (strcmp(tk->array[index].type, "KEYWORD") == 0)
            {
                if (strcmp(tk->array[index].lexeme, "int") == 0)
                {
                    type = 0;
                    returnType = 0;
                }
                else if (strcmp(tk->array[index].lexeme, "double") == 0)
                {
                    type = 1;
                    returnType = 1;
                }
                index = index + 1; // The next token, should be the function name {def int funcName(params)}
                getNextToken(tk, &index);
                if (strcmp(tk->array[index].type, "IDENTIFIER") == 0)
                {
                    funcName = tk->array[index].lexeme;
                }
            }

            // printf("Inside a function with function name: %s\n", funcName);
        }
        else if (strcmp(currentToken.lexeme, "fed") == 0)
        {
            insideFunction = 0; // function declaration over, no longer inside a function
            inParams = 0;       // reset parameters
            // printf("Left the function %s", funcName);
        }
        else if (strcmp(currentToken.lexeme, "if") == 0 || strcmp(currentToken.lexeme, "while") == 0 || strcmp(currentToken.lexeme, "else") == 0)
        {
            if (strcmp(currentToken.lexeme, "else") != 0)
            { // not an else than increment this counter
                insideBlock = insideBlock + 1;
            }
            // printf("******Found an if while or else\n");
            if (strcmp(currentToken.lexeme, "if") == 0)
            {
                snprintf(blockNames[insideBlock], sizeof(blockNames[insideBlock]), "if%d\0", ifCount);
                ifCount = ifCount + 1;
            }
            else if (strcmp(currentToken.lexeme, "while") == 0)
            {
                snprintf(blockNames[insideBlock], sizeof(blockNames[insideBlock]), "while%d\0", whileCount);
                // printf("Found a while and named it %s\n", blockNames[insideBlock]);
                whileCount = whileCount + 1;
            }
            else
            {
                printf("Was named %s", blockNames[insideBlock]);
                snprintf(blockNames[insideBlock], sizeof(blockNames[insideBlock]), "else%d\0", elseCount);
                printf("Found a else and named it %s\n", blockNames[insideBlock]);
                elseCount = elseCount + 1;
            }
            printf("Keyword Name %s at index %d\n", blockNames[insideBlock], insideBlock);
            // addLocalVariable(blockNames[insideBlock], "None", 0, localTable);
        }
        else if (strcmp(currentToken.lexeme, "fi") == 0 || strcmp(currentToken.lexeme, "od") == 0)
        {
            insideBlock = insideBlock - 1;
        }
        if (insideFunction == 1 && strcmp(tk->array[index].lexeme, "(") == 0 && inParams == 0) // If we're in some function and we're seeing a open bracket for the first time.
        {                                                                                      // Then this is the parameter declaration
            paramsCount = 0;
            //("Inside parameter statement\n");
            int *params = malloc(10 * sizeof(int));
            inParams = 1; // Ensure we don't start adding parmeters for a while or if statement
            char *lastType = "";
            index = index + 1;
            while (strcmp(tk->array[index].lexeme, ")") != 0)
            {                                                            // While inside parameter declaration
                while (strcmp(tk->array[index].type, "IDENTIFIER") != 0) // go to the identifier
                {
                    if (strcmp(tk->array[index].type, "KEYWORD") == 0)
                    {
                        if (strcmp(tk->array[index].lexeme, "int") == 0)
                        {
                            params[paramsCount] = 0;
                            lastType = "int";
                        }
                        else if (strcmp(tk->array[index].lexeme, "double") == 0)
                        {
                            lastType = "double";
                            params[paramsCount] = 1;
                        }
                    }
                    index = index + 1;
                    getNextToken(tk, &index);
                }
                if (strcmp(lastType, "") != 0)
                {
                    addLocalVariable(funcName, tk->array[index].lexeme, params[paramsCount], localTable); // Add parameters to the local table for the appropriate function
                    // printf("Found Parameter: %s with type: %s\n", tk->array[index].lexeme, lastType);
                    paramsCount = paramsCount + 1; // increment count;
                }
                index = index + 1;
            }
            buildingFunction = 0;
            // printf("Found end of parameters\n");
            // GetTokensAndAddGlobal(&index, tk, type, insideFunction, paramsCount, params, globalTable);
            int searchResult = SearchGlobalTable(globalTable, funcName);
            if (searchResult == -1)
            {
                addGlobalVariable(funcName, type, returnType, insideFunction, paramsCount, params, globalTable);
            }
            else
            {
                printf("Semantic error On line %d declaration %s has already been defined\n", lineCounter, funcName);
            }
        }
        else if (insideFunction == 0 && insideBlock == -1)
        {              // Not inside of a function dec, aka if we're in global scope
            type = -1; // set to null essentially
            char *varName = "";
            int *params = malloc(10 * sizeof(int));
            int paramsCount = 0;                             // I know there will be no parameters for a variable, and I'm sure this isn't a function declaration {no brackets () and not inside a function}
            if (strcmp(tk->array[index].lexeme, "int") == 0) // if the definition of some variable
            {
                type = 0;
            }
            else if (strcmp(tk->array[index].lexeme, "double") == 0) // if it's a double instead of an int
            {
                type = 1;
            }
            if (type >= 0)
            { // if we found a declaration of some identifier
                GetTokensAndAddGlobal(&index, tk, type, insideFunction, paramsCount, params, globalTable);
            }
        }
        else if (insideBlock != -1) // if we are inside a block of some kind if else or while
        {
            type = -1;
            if (strcmp(tk->array[index].lexeme, "int") == 0) // if the definition of some variable
            {
                type = 0;
            }
            else if (strcmp(tk->array[index].lexeme, "double") == 0) // if it's a double instead of an int
            {
                type = 1;
            }
            if (type >= 0)
            {
                // printf("adding %s to local table Index: %d", blockNames[insideBlock], index);
                GetTokensAndAddLocal(&index, tk, blockNames[insideBlock], type, localTable);
                // printf("changed index %d\n", index);
            }
        }
        //--End of adding values to global table
        // Start of adding values to local table
        if (insideFunction == 1 && strcmp(tk->array[index].type, "KEYWORD") == 0) // if inside of a function and we've found some keyword
        {
            type = -1; // set type to null to avoid adding syntax errors like if x; as a var declaration.
            char *varName;
            if (strcmp(tk->array[index].lexeme, "int") == 0)
            {
                type = 0;
            }
            else if (strcmp(tk->array[index].lexeme, "double") == 0)
            {
                type = 1;
            }
            if (type >= 0)
            { // if of type int or double
                GetTokensAndAddLocal(&index, tk, funcName, type, localTable);
            }
        }
        //--End of adding values to local table
        // Start of checking the Scope and Type--
        if (strcmp(tk->array[index].type, "IDENTIFIER") == 0 && buildingFunction == 0)
        {
            int searchResult = 0;
            // printf("Checking global table for: %s\n", tk->array[index].lexeme);
            searchResult = SearchGlobalTable(globalTable, tk->array[index].lexeme); // is this variable defined? It should be
            if (searchResult != -1)
            {
                printf("Found %s in global table\n", tk->array[index].lexeme);
            }
            else // Maybe a semantic error, but if we are inside a function we need to check the local table
            {
                // printf("No %s in global table\n", tk->array[index].lexeme);
                if (insideFunction == 1)
                {
                    // printf("inside a function checking the %s table for %s\n", funcName, tk->array[index].lexeme);
                    searchResult = SearchLocalTable(localTable, funcName, tk->array[index].lexeme);
                    if (insideBlock != -1 && searchResult == -1) // if we are in a block and the variable doesn't exist in this function
                    {
                        searchResult = SearchLocalTable(localTable, blockNames[insideBlock], tk->array[index].lexeme);
                    }
                    // if (searchResult == 1)
                    // {
                    //     printf("Found local variable %s\n", tk->array[index].lexeme);
                    // }
                    if (searchResult == -1)
                    {

                        printf("Semantic error %s does not exist at this scope %s on line %d\n", tk->array[index].lexeme, funcName, lineCounter);
                    }
                }
                else if (insideBlock != -1) // if we are inside a block but not inside a function check local table for the block name
                {
                    searchResult = SearchLocalTable(localTable, blockNames[insideBlock], tk->array[index].lexeme);
                    if (searchResult == -1)
                    {

                        printf("Semantic error %s does not exist at this scope %s on line %d\n", tk->array[index].lexeme, funcName, lineCounter);
                    }
                }
                else
                {
                    printf("Semantic error %s does not exist at this scope on line %d\n", tk->array[index].lexeme, lineCounter);
                }
            }
        }
        //--End of checking the Scope and Type
        index = index + 1; // point to the next token
    }
    PrintGlobal(globalTable);
    PrintLocal(localTable);
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
    TOKEN_ARRAY *tk = LexicalAnalysis("Test10.cp");
    STACK *stack = malloc(sizeof(STACK));
    printf("init stack\n");
    initializeStack(stack);
    // printTokenArray(tk);
    printf("Syntax Analysis\n------------------------------------------------------- \n");
    // printf("End of token stream: %s", tk->array[tk->size].lexeme);
    // SyntaxAnalysis(tk, stack);
    printf("Semantic Analysis\n------------------------------------------------------- \n");
    SemanticAnalysis(tk);
}