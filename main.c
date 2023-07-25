#include <stdio.h>
#include "lexer.h"

int main() {
  Lexer* lexer = create_lexer("hello_world.txt");

  printf(lexer->target_file_name);

  free_lexer(lexer);
}