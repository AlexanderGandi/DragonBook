//
// Created by ocean on 25/9/2026.
//

#ifndef DRAGONBOOK_LEXERUTILS_H
#define DRAGONBOOK_LEXERUTILS_H


bool isDigit(char c);
bool isHexDigit(char c);
bool isOctDigit(char c);
bool isJavaLetter(char c);
bool isLongSuffix(char c);
bool isFloatSuffix(char c);
bool isDoubleSuffix(char c);
bool isFloatingLiteralSuffix(char c);
bool isIntLiteralSuffix(char c);


#endif //DRAGONBOOK_LEXERUTILS_H
