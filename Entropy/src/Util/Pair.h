#pragma once

template<typename T>
struct Pair
{
	T* left;
	T* right;

	Pair(T* left, T* right)
		: left(left)
		, right(right)
	{

	}
};

template<typename T>
bool PairComparator(Pair<T> lhs, Pair<T> rhs)
{
	if (lhs.left < rhs.left)
	{
		return true;
	}
	if (lhs.left == rhs.left)
	{
		return lhs.right < rhs.right;
	}
	return false;
}