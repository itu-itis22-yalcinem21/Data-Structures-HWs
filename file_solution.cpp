/* @Author
StudentName : Emre Safa Yalçın
StudentID : 150210090
Date : 23.10.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int last_id;

class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        Employee() {
            id = 0;
            salary = 0;
            department = 0;
        };
        ~Employee();

        void set_salary(int Salary) {
            salary = Salary;
        }
        void set_id(int ID) {
            id = ID;
        }
        void set_department(int Department) {
            department = Department;
        }

        int get_id() {
            return id;
        }
        int get_salary() {
            return salary;
        }
        int get_department() {
            return department;
        }

        Employee* add(Employee* array, int& Size, int salary, int department) {
            Employee* Employees = new Employee[Size + 1];
            
            for (int i = 0; i < Size; i++) {
                Employees[i].set_id(array[i].get_id());
                Employees[i].set_salary(array[i].get_salary());
                Employees[i].set_department(array[i].get_department());
            }
            
            Employees[Size].set_id(array[Size - 1].get_id() + 1);
            Employees[Size].set_salary(salary);
            Employees[Size].set_department(department);

            Size++;

            return Employees;
            delete array;
        }

        void update(Employee* array, int Size, int id, int salary, int department) {
            int index = -1;

            for (int i = 0; i < Size; i++)
            {
                if (id == array[i].get_id()) {
                    index = i;
                    break;
                }
            }

            if (index == -1)
            {
                cout << "ERROR: An invalid ID to update" << endl;
            }
            
            array[index].set_salary(salary);
            array[index].set_department(department);
        }

        Employee* delete_id(Employee* array, int& Size, int id) {
            int index = -1;

            for (int i = 0; i < Size; i++)
            {
                if (id == array[i].get_id()) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                cout << "ERROR: There is no Employee" << endl;
                return array;
            }
            if (id < 1) {
                cout << "ERROR: An invalid ID to delete" << endl;
                return array;
            }
            
            Employee* Employees = new Employee[Size - 1];

            for (int i = 0; i < index; i++)
            {
                Employees[i].set_id(array[i].get_id());
                Employees[i].set_salary(array[i].get_salary());
                Employees[i].set_department(array[i].get_department());
            }
            for (int i = index; i < Size - 1; i++)
            {
                Employees[i].set_id(array[i + 1].get_id());
                Employees[i].set_salary(array[i + 1].get_salary());
                Employees[i].set_department(array[i + 1].get_department());
            }

            Size--;

            return Employees;
            delete array;
        }

};

int main() {
    ifstream myFile("hw1.csv");
    if (!myFile.is_open()) {
        cout << "File failed to open." << endl;
        return 0;
    }

    int size = -1;
    string line = "";

    while (getline(myFile, line)) {
        size++;
    }

    last_id = size;

    Employee* employees = new Employee[size];

    int id, salary, department;
    string id_str, salary_str, department_str;
    int index = 0;

    myFile.clear();
    myFile.seekg(0, ios::beg);
    getline(myFile, line);
    while (getline(myFile, line)) {
        stringstream ss(line);
        getline(ss, id_str, ';');
        getline(ss, salary_str, ';');
        getline(ss, department_str, ';');
        id = stoi(id_str);
        salary = stoi(salary_str);
        department = stoi(department_str);

        employees[index].set_id(id);
        employees[index].set_salary(salary);
        employees[index].set_department(department);

        index++;
    }
    
    myFile.close();

    ifstream myFile0("operations.txt");

    if (!myFile0.is_open()) {
        cout << "File failed to open." << endl;
    }

    string line0;
    string operation, str_id, str_salary, str_department;
    
    while (getline(myFile0, line0)){
        stringstream ss(line0);
        getline(ss, operation, ';');

        if (operation == "ADD") {
            getline(ss, str_salary, ';');
            getline(ss, str_department, ';');

            salary = stoi(str_salary);
            department = stoi(str_department);

            employees = employees->add(employees, size, salary, department);
        } else if (operation == "UPDATE") {
            getline(ss, str_id, ';');
            getline(ss, str_salary, ';');
            getline(ss, str_department, ';');

            id = stoi(str_id);
            salary = stoi(str_salary);
            department = stoi(str_department);

            employees->update(employees, size, id, salary, department);
        } else {
            getline(ss, str_id, ';');
            
            id = stoi(str_id);

            employees = employees->delete_id(employees, size, id);
        }
    }
    myFile0.close();

    ofstream myFile1("array_solution.csv");
    if (!myFile1.is_open()) {
        cout << "File failed to open." << endl;
    }
    
    myFile1 << "Employee_ID;Salary;Department" << "\n";
    for (int i = 0; i < size; i++) {
        myFile1 << employees[i].get_id() << ";" << employees[i].get_salary() << ";" << employees[i].get_department() << endl;
    }

    delete(employees);
    
    return 0;
}