#include <iostream>
#include <stdio.h>

typedef unsigned int UINT;
typedef unsigned long long ULL;

const unsigned long long MOD = 4294967296LL; // 2^32
const size_t LEN             = 33554432 + 1; // 2^25 - 1
const size_t N               = 16777216;     // 2^24

UINT next_rand(UINT a, UINT b, UINT &seed)
{
	seed = seed * a + b;

	return seed >> 8;
}

void update(UINT* tree, UINT* lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r, const UINT& val)
{
    if (lazy[v] != 0)
	{
		tree[v] = (tree[v] + (cr - cl + 1) * lazy[v] % MOD) % MOD;

		if (cl != cr)
		{
			lazy[2 * v + 1] += lazy[v];
			lazy[2 * v + 2] += lazy[v];
		}

		lazy[v] = 0;
	}

	if(cl > cr || cl > r || cr < l) // Current segment is not within range [i, j]
	{
		return;
	}

	if (l <= cl && cr <= r)
	{
		tree[v] = (tree[v] + (cr - cl + 1) * val % MOD) % MOD;

		if (cl != cr)
		{
			lazy[2 * v + 1] = (lazy[2 * v + 1] + val) % MOD;
			lazy[2 * v + 2] = (lazy[2 * v + 2] + val) % MOD;
		}

		return;
	}

	const UINT cmid  = (cl + cr) / 2;

	update(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(cmid, r), val);
	update(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(cmid + 1, l), r, val);

	tree[v] = (tree[2 * v + 1] + tree[2 * v + 2]) % MOD;
}

UINT sum(UINT* tree, UINT* lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r)
{
	if (cl > cr || cl > r || cr < l)
	{
		return 0;
	}

	if (lazy[v] != 0)
	{
		tree[v] = (tree[v] + (cr - cl + 1) * lazy[v] % MOD) % MOD;

		if (cl != cr)
		{
			lazy[2 * v + 1] = (lazy[2 * v + 1] + lazy[v]) % MOD;
			lazy[2 * v + 2] = (lazy[2 * v + 2] + lazy[v]) % MOD;
		}

		lazy[v] = 0;
	}

	if (l <= cl && cr <= r)
	{
		return tree[v];
	}

	const UINT cmid  = (cl + cr) / 2;

	const UINT l_sum = sum(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(cmid, r));
	const UINT r_sum = sum(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r);

	return (l_sum + r_sum) % MOD;
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

	std::cout << res_sum % MOD;

	delete []tree;
	delete []lazy;

	return 0;
}
