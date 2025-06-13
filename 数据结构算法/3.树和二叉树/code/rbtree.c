/* rbtree.c - 红黑树
 * create by: zw.duan
 * 完整实现红黑树的所有核心操作
 * 
 * 红黑树（Red-Black Tree）是一种自平衡的二叉搜索树
 * 
 * ===== 红黑树的五个性质 =====
 * 1. 每个节点要么是红色，要么是黑色
 * 2. 根节点是黑色的
 * 3. 所有叶子节点（NIL节点）都是黑色的
 * 4. 如果一个节点是红色的，则它的两个子节点都是黑色的（不能有连续的红色节点）
 * 5. 对于每个节点，从该节点到其所有后代叶子节点的简单路径上，均包含相同数目的黑色节点
 * 
 * ===== 红黑树的优势 =====
 * - 保证最坏情况下的时间复杂度为 O(log n)
 * - 插入、删除、查找操作都是 O(log n)
 * - 相比AVL树，旋转次数更少，适合频繁插入删除的场景
 * 
 * ===== 关键概念 =====
 * - 黑高（Black Height）：从某节点到叶子节点路径上黑色节点的数量
 * - NIL节点：虚拟的叶子节点，统一用一个哨兵节点表示，颜色为黑色
 * - 旋转操作：维护树平衡的基本操作，分为左旋和右旋
 */

#include "rbtree.h"
#include <time.h>

// 内部辅助函数声明
void DestroyRBTreeHelper(RBTree *T, RBNode *node);
int RBTreeDepthHelper(RBTree *T, RBNode *node);
int RBTreeNodeCountHelper(RBTree *T, RBNode *node);
void RBTransplant(RBTree *T, RBNode *u, RBNode *v);
RBNode* CreateNilNode(RBTree *T);

// ============= 完整的红黑树实现 =============

/**
 * 创建NIL哨兵节点
 * 
 * NIL节点是红黑树的关键设计：
 * - 统一处理边界情况，简化代码逻辑
 * - 所有真实节点的空子节点都指向同一个NIL节点
 * - NIL节点的颜色必须是黑色（满足性质3）
 * - NIL节点的父节点、左右子节点都指向自己
 * 
 * 图示：
 *     [10]B
 *    /     \
 *  [5]R   [15]R
 *  / \     / \
 * NIL NIL NIL NIL  <- 所有NIL节点实际是同一个哨兵节点
 */
RBNode* CreateNilNode(RBTree *T) {
    (void)T; // 避免未使用参数警告
    RBNode *nil = (RBNode*)malloc(sizeof(RBNode));
    if (nil == NULL) return NULL;
    
    nil->data = 0;  // NIL节点数据无意义
    nil->color = BLACK;  // NIL节点必须为黑色
    nil->parent = nil->left = nil->right = nil;  // 指向自己
    return nil;
}

/**
 * 初始化红黑树
 * 
 * 初始状态：
 * - root指向NIL节点（空树）
 * - 所有操作都以NIL作为边界条件
 */
void InitRBTree(RBTree *T) {
    T->nil = CreateNilNode(T);
    T->root = T->nil;
    T->size = 0;
}

/**
 * 判断红黑树是否为空
 */
bool RBTreeEmpty(RBTree *T) {
    return T->root == T->nil;
}

/**
 * 获取红黑树大小
 */
int RBTreeSize(RBTree *T) {
    return T->size;
}

/**
 * 创建红黑树结点
 * 
 * 新节点的初始状态：
 * - 颜色设为红色（减少对红黑树性质的破坏）
 * - 左右子节点和父节点都指向NIL
 * 
 * 为什么新节点是红色？
 * - 如果插入黑色节点，会立即违反性质5（黑高平衡）
 * - 插入红色节点最多违反性质4（红色父子），可以通过旋转和重新着色修复
 */
RBNode* CreateRBNode(RBTree *T, RBElemType data) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (node == NULL) return NULL;
    
    node->data = data;
    node->color = RED;  // 新结点默认为红色
    node->left = node->right = node->parent = T->nil;
    return node;
}

