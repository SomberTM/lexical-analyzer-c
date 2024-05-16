#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

int main() {
  Lexer* lexer = create_lexer("hello_world.txt");

  tokenize(lexer);
  for (int i = 0; i < lexer->num_tokens; i++) {
    Token* token = lexer->tokens[i];
    printf("Token: %d %s\n", token->type, get_token_str(token));
  }

  destroy_lexer(lexer);
}
