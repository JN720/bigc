#pragma once

#include "Result.h"
#include "Token.h"
#include <vector>
namespace tokenizer
{
    Result<TokenType> allowSingularTokens(char c, TokenType allowed[12], std::string error);
    Result<TokenType> getTokenType(char c, TokenType prev, std::string acc, bool prevWhitespace);
    Result<std::vector<Token>> tokenize(std::string str);
}
