#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

char* to_lower(const char* str) {
  size_t len = strlen(str);

  char* lowered = (char*) malloc(sizeof(char) * (len + 1));
  lowered[len] = '\0';

  for (int i = 0; str[i] != '\0'; i++) {
    char c = str[i];

    if (str[i] >= 65 && str[i] <= 90) {
      c += 32;
    } 

    lowered[i] = c;
  }
  return lowered;
}

int main() {
  Lexer* lexer = create_lexer("hello_world.txt");

  tokenize(lexer);
  for (int i = 0; i < lexer->num_tokens; i++) {
    Token* token = lexer->tokens[i];
    printf("Token: %d %s\n", token->type, token->value);
  }

  destroy_lexer(lexer);
}
