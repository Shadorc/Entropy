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
