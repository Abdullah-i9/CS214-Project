#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct node {
    int data; 
    string title;
    int priority;
    node* link; 
};

struct btreenode {
    btreenode *leftchild; 
    int data; 
    string title;
    int priority;
    btreenode *rightchild;
};

struct pq_node {
    int data;
    string title;
    int priority;
    pq_node* next;
};

class linklist {
    private:
    node *p;

    public:
    linklist() {
        p = NULL;
    }

    void append(int num, string title, int priority) {
        node *temp, *r;
        if (p == NULL) {
            temp = new node;
            temp->data = num;
            temp->title = title;
            temp->priority = priority;
            temp->link = NULL;
            p = temp;
        } else {
            temp = p;
            while (temp->link != NULL)
                temp = temp->link;
            r = new node;
            r->data = num;
            r->title = title;
            r->priority = priority;
            r->link = NULL;
            temp->link = r;
        }
    }

    bool del(int num) {
        node *old, *temp;
        temp = p;
        while (temp != NULL) {
            if (temp->data == num) {
                if (temp == p)
                    p = temp->link;
                else
                    old->link = temp->link;
                delete temp;
                return true;
            } else {
                old = temp;
                temp = temp->link;
            }
        }
        return false;
    }

    bool edit(int num, string newTitle, int newPriority) {
        node *temp = p;
        while (temp != NULL) {
            if (temp->data == num) {
                temp->title = newTitle;
                temp->priority = newPriority;
                return true;
            }
            temp = temp->link;
        }
        return false;
    }

    void display() {
        node *temp = p;
        if (temp == NULL) {
            cout << "No tasks available.\n";
            return;
        }
        while (temp != NULL) {
            cout << "[ID: " << temp->data << "] Title: " << temp->title 
                 << " | Priority: " << temp->priority << endl;
            temp = temp->link;
        }
    }

    node* getHead() {
         return p;
    }

    void clear() {
        node *q;
        while (p != NULL) {
            q = p->link;
            delete p;
            p = q;
        }
    }

    ~linklist() {
        clear();
    }
};

class btree {
    private:
    btreenode *root;

    void insert(btreenode **sr, int num, string title, int priority) {
        if (*sr == NULL) {
            *sr = new btreenode;
            (*sr)->leftchild = NULL;
            (*sr)->data = num;
            (*sr)->title = title;
            (*sr)->priority = priority;
            (*sr)->rightchild = NULL;
        } else {
            if (num < (*sr)->data)
                insert(&((*sr)->leftchild), num, title, priority);
            else
                insert(&((*sr)->rightchild), num, title, priority);
        }
    }

    bool search(btreenode *sr, int num, string &title, int &priority) {
        while (sr != NULL) {
            if (num == sr->data) {
                title = sr->title;
                priority = sr->priority;
                return true;
            } else if (num < sr->data)
                sr = sr->leftchild;
            else
                sr = sr->rightchild;
        }
        return false;
    }

    void locate(btreenode **sr, int num, btreenode **par, btreenode **x, bool *found) {
        btreenode *q = *sr;
        *found = false;
        *par = NULL;
        while (q != NULL) {
            if (q->data == num) {
                *found = true;
                *x = q;
                return;
            }
            *par = q;
            if (q->data > num)
                q = q->leftchild;
            else
                q = q->rightchild;
        }
    }

    void rem(btreenode **sr, int num) {
        bool found;
        btreenode *parent, *x, *xsucc;
        if (*sr == NULL) return;

        parent = x = NULL;
        locate(sr, num, &parent, &x, &found);
        if (found == false) return;

        if (x->leftchild != NULL && x->rightchild != NULL) {
            parent = x;
            xsucc = x->rightchild;
            while (xsucc->leftchild != NULL) {
                parent = xsucc;
                xsucc = xsucc->leftchild;
            }
            x->data = xsucc->data;
            x->title = xsucc->title;
            x->priority = xsucc->priority;
            x = xsucc;
        }

        if (x->leftchild == NULL && x->rightchild == NULL) {
            if (parent != NULL) {
                if (parent->rightchild == x)
                    parent->rightchild = NULL;
                else 
                    parent->leftchild = NULL;
            } else {
                *sr = NULL;
            }
            delete x;
            return;
        }

        if (x->leftchild == NULL && x->rightchild != NULL) {
            if (parent != NULL) {
                if (parent->leftchild == x)
                    parent->leftchild = x->rightchild;
                else
                    parent->rightchild = x->rightchild;
            } else {
                *sr = x->rightchild;
            }
            delete x;
            return;
        }

        if (x->leftchild != NULL && x->rightchild == NULL) {
            if (parent != NULL) {
                if (parent->leftchild == x)
                    parent->leftchild = x->leftchild;
                else
                    parent->rightchild = x->leftchild;
            } else {
                *sr = x->leftchild;
            }
            delete x;
            return;
        }
    }

    void del(btreenode *sr) {
        if (sr != NULL) {
            del(sr->leftchild);
            del(sr->rightchild);
            delete sr;
        }
    }

