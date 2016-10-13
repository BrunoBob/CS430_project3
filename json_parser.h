#ifndef __JSON_PARSER
#define __JSON_PARSER


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "raycaster.h"

//#define DEBUG

#define MAX_STRING_LENGHT 128
#define MAX_OBJECT 128

#define ERROR_PARSER 1

int readChar(FILE* json);

void expectChar(FILE* json, int c);

void skipSpace(FILE* json);

char* readString(FILE* json);

double readNumber(FILE* json);

double* ReadVector(FILE* json);

objectList parseFile(char* filename, objectList list, double* width, double* height);

#endif
