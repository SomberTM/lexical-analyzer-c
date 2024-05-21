#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "lexer.h"
#include "analyzer.h"

void printCallback(Token** tokens, size_t num_tokens) {
  Token* to_print = tokens[2];

  if (to_print->type != Unknown || to_print->value == NULL)
    return;

  syscall(SYS_write, 1, to_print->value, strlen(to_print->value));
}

int main() {
  Lexer* lexer = create_lexer("hello_world.txt");

  tokenize(lexer);

  
  for (int i = 0; i < lexer->num_tokens; i++) {
    Token* token = lexer->tokens[i];
    printf("Token: %d %s\n", token->type, token->value);
  }
  

  Analyzer* analyzer = create_analyzer();


  AnalyzerRule* rule = create_rule();
  rule->callback = &printCallback;

  AnalyzerPattern* quote_pattern = create_basic_or_pattern(DoubleQuote, SingleQuote);

  add_pattern(rule, create_basic_pattern(GreaterThan));
  add_pattern(rule, quote_pattern);
  add_pattern(rule, create_basic_pattern(Unknown));
  add_pattern(rule, quote_pattern);
  add_pattern(rule, create_basic_pattern(SemiColon));

  add_rule(analyzer, rule);

  execute(analyzer, lexer);

  destroy_lexer(lexer);
  destroy_analyzer(analyzer);
}
