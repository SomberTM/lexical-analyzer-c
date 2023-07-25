#include <malloc.h>
#include <string.h>
#include "lexer.h"

Lexer* create_lexer(char* file_name) {
  Lexer* lexer = malloc(sizeof(Lexer));
  Token* tokens = malloc(255 * sizeof(Token));

  char* target_file_name = malloc(sizeof(char) * strlen(file_name));
  strcpy(target_file_name, file_name);
  target_file_name[strlen(file_name)] = '\0';
  
  lexer->target_file_name = target_file_name;
  lexer->file_handle = NULL;
  lexer->tokens = tokens;
  lexer->num_tokens = 0;
  return lexer;
}

void free_lexer(Lexer* lexer) {
  if (lexer == NULL) return;
  free(lexer->target_file_name);
  free(lexer->tokens);
  free(lexer);
  lexer = NULL;
}

// First we read a line into an in memory buffer
// Then we read the line char by char until we reach \n
// At each char we need to check against our single token matches
// If it doesnt match then we need another buffer for idents which
// we add a char to if its not a single token match. We also need to check
// that buffer each time we add a char for specific keyword tokens such as if, else, etc.