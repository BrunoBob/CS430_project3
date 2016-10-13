#include "json_parser.h"

int line = 1;

// Read and exit if EOF or return it
int readChar(FILE* json) {
  int c = fgetc(json);
  #ifdef DEBUG
    printf("%c", c);
  #endif
  if (c == '\n') {
    line++;
  }
  if (c == EOF) {
    fprintf(stderr, "Error: Unexpected end of file on line number %d.\n", line);
    exit(ERROR_PARSER);
  }
  return c;
}

//Read the next string on the file and exit if no string detected
char* readString(FILE* json) {
  char buffer[MAX_STRING_LENGHT+1];
  int c = readChar(json);
  if (c != '"') {
    fprintf(stderr, "Error: Expected string on line %d.\n", line);
    exit(ERROR_PARSER);
  }
  c = readChar(json);
  int i = 0;
  while (c != '"') {
    if (i >= 128) {
      fprintf(stderr, "Error: Strings longer than 128 characters in length are not supported.\n");
      exit(ERROR_PARSER);
    }
    if (c == '\\') {
      fprintf(stderr, "Error: Strings with escape codes are not supported.\n");
      exit(ERROR_PARSER);
    }
    if (c < 32 || c > 126) {
      fprintf(stderr, "Error: Strings may contain only ascii characters.\n");
      exit(ERROR_PARSER);
    }
    buffer[i] = c;
    i += 1;
    c = readChar(json);
  }
  buffer[i] = 0;
  return strdup(buffer);
}

//Read the next number on the file and exit if no number detected
double readNumber(FILE* json) {
  double value;
  if(fscanf(json, "%lf", &value) == EOF){
    fprintf(stderr, "Error: Unexpected end of file on line number %d.\n", line);
    exit(ERROR_PARSER);
  }
  #ifdef DEBUG
    printf("%lf", value);
  #endif
  return value;
}

//Check if next character is the one given in parameter expected, exit if not
void expectChar(FILE* json, int expected) {
  int c = readChar(json);
  if (c != expected){
    fprintf(stderr, "Error: Expected '%c' on line %d.\n", expected, line);
    exit(ERROR_PARSER);
  }
}

//Read all the space character
void skipSpace(FILE* json){
  int c;
  do{
    c = fgetc(json);
    if(c == '\n'){
      line++;
    }
  } while(isspace(c));
  ungetc(c,json);
}

//Read the next vector on the file
double* ReadVector(FILE* json) {
  double* v = malloc(3*sizeof(double));
  expectChar(json, '[');
  skipSpace(json);
  v[0] = readNumber(json);
  skipSpace(json);
  expectChar(json, ',');
  skipSpace(json);
  v[1] = readNumber(json);
  skipSpace(json);
  expectChar(json, ',');
  skipSpace(json);
  v[2] = readNumber(json);
  skipSpace(json);
  expectChar(json, ']');
  return v;
}

objectList parseFile(char* filename, objectList list, double* width, double* height) {

  #ifdef DEBUG
    printf("Starting reading file %s\n", filename);
  #endif

  int c;
  int objectNumber = 0;
  FILE* json = fopen(filename, "r");

  objectList tempList = (objectList)malloc(sizeof(*tempList));
  list = tempList;

  if (json == NULL) {
    fprintf(stderr, "Error: Could not open file \"%s\"\n", filename);
    exit(ERROR_PARSER);
  }

  // Find the beginning of the list
  skipSpace(json);
  expectChar(json, '[');
  skipSpace(json);

  // Find all the objects
  while (objectNumber < MAX_OBJECT) {
    objectNumber++;
    #ifdef DEBUG
      printf("\nReading object number %d at line %d\n", objectNumber, line);
    #endif

    c = readChar(json);
    if (c == ']') {
      fprintf(stderr, "Error: This is the worst scene file EVER.\n");
      fclose(json);
      exit(ERROR_PARSER);
    }
    if (c == '{') {
      skipSpace(json);

      // Parse the object
      char* key = readString(json);
      if (strcmp(key, "type") != 0) {
        fprintf(stderr, "Error: Expected \"type\" key on line number %d.\n", line);
        exit(ERROR_PARSER);
      }

      skipSpace(json);
      expectChar(json, ':');
      skipSpace(json);

      char* value = readString(json);

      if (strcmp(value, "camera") == 0) {
        tempList->kind = -1 ;
      }
      else if (strcmp(value, "sphere") == 0) {
        tempList->kind = 0 ;
      }
      else if (strcmp(value, "plane") == 0) {
        tempList->kind = 1 ;
      }
      else {
        fprintf(stderr, "Error: Unknown type, \"%s\", on line number %d.\n", value, line);
        exit(ERROR_PARSER);
      }

      skipSpace(json);

      //Read all fields
      while (1) {
        c = readChar(json);

        //Check if end of object
        if (c == '}') {
          break;
        }
        //else read field
        else if (c == ',') {
          skipSpace(json);
          char* key = readString(json);
          skipSpace(json);
          expectChar(json, ':');
          skipSpace(json);

          if ((strcmp(key, "width") == 0) || (strcmp(key, "height") == 0) || (strcmp(key, "radius") == 0)) {
            double value = readNumber(json);
            if(strcmp(key, "radius") == 0){
              tempList->sphere.radius = value;
            }
            else if(strcmp(key, "width") == 0){
              *width = value;
            }
            else{
              *height = value;
            }
          }
          else if ((strcmp(key, "color") == 0) || (strcmp(key, "position") == 0) || (strcmp(key, "normal") == 0)) {
            double* value = ReadVector(json);
            if(strcmp(key, "color") == 0){
              tempList->color = value;
            }
            else if(strcmp(key, "position") == 0){
              tempList->position = value;
            }
            else{
              tempList->plane.normal = value;
            }
          }
          else {
            fprintf(stderr, "Error: Unknown property, \"%s\", on line %d.\n", key, line);
            exit(ERROR_PARSER);
          }
          skipSpace(json);
        }
        else {
          fprintf(stderr, "Error: Unexpected value on line %d\n", line);
          exit(ERROR_PARSER);
        }
      }

      skipSpace(json);
      c = readChar(json);
      if (c == ',') {
        if(tempList->kind != -1){
          tempList->next = (objectList)malloc(sizeof(*tempList));
          tempList = tempList->next;
        }
        skipSpace(json);
      }
      else if (c == ']') {
        fclose(json);
        #ifdef DEBUG
          printf("\nEnd of reading\n");
        #endif
        return list;
      }
      else {
        fprintf(stderr, "Error: Expecting ',' or ']' on line %d.\n", line);
        exit(ERROR_PARSER);
      }
    }
  }
  fprintf(stderr, "Error: Number of object superior to %d.\n", MAX_OBJECT);
  exit(ERROR_PARSER);
}
