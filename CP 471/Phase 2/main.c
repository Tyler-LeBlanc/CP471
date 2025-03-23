#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LexicalAnalysis.h"
#include "AST.h"
#include <ctype.h>

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

TOKEN_ARRAY *LexicalAnalysis(char *filename)
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
#define MAX_RULES 305
#define CHILD_ARRAY_SIZE 35

// So the heap has the form non-terminal, terminal and production. It's a little confusion and awful to read
// If there is no terminal and it is just a non-terminal, then the terminal slot is the first char of the non-terminal it leads to.
// If there is a terminal then the terminal is just the terminal and the non terminal is epsilon.

ParsingRule parsing_table[MAX_RULES] = {
    {"program", "f", "fdecls decs st_seq ."},
    {"fdecls", "f", "fdec fdecls_"},
    {"fdecls_", "int", "ε"},
    {"fdecls_", ";", "; fdec fdecls_"},
    {"fdecls_", "double", "ε"},
    {"fdec", "d", "def type fname ( params ) decs st_seq fed"},
    {"params", "t", "type var params_"},
    {"params_", ")", "ε"},
    {"params_", ",", ", type var params_"},
    {"fname", "i", "id"},
    {"decs", "d", "decl decs_"},
    {"decs_", "w", "ε"},
    {"decs_", "s", "ε"},
    {"decs_", "while", "ε"},
    {"decs_", "l", "ε"},
    {"decs_", "r", "ε"},
    {"decs_", "if", "ε"},
    {"decs_", "f", "ε"},
    {"decs_", "x", "ε"},
    {"decs_", ";", "ε"},
    {"decs_", "k", "ε"},
    {"decs_", "z", "ε"},
    {"decs_", "h", "ε"},
    {"decs_", "u", "ε"},
    {"decs_", "a", "ε"},
    {"decs_", "g", "ε"},
    {"decs_", "return", "ε"},
    {"decs_", "y", "ε"},
    {"decs_", "j", "ε"},
    {"decs_", "d", "ε"},
    {"decs_", "v", "ε"},
    {"decs_", "e", "ε"},
    {"decs_", "t", "ε"},
    {"decs_", "print", "ε"},
    {"decs_", "o", "ε"},
    {"decs_", "b", "ε"},
    {"decs_", "fed", "ε"},
    {"decs_", "q", "ε"},
    {"decs_", "n", "ε"},
    {"decs_", "i", "ε"},
    {"decs_", "m", "ε"},
    {"decs_", "p", "ε"},
    {"decs_", ".", "ε"},
    {"decs_", "c", "ε"},
    {"decl", "t", "type varlist"},
    {"type", "d", "double"},
    {"type", "i", "int"},
    {"varlist", "v", "var , varlist_"},
    {"varlist_", "w", "ε"},
    {"varlist_", "s", "ε"},
    {"varlist_", "while", "ε"},
    {"varlist_", "l", "ε"},
    {"varlist_", "r", "ε"},
    {"varlist_", "if", "ε"},
    {"varlist_", "f", "ε"},
    {"varlist_", "x", "ε"},
    {"varlist_", ";", "ε"},
    {"varlist_", "k", "ε"},
    {"varlist_", "z", "ε"},
    {"varlist_", "h", "ε"},
    {"varlist_", "u", "ε"},
    {"varlist_", "a", "ε"},
    {"varlist_", "g", "ε"},
    {"varlist_", "return", "ε"},
    {"varlist_", "y", "ε"},
    {"varlist_", "j", "ε"},
    {"varlist_", "d", "ε"},
    {"varlist_", "v", "ε"},
    {"varlist_", "e", "ε"},
    {"varlist_", "t", "ε"},
    {"varlist_", "print", "ε"},
    {"varlist_", "o", "ε"},
    {"varlist_", "b", "ε"},
    {"varlist_", "fed", "ε"},
    {"varlist_", "q", "ε"},
    {"varlist_", "n", "ε"},
    {"varlist_", "i", "ε"},
    {"varlist_", "m", "ε"},
    {"varlist_", "p", "ε"},
    {"varlist_", ".", "ε"},
    {"varlist_", "c", "ε"},
    {"varlist_", ",", ", var varlist_"},
    {"st_seq", "s", "statement ; st_seq"},
    {"statement", "w", "while bexpr do st_seq od"},
    {"statement", "else", "ε"},
    {"statement", "r", "return expr"},
    {"statement", "fi", "ε"},
    {"statement", ";", "ε"},
    {"statement", "v", "var = expr"},
    {"statement", "fed", "ε"},
    {"statement", "od", "ε"},
    {"statement", "i", "if bexpr then st_seq st_seq_ fi"},
    {"statement", "p", "print expr"},
    {"statement", ".", "ε"},
    {"st_seq_", "fi", "ε"},
    {"st_seq_", "e", "else st_seq"},
    {"expr", "t", "term expr_"},
    {"expr_", "else", "ε"},
    {"expr_", "==", "ε"},
    {"expr_", "fi", "ε"},
    {"expr_", "do", "ε"},
    {"expr_", ";", "ε"},
    {"expr_", "and", "ε"},
    {"expr_", "-", "- term expr_"},
    {"expr_", ">=", "ε"},
    {"expr_", ">", "ε"},
    {"expr_", "fed", "ε"},
    {"expr_", "od", "ε"},
    {"expr_", "then", "ε"},
    {"expr_", "<=", "ε"},
    {"expr_", "or", "ε"},
    {"expr_", "]", "ε"},
    {"expr_", "<", "ε"},
    {"expr_", "+", "+ term expr_"},
    {"expr_", ")", "ε"},
    {"expr_", ".", "ε"},
    {"expr_", ",", "ε"},
    {"expr_", "<>", "ε"},
    {"term", "f", "factor term_"},
    {"term_", "else", "ε"},
    {"term_", "==", "ε"},
    {"term_", "fi", "ε"},
    {"term_", "do", "ε"},
    {"term_", ";", "ε"},
    {"term_", "*", "* factor term_"},
    {"term_", "and", "ε"},
    {"term_", "-", "ε"},
    {"term_", ">=", "ε"},
    {"term_", ">", "ε"},
    {"term_", "/", "/ factor term_"},
    {"term_", "fed", "ε"},
    {"term_", "od", "ε"},
    {"term_", "then", "ε"},
    {"term_", "%", "% factor term_"},
    {"term_", "<=", "ε"},
    {"term_", "or", "ε"},
    {"term_", "]", "ε"},
    {"term_", "<", "ε"},
    {"term_", "+", "ε"},
    {"term_", ")", "ε"},
    {"term_", ".", "ε"},
    {"term_", ",", "ε"},
    {"term_", "<>", "ε"},
    {"exprseq", "e", "expr , exprseq_"},
    {"exprseq_", ")", "ε"},
    {"exprseq_", ",", ", expr exprseq_"},
    {"bexpr", "b", "bterm bexpr_"},
    {"bexpr_", "do", "ε"},
    {"bexpr_", "o", "or bfactor bterm_"},
    {"bexpr_", "then", "ε"},
    {"bexpr_", ")", "ε"},
    {"bterm", "b", "bfactor bterm_"},
    {"bterm_", "do", "ε"},
    {"bterm_", "a", "and bfactor bterm_"},
    {"bterm_", "then", "ε"},
    {"bterm_", "or", "ε"},
    {"bterm_", ")", "ε"},
    {"bfactor", "e", "expr comp expr"},
    {"bfactor", "n", "not bfactor"},
    {"bfactor", "(", "( bexpr )"},
    {"comp", ">", ">="},
    {"comp", "=", "=="},
    {"comp", "<", "<>"},
    {"var", "i", "id [ expr ]"},
    {"id", "l", "letter id_"},
    {"id_", "w", "ε"},
    {"id_", "s", "ε"},
    {"id_", "while", "ε"},
    {"id_", "else", "ε"},
    {"id_", "l", "ε"},
    {"id_", "r", "ε"},
    {"id_", "if", "ε"},
    {"id_", "[", "ε"},
    {"id_", "int", "ε"},
    {"id_", "fi", "ε"},
    {"id_", "f", "ε"},
    {"id_", "do", "ε"},
    {"id_", "x", "ε"},
    {"id_", ";", "ε"},
    {"id_", "*", "ε"},
    {"id_", "k", "ε"},
    {"id_", "z", "ε"},
    {"id_", "and", "ε"},
    {"id_", "h", "ε"},
    {"id_", "u", "ε"},
    {"id_", "a", "ε"},
    {"id_", "g", "ε"},
    {"id_", "-", "ε"},
    {"id_", ">=", "ε"},
    {"id_", "return", "ε"},
    {"id_", "y", "ε"},
    {"id_", "j", "ε"},
    {"id_", "d", "ε"},
    {"id_", "v", "ε"},
    {"id_", "e", "ε"},
    {"id_", "t", "ε"},
    {"id_", "print", "ε"},
    {"id_", "o", "ε"},
    {"id_", ">", "ε"},
    {"id_", "/", "ε"},
    {"id_", "=", "ε"},
    {"id_", "b", "ε"},
    {"id_", "fed", "ε"},
    {"id_", "od", "ε"},
    {"id_", "q", "ε"},
    {"id_", "n", "ε"},
    {"id_", "then", "ε"},
    {"id_", "%", "ε"},
    {"id_", "(", "ε"},
    {"id_", "<=", "ε"},
    {"id_", "i", "ε"},
    {"id_", "m", "ε"},
    {"id_", "or", "ε"},
    {"id_", "]", "ε"},
    {"id_", "<", "ε"},
    {"id_", "+", "ε"},
    {"id_", "double", "ε"},
    {"id_", "p", "ε"},
    {"id_", ")", "ε"},
    {"id_", ".", "ε"},
    {"id_", "c", "ε"},
    {"id_", ",", "ε"},
    {"id_", "<>", "ε"},
    {"number", "i", "integer"},
    {"number", "D", "Double"},
    {"Double", "i", "integer . integer"},
    {"integer", "d", "digit integer_"},
    {"integer_", "fi", "ε"},
    {"integer_", "do", "ε"},
    {"integer_", ";", "ε"},
    {"integer_", "*", "ε"},
    {"integer_", "and", "ε"},
    {"integer_", "-", "ε"},
    {"integer_", ">=", "ε"},
    {"integer_", "d", "digit integer_"},
    {"integer_", ">", "ε"},
    {"integer_", "/", "ε"},
    {"integer_", "fed", "ε"},
    {"integer_", "od", "ε"},
    {"integer_", "then", "ε"},
    {"integer_", "%", "ε"},
    {"integer_", "<=", "ε"},
    {"integer_", "or", "ε"},
    {"integer_", "]", "ε"},
    {"integer_", "<", "ε"},
    {"integer_", "+", "ε"},
    {"integer_", ")", "ε"},
    {"integer_", ".", "ε"},
    {"integer_", ",", "ε"},
    {"integer_", "<>", "ε"},
    {"factor", "f", "fname ( exprseq )"},
    {"factor", "v", "var"},
    {"factor", "n", "number"},
    {"factor", "(", "( expr )"},
    {"letter", "w", "w"},
    {"letter", "s", "s"},
    {"letter", "l", "l"},
    {"letter", "r", "r"},
    {"letter", "f", "f"},
    {"letter", "x", "x"},
    {"letter", "k", "k"},
    {"letter", "z", "z"},
    {"letter", "h", "h"},
    {"letter", "u", "u"},
    {"letter", "a", "a"},
    {"letter", "g", "g"},
    {"letter", "y", "y"},
    {"letter", "j", "j"},
    {"letter", "d", "d"},
    {"letter", "v", "v"},
    {"letter", "e", "e"},
    {"letter", "t", "t"},
    {"letter", "o", "o"},
    {"letter", "b", "b"},
    {"letter", "q", "q"},
    {"letter", "n", "n"},
    {"letter", "i", "i"},
    {"letter", "m", "m"},
    {"letter", "p", "p"},
    {"letter", "c", "c"},
    {"digit", "1", "1"},
    {"digit", "5", "5"},
    {"digit", "9", "9"},
    {"digit", "7", "7"},
    {"digit", "2", "2"},
    {"digit", "8", "8"},
    {"digit", "4", "4"},
    {"digit", "6", "6"},
    {"digit", "0", "0"},
    {"digit", "3", "3"},
    {"digits_", "d", "digits_"},
    {"exponent", "E", "E sign digit digits_"},
    {"exponent", "e", "e sign digit digits_"},
    {"sign", "1", "ε"},
    {"sign", "5", "ε"},
    {"sign", "9", "ε"},
    {"sign", "-", "-"},
    {"sign", "7", "ε"},
    {"sign", "2", "ε"},
    {"sign", "8", "ε"},
    {"sign", "4", "ε"},
    {"sign", "+", "+"},
    {"sign", "6", "ε"},
    {"sign", "0", "ε"},
    {"sign", "3", "ε"},
};

