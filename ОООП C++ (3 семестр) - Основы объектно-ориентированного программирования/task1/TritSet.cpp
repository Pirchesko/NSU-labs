#include "TritSet.h"

void TritSet::new_allocLength(unsigned int countTrits)
{
	countTrit = countTrits;
	if (countTrits % 16 == 0)
	{
		allocLength = countTrits / 16;
	}
	else
	{
		allocLength = countTrits / 16 + 1;
	}
}

unsigned int TritSet::min_uint(unsigned int x, unsigned int y) const
{
	if (x < y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

unsigned int TritSet::max_uint(unsigned int x, unsigned int y) const
{
	if (x > y)
	{
		return x;
	}
	else
	{
		return y;
	}
}

void TritSet::resizeTrits(unsigned int countTrits)
{
	unsigned int oldLenth = allocLength;
	new_allocLength(countTrits);
	unsigned int needLenth = min_uint(oldLenth, allocLength);
	unsigned int* temp = new unsigned int[needLenth];
	for (unsigned int i = 0; i < needLenth; i++)
	{
		temp[i] = trits[i];
	}
	delete[] trits;
	trits = new unsigned int[allocLength];
	for (unsigned int i = 0; i < needLenth; i++)
	{
		trits[i] = temp[i];
	}
	for (unsigned int i = needLenth; i < allocLength; i++)
	{
		trits[i] = 0;
	}
	delete[] temp;
}

TritSet::TritSet()
{
	countTrit = 0;
	allocLength = 0;
	trits = nullptr;
}

TritSet::TritSet(unsigned int countTrits)
{
	if (countTrits > 0)
	{
		new_allocLength(countTrits);
	}
	else
	{
		TritSet::TritSet();
		return;
	}
	trits = new unsigned int[allocLength];
	for (unsigned int i = 0; i < allocLength; i++)
	{
		trits[i] = 0;
	}
}

TritSet::TritSet(const TritSet& tritSet)
{
	new_allocLength(tritSet.countTrit);
	trits = new unsigned int[allocLength];
	for (unsigned int i = 0; i < allocLength; i++)
	{
		trits[i] = tritSet.trits[i];
	}
}

unsigned int TritSet::capacity() const
{
	return allocLength;
}

unsigned int TritSet::capacityMassive() const
{
	unsigned int length = _msize(trits) / sizeof(unsigned int);
	return length;
}

unsigned int TritSet::capacityTrit() const
{
	return countTrit;
}

TritSet::proxyTrit TritSet::operator[](unsigned int index)
{
	proxyTrit proxyTrit(*this, index);
	return proxyTrit;
}

TritSet& TritSet::operator = (const TritSet& tritSet)
{
	if (&tritSet == this)
	{
		return *this;
	}
	countTrit = tritSet.countTrit;
	allocLength = tritSet.allocLength;
	delete[] trits;
	trits = new unsigned int[allocLength];
	for (unsigned int i = 0; i < allocLength; i++)
	{
		trits[i] = (unsigned int)tritSet.trits[i];
	}
	return *this;
}

const TritSet TritSet::operator ! () const
{
	TritSet retTrit(*this);
	for (unsigned int i = 0; i < retTrit.countTrit; i++)
	{
		retTrit[i] = !retTrit[i];
	}
	return retTrit;
}

const TritSet& TritSet::operator | (TritSet& tritSet) const
{
	unsigned int maxCount = max_uint(countTrit, tritSet.countTrit);
	unsigned int minCount = min_uint(countTrit, tritSet.countTrit);
	TritSet* retTrit = new TritSet(*this);
	if ((*retTrit).countTrit < maxCount)
	{
		(*retTrit).resizeTrits(maxCount);
	}
	for (unsigned int i = 0; i < maxCount; i++)
	{
		(*retTrit)[i] |= tritSet[i];
	}
	return (*retTrit);
}

const TritSet& TritSet::operator & (TritSet& tritSet) const
{
	unsigned int maxCount = max_uint(countTrit, tritSet.countTrit);
	unsigned int minCount = min_uint(countTrit, tritSet.countTrit);
	TritSet* retTrit = new TritSet(*this);
	if ((*retTrit).countTrit < maxCount)
	{
		(*retTrit).resizeTrits(maxCount);
	}
	for (unsigned int i = 0; i < maxCount; i++)
	{
		(*retTrit)[i] &= tritSet[i];
	}
	return (*retTrit);
}

const TritSet& TritSet::operator |= (TritSet& tritSet)
{
	unsigned int maxCount = max_uint(countTrit, tritSet.countTrit);
	unsigned int minCount = min_uint(countTrit, tritSet.countTrit);
	if ((*this).countTrit < maxCount)
	{
		(*this).resizeTrits(maxCount);
	}
	for (unsigned int i = 0; i < maxCount; i++)
	{
		(*this)[i] |= tritSet[i];
	}
	return (*this);
}

const TritSet& TritSet::operator &= (TritSet& tritSet)
{
	unsigned int maxCount = max_uint(countTrit, tritSet.countTrit);
	unsigned int minCount = min_uint(countTrit, tritSet.countTrit);
	if ((*this).countTrit < maxCount)
	{
		(*this).resizeTrits(maxCount);
	}
	for (unsigned int i = 0; i < maxCount; i++)
	{
		(*this)[i] &= tritSet[i];
	}
	return (*this);
}

void TritSet::setTrit(unsigned int index, Trit trit)
{
	if ((trit == (Trit)0) && (index >= countTrit))
	{
		return;
	}
	if (index >= countTrit)
	{
		resizeTrits(index + 1);
	}
	unsigned int realIndex = index % 16;
	unsigned int left = trits[(index / 16)];
	if (realIndex != 15)
	{
		left = left >> (realIndex * 2 + 2);
		left = left << (realIndex * 2 + 2);
	}
	else
	{
		left = 0;
	}
	unsigned int right = trits[(index / 16)];
	if (realIndex != 0)
	{
		right = right << (32 - realIndex * 2);
		right = right >> (32 - realIndex * 2);
	}
	else
	{
		right = 0;
	}
	unsigned int t;
	switch (trit)
	{
	case ((Trit)-1): t = 3; break;
	case ((Trit)1): t = 1; break;
	default: t = 0;
	}
	t = t << ((index % 16) * 2);
	trits[(index / 16)] = 0;
	trits[(index / 16)] = t | left | right;
}

Trit TritSet::getTrit(unsigned int index) const
{
	if (index > countTrit)
	{
		return (Trit)0;
	}
	int a = trits[index / 16];
	a = a << (32 - (index * 2 + 2));
	a = a >> 30;
	return (Trit)a;
}

unsigned int TritSet::length() const
{
	unsigned int i = allocLength - 1;
	while (trits[i] == 0)
	{
		if (i == 0)
		{
			return 0; //0+1
		}
		i--;
	}
	unsigned int realIndex = i;
	i = 15;
	while (getTrit((realIndex * 16) + i) == (Trit)0)
	{
		i--;
	}
	return (realIndex * 16) + i + 1;
}

void TritSet::shrink()
{
	unsigned int size = length();
	if (size == 0)
	{
		resizeTrits(1);
	}
	else
	{
		resizeTrits(size);
	}
}

void TritSet::trim(unsigned int lastIndex)
{
	if (lastIndex + 1 < (*this).countTrit)
	{
		resizeTrits(lastIndex + 1);
	}
	else
	{
		return;
	}
}

unsigned int TritSet::cardinality(Trit value) const
{
	unsigned int size = length();
	unsigned int count = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		if (getTrit(i) == value)
		{
			count++;
		}
	}
	return count;
}

void TritSet::printTrits() const
{
	if (allocLength == 0)
	{
		std::cout << "No trits!" << std::endl;
	}
	for (unsigned int i = 0; i < countTrit; i++)
	{
		std::cout << getTrit(i) << std::endl;
	}
}

TritSet::~TritSet()
{
	delete[] trits;
}