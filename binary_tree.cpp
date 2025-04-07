/* @Author
StudentName : Emre Safa Yalçın
StudentID : 150210090
Date : 18.12.2023 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <time.h>

using namespace std;

int last_id = 0;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d){
            id = i;
            salary = s;
            department = d;
        }

        int get_id(){
            return id;
        }
        int get_salary(){
            return salary;
        }
        int get_department(){
            return department;
        }

        void set_id(int i){
            id = i;
        }
        void set_salary(int s){
            salary = s;
        }
        void set_department(int d){
            department = d;
        }
};

class Node{
    private:
        Employee* employee;
        Node* left;
        Node* right;
    public:
        Node(Employee* emp){
            employee = emp;
            left = nullptr;
            right = nullptr;
        }
        ~Node(){
            delete this->get_employee();

            if (this->get_left() != nullptr){
                delete this->get_left();
            }
            if (this->get_right() != nullptr){
                delete this->get_right();
            }
        }

        Employee* get_employee(){
            return employee;
        }
        Node* get_left(){
            return left;
        }
        Node* get_right(){
            return right;
        }

        void set_employee(Employee* emp){
            employee = emp;
        }
        void set_left(Node* l){
            left = l;
        }
        void set_right(Node* r){
            right = r;
        }
};

class BinaryTree{
    private:
        Node* root;
        
        void insertPrivate(Node*, Employee*);
        void removePrivate(Node* ptr, int id); // finds the node that is being removed and passes its parent, node and a boolean value
        // which represents which child of parent (left or right) is being removed(true if left, false if right).

        int* findSmallest(Node*, Node*); // Deletes the smallest ID's node, returns smallest employee and rearranges the tree.
        void removeRoot(); // Removes root node and rearranges the old tree without removed root node.
        void removeNode(Node*, Node*, bool); // Removes node and rearranges the old tree without it.
        // These 2 functions could be written in the remove() and removePrivate() functions but this way the code is much more readable.

        Employee* searchPrivate(Node*, int);
        int getHeightPrivate(Node*);
        void printToConsolePrivate(Node*);
        void printToFilePrivate(Node*, ofstream &output_file);

    public:
        BinaryTree(){
            root = nullptr;
        }

        void insert(Employee*);
        void remove(int id); // Checks if root is null. Calls the function removeRoot if id = root->employee->id. if not calls removePrivate.
        Employee* search(int);
        int getHeight();
        void printToFile(ofstream &output_file);
        void printToConsole();
};

void BinaryTree::insert(Employee* employee){
    if (root != nullptr){
        insertPrivate(root, employee);
    } else {
        root = new Node(employee);
    }
}

void BinaryTree::insertPrivate(Node* ptr, Employee* employee){
    if (employee->get_id() < ptr->get_employee()->get_id()){
        if (ptr->get_left() != nullptr){
            insertPrivate(ptr->get_left(), employee);
        } else {
            ptr->set_left(new Node(employee));
        }
    } else if (employee->get_id() > ptr->get_employee()->get_id()){
        if (ptr->get_right() != nullptr){
            insertPrivate(ptr->get_right(), employee);
        } else {
            ptr->set_right(new Node(employee));
        }
    } else {
        last_id--; // If the employee is already in the tree: employee->get_id() == ptr->get_employee()->get_id()
    }
}

void BinaryTree::remove(int id){
    if (root != nullptr){
        if (id == root->get_employee()->get_id()){
            removeRoot();
        } else
            removePrivate(root, id);
    } else
        cout << "ERROR: There is no Employee\n";
}

void BinaryTree::removePrivate(Node* ptr, int id){
    if (id < ptr->get_employee()->get_id() && ptr->get_left() != nullptr){
        ptr->get_left()->get_employee()->get_id() == id ?
        removeNode(ptr, ptr->get_left(), true) :
        removePrivate(ptr->get_left(), id);
    } else if (id > ptr->get_employee()->get_id() && ptr->get_right() != nullptr){
            ptr->get_right()->get_employee()->get_id() == id ?
            removeNode(ptr, ptr->get_right(), false) :
            removePrivate(ptr->get_right(), id);
    } else
        cout << "ERROR: An invalid ID to delete\n";
}

int* BinaryTree::findSmallest(Node* parent, Node* ptr){
    if (ptr->get_left() != nullptr){
        return findSmallest(ptr, ptr->get_left());
    } else {
        Employee* emp = ptr->get_employee();
        int* data = new int[3]{emp->get_id(), emp->get_salary(), emp->get_department()};
        if (parent->get_left() == ptr){
            parent->set_left(ptr->get_right());
            ptr->set_right(nullptr);
        } else {
            parent->set_right(ptr->get_right());
            ptr->set_right(nullptr);
        }

        delete ptr;
        
        return data;
    }
}

void BinaryTree::removeRoot(){
    Node* removedNode = root;

    // case 1: 0 children
    if (root->get_left() == nullptr && root->get_right() == nullptr){
        root = nullptr;
        delete removedNode;
    }
    // case 2: 1 children
    else if (root->get_left() != nullptr && root->get_right() == nullptr){
        root = root->get_left();
        removedNode->set_left(nullptr);
        delete removedNode;
    } else if (root->get_left() == nullptr && root->get_right() != nullptr){
        root = root->get_right();
        removedNode->set_right(nullptr);
        delete removedNode;
    }
    // case 3: 2 children
    else {
        Employee* emp = root->get_employee();
        int* arr = findSmallest(root, root->get_right());
        emp->set_id(arr[0]);
        emp->set_salary(arr[1]);
        emp->set_department(arr[2]);

        delete arr;
    }
}

void BinaryTree::removeNode(Node* parent, Node* ptr, bool left){
    Node* removedNode;

    //case 1: 0 children
    if (ptr->get_left() == nullptr && ptr->get_right() == nullptr){
        removedNode = ptr;
        left == true ? parent->set_left(nullptr) : parent->set_right(nullptr);
        delete removedNode;
    }
    //case 2: 1 children
    else if (ptr->get_left() == nullptr && ptr->get_right() != nullptr){
        removedNode = ptr;
        left == true ? parent->set_left(ptr->get_right()) : parent->set_right(ptr->get_right());
        ptr->set_right(nullptr);
        delete removedNode;
    } else if (ptr->get_left() != nullptr && ptr->get_right() == nullptr){
        removedNode = ptr;
        left == true ? parent->set_left(ptr->get_left()) : parent->set_right(ptr->get_left());
        ptr->set_left(nullptr);
        delete removedNode;
    }
    // case 3: 2 children
    else {
        Employee* emp = ptr->get_employee();
        int* arr = findSmallest(ptr, ptr->get_right());
        emp->set_id(arr[0]);
        emp->set_salary(arr[1]);
        emp->set_department(arr[2]);
    }
}

Employee* BinaryTree::search(int id){
    if (root != nullptr){
        return searchPrivate(root, id);
    } else
        return nullptr;
}

Employee* BinaryTree::searchPrivate(Node* ptr, int id){
    if (ptr != nullptr){
        if (id < ptr->get_employee()->get_id()){
            return searchPrivate(ptr->get_left(), id);
        } else if (id > ptr->get_employee()->get_id()){
            return searchPrivate(ptr->get_right(), id);
        } else
            return ptr->get_employee();
    } else 
        return nullptr;
}

int BinaryTree::getHeight(){
    return getHeightPrivate(root) + 1;
}

int BinaryTree::getHeightPrivate(Node* ptr){
    if (ptr == nullptr)
        return -1;
    
    int lh = getHeightPrivate(ptr->get_left());
    int rh = getHeightPrivate(ptr->get_right());

    return lh > rh ? lh + 1 : rh + 1;
}

void BinaryTree::printToFile(ofstream &output_file){
    if (root != nullptr){
        printToFilePrivate(root, output_file);
    }
}

void BinaryTree::printToFilePrivate(Node* ptr, ofstream &output_file){
    Employee* emp = ptr->get_employee();
    if (ptr->get_left() != nullptr){
        printToFilePrivate(ptr->get_left(), output_file);
    }
    output_file << emp->get_id() << ";" << emp->get_salary() << ";" << emp->get_department() << "\n";
    if (ptr->get_right() != nullptr){
        printToFilePrivate(ptr->get_right(), output_file);
    }
}

void BinaryTree::printToConsole(){
    if (root != nullptr){
        printToConsolePrivate(root);
    }
}

void BinaryTree::printToConsolePrivate(Node* ptr){
    Employee* emp = ptr->get_employee();
    cout << emp->get_id() << ";" << emp->get_salary() << ";" << emp->get_department() << "\n";
    if (ptr->get_left() != nullptr){
        printToConsolePrivate(ptr->get_left());
    }
    
    if (ptr->get_right() != nullptr){
        printToConsolePrivate(ptr->get_right());
    }
}

int main(int argc, char** argv){
    ifstream myFile(argv[1]);
    if (!myFile.is_open()) {
        cout << "File failed to open." << "\n";
        return 0;
    }

    string line = "";

    int id, salary, department;
    string id_str, salary_str, department_str;

    BinaryTree BST;

    getline(myFile, line); // for header line
    while (getline(myFile, line)) {
        stringstream ss(line);
        getline(ss, id_str, ';');
        getline(ss, salary_str, ';');
        getline(ss, department_str, ';');
        id = stoi(id_str);
        salary = stoi(salary_str);
        department = stoi(department_str);

        Employee* emp = new Employee(id, salary, department);

        BST.insert(emp);

        last_id++;
    }
    
    myFile.close();

    ifstream myFile0(argv[2]);
    if (!myFile0.is_open()){
        cout << "File failed to open." << "\n";
        return 0;
    }

    string line0;
    string operation, str_id, str_salary, str_department;
    
    while (getline(myFile0, line0)){
        stringstream ss(line0);
        getline(ss, operation, ';');

        if (operation == "ADD"){
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
            salary = stoi(str_salary);
            department = stoi(str_department);

            Employee* emp = new Employee(last_id + 1, salary, department);
            BST.insert(emp);

            last_id++;

        } else if (operation == "UPDATE"){
            getline(ss, str_id, ';');
            getline(ss, str_salary, ';');
            getline(ss, str_department, ';');
            id = stoi(str_id);
            salary = stoi(str_salary);
            department = stoi(str_department);

            Employee* emp = BST.search(id);
            if (emp != nullptr){
                emp->set_department(department);
                emp->set_salary(salary);
            } else {
                cout << "ERROR: An invalid ID to update\n";
            }
        } else if (operation == "DELETE"){
            getline(ss, str_id, ';');
            
            id = stoi(str_id);

            BST.remove(id);
        } else if (operation == "PRINT\r" || operation == "PRINT"){
            cout << "P\n";
            BST.printToConsole();
        } else {
            cout << "H " << BST.getHeight() << "\n";
        }
    }

    myFile0.close();

    ofstream myFile1("binary_tree.csv");
    if (!myFile1.is_open()) {
        cout << "File failed to open.xD" << "\n";
    }
    
    BST.printToFile(myFile1);

    return 0;
}