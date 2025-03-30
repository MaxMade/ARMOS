#ifndef _INC_KERNEL_ADT_RB_TREE_H_
#define _INC_KERNEL_ADT_RB_TREE_H_

#include <cstddef.h>
#include <cstdint.h>
#include <cstring.h>
#include <algorithm.h>
#include <functional.h>

/**
 * @file kernel/adt/rbtree.h
 * @brief Generic Red Black Tree
 */

/**
 * @class RBTree
 * @brief Red Black Tree
 * @tparam T Underlying type
 * @tparam Compare Compare function
 */
template<typename T, typename Compare = lib::less<T>>
class RBTree {
	public:
		/**
		 * @var BLACK
		 * @brief Color black
		 */
		static const uint8_t BLACK = 0;

		/**
		 * @var RED
		 * @brief Color red
		 */
		static const uint8_t RED = 1;

		/**
		 * @struct RBNode
		 * @brief RBTree node
		 */
		struct RBNode {
			/**
			 * @var parent
			 * @brief Pointer to parent node
			 */
			RBNode* parent;

			/**
			 * @var left
			 * @brief Pointer to left child
			 */
			RBNode* left;

			/**
			 * @var right
			 * @brief Pointer to right child
			 */
			RBNode* right;

			/**
			 * @var t
			 * @brief Underlying type
			 */
			T t;

			/**
			 * @var color
			 * @brief Color of node
			 */
			uint8_t	 color;

			/**
			 * @fn RBNode* next()
			 * @brief Returns pointer to next node in the tree
			 * @return
			 *
			 *	- Pointer to next larger element - Success
			 *	- nullptr - Failure
			 */
			RBNode* next() {
				RBNode* node = this;
				RBNode* parent = nullptr;

				if (node->right != &null_node) {
					/* One right, then keep on going left... */
					for (node = node->right; node->left != &null_node;node = node->left);

				} else {
					parent = node->parent;
					while (parent != &null_node && node == parent->right) {
						node = parent;
						parent = parent->parent;
					}
					node = parent;
				}

				return node != &null_node ? node : nullptr;
			}

			/**
			 * @fn RBNode* prev()
			 * @brief Returns pointer to previous node in the tree
			 * @return
			 *
			 *	- Pointer to previous larger element - Success
			 *	- nullptr - Failure
			 */
			RBNode* prev() {
				RBNode* node = this;
				RBNode* parent = nullptr;

				if (node->left != &null_node) {
					/* One left, then keep on going right... */
					for (node = node->left;
						node->right != &null_node;
						node = node->right);
				} else {
					parent = node->parent;
					while (parent != &null_node && node == parent->left) {
						node = parent;
						parent = parent->parent;
					}
					node = parent;
				}

				return node != &null_node ? node : nullptr;
			}
		};

	private:
		/**
		 * @var root
		 * @brief Root of the tree
		 */
		RBNode* root;

		/**
		 * @var null_node
		 * @brief Invalid node to signal nullptr
		 */
		static inline RBNode null_node;

		/**
		 * @var count
		 * @brief The number of the nodes in the tree
		 */
		size_t count;

		/**
		 * @var cmp
		 * @brief Compare function
		 */
		Compare cmp;

		/**
		 * @fn void rotate_left(RBNode* node)
		 * @brief Rotate subtree left
		 * @param node Root of subtree
		 */
		void rotate_left(RBNode* node) {
			RBNode* right = node->right;
			node->right = right->left;
			if (right->left != &null_node)
				right->left->parent = node;

			right->parent = node->parent;

			if (node->parent != &null_node) {
				if (node == node->parent->left) {
					node->parent->left = right;
				} else {
					node->parent->right = right;
				}
			} else {
				root = right;
			}
			right->left = node;
			node->parent = right;
		}

		/**
		 * @fn void rotate_right(RBNode* node)
		 * @brief Rotate subtree right
		 * @param node Root of subtree
		 */
		void rotate_right(RBNode* node) {
			RBNode* left = node->left;
			node->left = left->right;
			if (left->right != &null_node)
				left->right->parent = node;

			left->parent = node->parent;

			if (node->parent != &null_node) {
				if (node == node->parent->right) {
					node->parent->right = left;
				} else {
					node->parent->left = left;
				}
			} else {
				root = left;
			}
			left->right = node;
			node->parent = left;
		}

