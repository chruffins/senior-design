#include "../include/rbtree.h"

static chrus_rbnode *chrus_rbnode_create(void *data);
static void fix_insert(chrus_rbtree *tree, chrus_rbnode *current);
static void fix_delete(chrus_rbtree *tree, chrus_rbnode *current);
static void swap_node_data(chrus_rbnode *a, chrus_rbnode *b);
static chrus_rbnode *chrus_rbtree_min(chrus_rbnode *root);

chrus_rbtree *chrus_rbtree_create(chrus_rbtree_comparator comparator, chrus_rbtree_destructor destructor, chrus_rbtree_inserter inserter) {
    chrus_rbtree *new_tree = malloc(sizeof(chrus_rbtree));
    if (new_tree == NULL) return NULL;

    new_tree->comparator = comparator;
    new_tree->destructor = destructor;
    new_tree->inserter = inserter;
    new_tree->root = NULL;

    return new_tree;
}

void chrus_rbtree_destroy(chrus_rbtree *this) {
    // need to iterate through every node :skull:
}

chrus_rbnode *chrus_rbtree_find(chrus_rbtree *this, const void *key) {
    chrus_rbnode *current = this->root;

    while (current != NULL) {
        int result = this->comparator(key, current->key);
        if (!result) return current;

        current = result < 0 ? current->left : current->right;
    }

    return NULL;
}

chrus_rbnode *chrus_rbnode_create(void *key) {
    // this should set red to false, parent, left/right to NULL.
    chrus_rbnode *new_node = calloc(sizeof(chrus_rbnode), 1);
    if (new_node == NULL) return NULL;

    new_node->key = key;
    new_node->red = true;
}

int chrus_rbnode_child_direction(chrus_rbnode *parent, chrus_rbnode *child) {
    return parent->left == child ? 0 : 1;
}

void fix_insert(chrus_rbtree *tree, chrus_rbnode *current) {
    chrus_rbnode *parent;
    chrus_rbnode *gp;
    chrus_rbnode *uncle;
    do {
        parent = current->parent;
        gp = parent->parent;

        int dir = chrus_rbnode_child_direction(gp, parent); // 1 is right, 0 is left
        uncle = dir == 0 ? gp->right : gp->left;

        if (uncle && uncle->red) { // null nodes are black...
            parent->red = false;
            uncle->red = false;
            gp->red = true;
            current = gp;
        } else {
            // rotation time...
            int pcd = chrus_rbnode_child_direction(parent, current);
            if (pcd != dir) {
                current = current->parent;
                chrus_rbnode_rotate(current, dir);
            }

            current->parent->red = false;
			current->parent->parent->red = true;
			chrus_rbnode_rotate(current->parent->parent, pcd);
        }
    } while (current->parent->red);
}

chrus_rbnode *chrus_rbtree_min(chrus_rbnode *root) {
    chrus_rbnode *current = root;

    while (current->left) {
        current = current->left;
    }
    return current;
}

chrus_rbnode *chrus_rbtree_insert(chrus_rbtree *this, const void *key) {
    chrus_rbnode *current = this->root;
    // case 1: inserted node is root.
    // root is always black.
    if (current == NULL) {
        this->root = chrus_rbnode_create(key);
        this->root->value = this->inserter(key);
        this->root->red = false;
        return this->root;
    }

    // insert like normal BST
    chrus_rbnode *parent;
    int result;
    while (current) {
        result = this->comparator(key, current->key);
        if (result == 0) return current;

        parent = current;
        current = result < 0 ? current->left : current->right;
    }

    if (result > 0) {
        parent->right = chrus_rbnode_create(key);
        parent->right->value = this->inserter(key);
        current = parent->right;
    } else {
        parent->left = chrus_rbnode_create(key);
        parent->left->value = this->inserter(key);
        current = parent->left;
    }

    // case 2: parent is black. black nodes can have red children
    // else: case 3 and 4
    chrus_rbnode *gp;
    //chrus_rbnode *uncle;
    if (parent->red == false) {
        return current;
    }

    // implies parent is red now, and the existence of a grandparent
    gp = parent->parent;
    if (gp->left->red && gp->right->red) {
        fix_insert(this, current);
        // apparently the tree doesn't need to be rotated...
        // but the root should be recolored black.
        this->root->red = false;
        return current;
    }
}

