#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

const char LEXER_CHAR_TOKENS[8] = { '(', ')', '+', '-', '*', '/', '{', '}' };

int is_char_token(char token) {
  for (int i = 0; i < 8; i++) {
    if (token == LEXER_CHAR_TOKENS[i])
      return 1;
  }

  return 0;
}

Lexer* create_lexer(char* file_name) {
  Lexer* lexer = malloc(sizeof(Lexer));
  Token** tokens = malloc(255 * sizeof(Token*));

  char* target_file_name = malloc(sizeof(char) * strlen(file_name));
  strcpy(target_file_name, file_name);
  target_file_name[strlen(file_name)] = '\0';
  
  lexer->target_file_name = target_file_name;
  lexer->tokens = tokens;
  lexer->num_tokens = 0;
  return lexer;
}

void destroy_lexer(Lexer* lexer) {
  if (lexer == NULL) return;
  
  free(lexer->target_file_name);

  for (int i = 0; i < lexer->num_tokens; i++) {
    Token* token = lexer->tokens[i];

    if (token->value != NULL) {
      free(token->value);
      token->value = NULL;
    }

    free(token);
    lexer->tokens[i] = NULL;
  }

  free(lexer->tokens);
  lexer->tokens = NULL;

  free(lexer);
  lexer = NULL;
}

Token* get_token(char* raw_token) {
  Token* token = malloc(sizeof(Token));

  if (!strcmp(raw_token, "if")) {
    token->type = If;
  } else if (!strcmp(raw_token, "for")) {
    token->type = For;
  } else if (!strcmp(raw_token, "=")) {
    token->type = Equal;
  } else if (!strcmp(raw_token, "(")) {
    token->type = LeftParen;
  } else if (!strcmp(raw_token, ")")) {
    token->type = RightParen;
  } else if (!strcmp(raw_token, ":")) {
    token->type = SemiColon;
  } else {
    token->type = Unknown;
    
    char* value = malloc(sizeof(char) * strlen(raw_token));
    strcpy(value, raw_token);
    value[strlen(raw_token)] = '\0';

    token->value = value;
  }

  return token;
}

void tokenize_line(Lexer* lexer, char* line) {
  char raw_token[LEXER_MAX_TOKEN_SIZE] = {'\0'};
  size_t raw_token_size = 0;

  for (int i = 0; i < LEXER_MAX_TOKEN_SIZE; i++) {
    if (is_char_token(line[i])) {
      // tokenize up to this point if needed
      if (raw_token_size > 0) {
        raw_token[raw_token_size] = '\0';

        Token* token = get_token(raw_token);
        lexer->tokens[lexer->num_tokens] = token;
        lexer->num_tokens++;

        raw_token_size = 0;
      }

      raw_token[0] = line[i];
      raw_token[1] = '\0';

      Token* token = get_token(raw_token);
      lexer->tokens[lexer->num_tokens] = token;
      lexer->num_tokens++;

      raw_token_size = 0;
    }
    else if (isspace(line[i]) || line[i] == '\0') {
      raw_token[raw_token_size] = '\0';

      Token* token = get_token(raw_token);
      lexer->tokens[lexer->num_tokens] = token;
      lexer->num_tokens++;

      raw_token_size = 0;

      if (line[i] == '\0')
        break;
    } else {
      raw_token[raw_token_size] = line[i];
      raw_token_size++;
    }
  }
}

void tokenize(Lexer* lexer) {
  FILE* fd = fopen(lexer->target_file_name, "r");
  if (fd == NULL) return;

  char line[LEXER_MAX_LINE_SIZE] = {'\0'};
  size_t line_size = 0;

  unsigned long c;

  while (!feof(fd)) {
    fread(&c, sizeof(char), 1, fd);

    if (c == '\n') {
      line[line_size] = '\0';
      tokenize_line(lexer, line);
      line_size = 0;
    } else {
      line[line_size] = c;
      line_size++;
    }
  }

  fclose(fd);
  fd = NULL;
}