		/**
		 * @fn void insert_fixup(RBNode* node)
		 * @brief Fixup node colors after insertion
		 * @param node Inserted node
		 */
		void insert_fixup(RBNode* node) {
			RBNode	*uncle = nullptr;

			/* While not at the root and need fixing... */
			while (node != root && node->parent->color == RED) {
				/* If our parent is left child of our grandparent... */
				if (node->parent == node->parent->parent->left) {
					uncle = node->parent->parent->right;

					/* If our uncle is red... */
					if (uncle->color == RED) {
						/* Paint the parent and the uncle black... */
						node->parent->color = BLACK;
						uncle->color = BLACK;

						/* And the grandparent red... */
						node->parent->parent->color = RED;

						/* And continue fixing the grandparent */
						node = node->parent->parent;
					} else {				/* Our uncle is black... */
						/* Are we the right child? */
						if (node == node->parent->right) {
							node = node->parent;
							RBTree::rotate_left(node);
						}
						/* Now we're the left child, repaint and rotate... */
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						RBTree::rotate_right(node->parent->parent);
					}
				} else {
					uncle = node->parent->parent->left;

					/* If our uncle is red... */
					if (uncle->color == RED) {
						/* Paint the parent and the uncle black... */
						node->parent->color = BLACK;
						uncle->color = BLACK;

						/* And the grandparent red... */
						node->parent->parent->color = RED;

						/* And continue fixing the grandparent */
						node = node->parent->parent;

					/* Our uncle is black... */
					} else {
						/* Are we the right child? */
						if (node == node->parent->left) {
							node = node->parent;
							RBTree::rotate_right(node);
						}
						/* Now we're the right child, repaint and rotate... */
						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						RBTree::rotate_left(node->parent->parent);
					}
				}
			}
			root->color = BLACK;
		}


		/**
		 * @fn void remove_fixup(RBNode* child, RBNode* child_parent)
		 * @brief Fixup node colors after removal
		 */
		void remove_fixup(RBNode* child, RBNode* child_parent) {
			RBNode* sibling;
			int go_up = 1;

			/* determine sibling to the node that is one-black short */
			if (child_parent->right == child)
				sibling = child_parent->left;
			else
				sibling = child_parent->right;

			while (go_up) {
				if (child_parent == &null_node) {
					/* removed parent==black from root, every path, so ok */
					return;
				}

				if (sibling->color == RED) {
					/* rotate to get a black sibling */
					child_parent->color = RED;
					sibling->color = BLACK;
					if (child_parent->right == child)
						RBTree::rotate_right(child_parent);
					else	RBTree::rotate_left(child_parent);

					/* new sibling after rotation */
					if (child_parent->right == child) sibling = child_parent->left;
					else sibling = child_parent->right;
				}

				if (child_parent->color == BLACK && sibling->color == BLACK
						&& sibling->left->color == BLACK && sibling->right->color == BLACK){

					/* fixup local with recolor of sibling */
					if (sibling != &null_node)
						sibling->color = RED;

					child = child_parent;
					child_parent = child_parent->parent;
					/* prepare to go up, new sibling */
					if (child_parent->right == child)
						sibling = child_parent->left;
					else
						sibling = child_parent->right;
				}

				else go_up = 0;
			}

			if (child_parent->color == RED && sibling->color == BLACK
					&& sibling->left->color == BLACK && sibling->right->color == BLACK) {

				/* move red to sibling to rebalance */
				if (sibling != &null_node)
					sibling->color = RED;

				child_parent->color = BLACK;
				return;
			}

			/* Get a new sibling, by rotating at sibling. See which child
			 * of sibling is red
			 */
			if (child_parent->right == child && sibling->color == BLACK
					&& sibling->right->color == RED && sibling->left->color == BLACK) {

				sibling->color = RED;
				sibling->right->color = BLACK;
				RBTree::rotate_left(sibling);
				/* new sibling after rotation */
				if (child_parent->right == child)
					sibling = child_parent->left;
				else
					sibling = child_parent->right;

			} else if (child_parent->left == child && sibling->color == BLACK
					&& sibling->left->color == RED && sibling->right->color == BLACK) {

				sibling->color = RED;
				sibling->left->color = BLACK;
				RBTree::rotate_right(sibling);
				/* new sibling after rotation */
				if (child_parent->right == child)
					sibling = child_parent->left;
				else
					sibling = child_parent->right;
			}

			/* now we have a black sibling with a red child. rotate and exchange colors. */
			sibling->color = child_parent->color;
			child_parent->color = BLACK;
			if (child_parent->right == child) {
				sibling->left->color = BLACK;
				RBTree::rotate_right(child_parent);

			} else {
				sibling->right->color = BLACK;
				RBTree::rotate_left(child_parent);
			}
		}

