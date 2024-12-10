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
        if (nodes[p].mark == 2 || p == -1) return 0;
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
        if (nodes[nodes[p].left_son].right_sibling == -1) {
            nodes[p].size = getsize(nodes[p].left_son) + 1;
        }
        else {
            nodes[p].size = getsize(nodes[p].left_son) + getsize(nodes[nodes[p].left_son].right_sibling) + 1;
        }
    }

    Node* RightSon(Node* p) {
        if (p == nullptr) return nullptr;
        if (p->left_son == -1) return nullptr;
        if (nodes[p->left_son].right_sibling == -1) return nullptr;
        return &nodes[nodes[p->left_son].right_sibling];
    }

    int rotateRight(int p) {
        cout << " RR ";

        if (nodes[p].left_son == -1) return p;
        int q = nodes[p].left_son;
        int a = -1;
        int b = -1;
        int c = -1;

        if (nodes[q].left_son != -1) a = nodes[q].left_son;
        if (nodes[a].right_sibling != -1 && a != -1) b = nodes[a].right_sibling;
        if (nodes[q].right_sibling != -1) c = nodes[q].right_sibling;


        //if (nodes[nodes[p].left_son].right_sibling != -1 && nodes[p].left_son != -1) c = nodes[nodes[p].left_son].right_sibling;

        if (b != -1) {
            nodes[b].right_sibling = c;
            nodes[b].parent = p;
        }

        if (a != -1)
            nodes[a].right_sibling = p; // Правый сын q
        else {
            int new_node = insert(nodes[q].left_son, -1, '*'+nodes.size()+nodes.size(), 2);
            nodes[q].left_son = new_node;
            nodes[new_node].parent = q;
            a = new_node;

            nodes[a].right_sibling = p;
        }



        nodes[p].left_son = b;

        nodes[q].parent = nodes[p].parent;
        nodes[q].right_sibling = nodes[p].right_sibling;
        nodes[p].right_sibling = -1;
        nodes[p].parent = q;

        if(nodes[q].parent != -1) {
            if (nodes[nodes[q].parent].left_son == p) {
                nodes[nodes[q].parent].left_son = q;
            }
            else {
                nodes[nodes[nodes[q].parent].left_son].right_sibling = q;
            }
        }
        fixsize(p);
        fixsize(q);
        return q;
    }



    int rotateLeft(int q) {
        cout<<" LL ";
        if (nodes[q].left_son == -1 || nodes[nodes[q].left_son].right_sibling == -1) return q;

        int p = nodes[nodes[q].left_son].right_sibling;
        int a = -1;
        int b = -1;
        int c = -1;

        if (nodes[q].left_son != -1) a = nodes[q].left_son;
        if (nodes[p].left_son != -1) b = nodes[p].left_son;
        if (nodes[nodes[p].left_son].right_sibling != -1 && nodes[p].left_son != -1) c = nodes[nodes[p].left_son].right_sibling;

        if(nodes[a].mark == 2) {
            nodes[nodes[a].parent].left_son = -1;
            nodes[a].parent = -1;
            nodes[a].right_sibling = -1;
            nodes[a].mark = 1;
            a = -1;
        }

        if (a != -1) {
            nodes[a].right_sibling = b;
        }

        if(b != -1) {
            nodes[b].right_sibling = -1;
            nodes[b].parent = q;
        }

        if (a != -1)
            nodes[a].right_sibling = b;
        else if (b != -1) {
            int new_node = insert(nodes[q].left_son, -1, '*'+nodes.size()+nodes.size(), 2);
            nodes[q].left_son = new_node;
            nodes[new_node].parent = q;
            a  = new_node;

            nodes[a].right_sibling = b;
        }

        nodes[p].left_son = q;

        nodes[p].right_sibling = nodes[q].right_sibling;
        nodes[q].right_sibling = c;

        nodes[p].parent = nodes[q].parent;
        //nodes[q].parent = p;

        if(nodes[p].parent != -1) {
            if(nodes[nodes[p].parent].left_son == q) {
                nodes[nodes[p].parent].left_son = p;
            }else {
               nodes[ nodes[nodes[p].parent].left_son].right_sibling = p;
            }
        }

        nodes[q].parent = p;
        fixsize(p);
        fixsize(q);
        return p;
    }

    int insertroot(int p, int key, char name, int mark) {
        if (p == -1) {
            nodes.push_back(Node(key, name, mark));
            //fixsize(nodes.size()-1);
            //cout<<" insertroot but not rotate ";
            return nodes.size() - 1;
        }

        if (nodes[p].key > key) {
            if (nodes[p].mark == 2) {
                cout<< "12345";
                nodes[p] = Node(key, name, mark);

                nodes[p].key = key;
                nodes[p].mark = 0;
            }else {
                int new_node = insertroot(nodes[p].left_son, key, name, mark);
                nodes[p].left_son = new_node;
                nodes[new_node].parent = p;
            }
            return rotateRight(p);
        } else {
            //cout<<"else root "<<ROOT()<<" :";
            if (nodes[p].left_son == -1) {
                int new_node = insert(nodes[p].left_son, -1, '*'+nodes.size(), 2);
                nodes[p].left_son = new_node;
                nodes[new_node].parent = p;
            }
            int new_node = insertroot(nodes[nodes[p].left_son].right_sibling, key, name, mark);
            nodes[nodes[p].left_son].right_sibling = new_node;
            nodes[new_node].parent = p;
            return rotateLeft(p);
        }
    }

    public:
    int insert(int p, int key, char name, int mark = 0) {
        if (p == -1) {
            nodes.push_back(Node(key, name, mark));
            //fixsize(nodes.size()-1);

            return nodes.size() - 1;
        }

        // Randomized insertion
        int s = getsize(p);
        if (rand() % (s + 1) == 0) {
            //cout<<"insertroot!!";
            return insertroot(p, key, name, mark);
        }

        if (nodes[p].key > key) { // вставка в левое поддерево
            if (nodes[p].mark == 2) {
                cout<< "12345";
                nodes[p] = Node(key, name, mark);

                nodes[p].key = key;
                nodes[p].mark = 0;
            }
            else {
                int new_node = insert(nodes[p].left_son, key, name, mark);
                nodes[p].left_son = new_node;
                nodes[new_node].parent = p;
            }
        } else {  // вставка в правое поддерево
            if (nodes[p].left_son == -1) {
                int new_node = insert(nodes[p].left_son, -1, '*'+nodes.size(), 2);
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
            if (nodes[nodeIndex].left_son != -1)
                Print(nodes[nodes[nodeIndex].left_son].right_sibling, level + 1, true);

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

    void printTable() {
        for(int i = 0; i < nodes.size(); i++) {
            const Node& node = nodes[i];
            cout << "Index: " << i
                 << ", Key: " << node.key
                 << ", Name: " << node.name
                 << ", Left Son: " << node.left_son
                 << ", Right Sibling: " << node.right_sibling
                 << ", Parent: " << node.parent
                 << ", Mark: " << node.mark
                 << ", Size: " << node.size
                 << endl;
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

                if (nodes[index].mark == 1) {
                    index = nodes[index].left_son;
                    continue;
                }

                traversalStack.push(index);
                index = nodes[index].left_son;
                //cout<<traversalStack.top()<<" ";

            }
            //cout<< endl;
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
    srand(20);
    Tree t;
    int root = -1;

    vector<int> valuesA;
    vector<int> valuesB;

    //valuesA.push_back(16);
    for (int i = 0 ; i < 20; ++i) {
        valuesA.push_back(i);
        //valuesA.push_back(rand() % 10000);
    }

    /*for (int i = 15; i > 5; --i) {
        valuesA.push_back(i);
        valuesB.push_back(i);
    }*/
    for(int val : valuesA) {
        cout<< val<<" ";
    }


    for (int val : valuesA) {
        cout<< val<<" ";
        root = t.insert(root, val, 'A');
    }
    cout<<endl;

    t.Print(root);
    int p = 0;
    t.printTable();
    cout<< t.begin()->name<<endl<<endl;
    Tree::iterator it1 = t.begin();
    for (Tree::iterator it = t.begin(); it != t.end(); ++it) {
            if (it->mark == 2) continue;
            std::cout << *it << " (" << it->name << ")\n";
            p++;
    }
    cout<<p<<" "<<valuesA.size()<<endl;
    return 0;
}