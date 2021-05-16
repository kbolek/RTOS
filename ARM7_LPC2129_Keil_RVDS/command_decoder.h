#ifndef COMMAND_DECODER_H
#define COMMAND_DECODER_H

#define MAX_TOKEN_NR 2 
#define MAX_KEYWORD_STRING_LTH 20
#define MAX_KEYWORD_NR 5

typedef enum KeywordCode {STEP, GOTO, CALLIB, ID, BUTTON, STATE}KeywordCode;
typedef enum TokenType { KEYWORD, NUMBER, STRING}TokenType;

typedef union TokenValue 
{
	enum KeywordCode eKeyword;
	unsigned int uiNumber;
	char *pcString;
}TokenValue;

typedef struct Token 
{
	enum TokenType eType; 
	union TokenValue uValue;
}Token;


typedef struct Keyword
{
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1]; 
}Keyword;


extern struct Keyword asKeywordList[MAX_KEYWORD_NR];
extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;


void DecodeMsg(char *pcString);
void DecoderReset(void);

#endif
