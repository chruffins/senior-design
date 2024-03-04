#include "../include/rbtree.h"

static chrus_rbnode *chrus_rbnode_create(chrus_rbkey data);
static void fix_insert(chrus_rbtree *tree, chrus_rbnode *current);
static void fix_delete(chrus_rbtree *tree, chrus_rbnode *current);
static void swap_node_data(chrus_rbnode *a, chrus_rbnode *b);
static chrus_rbnode *chrus_rbtree_min(chrus_rbnode *root);
static void delete_node(chrus_rbtree *tree, chrus_rbnode *this);

static void chrus_rbnode_rotate(chrus_rbtree* tree, chrus_rbnode *current, bool right);

static int default_comparator(chrus_rbkey search, chrus_rbkey nodekey) {
    if (search.keynum == nodekey.keynum) return 0;

    return search.keynum < nodekey.keynum ? -1 : 1;
}

chrus_rbtree *chrus_rbtree_create(chrus_rbtree_comparator comparator, chrus_rbtree_destructor destructor, chrus_rbtree_inserter inserter) {
    chrus_rbtree *new_tree = malloc(sizeof(chrus_rbtree));
    if (new_tree == NULL) return NULL;

    if (comparator == NULL) comparator = default_comparator;

    new_tree->comparator = comparator;
    new_tree->destructor = destructor;
    new_tree->inserter = inserter;
    new_tree->root = NULL;

    return new_tree;
}

static void delete_node(chrus_rbtree *tree, chrus_rbnode *this) {
    if (this == NULL) return;

    delete_node(tree, this->left);
    delete_node(tree, this->right);

    // TODO: what to do with key
    if (tree->destructor) tree->destructor(this->value);

    free(this);
}

void chrus_rbtree_destroy(chrus_rbtree *this) {
    delete_node(this, this->root);
    free(this);
}

chrus_rbnode *chrus_rbtree_find(chrus_rbtree *this, chrus_rbkey key) {
    chrus_rbnode *current = this->root;

    while (current != NULL) {
        int result = this->comparator(key, current->key);
        if (!result) return current;

        current = result < 0 ? current->left : current->right;
    }

    return NULL;
}

chrus_rbnode *chrus_rbnode_create(chrus_rbkey key) {
    // this should set red to false, parent, left/right to NULL.
    chrus_rbnode *new_node = calloc(sizeof(chrus_rbnode), 1);
    if (new_node == NULL) return NULL;

    new_node->key = key;
    new_node->red = true;

    return new_node;
}

int chrus_rbnode_child_direction(chrus_rbnode *parent, chrus_rbnode *child) {
    return parent->left == child ? 0 : 1;
}

void fix_insert(chrus_rbtree *tree, chrus_rbnode *current) {
    chrus_rbnode *parent;
    chrus_rbnode *gp;
    chrus_rbnode *uncle;
    /* currently the current and parent nodes are both red */
    do {
        parent = current->parent;
        gp = parent->parent;

        int gp_parent_dir = chrus_rbnode_child_direction(gp, parent); // 1 is right, 0 is left

        /* if the parent is on the left, then the uncle is on the right and vice versa */
        uncle = gp_parent_dir == 0 ? gp->right : gp->left;

        if (uncle && uncle->red == true) { // null nodes are black...
            /* set parent and uncle to black */
            parent->red = false;
            uncle->red = false;

            /* set grandparent to red then set current as gp */
            gp->red = true;
            current = gp;
        } else {
            /* implies lack of uncle or black uncle */
            // rotation time...

            /* if parent on LEFT of gp AND current on RIGHT of parent rotate LEFT */
            int parent_child_dir = chrus_rbnode_child_direction(parent, current);
            if (parent_child_dir != gp_parent_dir) {
                current = current->parent;
                chrus_rbnode_rotate(tree, current, gp_parent_dir);
            }

            current->parent->red = false;
			current->parent->parent->red = true;

            /* always rotate OPPOSITE of gp_parent_dir */
			chrus_rbnode_rotate(tree, current->parent->parent, 1 - gp_parent_dir);
        }
    } while (current->parent && current->parent->red == true);
}

chrus_rbnode *chrus_rbtree_min(chrus_rbnode *root) {
    chrus_rbnode *current = root;

    while (current->left) {
        current = current->left;
    }
    return current;
}

chrus_rbnode *chrus_rbtree_insert(chrus_rbtree *this, chrus_rbkey key) {
    chrus_rbnode *current = this->root;
    // case 1: inserted node is root.
    // root is always black.
    if (current == NULL) {
        this->root = chrus_rbnode_create(key);
        if (this->inserter) this->root->value = this->inserter(key);
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
        if (this->inserter) parent->right->value = this->inserter(key);
        current = parent->right;
    } else {
        parent->left = chrus_rbnode_create(key);
        if (this->inserter) parent->left->value = this->inserter(key);
        current = parent->left;
    }

    current->parent = parent;

    // case 2: parent is black. black nodes can have red children
    // else: case 3 and 4
    //chrus_rbnode *gp;
    //chrus_rbnode *uncle;
    if (parent->red == false) {
        return current;
    } else {
        // implies the existence of a grandparent
        fix_insert(this, current);
        this->root->red = false;
        return current;
    }
}

