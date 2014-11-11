#include <iostream>

typedef unsigned int UINT;
typedef unsigned long long ULL;

namespace
{
    const unsigned long long MOD = 4294967296LL; // 2^32
    const size_t LEN             = 33554432 + 1; // 2^25 + 1
    const size_t N               = 16777216;     // 2^24
}

UINT next_rand(UINT a, UINT b, UINT &seed)
{
	seed = seed * a + b;

	return seed >> 8;
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

void update(UINT* tree, UINT v, UINT cl, UINT cr, UINT l, UINT r, const UINT& val)
{
    if (l > r)
    {
    	return;
    }

    if (cl == cr)
    {
        tree[v] = (tree[v] + val) % MOD;
        return;
    }

	const UINT cmid  = (cl + cr) / 2;

	update(tree, 2 * v + 1, cl, cmid, l, std::min(r, cmid), val);
	update(tree, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r, val);

	tree[v] = (tree[2 * v + 1] + tree[2 * v + 2]) % MOD;
}

UINT sum(UINT* tree, UINT v, UINT cl, UINT cr, UINT l, UINT r)
{
	if (l > r)
	{
		return 0;
	}

	if (l <= cl && cr <= r)
	{
		return tree[v];
	}

	const UINT cmid  = (cl + cr) / 2;

	const UINT l_sum = sum(tree, 2 * v + 1, cl, cmid, l, std::min(r, cmid));
	const UINT r_sum = sum(tree, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r);

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

	for (size_t i = 0; i < LEN; ++i)
	{
		tree[i] = 0;
	}

    ULL res_sum = 0;
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

		update(tree, 0, 0, N - 1, l, r, add);
	}

	for (size_t i = 0; i < q; ++i)
	{
		UINT l = next_rand(a, b, seed);
		UINT r = next_rand(a, b, seed);

		if (l > r)
		{
			std::swap(l, r);
		}

		res_sum = (res_sum + sum(tree, 0, 0, N - 1, l, r)) % MOD;
	}

	std::cout << res_sum % MOD;

	delete []tree;

	return 0;
}