/**
 * 在红黑树中搜索
 * 
 * 搜索过程与普通BST相同：
 * - 从根节点开始
 * - 比较目标值与当前节点值
 * - 小于则向左，大于则向右
 * - 直到找到目标或到达NIL节点
 * 
 * 时间复杂度：O(log n) - 由红黑树的平衡性保证
 */
RBNode* RBTreeSearch(RBTree *T, RBElemType key) {
    RBNode *current = T->root;
    
    while (current != T->nil && current->data != key) {
        if (key < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return (current == T->nil) ? NULL : current;
}

/**
 * 查找最小值结点
 * 
 * 原理：BST的最小值总是在最左边
 * 
 * 图示：
 *       10
 *      /  \
 *     5    15
 *    / \
 *   2   7
 *  /     \
 * 1       8  <- 最小值是1，位于最左边
 */
RBNode* RBTreeMinimum(RBTree *T, RBNode *x) {
    if (x == NULL || x == T->nil) return NULL;
    while (x->left != T->nil) {
        x = x->left;
    }
    return x;
}

/**
 * 查找最大值结点
 * 
 * 原理：BST的最大值总是在最右边
 */
RBNode* RBTreeMaximum(RBTree *T, RBNode *x) {
    if (x == NULL || x == T->nil) return NULL;
    while (x->right != T->nil) {
        x = x->right;
    }
    return x;
}

/**
 * 查找后继结点
 * 
 * 后继节点：中序遍历中的下一个节点
 * 
 * 两种情况：
 * 1. 如果x有右子树，则后继是右子树的最小值
 * 2. 如果x没有右子树，则向上找到第一个"左祖先"
 * 
 * 图示情况1：
 *     10
 *    /  \
 *   5    15
 *       /  \
 *      12   20
 *        \
 *         14  <- 15的后继是12（右子树最小值）
 * 
 * 图示情况2：
 *     10
 *    /  \
 *   5    15  <- 5的后继是10（第一个左祖先）
 *  / \
 * 2   7
 */
RBNode* RBTreeSuccessor(RBTree *T, RBNode *x) {
    if (x == NULL || x == T->nil) return NULL;
    
    if (x->right != T->nil) {
        return RBTreeMinimum(T, x->right);
    }
    
    RBNode *y = x->parent;
    while (y != T->nil && x == y->right) {
        x = y;
        y = y->parent;
    }
    return (y == T->nil) ? NULL : y;
}

/**
 * 查找前驱结点
 * 
 * 前驱节点：中序遍历中的前一个节点
 * 原理与后继相反
 */
RBNode* RBTreePredecessor(RBTree *T, RBNode *x) {
    if (x == NULL || x == T->nil) return NULL;
    
    if (x->left != T->nil) {
        return RBTreeMaximum(T, x->left);
    }
    
    RBNode *y = x->parent;
    while (y != T->nil && x == y->left) {
        x = y;
        y = y->parent;
    }
    return (y == T->nil) ? NULL : y;
}

/**
 * 左旋操作
 * 
 * 左旋是红黑树维护平衡的基本操作之一
 * 
 * 左旋前：          左旋后：
 *     x               y
 *    / \             / \
 *   α   y           x   γ
 *      / \         / \
 *     β   γ       α   β
 * 
 * 操作步骤：
 * 1. 设置y = x.right
 * 2. 将y的左子树β转为x的右子树
 * 3. 将x的父节点连接到y
 * 4. 将x设为y的左子节点
 * 
 * 旋转保持了BST的性质：α < x < β < y < γ
 * 时间复杂度：O(1)
 */
void LeftRotate(RBTree *T, RBNode *x) {
    RBNode *y = x->right;  // 设置y
    x->right = y->left;    // 将y的左子树转换为x的右子树
    
    if (y->left != T->nil) {
        y->left->parent = x;
    }
    
    y->parent = x->parent; // 连接y的父节点
    if (x->parent == T->nil) {
        T->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;  // 将x作为y的左子节点
    x->parent = y;
}

/**
 * 右旋操作
 * 
 * 右旋前：          右旋后：
 *     x               y
 *    / \             / \
 *   y   γ           α   x
 *  / \                 / \
 * α   β               β   γ
 * 
 * 右旋是左旋的镜像操作
 */
void RightRotate(RBTree *T, RBNode *x) {
    RBNode *y = x->left;   // 设置y
    x->left = y->right;    // 将y的右子树转换为x的左子树
    
    if (y->right != T->nil) {
        y->right->parent = x;
    }
    
    y->parent = x->parent; // 连接y的父节点
    if (x->parent == T->nil) {
        T->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    
    y->right = x; // 将x作为y的右子节点
    x->parent = y;
}

/**
 * 插入修复操作
 * 
 * 插入红色节点后可能违反的性质：
 * - 性质4：如果父节点也是红色，则出现连续红色节点
 * 
 * 修复策略：通过旋转和重新着色维护红黑树性质
 * 
 * 修复的三种情况（以z为新插入节点，p为父节点，pp为祖父节点，u为叔叔节点）：
 * 
 * 情况1：叔叔u是红色
 * 修复：重新着色
 *   pp(B)           pp(R)
 *   /   \    -->    /   \
 * p(R)  u(R)      p(B)  u(B)
 * /               /
 *z(R)            z(R)
 * 然后将pp设为当前节点继续向上修复
 * 
 * 情况2：叔叔u是黑色，z是右子节点
 * 修复：左旋转换为情况3
 *   pp(B)           pp(B)
 *   /   \    -->    /   \
 * p(R)  u(B)      z(R)  u(B)
 *   \             /
 *   z(R)        p(R)
 * 
 * 情况3：叔叔u是黑色，z是左子节点
 * 修复：重新着色+右旋
 *   pp(B)           p(B)
 *   /   \    -->    /   \
 * p(R)  u(B)      z(R)  pp(R)
 * /                       \
 *z(R)                     u(B)
 */
void RBInsertFixup(RBTree *T, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right; // y是z的叔叔
            if (y->color == RED) {
                // 情况1：z的叔叔y是红色
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // 情况2：z的叔叔y是黑色且z是右子节点
                    z = z->parent;
                    LeftRotate(T, z);
                }
                // 情况3：z的叔叔y是黑色且z是左子节点
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(T, z->parent->parent);
            }
        } else {
            // 镜像情况：z的父节点是祖父节点的右子节点
            RBNode *y = z->parent->parent->left; // y是z的叔叔
            if (y->color == RED) {
                // 情况1：z的叔叔y是红色
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // 情况2：z的叔叔y是黑色且z是左子节点
                    z = z->parent;
                    RightRotate(T, z);
                }
                // 情况3：z的叔叔y是黑色且z是右子节点
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK; // 确保根节点是黑色
}

/**
 * 红黑树插入
 * 
 * 插入过程：
 * 1. 按BST规则找到插入位置
 * 2. 插入红色节点
 * 3. 调用修复函数维护红黑树性质
 * 
 * 时间复杂度：O(log n)
 */
bool RBTreeInsert(RBTree *T, RBElemType key) {
    RBNode *z = CreateRBNode(T, key);
    if (z == NULL) return false;
    
    RBNode *y = T->nil;
    RBNode *x = T->root;
    
    // 找到插入位置
    while (x != T->nil) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else if (z->data > x->data) {
            x = x->right;
        } else {
            // 重复键值
            free(z);
            return false;
        }
    }
    
    z->parent = y;
    if (y == T->nil) {
        T->root = z;  // 树是空的
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    
    RBInsertFixup(T, z);
    T->size++;
    return true;
}

/**
 * 节点移植操作
 * 
 * 用子树v替换子树u：
 * - 将u的父节点连接到v
 * - 不处理v的子节点（调用者负责）
 * 
 * 图示：
 *     p              p
 *     |      -->     |
 *     u              v
 *    / \            / \
 *   ...            ...
 */
void RBTransplant(RBTree *T, RBNode *u, RBNode *v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

/**
 * 删除修复操作
 * 
 * 删除黑色节点后可能违反的性质：
 * - 性质5：某条路径上的黑色节点数量减少
 * 
 * 修复策略：通过旋转和重新着色恢复黑高平衡
 * 
 * 修复的四种情况（以x为替换节点，w为x的兄弟节点）：
 * 
 * 情况1：兄弟w是红色
 * 转换：将w变为黑色，父节点变为红色，左旋
 *    p(B)            w(B)
 *   /   \     -->   /   \
 * x(B)  w(R)      p(R)  ...
 *      /  \       /  \
 *    wl(B) wr(B) x(B) wl(B)
 * 
 * 情况2：兄弟w是黑色，w的两个子节点都是黑色
 * 修复：将w变为红色，将问题向上传递
 *    p(?)            p(?)
 *   /   \     -->   /   \
 * x(B)  w(B)      x(B)  w(R)
 *      /  \            /  \
 *    wl(B) wr(B)     wl(B) wr(B)
 * 
 * 情况3：兄弟w是黑色，w的左子节点是红色，右子节点是黑色
 * 转换：交换w和wl的颜色，右旋w
 *    p(?)            p(?)
 *   /   \     -->   /   \
 * x(B)  w(B)      x(B)  wl(B)
 *      /  \              \
 *    wl(R) wr(B)          w(R)
 *                           \
 *                          wr(B)
 * 
 * 情况4：兄弟w是黑色，w的右子节点是红色
 * 修复：调整颜色，左旋，完成修复
 *    p(?)            w(?)
 *   /   \     -->   /   \
 * x(B)  w(B)      p(B)  wr(B)
 *      /  \       /  \
 *   wl(?) wr(R) x(B) wl(?)
 */
void RBDeleteFixup(RBTree *T, RBNode *x) {
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                // 情况1：x的兄弟w是红色
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                // 情况2：x的兄弟w是黑色，且w的两个子节点都是黑色
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    // 情况3：x的兄弟w是黑色，w的左子节点是红色，右子节点是黑色
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(T, w);
                    w = x->parent->right;
                }
                // 情况4：x的兄弟w是黑色，且w的右子节点是红色
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(T, x->parent);
                x = T->root;
            }
        } else {
            // 镜像情况：x是右子节点
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                // 情况1：x的兄弟w是红色
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                // 情况2：x的兄弟w是黑色，且w的两个子节点都是黑色
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    // 情况3：x的兄弟w是黑色，w的右子节点是红色，左子节点是黑色
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(T, w);
                    w = x->parent->left;
                }
                // 情况4：x的兄弟w是黑色，且w的左子节点是红色
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

/**
 * 红黑树删除
 * 
 * 删除过程比插入复杂，需要考虑三种情况：
 * 1. 被删节点没有子节点：直接删除
 * 2. 被删节点有一个子节点：用子节点替换
 * 3. 被删节点有两个子节点：用后继节点替换，然后删除后继节点
 * 
 * 关键点：
 * - 只有删除黑色节点才可能破坏红黑树性质
 * - 用y记录实际被删除的节点颜色
 * - 如果删除的是黑色节点，需要调用修复函数
 * 
 * 时间复杂度：O(log n)
 */
bool RBTreeDelete(RBTree *T, RBElemType key) {
    RBNode *z = RBTreeSearch(T, key);
    if (z == NULL) return false;
    
    // 将找到的节点重新设置为内部节点（不是NULL）
    RBNode *y = z;
    RBNode *x;
    RBColor y_original_color = y->color;
    
    if (z->left == T->nil) {
        x = z->right;
        RBTransplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        RBTransplant(T, z, z->left);
    } else {
        y = RBTreeMinimum(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            RBTransplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RBTransplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    if (y_original_color == BLACK) {
        RBDeleteFixup(T, x);
    }
    
    free(z);
    T->size--;
    return true;
}

/**
 * 先序遍历
 * 遍历顺序：根 -> 左子树 -> 右子树
 */
void RBTreePreOrder(RBTree *T, RBNode *x) {
    if (x != T->nil) {
        printf("%d(%s) ", x->data, x->color == RED ? "R" : "B");
        RBTreePreOrder(T, x->left);
        RBTreePreOrder(T, x->right);
    }
}

/**
 * 中序遍历
 * 遍历顺序：左子树 -> 根 -> 右子树
 * 结果是有序序列
 */
void RBTreeInOrder(RBTree *T, RBNode *x) {
    if (x != T->nil) {
        RBTreeInOrder(T, x->left);
        printf("%d(%s) ", x->data, x->color == RED ? "R" : "B");
        RBTreeInOrder(T, x->right);
    }
}

/**
 * 后序遍历
 * 遍历顺序：左子树 -> 右子树 -> 根
 */
void RBTreePostOrder(RBTree *T, RBNode *x) {
    if (x != T->nil) {
        RBTreePostOrder(T, x->left);
        RBTreePostOrder(T, x->right);
        printf("%d(%s) ", x->data, x->color == RED ? "R" : "B");
    }
}

/**
 * 验证红黑树性质
 * 
 * 验证所有五个红黑树性质：
 * 1. 每个节点颜色是红或黑 - CheckRBProperty1
 * 2. 根节点是黑色 - CheckRBProperty2  
 * 3. NIL节点是黑色 - 由NIL节点创建保证
 * 4. 红色节点的子节点都是黑色 - CheckRBProperty4
 * 5. 每条路径的黑色节点数相同 - CheckRBProperty5
 */
bool ValidateRBTree(RBTree *T) {
    if (T->root == T->nil) return true;
    
    // 检查根节点是否为黑色
    if (T->root->color != BLACK) return false;
    
    // 检查所有性质
    return CheckRBProperty1(T, T->root) && 
           CheckRBProperty2(T) && 
           CheckRBProperty4(T, T->root) &&
           (CheckRBProperty5(T, T->root) != -1);
}

/**
 * 检查性质1：每个节点要么是红色，要么是黑色
 */
bool CheckRBProperty1(RBTree *T, RBNode *node) {
    if (node == T->nil) return true;
    
    if (node->color != RED && node->color != BLACK) return false;
    
    return CheckRBProperty1(T, node->left) && 
           CheckRBProperty1(T, node->right);
}

/**
 * 检查性质2：根节点是黑色
 */
bool CheckRBProperty2(RBTree *T) {
    return T->root == T->nil || T->root->color == BLACK;
}

/**
 * 检查性质4：如果一个节点是红色，则它的两个子节点都是黑色
 */
bool CheckRBProperty4(RBTree *T, RBNode *node) {
    if (node == T->nil) return true;
    
    if (node->color == RED) {
        if (node->left->color == RED || node->right->color == RED) {
            return false;
        }
    }
    
    return CheckRBProperty4(T, node->left) && 
           CheckRBProperty4(T, node->right);
}

/**
 * 检查性质5：对于每个节点，从该节点到其所有后代叶子节点的简单路径上，
 * 均包含相同数目的黑色节点
 * 
 * 返回值：黑高值，-1表示违反性质5
 */
int CheckRBProperty5(RBTree *T, RBNode *node) {
    if (node == T->nil) return 1; // NIL节点贡献1个黑色节点
    
    int leftBlackHeight = CheckRBProperty5(T, node->left);
    int rightBlackHeight = CheckRBProperty5(T, node->right);
    
    if (leftBlackHeight == -1 || rightBlackHeight == -1 || 
        leftBlackHeight != rightBlackHeight) {
        return -1; // 黑高不相等
    }
    
    return leftBlackHeight + (node->color == BLACK ? 1 : 0);
}

/**
 * 打印红黑树
 */
void PrintRBTree(RBTree *T) {
    printf("红黑树中序遍历：");
    RBTreeInOrder(T, T->root);
    printf("\n");
    printf("树的大小：%d\n", T->size);
}

/**
 * 打印红黑树结构
 * 
 * 横向显示树的结构，方便观察：
 * - 右子树在上方
 * - 根节点在中间  
 * - 左子树在下方
 * - 用缩进表示层次关系
 */
void PrintRBTreeStructure(RBTree *T, RBNode *node, int space) {
    if (node == T->nil) return;
    
    space += 4;
    PrintRBTreeStructure(T, node->right, space);
    
    printf("\n");
    for (int i = 4; i < space; i++) printf(" ");
    printf("%d(%s)\n", node->data, node->color == RED ? "R" : "B");
    
    PrintRBTreeStructure(T, node->left, space);
}

/**
 * 计算红黑树深度
 * 深度 = 从根到最远叶子节点的路径长度
 */
int RBTreeDepth(RBTree *T) {
    return RBTreeDepthHelper(T, T->root);
}

int RBTreeDepthHelper(RBTree *T, RBNode *node) {
    if (node == T->nil) return 0;
    
    int leftDepth = RBTreeDepthHelper(T, node->left);
    int rightDepth = RBTreeDepthHelper(T, node->right);
    
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

/**
 * 计算红黑树结点数
 */
int RBTreeNodeCount(RBTree *T, RBNode *node) {
    return RBTreeNodeCountHelper(T, node);
}

int RBTreeNodeCountHelper(RBTree *T, RBNode *node) {
    if (node == T->nil) return 0;
    return RBTreeNodeCountHelper(T, node->left) + RBTreeNodeCountHelper(T, node->right) + 1;
}

/**
 * 计算黑高
 * 
 * 黑高：从节点到叶子节点路径上黑色节点的数量
 * 红黑树的关键性质之一
 */
int RBTreeBlackHeight(RBTree *T, RBNode *node) {
    if (node == T->nil) return 1;
    
    int leftBlackHeight = RBTreeBlackHeight(T, node->left);
    return leftBlackHeight + (node->color == BLACK ? 1 : 0);
}

/**
 * 销毁红黑树
 */
void DestroyRBTree(RBTree *T) {
    DestroyRBTreeHelper(T, T->root);
    if (T->nil != NULL) {
        free(T->nil);
    }
    T->root = NULL;
    T->nil = NULL;
    T->size = 0;
}

void DestroyRBTreeHelper(RBTree *T, RBNode *node) {
    if (node != T->nil) {
        DestroyRBTreeHelper(T, node->left);
        DestroyRBTreeHelper(T, node->right);
        free(node);
    }
}

/**
 * 从数组构建红黑树
 */
RBTree CreateRBTreeFromArray(RBElemType *arr, int size) {
    RBTree T;
    InitRBTree(&T);
    
    for (int i = 0; i < size; i++) {
        RBTreeInsert(&T, arr[i]);
    }
    
    return T;
}

/**
 * 红黑树操作性能测试
 * 
 * 红黑树的性能优势：
 * - 所有操作都是O(log n)
 * - 最坏情况的高度不超过2*log(n+1)
 * - 旋转次数少，适合频繁插入删除
 */
void PerformanceTest_RBTreeOps(int size) {
    printf("红黑树操作性能测试（%d个元素）\n", size);
    
    RBTree T;
    InitRBTree(&T);
    
    clock_t start, end;
    
    // 插入性能测试
    start = clock();
    for (int i = 0; i < size; i++) {
        RBTreeInsert(&T, i);
    }
    end = clock();
    printf("插入%d个元素耗时: %.2fms\n", size, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    
    // 搜索性能测试
    start = clock();
    for (int i = 0; i < size; i++) {
        RBTreeSearch(&T, i);
    }
    end = clock();
    printf("搜索%d个元素耗时: %.2fms\n", size, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    
    // 删除性能测试
    start = clock();
    for (int i = 0; i < size; i++) {
        RBTreeDelete(&T, i);
    }
    end = clock();
    printf("删除%d个元素耗时: %.2fms\n", size, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    
    DestroyRBTree(&T);
}

/**
 * 红黑树与BST性能比较
 * 
 * 优势对比：
 * 1. 平衡性：红黑树保证O(log n)，BST可能退化为O(n)
 * 2. 插入删除：红黑树旋转次数少，AVL树旋转多
 * 3. 实际应用：STL的map/set、Java的TreeMap都使用红黑树
 */
void PerformanceCompare_RBTreeVsBST(int size) {
    printf("红黑树与BST性能比较（%d个元素）\n", size);
    printf("红黑树保证了O(log n)的时间复杂度\n");
    printf("普通BST在最坏情况下可能退化为O(n)\n");
    printf("红黑树的高度不超过2*log(n+1)\n");
    printf("AVL树虽然更平衡，但旋转次数更多\n");
}