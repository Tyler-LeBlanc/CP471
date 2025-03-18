#ifndef LEXICALANALYSIS_H_
#define LEXICALANALYSIS_H_

typedef struct{
    char *buffer1;
    char *buffer2;
    char *currentBuffer;
    char *nextBuffer;
    FILE *file;
    size_t bufferSize;
    size_t currentPos;
} DOUBLE_BUFFER;


typedef struct{
    char lexeme[100];
    char type[20];
} TOKEN;

typedef struct{
    TOKEN *array;
    int size;
    int capacity;
} TOKEN_ARRAY;

typedef struct {
    int state;
    const char *type;
} StateTokenMap;

DOUBLE_BUFFER *initializeBuffer(const char *filename);
void switchBuffer(DOUBLE_BUFFER *db);
char getNextChar(DOUBLE_BUFFER *db, int *line);
void freeDB(DOUBLE_BUFFER *db);


void initializeArray(TOKEN_ARRAY *tokenArray);
void addToken(TOKEN_ARRAY *tokenArray, TOKEN token);

#endif