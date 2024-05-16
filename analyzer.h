#ifndef ANALYZER_H
#define ANALYZER_H

#include "lexer.h"

#define ANALYZER_MAX_RULES 64
#define ANALYZER_RULE_MAX_PATTERNS 16

typedef enum {
  Or,
  And
} Condition;

typedef struct {
  Condition condition;
  union AnalyzerPattern pattern;
} AnalyzerConditional;

typedef struct {
  TokenType token_type;
  AnalyzerConditional* conditional; 
} ComplexAnalyzerPattern;

typedef union {
  TokenType token_type;
  ComplexAnalyzerPattern* pattern;
} AnalyzerPattern;

typedef void (*AnalyzerCallback)(Token**, size_t);

typedef struct {
  AnalyzerPattern* patterns;
  size_t num_patterns;
  AnalyzerCallback callback;
} AnalyzerRule;

typedef struct {
  AnalyzerRule* rules;
  size_t num_rules;
} Analyzer;

extern Analyzer* create_analyzer();
extern AnalyzerRule* create_rule(Analyzer*);

extern AnalyzerPattern create_pattern(TokenType);
extern AnalyzerPattern create_pattern(ComplexAnalyzerPattern*);
extern ComplexAnalyzerPattern* create_or_pattern(TokenType, TokenType);
extern ComplexAnalyzerPattern* create_or_pattern(TokenType, ComplexAnalyzerPattern*);
extern ComplexAnalyzerPattern* create_and_pattern(TokenType, TokenType);
extern ComplexAnalyzerPattern* create_and_pattern(TokenType, ComplexAnalyzerPattern*);


#endif /* ANALYZER_H */
