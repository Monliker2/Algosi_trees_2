#include <iostream>
#include <vector>
#include <cstdlib> // Для rand()

using namespace std;

struct Node {
    int key;
    char name;

    int left_son;
    int right_sibling;
    int parent;
    int mark;
    int size;

    Node(int k, char n, int m = 0) {
        key = k;
        left_son = right_sibling = -1;
        size = 1;
        mark = m;
        name = n;
        parent = -1;
    }
};

class Tree {
private:
    //vector<Node> nodes;

public:
    vector<Node> nodes;
    int getsize(int p) {
        if (p < 0 || p >= nodes.size()) return 0;
        return nodes[p].size;
    }

    void fixsize(int p) {
        int size = 1;
        int leftSon = nodes[p].left_son;

        if (leftSon != -1) {
            size += getsize(leftSon);

            int rightSibling = nodes[leftSon].right_sibling;
            while (rightSibling != -1) {
                size += getsize(rightSibling);
                rightSibling = nodes[rightSibling].right_sibling;
            }
        }
        nodes[p].size = size;
    }

    int rotateright(int p) {
        int q = nodes[p].left_son;
        if (q == -1) return p;

        nodes[p].left_son = nodes[q].right_sibling;
        if (nodes[q].right_sibling != -1) {
            nodes[nodes[q].right_sibling].parent = p;
        }
        nodes[q].right_sibling = p;
        nodes[q].parent = nodes[p].parent;
        nodes[p].parent = q;

        fixsize(p);
        fixsize(q);
        return q;
    }

    int rotateleft(int p) {
        int q = nodes[p].right_sibling;
        if (q == -1) return p;

        nodes[p].right_sibling = nodes[q].left_son;
        if (nodes[q].left_son != -1) {
            nodes[nodes[q].left_son].parent = p;
        }
        nodes[q].left_son = p;
        nodes[q].parent = nodes[p].parent;
        nodes[p].parent = q;

        fixsize(p);
        fixsize(q);
        return q;
    }

    int insertroot(int p, int key, char name, int mark = 0) {
        if (p == -1) {
            nodes.push_back(Node(key, name, mark));
            return nodes.size() - 1;
        }

        if (key < nodes[p].key) {
            int new_node = insertroot(nodes[p].left_son, key, name, mark);
            nodes[p].left_son = new_node;
            nodes[new_node].parent = p;
            return rotateright(p);
        } else {
            int new_node = insertroot(nodes[p].right_sibling, key, name, mark);
            nodes[p].right_sibling = new_node;
            nodes[new_node].parent = p;
            return rotateleft(p);
        }
    }

    int insert(int p, int key, char name, int mark = 0) {
        if (p == -1) {
            nodes.push_back(Node(key, name, mark));
            return nodes.size() - 1;
        }

        // Randomized insertion
        if (rand() % (getsize(p) + 1) == 0) {
            return insertroot(p, key, name, mark);
        }

        if (nodes[p].key > key) {
            int new_node = insert(nodes[p].left_son, key, name, mark);
            nodes[p].left_son = new_node;
            nodes[new_node].parent = p;
        } else {
            int new_node = insert(nodes[p].right_sibling, key, name, mark);
            nodes[p].right_sibling = new_node;
            nodes[new_node].parent = p;
        }

        fixsize(p);
        return p;
    }

    void Print(int nodeIndex, int level = 0, bool isRight = false) {
        if (nodeIndex != -1) {
            // Сначала выводим правого брата
            Print(nodes[nodeIndex].right_sibling, level + 1, true);

            // Отступы для уровня
            for (int i = 1; i <= level; i++) {
                cout << ".  ";
            }

            // Выводим текущий узел
            if (isRight) {
                cout << "┌─ "; // Для правых ветвей
            } else {
                cout << "└─ "; // Для остальных узлов
            }

            cout << nodes[nodeIndex].key << endl;

            // Выводим левого сына
            Print(nodes[nodeIndex].left_son, level + 1, false);
        }
    }


    void printNode(int nodeIndex) {
        if (nodeIndex != -1) {
            const Node& node = nodes[nodeIndex];
            cout << "Key: " << node.key
                 << ", Name: " << node.name
                 << ", Left Son: " << node.left_son
                 << ", Right Sibling: " << node.right_sibling
                 << ", Parent: " << node.parent
                 << ", Mark: " << node.mark
                 << ", Size: " << node.size
                 << endl;
            printNode(node.left_son);  // Рекурсивно выводим левых сыновей
            printNode(node.right_sibling);  // Рекурсивно выводим правых братьев
        }
    }

};



int main() {
    srand(10);
    Tree t;
    int root = -1;

    int numNodes = 15;
    vector<int> valuesA;
    vector<int> valuesB;
    for (int i = 0; i < numNodes; ++i) {
        valuesA.push_back(i);
        valuesB.push_back(i);
    }

    for (int val : valuesA) {
        root = t.insert(root, val, 'A');
    }

    t.Print(root);

    t.printNode(root);

    return 0;
}