void reverse(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

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
// use: push(&stack, char)
void push(STACK *stack, const char *value)
{
    if (isFull(stack))
    {
        // printf("Stack is full.\n");
        return;
    }
    // Allocate and copy the string to store on the stack
    stack->array[++stack->top] = strdup(value);
}

// Function to remove value from the stack
// use: pop(&stack, char)
char *pop(STACK *stack)
{
    if (isEmpty(stack))
    {
        printf("Stack is empty.");
        return NULL;
    }

    return stack->array[stack->top--];
}

// Function to check what the top value of the stack is
char *peek(STACK *stack)
{
    if (!isEmpty(stack))
    {
        return stack->array[stack->top];
    }

    return NULL;
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
//  use: insert(&root, char)
void add_child(NODE *parent, NODE *child)
{
    parent->children[parent->child_count] = child;
    parent->child_count++;
}

int getNextToken(TOKEN current_token, int token_index, TOKEN_ARRAY *tk)
{
    printf("Found the token: %s\n", current_token.lexeme);
    token_index += 1;
    current_token = tk->array[token_index];
    // printf("Changed the token to: %s\n", current_token.lexeme);
    while (strcmp(current_token.lexeme, " ") == 0 || strcmp(current_token.lexeme, "\n") == 0 || strcmp(current_token.lexeme, "\t") == 0)
    {
        token_index += 1;
        current_token = tk->array[token_index];
        // printf("Found whitespace incrementing the index %d\n", token_index);
    }
    return token_index;
}

/*F
 *   Phase 2 main function
 *   Runs the syntax analyzer
 */

NODE *SyntaxAnalysis(TOKEN_ARRAY *tk, STACK *stack)
{

    // push initial symbol onto the stack
    push(stack, "program");

    // initialize token index
    int token_index = 0;

    // create the root of the AST and add starting production
    NODE *root = createNode("Program");
    NODE *current_ast = root;

    // initilize buffer to seperate rules
    char *production_buffer[256];
    int temp = 0;

    // while (temp < tk->size)
    // {
    //     printf("Temp: %d vs 102 %s\n", temp, tk->array[temp].lexeme);
    //     temp += 1;
    // }
    // main loop, runs while stack has productions
    while (!isEmpty(stack) && token_index < tk->size)
    {
        // get the top of stack(current production)
        TOKEN current_token = tk->array[token_index];
        // printf("Size of the tokens: %d\n", tk->size);

        char *current_production = pop(stack);
        // printf("popped: %s From stack Looking For: %s With Type: %s\n", current_production, current_token.lexeme, current_token.type);
        if (strcmp(current_production, current_token.lexeme) == 0)
        {
            token_index = getNextToken(current_token, token_index, tk);
        }

        // get the current token object
        // printf("Trying to get parsing rule from LL(1) rules: %d\n", MAX_RULES);

        for (int i = 0; i < MAX_RULES; i++)
        { // traverse the whole heap looking for the current production
            // printf("Production found: %s\n", parsing_table[i].non_terminal);
            if (strcmp(parsing_table[i].non_terminal, current_production) == 0) // If we've found the current production
            {
                // printf("Production found: %s, Non-terminal: %s, Terminal: %s\n", parsing_table[i].production, parsing_table[i].non_terminal, parsing_table[i].terminal);
                if (strcmp(parsing_table[i].production, "ε") == 0) // found a terminal
                {
                    // printf("Terminal: %s\n", parsing_table[i].terminal);
                    //   if(strcmp(current_token.lexeme, ""))
                    if (strcmp(parsing_table[i].terminal, current_token.lexeme) == 0)
                    {
                        token_index = getNextToken(current_token, token_index, tk);
                    }
                    else
                    {
                        // printf("Syntax Error\n");
                        //   Syntax error
                    }
                }
                char *strProduction = strdup(parsing_table[i].production);
                if (strcmp(strProduction, "ε") == 0) // if we found a terminal
                {
                    push(stack, parsing_table[i].terminal); // add the terminal to the stack, not epsilon.
                }
                else
                {
                    reverse(strProduction);                                 // Reverse the production to ensure proper stack traversal ordering.
                    char *formattedProduction = strtok(strProduction, " "); // Split collection of non-terminals into individual non terminals and push each of them into the stack
                    // printf("formatted Production: %s", formattedProduction);
                    while (formattedProduction != NULL)
                    {
                        // In here each production rule is split at the whitespaces, each of these productions needs to be pushed into the stack
                        reverse(formattedProduction); // Unreverse the individual word so it pushes onto it properly

                        push(stack, formattedProduction);
                        // printf("pushing to the stack\n");
                        //  printf("Current non-terminal: %s\n", formattedProduction);
                        formattedProduction = strtok(NULL, " "); // Split collection of non-terminals into individual non terminals and push each of them into the stack
                    }
                }
            }
        }
        // printf("re-run the loop\n");

        // check if top of stack is terminal
        // Overall Idea: Continue to push non terminal rules into the stack, when you pop off a rule, add the children of that rule to the stack
        // Continue this expansion until we start to reach non-terminals. Once we hit a non terminal compare it with the current token. If it's a match then we're ready to move to the next token
        // If not then we continue to expand the production rules and find more non-terminals which hopefully match.
        // If the stack is empty but we've not fully explored the stack then we have a syntax error.
        // handle top of stack is non-terminal
    }
    printf("Done Syntax Analysis\n");
}

/*
 *   Main Function
 */
int main()
{

    TOKEN_ARRAY *tk = LexicalAnalysis("Test1.cp");

    STACK *stack = malloc(sizeof(STACK));
    printf("init stack\n");
    initializeStack(stack);

    NODE *ast_root = SyntaxAnalysis(tk, stack);
}