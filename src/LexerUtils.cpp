//
// Created by ocean on 25/9/2026.
//

#include "LexerUtils.h"

#include <cctype>

bool isDigit(const char c) {
    return '0' <= c && c <= '9';
}

bool isHexDigit(const char c) {
    return isDigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

bool isOctDigit(const char c) {
    return '0' <= c && c <= '7';
}

bool isJavaLetter(const char c) {
    return isalpha(c) || c == '_' || c == '$';
}

bool isLongSuffix(const char c) {
    return c == 'l' || c == 'L';
}

bool isFloatSuffix(const char c) {
    return c == 'f' || c == 'F';
}

bool isDoubleSuffix(const char c) {
    return c == 'd' || c == 'D';
}

bool isFloatingLiteralSuffix(const char c) {
    return isFloatSuffix(c) || isDoubleSuffix(c);
}

bool isIntLiteralSuffix(const char c) {
    return isLongSuffix(c);
}
