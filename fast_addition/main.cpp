#include <iostream>
#include <stdio.h>

typedef unsigned int UINT;

namespace
{
    const unsigned long long MOD = 4294967296LL;
    const size_t LEN             = 33554432;
}

UINT next_rand(int a, int b, UINT &seed)
{
	seed = seed * a + b;

	return seed % 16;
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

    	tree[v] = tree[2 * v + 1] + tree[2 * v + 2];
	}
}

void update(UINT* tree, UINT* lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r, const UINT &val)
{
    if (l > r)
    {
    	return;
    }

    if (lazy[v] != 0)
    {
    	tree[v] += lazy[v]; //TODO ?

    	if (cl != cr)
    	{
    		lazy[2 * v + 1] += lazy[v];
    		lazy[2 * v + 2] += lazy[v];
    	}

    	lazy[v] = 0;
    }

    if (l <= cl && cr <= r)
    {
    	tree[v] += val;  //TODO ?

    	if (cl != cr)
    	{
    		lazy[2 * v + 1] += val;
    		lazy[2 * v + 2] += val;
    	}

    	return;
    }

	const UINT cmid  = (cl + cr) / 2;

	update(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(r, cmid), val);
	update(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r, val);

	tree[v] = tree[2 * v + 1] + tree[2 * v + 2];
}

UINT sum(UINT* tree, UINT* lazy, UINT v, UINT cl, UINT cr, UINT l, UINT r)
{
	if (l > r)
	{
		return 0;
	}

	if (lazy[v] != 0)
	{
		tree[v] += lazy[v];  //TODO

		if (cl != cr)
		{
			lazy[2 * v + 1] = lazy[v];
			lazy[2 * v + 2] = lazy[v];
		}

		lazy[v] = 0;
	}

	if (l <= cl && cr <= r)
	{
		return tree[v]; //TODO ?
	}

	const UINT cmid  = (cl + cr) / 2;

	const UINT l_sum = sum(tree, lazy, 2 * v + 1, cl, cmid, l, std::min(r, cmid));
	const UINT r_sum = sum(tree, lazy, 2 * v + 2, cmid + 1, cr, std::max(l, cmid + 1), r);

	return l_sum + r_sum;
}

int main()
{
	UINT m, q;
	UINT a, b;

	freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);

	std::cin >> m >> q;
	std::cin >> a >> b;

	UINT* arr = new UINT[6]{5, 8, 2, 7, 12, 4};

	UINT CLEN = 6 * 2 + 1;

	UINT* tree  = new UINT[LEN];
	UINT* lazy  = new UINT[LEN];

	for (size_t i = 0; i < LEN; ++i)
	{
		tree[i] = 0;
		lazy[i] = 0;
	}

	build(arr, tree, 0, 0, 5);  // start = 0, left = 0, right = N

	// tr, sm, v = 0, cl = 0, cr = N - 1, query_l = left_ind, query_r = right_ind
	//std::cout << std::endl << sum(tr, sm, 0, 0, 5, 3, 5) << std::endl;

    /*UINT res_sum = 0;

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

		update(tr, sm, 0, 0, LEN - 1, l, r, add);
	}*/

	UINT add = 1;
	update(tree, lazy, 0, 0, 5, 0, 5, add);

	std::cout << std::endl << sum(tree, lazy, 0, 0, 5, 0, 5) << std::endl;

	delete []tree;
	delete []lazy;

	return 0;
}
