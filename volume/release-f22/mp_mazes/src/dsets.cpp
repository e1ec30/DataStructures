/* Your code here! */
#include "dsets.h"
#include <cstdio>

void DisjointSets::addelements(int num) {
	for (int i = 0; i < num; i++) elems_.push_back(-1);
}

int DisjointSets::find(int elem) {
	if (elems_[elem] < 0) return elem;
	else {
		int root = find(elems_[elem]);
		elems_[elem] = root;
		return root;
	}
	
}

int DisjointSets::size(int elem) {
	return -elems_[find(elem)];
}

void DisjointSets::setunion(int a, int b) {
	int root_a = find(a);
	int root_b = find(b);

	if (root_a == root_b) return;

	int newSize = elems_[root_a] + elems_[root_b];
	// e1ec30: the smaller becomes a child of the bigger
	
	if (elems_[root_a] <= elems_[root_b]) {
		elems_[root_b] = root_a;
		elems_[root_a] = newSize;
	}
	else {
		elems_[root_a] = root_b;
		elems_[root_b] = newSize;
	}
	printf("union(%d, %d): %d\n", a, b, newSize);
}
