#pragma once

#include "lex.hpp"

bool isRegister(TokenType reg);
bool isRegister8(TokenType reg);
bool isRegister16(TokenType reg);
bool isRegister32(TokenType reg);
bool isRegister64(TokenType reg);
bool isRegisterExt(TokenType reg);

