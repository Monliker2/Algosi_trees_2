#include <iostream>
#include <vector>
#include <cstdlib>
#include <stack>

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
    int getRightSon(int p) {
        if (p < 0 || p >= nodes.size()) return -1;
        if (nodes[p].left_son == -1) return -1;
        if (nodes[nodes[p].left_son].right_sibling == -1) return -1;
        return nodes[nodes[p].left_son].right_sibling;
    }


    vector<Node> nodes;
    int getsize(int p) {
        if (p < 0 || p >= nodes.size()) return 0;
        return nodes[p].size;
    }

    int getIndex(Node* p) {
        for (int i = 0; i < nodes.size(); i++) {
            if (&nodes[i] == p) {
                return i;
            }
        }
        return -1;
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


    Node* rotateRight(Node* p) {
        if (p->left_son == -1) return p;
        Node* q = &nodes[p->left_son];

        if(!q) return p;
        p->left_son = nodes[q->left_son].right_sibling;
        nodes[q->left_son].right_sibling = getIndex(p);

        q->size = p->size;
        fixsize(getIndex(p));
        return q;
    }

    Node* rotateLeft(Node* q) {
        if (nodes[q->left_son].right_sibling == -1) return q;
        Node* p = &nodes[nodes[q->left_son].right_sibling];

        if(!p) return q;
        nodes[q->left_son].right_sibling = p->left_son;
        p->left_son = getIndex(q);

        p->size = q->size;
        fixsize(getIndex(p));
        return q;

    }

    int insertroot(Node* p, int key, char name, int mark = 0) {
        if (!p) {
            nodes.push_back(Node(key, name, mark));
            return nodes.size() - 1;
        }
        if(key < p->key) {
            p->left_son = insertroot(&nodes[p->left_son], key, name, mark);
            return getIndex(rotateRight(p));
        } else {
            if (p->left_son == -1) {
                int new_node = insert(-1, -1, '*', 2);
                p->left_son = new_node;
                nodes[new_node].parent = getIndex(p);
            }
            nodes[p->left_son].right_sibling = insertroot(&nodes[nodes[p->left_son].right_sibling], key, name, mark);
            return getIndex(rotateLeft(p));
        }

    }

    public:
    int insert(int p, int key, char name, int mark = 0) {
        if (p == -1) {
            nodes.push_back(Node(key, name, mark));
            return nodes.size() - 1;
        }

        // Randomized insertion
        if (rand() % (getsize(p) + 1) == 0) {
            return insertroot(&nodes[p], key, name, mark);
        }

        if (nodes[p].key > key) { // вставка в левое поддерево
            if (nodes[p].mark == 2) {
                nodes[p] = Node(key, name, mark);
            }
            int new_node = insert(nodes[p].left_son, key, name, mark);
            nodes[p].left_son = new_node;
            nodes[new_node].parent = p;
        } else {  // вставка в правое поддерево
            if (nodes[p].left_son == -1) {
                int new_node = insert(nodes[p].left_son, -1, '*', 2);
                nodes[p].left_son = new_node;
                nodes[new_node].parent = p;
            }
            int new_node = insert(nodes[nodes[p].left_son].right_sibling, key, name, mark);
            nodes[nodes[p].left_son].right_sibling = new_node;
            nodes[new_node].parent = p;
        }

        fixsize(p);
        return p;
    }

    int ROOT() const{
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].parent == -1) {
                return i;
            }
        }
        return -1;
    }

    void Print(int nodeIndex, int level = 0, bool isRight = false) {
        if (nodeIndex != -1) {
            // Сначала выводим правого брата
            Print(getRightSon(nodeIndex), level + 1, true);

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

            cout << nodes[nodeIndex].key << "(" << nodes[nodeIndex].name << ")";
            cout << endl;

            // Выводим левого сына
            Print(nodes[nodeIndex].left_son, level + 1, false);
        }
    }

    void printNode(int nodeIndex) {
        if (nodeIndex != -1) {
            const Node& node = nodes[nodeIndex];
            cout << "Index: " << nodeIndex
                 << ", Key: " << node.key
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


    class iterator {
    private:
        const vector<Node>& nodes; // Ссылка на дерево
        int current;              // Текущий индекс узла
        stack<int> traversalStack; // Стек для обхода

        // Перемещаемся к самому левому узлу
        void pushLeft(int index) {
            while (index != -1) {

                traversalStack.push(index);
                index = nodes[index].left_son;
                //cout<<traversalStack.top()<<" ";

            }
            //cout<< endl;
            //cout<<traversalStack.top()<<endl;
        }

        int ROOT1() const {
            for (int i = 0; i < nodes.size(); ++i) {
                if (nodes[i].parent == -1) {
                    return i;
                }
            }
            return -1;
        }

    public:
        // Конструктор итератора
        iterator(const vector<Node>& nodes, int start)
            : nodes(nodes), current(-1) {
            if (start != -1) {
                pushLeft(start); // Добавляем все левые узлы
                if (!traversalStack.empty()) {
                    current = traversalStack.top();
                }
            }
        }

        // Разыменование: возвращает текущий узел
        const int operator*() const {
            return nodes[current].key;
        }


        const Node* operator->() const {
            return &nodes[current];
        }

        // Проверка на неравенство (используется для завершения итерации)
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }

        // Инкремент: перемещаемся к следующему узлу в симметричном порядке
        iterator& operator++() {
            if (!traversalStack.empty()) {
                current = traversalStack.top();
                traversalStack.pop();

                // Переход к правому сыну
                //getRightSon(int p)
                //return nodes[nodes[p].left_son].right_sibling;
                if (nodes[current].left_son != -1) {
                    if (nodes[nodes[current].left_son].right_sibling != -1) {
                        pushLeft(nodes[nodes[current].left_son].right_sibling);
                    }
                }

                if (!traversalStack.empty()) {
                    current = traversalStack.top();
                } else {
                    current = -1; // Обход завершён
                }
            }
            return *this;
        }




    };
    iterator begin() const {
        return iterator(nodes, ROOT());
    }

    iterator end() const {
        return iterator(nodes, -1);
    }
};



int main() {
    srand(25 );
    Tree t;
    int root = -1;

    int numNodes = 15;
    vector<int> valuesA;
    vector<int> valuesB;

    valuesA.push_back(16);
    for (int i = 5; i < numNodes; ++i) {
        valuesA.push_back(i);
        valuesB.push_back(i);
    }

    for (int val : valuesA) {
        cout<< val<<" ";
        root = t.insert(root, val,  char('A' + val));
    }
    cout<<endl;

    t.Print(root);

    t.printNode(root);
    cout<< t.begin()->name<<endl<<endl;
    for (Tree::iterator it = t.begin(); it != t.end(); ++it) {
            if (it->mark == 2) continue;
            std::cout << *it << " (" << it->name << ")\n";
    }
    return 0;
}