		/**
		 * @fn void change_parent_ptr(RBNode* parent, RBNode* oldNode, RBNode* newNode)
		 * @brief Update parent pointers of child trees of \c parent
		 */
		void change_parent_ptr(RBNode* parent, RBNode* oldNode, RBNode* newNode) {
			if (parent == &null_node) {
				if (root == oldNode)
					root = newNode;

			} else if (parent->left == oldNode) {
				parent->left = newNode;

			} else if (parent->right == oldNode) {
				parent->right = newNode;
			}
		}

		/**
		 * @fn void change_child_ptr(RBNode* child, RBNode* oldNode, RBNode* newNode)
		 * @brief Update parent pointer of a node \c child
		 * */
		void change_child_ptr(RBNode* child, RBNode* oldNode, RBNode* newNode) {
			if (child == &null_node)
				return;

			if (child->parent == oldNode)
				child->parent = newNode;
		}

	public:
		/**
		 * @fn RBTree()
		 * @brief Initialize RBTree
		 */
		RBTree() {
			/* Initialize it */
			memset(&null_node, 0, sizeof(null_node));
			root = &null_node;
			count = 0;
		}

		/**
		 * @fn size_t size() const
		 * @brief Get number of elements
		 */
		size_t size() const {
			return count;
		}

		/**
		 * @fn bool empty() const
		 * @brief Check if tree is empty
		 */
		bool empty() const {
			return count == 0;
		}

		/**
		 * @fn RBNode* insert(RBNode* data)
		 * @brief Insert data into the tree.
		 * @param data Element to be inserted
		 * @return Pointer to data
		 * @warning This RB tree implementation allows dupilicate entries
		 */
		RBNode* insert(RBNode* data) {
			bool r = false;

			/* We start at the root of the tree */
			RBNode	*node = root;
			RBNode	*parent = &null_node;

			/* Lets find the new parent... */
			while (node != &null_node) {
				/* Compare two keys */
				r = cmp(data->t, node->t);
				parent = node;

				if (r) {
					node = node->left;
				} else {
					node = node->right;
				}
			}

			/* Initialize the new node */
			data->parent = parent;
			data->left = data->right = &null_node;
			data->color = RED;
			count++;

			/* Insert it into the tree... */
			if (parent != &null_node) {
				if (r) {
					parent->left = data;
				} else {
					parent->right = data;
				}
			} else {
				root = data;
			}

			/* Fix up the red-black properties... */
			RBTree::insert_fixup(data);

			return data;
		}

		/**
		 * @fn RBNode* remove(const T& other)
		 * @brief Remove (single) matching node from tree
		 * @param other Matching data
		 * @return
		 *
		 *	- Pointer to removed node - Success
		 *	- nullptr - Failure
		 */
		RBNode* remove(const T& other) {
			RBNode* to_delete = RBTree::search(other);
			if (to_delete == nullptr)
				return nullptr;

			return remove(to_delete);
		}

