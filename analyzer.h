#ifndef ANALYZER_H
#define ANALYZER_H

#include "lexer.h"

#define ANALYZER_MAX_RULES 64
#define ANALYZER_RULE_MAX_PATTERNS 16

typedef struct analyzer_pattern AnalyzerPattern;

typedef struct {
  TokenType token_type;
  AnalyzerPattern* pattern; 
} AnalyzerOrPattern;

struct analyzer_pattern {
  TokenType token_type;
  AnalyzerOrPattern* or_pattern;
};

typedef void (*AnalyzerCallback)(Token**, size_t);

typedef struct {
  AnalyzerPattern** patterns;
  size_t num_patterns;
  AnalyzerCallback callback;
} AnalyzerRule;

typedef struct {
  AnalyzerRule** rules;
  size_t num_rules;
} Analyzer;

extern Analyzer* create_analyzer();
extern void destroy_analyzer(Analyzer*);

extern AnalyzerRule* create_rule();
extern void add_rule(Analyzer*, AnalyzerRule*);
extern void add_pattern(AnalyzerRule*, AnalyzerPattern*);

extern int check_pattern(AnalyzerPattern*, TokenType);
extern int check_rule(AnalyzerRule*, Token**, size_t*);

extern void print_rule(AnalyzerRule*);
extern void print_pattern(AnalyzerPattern*);

extern AnalyzerPattern* create_basic_pattern(TokenType);
extern AnalyzerPattern* create_basic_or_pattern(TokenType, TokenType);
extern AnalyzerPattern* create_complex_or_pattern(TokenType, AnalyzerPattern*);

extern void execute(Analyzer*, Lexer*);

#endif /* ANALYZER_H */
