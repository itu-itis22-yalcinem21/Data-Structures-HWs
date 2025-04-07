/* @Author
StudentName : Emre Safa Yalçın
StudentID : 150210090
Date : 29.10.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>

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
        }

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
            
            Employees[Size].set_id(++last_id);
            Employees[Size].set_salary(salary);
            Employees[Size++].set_department(department);

            return Employees;
            delete array;
        }

        void update(Employee* array, int Size, int id, int salary, int department) {
            int index = -1;

            for (int i = 0; i < Size; i++) {
                if (id == array[i].get_id()) {
                    index = i;
                    break;
                }
            }

            if (index == -1 || id < 1) {
                cout << "ERROR: An invalid ID to update" << "\n";
            } else {
            array[index].set_salary(salary);
            array[index].set_department(department);
            }
        }

        Employee* delete_id(Employee* array, int& Size, int id) {
            if (Size == 0){
                cout << "ERROR: There is no Employee" << "\n";
                return array;
            }

            int index = -1;

            for (int i = 0; i < Size; i++) {
                if (id == array[i].get_id()) {
                    index = i;
                    break;
                }
            }

            if (index == -1 || id < 1) {
                cout << "ERROR: An invalid ID to delete" << "\n";
                return array;
            }
            
            Employee* Employees = new Employee[Size - 1];

            for (int i = 0; i < index; i++){
                Employees[i].set_id(array[i].get_id());
                Employees[i].set_salary(array[i].get_salary());
                Employees[i].set_department(array[i].get_department());
            }
            for (int i = index; i < Size - 1; i++){
                Employees[i].set_id(array[i + 1].get_id());
                Employees[i].set_salary(array[i + 1].get_salary());
                Employees[i].set_department(array[i + 1].get_department());
            }

            Size--;

            return Employees;
            delete array;
        }

};

int main(int argc, char** argv) {
    // clock_t start = clock();
    // clock_t end = clock();

    ifstream myFile(argv[1]);
    if (!myFile.is_open()) {
        cout << "File failed to open." << "\n";
        return 0;
    }

    int size = 0;
    string line = "";

    getline(myFile, line); // For header line
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
        employees[index++].set_department(department);
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

        if (operation == "ADD") {
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
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
        cout << "File failed to open." << "\n";
    }
    
    myFile1 << "Employee_ID;Salary;Department" << "\n";
    for (int i = 0; i < size; i++) {
        myFile1 << employees[i].get_id() << ";" << employees[i].get_salary() << ";" << employees[i].get_department() << '\n';
    }

    delete(employees);
    
    return 0;
}