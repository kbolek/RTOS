#include "command_decoder.h"
#include "string.h"

struct Keyword asKeywordList[MAX_KEYWORD_NR]= {
	{CALLIB, "callib"},
	{GOTO, "goto"},
	{ID, "id"},
	{BUTTON, "button"}
	};
struct Token asToken[MAX_TOKEN_NR];
enum State {TOKEN,DELIMITER};
unsigned char ucTokenNr;

unsigned char ucFindTokensInString (char *pcString){
	unsigned char ucCharacterCounter;
	unsigned char ucCurrentCharacter;
	enum State eState = DELIMITER;
	 ucTokenNr = 0;
	
	for(ucCharacterCounter=0;;ucCharacterCounter++){
		ucCurrentCharacter = pcString[ucCharacterCounter];
		switch(eState){
			case TOKEN:
			{
				if(ucCurrentCharacter == DELIMITER_CHAR){
					eState = DELIMITER;
				}
				else if((ucCurrentCharacter==NULL)|(ucTokenNr == MAX_TOKEN_NR)){
					return ucTokenNr;
				}
				break;
			}
			case DELIMITER:
			{
				if(ucCurrentCharacter != DELIMITER_CHAR){
					eState = TOKEN;
					asToken[ucTokenNr].uValue.pcString = pcString + ucCharacterCounter;
					ucTokenNr = ucTokenNr + 1;

				}
				else if((ucCurrentCharacter==NULL)|(ucTokenNr == MAX_TOKEN_NR)){
					return ucTokenNr;
				}

				break;
			}
		}
	}
}

enum Result eStringToKeyword(char pcStr[],enum KeywordCode *peKeywordCode){
	unsigned char ucKeywordCounter;
	for(ucKeywordCounter=0;ucKeywordCounter<MAX_KEYWORD_NR;ucKeywordCounter++)
	{
		if (eCompareString(pcStr,asKeywordList[ucKeywordCounter].cString) == EQUAL) 
		{
			*peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
			return OK;
		}
	}
	return ERROR;
};

void DecodeTokens(void){
	
	unsigned char ucTokenCounter;
	unsigned int *puiHexStringValue;
	Token *psCurrentToken = asToken; 
	KeywordCode *peKeyword;
	
	 for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++){
 		peKeyword = &(psCurrentToken->uValue.eKeyword);
 		puiHexStringValue = &(psCurrentToken->uValue.uiNumber);

		if(eStringToKeyword(psCurrentToken->uValue.pcString, peKeyword) == OK ){
			psCurrentToken->eType = KEYWORD;
		}
		else if( OK == eHexStringToUInt(psCurrentToken->uValue.pcString, puiHexStringValue) ){
			psCurrentToken->eType = NUMBER;
		}
		else {
			psCurrentToken->eType = STRING;
		}
		psCurrentToken++;
	}

}

void DecodeMsg(char *pcString){
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, DELIMITER_CHAR, NULL);
	DecodeTokens();
}
