#ifndef STRING_H
#define STRING_H

#define NULL 0
//#define DELIMITER_CHAR 0x20

enum CompResult { DIFFERENT, EQUAL };
enum Result { OK, ERROR };

void CopyString(char pcSource[],char pcDestination[]);
enum CompResult eCompareString(char pcStr1[], char pcStr2[]);
void AppendString(char pcSourceStr[], char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar);
void UIntToHexStr(unsigned int uiValue, char pcStr[]);
//enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);
enum Result eIntStringToUInt(char pcStr[], unsigned int *puiValue);
void UIntToIntStr (unsigned int uiValue, char pcStr[]);


#endif
