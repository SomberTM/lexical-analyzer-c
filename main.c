#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"
#include "analyzer.h"

void printCallback(Token** tokens, size_t num_tokens) {
  printf("Hi %s\n", tokens[3]->value);
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

  add_pattern(rule, create_basic_pattern(Unknown));
  add_pattern(rule, create_basic_pattern(LeftParen));
  add_pattern(rule, create_basic_pattern(DoubleQuote));
  add_pattern(rule, create_basic_pattern(Unknown));
  add_pattern(rule, create_basic_pattern(DoubleQuote));
  add_pattern(rule, create_basic_pattern(RightParen));
  add_rule(analyzer, rule);

  for (size_t i = 0; i < analyzer->num_rules; i++) {
    AnalyzerRule* rule = analyzer->rules[i];
    printf("Rule %ld With %ld Patterns\n", i, rule->num_patterns);
    for (size_t j = 0; j < rule->num_patterns; j++) {
      AnalyzerPattern* pattern = rule->patterns[j];
      printf("Pattern For Token %d\n", pattern->token_type);
    }
  }

  execute(analyzer, lexer);

  destroy_lexer(lexer);
  destroy_analyzer(analyzer);
}
