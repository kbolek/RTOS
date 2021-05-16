#include "string.h"

void CopyString(char pcSource[], char pcDestination[]){
	
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter=0;(pcSource[ucCharacterCounter]!=NULL);ucCharacterCounter++){
		pcDestination[ucCharacterCounter]=pcSource[ucCharacterCounter];
	}
	pcDestination[ucCharacterCounter]=pcSource[ucCharacterCounter];
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[]){
	
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter=0;(pcStr1[ucCharacterCounter]!=NULL)&&(pcStr2[ucCharacterCounter]!=NULL);ucCharacterCounter++){
		if(pcStr1[ucCharacterCounter]!=pcStr2[ucCharacterCounter]){
			return DIFFERENT;
		}	
	}
	if(pcStr1[ucCharacterCounter]!=pcStr2[ucCharacterCounter]){
		return DIFFERENT;
	}	
	return EQUAL;
}	

void AppendString (char pcSourceStr[], char pcDestinationStr[]){
	
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter=0;pcDestinationStr[ucCharacterCounter]!=NULL;ucCharacterCounter++){}
	CopyString(pcSourceStr,pcDestinationStr+ucCharacterCounter);
}

void ReplaceCharactersInString(char pcString[], char cOldChar,char cNewChar){
	
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter=0;pcString[ucCharacterCounter]!=NULL;ucCharacterCounter ++){
		if(pcString[ucCharacterCounter]==cOldChar){
			pcString[ucCharacterCounter]=cNewChar;
		}
	}	
}

void UIntToHexStr (unsigned int uiValue, char pcStr[]){
       
	unsigned char ucActualTetrad;
	unsigned char ucTetradCounter;
 
	pcStr[0] = '0';
	pcStr[1] = 'x';
	pcStr[6] = NULL;
 
	for(ucTetradCounter = 0; ucTetradCounter < 4; ucTetradCounter++){
		ucActualTetrad = (uiValue >> (ucTetradCounter*4)) & 0xF;
		if(ucActualTetrad < 10){
			pcStr[5 - ucTetradCounter] = '0' + ucActualTetrad;
		}
		else{
			pcStr[5 - ucTetradCounter] = 'A' + ucActualTetrad - 10;
		}
	}
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue){
	
	char cActualChar;
	unsigned char ucCharCounter;
	*puiValue=0;
	
	if((pcStr[0] != '0')||(pcStr[1] != 'x')||(pcStr[2] == NULL)){
		return (ERROR);
	}
	for(ucCharCounter=2; pcStr[ucCharCounter]!=NULL; ucCharCounter++){
		cActualChar = pcStr[ucCharCounter];
		if (ucCharCounter>=6){
			return (ERROR);
		}
		*puiValue= *puiValue<<4;
		if((cActualChar>='0')&&(cActualChar<='9')){
			*puiValue= *puiValue + (cActualChar-'0');
		}else if((cActualChar>='A')&&(cActualChar<='F')){
			*puiValue= *puiValue + (cActualChar-'A'+10);		
		}else{
			return (ERROR);
		}
	}
	return (OK);
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){

	unsigned char ucCharCounter;

	for (ucCharCounter=0; pcDestinationStr[ucCharCounter]!=NULL; ucCharCounter++){}
	UIntToHexStr(uiValue, pcDestinationStr+ucCharCounter);
}
