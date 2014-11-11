#include <iostream>
#include <stdio.h>

typedef unsigned int UINT;
typedef unsigned long long ULL;

namespace
{
    const unsigned long long MOD = 4294967296LL; // 2^32
    const size_t LEN             = 33554432 + 1; // 2^25 + 1
    const size_t N               = 16777216;
}

UINT next_rand(int a, int b, UINT &seed)
{
	seed = seed * a + b;

	return seed >> 8;
}

void check_for_overflow(ULL value, const int number)
{
	if (value > MOD)
	{
		std::cout << "OVERFLOW, LINE: " << number << std::endl;
	}
}

void build(UINT* arr, UINT* tree, UINT v, UINT cl, UINT cr)
{
	if (cr == cl)
	{
		tree[v] = arr[cl];
	}
    else
	{
    	const UINT cmid  = (cl + cr) / 2;

    	build(arr, tree, 2 * v + 1, cl, cmid);
    	build(arr, tree, 2 * v + 2, cmid + 1, cr);

    	tree[v] = ((ULL)tree[2 * v + 1] + (ULL)tree[2 * v + 2]) % MOD;
	}
}

void update(UINT* tree, UINT* lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r, const UINT& val)
{
    if (l > r)
    {
    	return;
    }

    if (lazy[v] != 0)
    {
    	tree[v] = ((ULL)tree[v] + (ULL)(r - l + 1) * (ULL)lazy[v]) % MOD;

    	if (cl != cr)
    	{
    		lazy[2 * v + 1] = lazy[v];
    		lazy[2 * v + 2] = lazy[v];
    	}

    	lazy[v] = 0;
    }

    if (l <= cl && cr <= r)
    {
    	tree[v] = ((ULL)tree[v] + (ULL)(r - l + 1) * (ULL)val % MOD) % MOD;

    	if (cl != cr)
    	{
    		lazy[2 * v + 1] = (lazy[2 * v + 1] + val) % MOD;
    		lazy[2 * v + 2] = (lazy[2 * v + 2] + val) % MOD;
    	}

    	return;
    }

	const UINT cmid  = (cl + cr) / 2;

	update(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(r, cmid), val);
	update(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r, val);

	tree[v] = (tree[2 * v + 1] + tree[2 * v + 2]) % MOD;
}

ULL sum(UINT* tree, UINT* lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r)
{
	if (l > r)
	{
		return 0;
	}

	if (lazy[v] != 0)
	{
		tree[v] = ((ULL)tree[v] + (ULL)(r - l + 1) * (ULL)lazy[v] % MOD) % MOD;

		if (cl != cr)
		{
			lazy[2 * v + 1] = lazy[v];
			lazy[2 * v + 2] = lazy[v];
		}

		lazy[v] = 0;
	}

	if (l <= cl && cr <= r)
	{
		return tree[v];
	}

	const UINT cmid  = (cl + cr) / 2;

	const UINT l_sum = sum(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(r, cmid));
	const UINT r_sum = sum(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r);

	return ((ULL)l_sum + (ULL)r_sum) % MOD;
}

int main()
{
	UINT m, q;
	UINT a, b;

	freopen("fastadd.in",  "r", stdin);
	freopen("fastadd.out", "w", stdout);

	std::cin >> m >> q;
	std::cin >> a >> b;

	UINT* tree  = new UINT[LEN];
	UINT* lazy  = new UINT[LEN];

	for (size_t i = 0; i < LEN; ++i)
	{
		tree[i] = 0;
		lazy[i] = 0;
	}

    UINT res_sum = 0;
	UINT seed = 0;

	for (size_t i = 0; i < m; ++i)
	{
		UINT add = next_rand(a, b, seed);
		UINT l   = next_rand(a, b, seed);
		UINT r   = next_rand(a, b, seed);

		if (l > r)
		{
			std::swap(l, r);
		}

		update(tree, lazy, 0, 0, N - 1, l, r, add);

		res_sum = ((ULL)res_sum + (ULL)(r - l + 1) * (ULL)add) % MOD;
	}

	for (size_t i = 0; i < q; ++i)
	{
		UINT l = next_rand(a, b, seed);
		UINT r = next_rand(a, b, seed);

		if (l > r)
		{
			std::swap(l, r);
		}

		res_sum = (res_sum + sum(tree, lazy, 0, 0, N - 1, l, r)) % MOD;
	}

	std::cout << res_sum;

	delete []tree;
	delete []lazy;

	return 0;
}
