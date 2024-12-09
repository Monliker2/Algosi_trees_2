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

    int getIndex(int p) {
        for (int i = 0; i < nodes.size(); i++) {
            if (&nodes[i] == &nodes[p]) {
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

    public:
    int insert(int p, int key, char name, int mark = 0) {
        if (p == -1) {
            nodes.push_back(Node(key, name, mark));
            return nodes.size() - 1;
        }

        // Randomized insertion
        if (rand() % (getsize(p) + 1) == 0) {
            return insertroot(p, key, name, mark);
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
        return getIndex(p);
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
            }
            //cout<<traversalStack.top()<<endl;
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

    // Итератор начала (первый узел в симметричном порядке)
    iterator begin() const {
        return iterator(nodes, nodes.empty() ? -1 : 0);
    }

    // Итератор конца (завершение обхода)
    iterator end() const {
        return iterator(nodes, -1);
    }

    // Перегрузка оператора -> для доступа к текущему узлу

    // Остальной функционал дерева (вставка, удаление и т.д.)
};



int main() {
    srand(20 );
    Tree t;
    int root = -1;

    int numNodes = 15;
    vector<int> valuesA;
    vector<int> valuesB;
    for (int i = 5; i < numNodes; ++i) {
        valuesA.push_back(i);
        valuesB.push_back(i);
    }

    for (int val : valuesA) {
        root = t.insert(root, val,  char('A' + val));
    }

    t.Print(root);

    t.printNode(root);

    for (Tree::iterator it = t.begin(); it != t.end(); ++it) {
            if (it->mark == 2) continue;
            std::cout << *it << " (" << it->name << ")\n";
    }
    return 0;
}