    public:
    btree() {
        root = NULL;
    }

    void buildtree(int num, string title, int priority) {
        insert(&root, num, title, priority);
    }

    bool searchbst(int num, string &title, int &priority) {
        return search(root, num, title, priority);
    }

    void remove(int num) {
        rem(&root, num);
    }

    void clear() {
        del(root);
        root = NULL;
    }

    ~btree() {
        clear();
    }
};

class priority_queue {
    private:
    pq_node *head;

    public:
    priority_queue() {
         head = NULL; 
    }

    void push(int num, string title, int priority) {
        pq_node* temp = new pq_node;
        temp->data = num;
        temp->title = title;
        temp->priority = priority;
        temp->next = NULL;

        if (head == NULL || priority < head->priority) {
            temp->next = head;
            head = temp;
        } else {
            pq_node* start = head;
            while (start->next != NULL && start->next->priority <= priority) {
                start = start->next;
            }
            temp->next = start->next;
            start->next = temp;
        }
    }

    void display() {
        pq_node* temp = head;
        if (temp == NULL) {
            cout << "No tasks in Priority Queue.\n";
            return;
        }
        cout << "\n--- Tasks Sorted by Priority ---\n";
        while (temp != NULL) {
            cout << "[Priority: " << temp->priority << "] ID: " << temp->data 
                 << " | Title: " << temp->title << endl;
            temp = temp->next;
        }
    }

    void clear() {
        while (head != NULL) {
            pq_node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    ~priority_queue() {
        clear(); 
    }
};

void saveToFile(linklist &list) {
    ofstream outFile("tasks.txt");
    if (!outFile) return;

    node* temp = list.getHead();
    while (temp != NULL) {
        outFile << temp->data << " " << temp->priority << " " << temp->title << endl;
        temp = temp->link;
    }
    outFile.close();
}

void reloadAll(linklist &list, btree &bst, priority_queue &pq) {
    bst.clear();
    pq.clear();
    node* temp = list.getHead();
    while (temp != NULL) {
        bst.buildtree(temp->data, temp->title, temp->priority);
        pq.push(temp->data, temp->title, temp->priority);
        temp = temp->link;
    }
    saveToFile(list);
}

void loadFromFile(linklist &list, btree &bst, priority_queue &pq) {
    ifstream inFile("tasks.txt");
    if (!inFile) return;

    int id, priority;
    string title;
    while (inFile >> id >> priority >> title) {
        list.append(id, title, priority);
    }
    inFile.close();
    reloadAll(list, bst, pq);
}

int main() {
    linklist list;
    btree bst;
    priority_queue pq;

    loadFromFile(list, bst, pq);

    int choice;
    do {
        cout << "\n--- TASK MANAGEMENT SYSTEM ---\n";
        cout << "=========================================\n";
        cout << "1. Add a Task\n";
        cout << "2. Edit a Task\n";
        cout << "3. Delete a Task\n";
        cout << "4. Print All Tasks\n";
        cout << "5. Search Task by ID\n";
        cout << "6. View Tasks by Priority\n";
        cout << "7. Exit\n";
        cout << "=========================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, priority;
            string title;
            cout << "Enter Task ID: "; cin >> id;

            string test_title; int test_p;
            if (bst.searchbst(id, test_title, test_p)) {
                cout << "Error: Task ID already exists!\n";
                continue;
            }

            cout << "Enter Title: "; cin >> title;
            cout << "Enter Priority: "; cin >> priority;

            list.append(id, title, priority);
            reloadAll(list, bst, pq);
            cout << "Task added successfully!\n";

        } else if (choice == 2) {
            int id;
            cout << "Enter Task ID to edit: "; cin >> id;

            string title; int p;
            if (bst.searchbst(id, title, p)) {
                cout << "Current details: " << title << " (Priority: " << p << ")\n";
                cout << "Enter New Title: "; cin >> title;
                cout << "Enter New Priority: "; cin >> p;

                list.edit(id, title, p);
                reloadAll(list, bst, pq);
                cout << "Task updated successfully.\n";
            } else {
                cout << "Task not found.\n";
            }

        } else if (choice == 3) {
            int id;
            cout << "Enter Task ID to delete: "; cin >> id;

            if (list.del(id)) {
                bst.remove(id); 
                reloadAll(list, bst, pq);
                cout << "Task deleted successfully!\n";
            } else {
                cout << "Task not found.\n";
            }

        } else if (choice == 4) {
            cout << "\n--- All Tasks ---\n";
            list.display();

        } else if (choice == 5) {
            int id;
            cout << "Enter Task ID to search: "; cin >> id;

            string title; int p;
            if (bst.searchbst(id, title, p)) {
                cout << "Found in BST! Title: " << title << " | Priority: " << p << endl;
            } else {
                cout << "Task not found.\n";
            }

        } else if (choice == 6) {
            pq.display();

        } else if (choice == 7) {
            cout << "Exiting program.\n";
        } else {
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 7);

    return 0;
}