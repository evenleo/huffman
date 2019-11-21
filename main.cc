#include <iostream>
#include <queue>
#include <map>
#include <string>

class Node {
public:
    char c;        
    int frequency;
    Node *left;
    Node *right;

    Node(char _c, int f, Node *l = nullptr, Node *r = nullptr)
        : c(_c), frequency(f), left(l), right(r) {}
    bool operator<(const Node &node) const { //重载<，优先队列的底层数据结构std::heap是最大堆
        return frequency > node.frequency;
    }
};

class huffmanTree {
public:
    struct charater {
        char c;
        int f;
    };
    huffmanTree(std::initializer_list<charater> l) {
        for (auto i : l) {
            Node n(i.c, i.f);
            q.push(n);
        }
        makeHuffmanTree();
    }
    ~huffmanTree() {
        Node node = q.top();
        _deleteTree(node.left);
        _deleteTree(node.right);
    }
    void makeHuffmanTree() {
        while (q.size() != 1) {
            Node *left = new Node(q.top()); q.pop();
            Node *right = new Node(q.top()); q.pop();
            Node node('R', left->frequency + right->frequency, left, right);
            q.push(node);
        }
    }
    void huffmanCode(std::map<char, std::string>& codeMap) {
        Node node = q.top(); 
        std::string prefix;
        _huffmanCode(&node, prefix, codeMap);
    }
    
    struct nodeBacklog {  //回溯点
        Node* node;
        int next_sub_idx;
    };
    enum {
        LeftIndex,
        RightIndex
    };
    enum { MaxLevel = 64 };

    /*
    * 以下部分实现树形打印二叉树，手动实现栈功能，原因是std::stack无法随机访问栈空间，
    * 而程序需要用到随机访问占空间的功能，这里用局部定义栈数组，能自动释放空间，
    * 这里的实现对深层次理解指针和地址的关系非常有帮助，个人觉得非常巧妙，值得学习
    */
    static void nbl_push(nodeBacklog *nbl, nodeBacklog **top, nodeBacklog **bottom) {
        if (*top - *bottom < MaxLevel) { 
            (*(*top)++) = *nbl;  //这里其实已经将nbl的值拷贝到*top指向的位置，不能直接*top = nbl，否则会随nbl变化
        }
    }
    static nodeBacklog* nbl_pop(nodeBacklog **top, nodeBacklog **bottom) {
        return *top > *bottom ? --*top : nullptr;
    }
    static bool is_leaf(Node* n) {
        return n->left == nullptr && n->right == nullptr;
    }
    
    void watch() {
        Node node = q.top();
        _treeWatch(&node);
    }

private:
    void _deleteTree(Node* n) {
        if (!n) return ;
        _deleteTree(n->left);
        _deleteTree(n->right);
        delete n;
    }
    void _treeWatch(Node* root);
    static void _printNode(Node* n) {
        if (n != nullptr) {
            printf("%c:%d\n", n->c, n->frequency);
        }
    }
    
    void _huffmanCode(Node *root, std::string& prefix, 
                      std::map<char, std::string>& codeMap) {
        std::string tmp = prefix;
        if (root->left != nullptr) {
            prefix += '0';
            if (is_leaf(root->left)) {
                codeMap[root->left->c] = prefix;
            } else {
                _huffmanCode(root->left, prefix, codeMap);
            }
        }
        if (root->right != nullptr) {
            prefix = tmp;
            prefix += '1';
            if (is_leaf(root->right)) {
                codeMap[root->right->c] = prefix;
            } else {
                 _huffmanCode(root->right, prefix, codeMap);
            }
        }
    }
    
private:
    std::priority_queue<Node> q;
};

void huffmanTree::_treeWatch(Node* root) {
    int level = 0;
    Node* node = root;
    nodeBacklog nbl;
    nodeBacklog *p_nbl = nullptr;
    nodeBacklog *top, *bottom, nbl_stack[MaxLevel];
    top = bottom = nbl_stack;
    
    for (;;) {
        if (node != nullptr) {
            int sub_index = p_nbl != nullptr ? p_nbl->next_sub_idx : RightIndex;
            p_nbl = nullptr;
            if (is_leaf(node) || sub_index == LeftIndex) {
                nbl.node = nullptr;
                nbl.next_sub_idx = RightIndex;
            } else {
                nbl.node = node;
                nbl.next_sub_idx = LeftIndex;
            }
            nbl_push(&nbl, &top, &bottom);
            level++;
            if (sub_index == RightIndex) {
                for (int i = 1; i < level; ++i) {
                    if (i == level - 1) {
                        printf("%-8s", "|-------");
                    } else {
                        if (nbl_stack[i - 1].node != nullptr) {
                            printf("%-8s", "|");
                        } else {
                            printf("%-8s", " ");
                        }
                    }
                }
                _printNode(node);
            }
            node = sub_index == LeftIndex ? node->left : node->right;
        } else {
            p_nbl = nbl_pop(&top, &bottom);
            if (p_nbl == nullptr) 
                break;
            node = p_nbl->node;
            level--;
        }
    }
}

void visitCode(std::map<char, std::string>& codeMap) {
    for (auto m : codeMap) {
        std::cout << m.first << ":" << m.second << std::endl;
    }
}

void testHuffmanTree() {
    huffmanTree htree = {   {'A', 20},
                            {'B', 30},
                            {'C', 60},
                            {'D', 120},
                            {'E', 350},
                            {'F', 450}
                        };
    htree.watch();  
    std::cout << "huffmanCode:" << std::endl;
    std::map<char, std::string> codeMap;
    htree.huffmanCode(codeMap);
    visitCode(codeMap);
}

int main()
{
    testHuffmanTree();

    system("pause");
    return 0;
}