		/**
		 * @fn RBNode* remove(RBNode* node)
		 * @brief Remove node from tree
		 * @param node Node
		 * @warning The node must be part of the tree
		 * @return Pointer to removed node
		 */
		RBNode* remove(RBNode* node) {
			RBNode* child;
			RBNode* to_delete = node;

			/* Non-comprehensive test */
			auto result = search(node->t);
			assert(result != nullptr);

			count--;

			/* make sure we have at most one non-leaf child */
			if (to_delete->left != &null_node && to_delete->right != &null_node) {
				/* swap with smallest from right subtree (or largest from left) */
				RBNode* smright = to_delete->right;
				while (smright->left != &null_node)
					smright = smright->left;
				/* swap the smright and to_delete elements in the tree,
				 * but the RBNode is first part of user data struct
				 * so cannot just swap the keys and data pointers. Instead
				 * readjust the pointers left,right,parent */

				/* swap colors - colors are tied to the position in the tree */
				lib::swap(to_delete->color, smright->color);

				/* swap child pointers in parents of smright/to_delete */
				change_parent_ptr(to_delete->parent, to_delete, smright);
				if (to_delete->right != smright)
					change_parent_ptr(smright->parent, smright, to_delete);

				/* swap parent pointers in children of smright/to_delete */
				change_child_ptr(smright->left, smright, to_delete);
				change_child_ptr(smright->left, smright, to_delete);
				change_child_ptr(smright->right, smright, to_delete);
				change_child_ptr(smright->right, smright, to_delete);
				change_child_ptr(to_delete->left, to_delete, smright);

				if (to_delete->right != smright)
					change_child_ptr(to_delete->right, to_delete, smright);

				if (to_delete->right == smright) {
					/* set up so after swap they work */
					to_delete->right = to_delete;
					smright->parent = smright;
				}

				/* swap pointers in to_delete/smright nodes */
				lib::swap(to_delete->parent, smright->parent);
				lib::swap(to_delete->left, smright->left);
				lib::swap(to_delete->right, smright->right);

				/* now delete to_delete (which is at the location where the smright previously was) */
			}

			if (to_delete->left != &null_node) child = to_delete->left;
			else child = to_delete->right;

			/* unlink to_delete from the tree, replace to_delete with child */
			change_parent_ptr(to_delete->parent, to_delete, child);
			change_child_ptr(child, to_delete, to_delete->parent);

			if (to_delete->color == RED) {
				/* if node is red then the child (black) can be swapped in */
			} else if (child->color == RED) {
				/* change child to BLACK, removing a RED node is no problem */
				if (child!=&null_node)
					child->color = BLACK;
			} else {
				RBTree::remove_fixup(child, to_delete->parent);
			}

			/* unlink completely */
			to_delete->parent = &null_node;
			to_delete->left = &null_node;
			to_delete->right = &null_node;
			to_delete->color = BLACK;

			return to_delete;
		}

		/**
		 * @fn RBNode* search(const T& other)
		 * @brief Find key in tree
		 * @param other Matching data
		 * @return:
		 *
		 *	- Pointer to node containing other - Success
		 *	- nullptr - Failure
		 */
		RBNode* search(const T& other) {
			RBNode* node = root;

			while (node != &null_node) {
				if (node->t == other)
					return node;

				bool r = cmp(other, node->t);

				if (r) {
					node = node->left;

				} else {
					node = node->right;
				}
			}

			return nullptr;
		}

		/**
		 * @fn RBNode* first()
		 * @brief Return fist entry
		 * @return
		 *
		 *	- Pointer to first element - Success
		 *	- nullptr - Failure
		 */
		RBNode* first() {
			RBNode* node = root;

			if (root != &null_node)
				for (node = root; node->left != &null_node; node = node->left);

			return node != &null_node ? node : nullptr;
		}

		/**
		 * @fn RBNode* last()
		 * @brief Return last entry
		 * @return
		 *
		 *	- Pointer to last element - Success
		 *	- nullptr - Failure
		 */
		RBNode* last() {
			RBNode* node = root;

			if (root != &null_node)
				for (node = root; node->right != &null_node; node = node->right);

			return node != &null_node ? node : nullptr;
		}
};

#endif /* ifndef _INC_KERNEL_ADT_RB_TREE_H_ */