/* TODO: compact this and regular insert together?*/
chrus_rbnode *chrus_rbtree_insert_pair(chrus_rbtree* this, chrus_rbkey key, void* value) {
    chrus_rbnode *current = this->root;
    // case 1: inserted node is root.
    // root is always black.
    if (current == NULL) {
        this->root = chrus_rbnode_create(key);
        this->root->value = value;
        this->root->red = false;
        return this->root;
    }

    // insert like normal BST
    chrus_rbnode *parent;
    int result;
    while (current) {
        result = this->comparator(key, current->key);
        if (result == 0) {
            current->value = value;
            return current;
        }
        parent = current;
        current = result < 0 ? current->left : current->right;
    }

    if (result > 0) {
        parent->right = chrus_rbnode_create(key);
        parent->right->value = value;
        current = parent->right;
    } else {
        parent->left = chrus_rbnode_create(key);
        parent->left->value = value;
        current = parent->left;
    }

    current->parent = parent;

    // case 2: parent is black. black nodes can have red children
    // else: case 3 and 4
    //chrus_rbnode *gp;
    //chrus_rbnode *uncle;
    if (parent->red == false) {
        return current;
    } else {
        // implies the existence of a grandparent
        fix_insert(this, current);
        this->root->red = false;
        return current;
    }
}

static void chrus_rbnode_rotate(chrus_rbtree* tree, chrus_rbnode *current, bool right) {
    /* if rotating right then other = left
    if rotating left then other = right */
    chrus_rbnode *other = right ? current->left : current->right;
    if (current == tree->root) tree->root = other;

    if (right) {
        // modifying current
        current->left = other->right;
        if (current->left) current->left->parent = current;

        // modifying other
        other->parent = current->parent;
        if (other->parent) {
            if (current == current->parent->left) {
                current->parent->left = other;
            } else {
                current->parent->right = other;
            }
        }

        other->right = current;
        current->parent = other;
    } else {
        /* if you're rotating left then you turn the current node into the left child of your right child*/

        // modifying current
        current->right = other->left;
        if (current->right) current->right->parent = current;

        // modifying other
        other->parent = current->parent;
        if (other->parent) {
            if (current == current->parent->left) {
                current->parent->left = other;
            } else {
                current->parent->right = other;
            }
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

    chrus_rbkey tempkey = b->key;
    b->key = a->key;
    a->key = tempkey;
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
                chrus_rbnode_rotate(tree, current->parent, current_dir);
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
                    chrus_rbnode_rotate(tree, sibling, !current_dir);
                    sibling = current->parent->right;
                }

                sibling->red = current->parent->red;
                current->parent->red = false;
                sibling->right->red = false;
                chrus_rbnode_rotate(tree, current->parent, current_dir);
                break;
            }
        } else {
            sibling = current->parent->left;

            if (sibling->red) {
                sibling->red = false;
                current->parent->red = true;
                chrus_rbnode_rotate(tree, current->parent, current_dir);
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
                    chrus_rbnode_rotate(tree, sibling, !current_dir);
                    sibling = current->parent->left;
                }

                sibling->red = current->parent->red;
                current->parent->red = false;
                sibling->left->red = false;
                chrus_rbnode_rotate(tree, current->parent, current_dir);
                break;
            }
        }
    } while (current != tree->root->left);
}

// returns the key if found...
int chrus_rbtree_delete(chrus_rbtree *this, chrus_rbkey key) {
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

chrus_rbkey chrus_rbkey_create(const void* keyptr) {
    return (chrus_rbkey){ .keyptr = keyptr };
}

chrus_rbkey chrus_rbkey_from_uint(uint64_t key) {
    return (chrus_rbkey){ .keynum = key };
}

int chrus_rbtree_height(chrus_rbtree* this, chrus_rbnode* current) {
    if (current == NULL) return 0;

    int left_height = chrus_rbtree_height(this, current->left);
    int right_height = chrus_rbtree_height(this, current->right);

    if (left_height > right_height) {
        return left_height + 1;
    } else {
        return right_height + 1;
    }
}

int chrus_rbtree_black_height(chrus_rbtree* this, chrus_rbnode* current) {
    if (current == NULL) return 0;

    int left_height = chrus_rbtree_height(this, current->left);
    int right_height = chrus_rbtree_height(this, current->right);

    if (left_height > right_height) {
        return left_height + (1 - current->red);
    } else {
        return right_height + (1 - current->red);
    }
}

bool chrus_rbtree_valid(chrus_rbtree* this, chrus_rbnode* current) {
    if (current == NULL) return true;

    /* root is BLACK */
    if (this->root == current && current->red == true) return false;

    /* red nodes don't have red children */
    if (current->red == true) {
        if ((current->left && current->left->red) || (current->right && current->right->red)) return false;
    }

    return true && chrus_rbtree_valid(this, current->left) && chrus_rbtree_valid(this, current->right);
}