#include "Trit.h"

std::ostream& operator << (std::ostream& outStream, const Trit trit)
{
    switch (trit)
    {
    case ((Trit)-1): return outStream << "False";
    case ((Trit)1): return outStream << "True";
    default: return outStream << "Unknown";
    }
}

const Trit operator ! (const Trit trit)
{
    switch (trit)
    {
    case ((Trit)-1): return (Trit)1;
    case ((Trit)1): return (Trit)-1;
    default: return (Trit)0;
    }
}

const Trit operator ~ (const Trit trit)
{
    switch (trit)
    {
    case ((Trit)-1): return (Trit)1;
    case ((Trit)1): return (Trit)-1;
    default: return (Trit)0;
    }
}

const Trit operator & (const Trit x, const Trit y)
{
    if ((int)x < (int)y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

const Trit operator | (const Trit x, const Trit y)
{
    if ((int)x > (int)y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

Trit operator &= (Trit& x, const Trit y)
{
    x = x & y;
    return x;
}

Trit operator |= (Trit& x, const Trit y)
{
    x = x | y;
    return x;
}