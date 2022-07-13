#pragma once

#include "Trit.h"

class TritSet
{
	unsigned int* trits; //массив тритов
	unsigned int countTrit; //количество тритов в массиве 
	unsigned int allocLength; //реальный размер массива
	void new_allocLength(unsigned int countTrits);
	unsigned int min_uint(unsigned int x, unsigned int y) const;
	unsigned int max_uint(unsigned int x, unsigned int y) const;
	void resizeTrits(unsigned int countTrits);
	struct proxyTrit
	{
	private:
		TritSet& adressOfTritSet;
		unsigned int index;
	public:
		proxyTrit(TritSet& adress, unsigned int index) : adressOfTritSet(adress), index(index)
		{
			//пусто
		};
		operator Trit() const
		{
			return adressOfTritSet.getTrit(index);
		}
		proxyTrit& operator = (Trit trit)
		{
			adressOfTritSet.setTrit(index, trit);
			return *this;
		}
		proxyTrit& operator = (const proxyTrit& trit)
		{
			adressOfTritSet.setTrit(index, trit);
			return *this;
		}
		proxyTrit& operator |= (Trit trit)
		{
			adressOfTritSet.setTrit(index, adressOfTritSet.getTrit(index) | trit);
			return *this;
		}
		proxyTrit& operator &= (Trit trit)
		{
			adressOfTritSet.setTrit(index, adressOfTritSet.getTrit(index) & trit);
			return *this;
		}
	};
public:
	TritSet();
	TritSet(unsigned int countTrits);
	TritSet(const TritSet& tritSet);
	unsigned int capacity() const;
	unsigned int capacityMassive() const;
	unsigned int capacityTrit() const;
	proxyTrit operator[](unsigned int index);
	TritSet& operator = (const TritSet& tritSet);
	const TritSet operator ! () const;
	const TritSet& operator | (TritSet& tritSet) const;
	const TritSet& operator & (TritSet& tritSet) const;
	const TritSet& operator |= (TritSet& tritSet);
	const TritSet& operator &= (TritSet& tritSet);
	void setTrit(unsigned int index, Trit trit);
	Trit getTrit(unsigned int index) const;
	unsigned int length() const;
	void shrink();
	void trim(unsigned int lastIndex);
	unsigned int cardinality(Trit value) const;
	void printTrits() const;
	~TritSet();
};