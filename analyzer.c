#include <stdlib.h>
#include <stdio.h>

#include "analyzer.h"

Analyzer* create_analyzer() {
  Analyzer* analyzer = malloc(sizeof(Analyzer));
  AnalyzerRule** rules = malloc(ANALYZER_MAX_RULES * sizeof(AnalyzerRule*));

  analyzer->rules = rules;
  analyzer->num_rules = 0;

  return analyzer;
}

void free_analyzer_pattern(AnalyzerPattern* pattern) {
  if (pattern->or_pattern == NULL) {
    free(pattern);
    pattern = NULL;
    return;
  }

  AnalyzerOrPattern* or_pattern = pattern->or_pattern;
  free_analyzer_pattern(or_pattern->pattern);

  free(or_pattern);
  pattern->or_pattern = NULL;

  free(pattern);
  pattern = NULL;
}

void destroy_analyzer(Analyzer* analyzer) {
  for (size_t i = 0; i < analyzer->num_rules; i++) {
    AnalyzerRule* rule = analyzer->rules[i];
    if (rule == NULL) continue;
    
    for (size_t j = 0; j < rule->num_patterns; j++) {
      AnalyzerPattern* pattern = rule->patterns[j];
      if (pattern == NULL) continue;

      free_analyzer_pattern(pattern);
    }

    free(rule->patterns);
    rule->patterns = NULL;

    free(rule);
    rule = NULL;
  }

  free(analyzer->rules);
  analyzer->rules = NULL;
  free(analyzer);
  analyzer = NULL;
}

AnalyzerRule* create_rule() {
  AnalyzerRule* analyzer_rule = malloc(sizeof(AnalyzerRule));
  AnalyzerPattern** patterns = malloc(ANALYZER_RULE_MAX_PATTERNS * sizeof(AnalyzerPattern*));

  analyzer_rule->patterns = patterns;
  analyzer_rule->num_patterns = 0;
  analyzer_rule->callback = NULL;

  return analyzer_rule;
}

void add_rule(Analyzer* analyzer, AnalyzerRule* rule) {
  if (analyzer->num_rules >= ANALYZER_MAX_RULES) return;

  analyzer->rules[analyzer->num_rules] = rule;
  analyzer->num_rules++;
}

void add_pattern(AnalyzerRule* rule, AnalyzerPattern* pattern) {
  if (rule->num_patterns >= ANALYZER_RULE_MAX_PATTERNS) return;

  rule->patterns[rule->num_patterns] = pattern;
  rule->num_patterns++;
}

AnalyzerPattern* create_basic_pattern(TokenType target) {
  AnalyzerPattern* pattern = malloc(sizeof(AnalyzerPattern));

  pattern->or_pattern = NULL;
  pattern->token_type = target;
  return pattern;
}

/**
* Unnecessarily complex
*/
AnalyzerPattern* create_basic_or_pattern(TokenType left, TokenType right) {
  AnalyzerPattern* pattern = malloc(sizeof(AnalyzerPattern));
  AnalyzerOrPattern* or_pattern = malloc(sizeof(AnalyzerOrPattern)); 

  pattern->or_pattern = or_pattern;
  or_pattern->token_type = left;

  AnalyzerPattern* or_pattern_pattern = malloc(sizeof(AnalyzerPattern));
  or_pattern_pattern->or_pattern = NULL;
  or_pattern_pattern->token_type = right;

  or_pattern->pattern = or_pattern_pattern;

  return pattern;
}

AnalyzerPattern* create_complex_or_pattern(TokenType left, AnalyzerPattern* right) {
  AnalyzerPattern* pattern = malloc(sizeof(AnalyzerPattern));
  AnalyzerOrPattern* or_pattern = malloc(sizeof(AnalyzerOrPattern));

  pattern->or_pattern = or_pattern;
  
  or_pattern->token_type = left;
  or_pattern->pattern = right;

  return pattern;
}

int check_pattern(AnalyzerPattern* pattern, TokenType token_type) {
  if (pattern->or_pattern == NULL) {
    return pattern->token_type == token_type;
  }

  AnalyzerOrPattern* or_pattern = pattern->or_pattern;
  return or_pattern->token_type == token_type || check_pattern(or_pattern->pattern, token_type);
}

int check_rule(AnalyzerRule* rule, Token** tokens, size_t* depth_ptr) {
  size_t depth = *depth_ptr;
  if (depth >= rule->num_patterns)
    return 1;

  Token* token = tokens[depth];
  AnalyzerPattern* pattern = rule->patterns[depth];

  *depth_ptr += 1;
  return check_pattern(pattern, token->type) && check_rule(rule, tokens, depth_ptr);
}

void print_pattern(AnalyzerPattern* pattern) {
  AnalyzerOrPattern* or_pattern = pattern->or_pattern;

  if (or_pattern != NULL) {
    printf("(%d || ", or_pattern->token_type);
    print_pattern(or_pattern->pattern);
    printf(")");

    return;
  } 

  printf("%d", pattern->token_type);
}

void print_rule(AnalyzerRule* rule) {
  for (size_t i = 0; i < rule->num_patterns; i++) {
    printf("(");
    print_pattern(rule->patterns[i]);
    printf(")");

    if (i < rule->num_patterns - 1)
      printf(" && ");
  }
  printf("\n");
}

void execute(Analyzer* analyzer, Lexer* lexer) {
  if (lexer->num_tokens == 0 || analyzer->num_rules == 0) return;

  for (size_t i = 0; i < analyzer->num_rules; i++) {
    AnalyzerRule* rule = analyzer->rules[i];
    print_rule(rule);
  }

  for (size_t i = 0; i < lexer->num_tokens; i++) {
    Token* token = lexer->tokens[i];

    for (size_t j = 0; j < analyzer->num_rules; j++) {
      AnalyzerRule* rule = analyzer->rules[j];

      size_t depth = 0;
      if (check_rule(rule, lexer->tokens + i, &depth)) {
        if (rule->callback != NULL) 
          rule->callback(lexer->tokens + i, rule->num_patterns);

        i += depth;
        break;
      }
    }
  }
}
