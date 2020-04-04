#ifndef _MEOW_H_
#define _MEOW_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
	struct Node *l, *r;
	int key, _key, sz;
	void *val;
};

void Meow_MaintainSize(struct Node *root);
void Meow_Balance(struct Node **root);
void Meow_Insert(struct Node **root, int key, int _key, void *val);

void *Meow_PeekBegin(struct Node *root);
void *Meow_PopBegin(struct Node **root);

void _Debug_Meow_PrintTree(struct Node *root, int dep);

#endif // _MEOW_H_
