#include <ctime>
#include <iostream>
#include <stdio.h>
#include <vector>

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

void update(std::vector <UINT> &tree, std::vector <UINT> &lazy, UINT v,
		    UINT cl, UINT cr, UINT l, UINT r, const UINT& val)
{
    if (v <= N - 1 && lazy[v] != 0)
	{
		tree[v] = (tree[v] + (cr - cl + 1) * lazy[v]) % MOD;

		if (cl != cr)
		{
			if (2 * v + 2 > N - 1)
			{
				tree[2 * v + 1] = (tree[2 * v + 1] + lazy[v]) % MOD;
				tree[2 * v + 2] = (tree[2 * v + 2] + lazy[v]) % MOD;
			}
			else
			{
				lazy[2 * v + 1] = (lazy[2 * v + 1] + lazy[v]) % MOD;
				lazy[2 * v + 2] = (lazy[2 * v + 2] + lazy[v]) % MOD;
			}
		}

		lazy[v] = 0;
	}

	if(cl > cr || cl > r || cr < l)
	{
		return;
	}

	if (l <= cl && cr <= r)
	{
		tree[v] = (tree[v] + (cr - cl + 1) * val) % MOD;

		if (cl != cr)
		{
			if (2 * v + 2 > N - 1)
			{
				tree[2 * v + 1] = (tree[2 * v + 1] + val) % MOD;
				tree[2 * v + 2] = (tree[2 * v + 2] + val) % MOD;
			}
			else
			{
				lazy[2 * v + 1] = (lazy[2 * v + 1] + val) % MOD;
				lazy[2 * v + 2] = (lazy[2 * v + 2] + val) % MOD;
			}
		}

		return;
	}

	const UINT cmid  = (cl + cr) / 2;

	update(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(cmid, r), val);
	update(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(cmid + 1, l), r, val);

	tree[v] = (tree[2 * v + 1] + tree[2 * v + 2]) % MOD;
}

UINT sum(std::vector <UINT> &tree, std::vector <UINT> &lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r)
{
	if (cl > cr || cl > r || cr < l)
	{
		return 0;
	}

	if (v <= N - 1 && lazy[v] != 0)
	{
		tree[v] = (tree[v] + (cr - cl + 1) * lazy[v]) % MOD;

		if (cl != cr)
		{
			if (2 * v + 2 > N - 1)
			{
				tree[2 * v + 1] = (tree[2 * v + 1] + lazy[v]) % MOD;
				tree[2 * v + 2] = (tree[2 * v + 2] + lazy[v]) % MOD;
			}
			else
			{
				lazy[2 * v + 1] = (lazy[2 * v + 1] + lazy[v]) % MOD;
				lazy[2 * v + 2] = (lazy[2 * v + 2] + lazy[v]) % MOD;
			}
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
	clock_t begin = clock();

	UINT m, q;
	UINT a, b;

	freopen("fastadd.in",  "r", stdin);
	freopen("fastadd.out", "w", stdout);

	std::cin >> m >> q;
	std::cin >> a >> b;

	std::vector <UINT> tree(LEN);
	std::vector <UINT> lazy(N);

	size_t i = 0;

	for (; i < N; ++i)
	{
		tree[i] = 0;
		lazy[i] = 0;
	}

	for(; i < LEN; ++i)
	{
		tree[i] = 0;
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

	std::cout << res_sum;

	clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << std::endl << elapsed_secs;

	return 0;
}
