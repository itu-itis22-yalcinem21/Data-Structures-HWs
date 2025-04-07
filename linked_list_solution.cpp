#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int last_id = 0;

class Employee{
    private:
        int id;
        int salary;
        int department;
        Employee* next;
    public:
        Employee(){
            id = 0;
            salary = 0;
            department = 0;
            next = nullptr;
        }
        Employee(int Id, int Salary, int Department){
            id = Id;
            salary = Salary;
            department = Department;
            next = nullptr;
        }
        ~Employee(){
            if (this->next != nullptr){
                delete this->next;
            }
        }
        void set_id(int data){
            id = data;
        }
        void set_salary(int data){
            salary = data;
        }
        void set_department(int data){
            department = data;
        }
        void set_next(Employee* data){
            next = data;
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
        Employee* get_next(){
            return next;
        }
};

class singleLinkedList{
    private:
        Employee* head;
        Employee* tail;
    public:
        singleLinkedList(){
            head = nullptr;
            tail = nullptr;
        };
        ~singleLinkedList(){
            delete head;
        };
        Employee* get_head(){
            return head;
        }
        void add_employee(int SALARY, int DEPARTMENT){
            if (head == nullptr){
                head = new Employee(++last_id, SALARY, DEPARTMENT);
                tail = head;
            } else {
                tail->set_next(new Employee(++last_id, SALARY, DEPARTMENT));
                tail = tail->get_next();
            }
        }

        void update_employee(int ID, int SALARY, int DEPARTMENT){
            Employee* ptr = head;
            while (ptr){
                if (ptr->get_id() == ID){
                    ptr->set_salary(SALARY);
                    ptr->set_department(DEPARTMENT);
                    break;
                }
                ptr = ptr->get_next();
            }
            if (ptr == nullptr){
                cout << "ERROR: An invalid ID to update" << "\n";
            }
        }

        void delete_employee(int ID){
            Employee* ptr = head;
            bool error = true;

            if (head == nullptr){
                cout << "ERROR: There is no Employee" << "\n";
            } else if (head->get_id() == ID){
                if (tail == head){
                    tail = nullptr;
                }  
                head = head->get_next();
                ptr->set_next(nullptr);
                delete ptr;
            } else if (tail->get_id() == ID){
                while (ptr->get_next()->get_id() != ID){
                    ptr = ptr->get_next();
                }
                delete tail;
                tail = ptr;
                ptr->set_next(nullptr);
            } else {
                while (ptr->get_next()){
                    if (ptr->get_next()->get_id() == ID){
                        Employee* p = ptr->get_next()->get_next();
                        ptr->get_next()->set_next(nullptr);
                        delete ptr->get_next();
                        ptr->set_next(p);
                        
                        break;
                    }
                    ptr = ptr->get_next();
                }
                if (ptr == tail){
                    cout << "ERROR: An invalid ID to delete" << "\n";
                }
            }
        }
};

int main(int argc, char** argv){
    ifstream myFile(argv[1]);
    if (!myFile.is_open()){
        cout << "File failed to open." << "\n";
        return 0;
    }

    singleLinkedList* Employees = new singleLinkedList;

    int id, salary, department;
    string line, id_str, salary_str, department_str;

    getline(myFile, line); // For header line
    while (getline(myFile, line)){
        stringstream ss(line);
        getline(ss, id_str, ';');
        getline(ss, salary_str, ';');
        getline(ss, department_str, '\r');
        id = stoi(id_str);
        salary = stoi(salary_str);
        department = stoi(department_str);

        Employees->add_employee(salary, department);
    }

    myFile.close();

    ifstream myFile0(argv[2]);
    if (!myFile0.is_open()) {
        cout << "File failed to open." << "\n";
        return 0;
    }

    string row;
    string operation, str_id, str_salary, str_department;

    while (getline(myFile0, row)){
        stringstream ss(row);
        getline(ss, operation, ';');
        if (operation == "ADD"){
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
            salary = stoi(str_salary);
            department = stoi(str_department);

            Employees->add_employee(salary, department);
        } else if (operation == "UPDATE"){
            getline(ss, str_id, ';');
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
            id = stoi(str_id);
            salary = stoi(str_salary);
            department = stoi(str_department);

            Employees->update_employee(id, salary, department);
        } else {
            getline(ss, str_id, '\r');
            id = stoi(str_id);

            Employees->delete_employee(id);
        }
    }

    myFile0.close();

    ofstream myFile1("linked_list_solution.csv");
    if (!myFile1.is_open()) {
        cout << "File failed to open." << "\n";
        return 0;
    }

    myFile1 << "Employee_ID;Salary;Department" << "\n";
    Employee* ptr = Employees->get_head();
    while (ptr){
        myFile1 << ptr->get_id() << ';' << ptr->get_salary() << ';' << ptr->get_department() << '\n';
        ptr = ptr->get_next();
    }

    return 0;
}