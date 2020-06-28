#pragma once

#include <stdbool.h>

int GetIntParam(const char* queryString, const char* name);

bool StartsWith(const char* s, const char* prefix);

// case-insensitive
bool EndsWithCI(const char* s, const char* suffix);

char CharToHexDigit(char c);

char DecodeComplexChar(const char* s);

void UrlToString(char* dest, const char* src);