chrus_rbnode *chrus_rbnode_rotate(chrus_rbnode *current, bool right) {
    chrus_rbnode *other = right ? current->left : current->right;

    if (right) {
        // modifying current
        current->left = other->right;
        if (current->left) current->left->parent = current;

        // modifying other
        other->parent = current->parent;
        if (current == current->parent->left) {
            current->parent->left = other;
        } else {
            current->parent->right = other;
        }

        other->right = current;
        current->parent = other;
    } else {
        // modifying current
        current->right = other->left;
        if (current->right) current->right->parent = current;

        // modifying other
        other->parent = current->parent;
        if (current == current->parent->left) {
            current->parent->left = other;
        } else {
            current->parent->right = other;
        }

        other->left = current;
        current->parent = other;
    }
}

chrus_rbnode *chrus_rbtree_successor(chrus_rbtree *tree, chrus_rbnode *node) {
    if (node->right) return chrus_rbtree_min(node->right);

    chrus_rbnode *parent = node->parent;
    while (parent && node == parent->right) {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

void swap_node_data(chrus_rbnode *a, chrus_rbnode *b) {
    if (a == b) return;

    void *temp = b->value;
    b->value = a->value;
    a->value = temp;

    temp = b->key;
    b->key = a->key;
    a->key = temp;
}

void fix_delete(chrus_rbtree *tree, chrus_rbnode *current) {
    // try not to kill yourself challenge
    chrus_rbnode *sibling;
    int current_dir = chrus_rbnode_child_direction(current->parent, current);
    do {
        if (current_dir == 0) {
            sibling = current->parent->right;

            if (sibling->red) {
                sibling->red = false;
                current->parent->red = true;
                chrus_rbnode_rotate(current->parent, current_dir);
                sibling = current->parent->right;
            }

            if ((!sibling->right || !sibling->right->red) && (!sibling->left || !sibling->left->red)) {
                sibling->red = true;
                if (current->parent->red) {
                    current->parent->red = false;
                    break;
                } else {
                    current = current->parent;
                }
            } else {
                if (!sibling->right || !sibling->right->red) {
                    sibling->left->red = false;
                    sibling->red = true;
                    chrus_rbnode_rotate(sibling, !current_dir);
                    sibling = current->parent->right;
                }

                sibling->red = current->parent->red;
                current->parent->red = false;
                sibling->right->red = false;
                chrus_rbnode_rotate(current->parent, current_dir);
                break;
            }
        } else {
            sibling = current->parent->left;

            if (sibling->red) {
                sibling->red = false;
                current->parent->red = true;
                chrus_rbnode_rotate(current->parent, current_dir);
                sibling = current->parent->left;
            }

            if ((!sibling->right || !sibling->right->red) && (!sibling->left || !sibling->left->red)) {
                sibling->red = true;
                if (current->parent->red) {
                    current->parent->red = false;
                    break;
                } else {
                    current = current->parent;
                }
            } else {
                if (!sibling->left || !sibling->left->red) {
                    sibling->right->red = false;
                    sibling->red = true;
                    chrus_rbnode_rotate(sibling, !current_dir);
                    sibling = current->parent->left;
                }

                sibling->red = current->parent->red;
                current->parent->red = false;
                sibling->left->red = false;
                chrus_rbnode_rotate(current->parent, current_dir);
                break;
            }
        }
    } while (current != tree->root->left);
}

// returns the key if found...
int chrus_rbtree_delete(chrus_rbtree *this, void *key) {
    // let's get the node
    chrus_rbnode *dl = chrus_rbtree_find(this, key);
    if (!dl) return 1;

    // two children, so we just swap values with the in-order successor
    // and then delete the successor instead
    if (dl->left && dl->right) {
        chrus_rbnode *succ = chrus_rbtree_successor(this, dl);
        swap_node_data(dl, succ);
        dl = succ;
    }

    // successor implies less than two children (why?)
    // so move on to other cases

    // this means no children
    // which has some pretty simple cases
    if (!dl->left && !dl->right) {
        if (dl == this->root) {
            this->root = NULL;
            //free(dl);
            //return retdata;
        }

        if (dl->red) {
            int dir = chrus_rbnode_child_direction(dl->parent, dl);
            if (dir) {
                dl->parent->right = NULL;
            } else {
                dl->parent->left = NULL;
            }
        } else {
            fix_delete(this, dl);
        }
    } else {
        // implying there is one child
        chrus_rbnode *child = dl->left ? dl->left : dl->right;

        // if a node has only one child, it must be a red child
        // obviously this implies the node is black
        child->red = false;

        child->parent = dl->parent;
        if (dl == dl->parent->left) {
            dl->parent->left = child;
        } else {
            dl->parent->right = child;
        }
    }

    this->destructor(dl->value);
    free(dl);
    return 0;
}
