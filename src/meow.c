#include "meow.h"

void Meow_MaintainSize(struct Node *root) {
	root->sz = 1 + (root->l ? root->l->sz : 0) + (root->r ? root->r->sz : 0);
}

void Meow_Balance(struct Node **root) {
	if ((*root)->l && (*root)->l->sz > ((*root)->r ? (*root)->r->sz * 2 : 5)) {
		struct Node *tmp = (*root)->l;
		(*root)->l = tmp->r;
		tmp->r = *root;
		*root = tmp;

		Meow_MaintainSize((*root)->r);
		Meow_MaintainSize(*root);
	} else if ((*root)->r && (*root)->r->sz > ((*root)->l ? (*root)->l->sz * 2 : 5)) {
		struct Node *tmp = (*root)->r;
		(*root)->r = tmp->l;
		tmp->l = *root;
		*root = tmp;

		Meow_MaintainSize((*root)->l);
		Meow_MaintainSize(*root);
	}
}

void Meow_Insert(struct Node **root, int key, int _key, void *val) {
	if (*root == 0) {
		*root = calloc(1, sizeof(struct Node));
		(*root)->sz = 1;
		(*root)->key = key;
		(*root)->val = val;
		return;
	}

	if (key < (*root)->key || (key == (*root)->key && _key < (*root)->_key)) Meow_Insert(&(*root)->l, key, _key, val);
	else Meow_Insert(&(*root)->r, key, _key, val);

	Meow_MaintainSize(*root);
	Meow_Balance(root);
}

void *Meow_PeekBegin(struct Node *root) {
	if (!root) return 0;
	if (root->l) return Meow_PeekBegin(root->l);
	return root->val;
}

void *Meow_PopBegin(struct Node **root) {
	assert(root || (fprintf(stderr, "Try to %s in empty node\n", __func__), 0));
	if ((*root)->l) return Meow_PopBegin(&(*root)->l);

	struct Node *me = *root;
	if ((*root)->r) *root = ((*root)->r);
	else *root = 0;

	void *rt = me->val;
	free(me);

	return rt;
}

void _Debug_Meow_PrintTree(struct Node *root, int dep) {
	if (!root) return;
	for (int i = 0; i < dep; ++i) fprintf(stderr, " ");
	fprintf(stderr, "key %d process %p\n", root->key, root->val);
	if (root->l) _Debug_Meow_PrintTree(root->l, dep + 2);
	if (root->r) _Debug_Meow_PrintTree(root->r, dep + 2);
}
