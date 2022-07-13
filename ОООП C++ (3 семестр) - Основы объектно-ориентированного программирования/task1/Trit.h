#pragma once

#include <iostream>

enum class Trit	
{
	False = -1, //11
	Unknown = 0, //00
	True = 1,	//01
}; //10=-2 в дополнительном коде

std::ostream& operator << (std::ostream& outStream, const Trit trit);
const Trit operator ! (const Trit trit);
const Trit operator ~ (const Trit trit);
const Trit operator & (const Trit x, const Trit y);
const Trit operator | (const Trit x, const Trit y);
Trit operator &= (Trit& x, const Trit y);
Trit operator |= (Trit& x, const Trit y);