#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

using namespace std;

int last_id = 0;

class Employee{
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(){
            id = 0;
            salary = 0;
            department = 0;
        }
        Employee(int Id, int Salary, int Department){
            id = Id;
            salary = Salary;
            department = Department;
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
        int get_id(){
            return id;
        }
        int get_salary(){
            return salary;
        }
        int get_department(){
            return department;
        }
};

int main(int argc, char** argv){
    ifstream myFile(argv[1]);
    if (!myFile.is_open()){
        cout << "File failed to open." << "\n";
        return 0;
    }

    int id, salary, department;
    string line, id_str, salary_str, department_str;

    list<Employee> Employees;
    list<Employee>::iterator it = Employees.begin();

    Employee* ptr;

    getline(myFile, line); // For header line
    while (getline(myFile, line)){
        stringstream ss(line);
        getline(ss, id_str, ';');
        getline(ss, salary_str, ';');
        getline(ss, department_str, '\r');
        id = stoi(id_str);
        salary = stoi(salary_str);
        department = stoi(department_str);

        ptr = new Employee(++last_id, salary, department);

        Employees.push_back(*ptr);
    }

    myFile.close();

    ifstream myFile0(argv[2]);
    if (!myFile0.is_open()){
        cout << "File failed to open." << "\n";
        return 0;
    }

    string operation, str_id, str_salary, str_department;
    int i = 0;
    bool error = false;

    while (getline(myFile0, line)){
        stringstream ss(line);
        getline(ss, operation, ';');
        if (operation == "ADD"){
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
            salary = stoi(str_salary);
            department = stoi(str_department);

            ptr = new Employee(++last_id, salary, department);
            Employees.push_back(*ptr);
        } else if (operation == "UPDATE"){
            it = Employees.begin();
            error = false;

            getline(ss, str_id, ';');
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
            id = stoi(str_id);
            salary = stoi(str_salary);
            department = stoi(str_department);
            
            while (it->get_id() != id){
                it++;
                if (it == Employees.end()){
                    cout << "ERROR: An invalid ID to update" << '\n';
                    error = true;
                    break;
                }
            }
            if (error){
                continue;
            }
        
            it->set_salary(salary);
            it->set_department(department);
        } else {
            it = Employees.begin();
            error = false;

            getline(ss, str_id, '\r');
            id = stoi(str_id);

            while (it->get_id() != id){
                it++;
                if (it == Employees.end()){
                    cout << "ERROR: An invalid ID to delete" << '\n';
                    error = true;
                    break;
                }
            }
            if (error){
                continue;
            }
            
            Employees.erase(it);
        }
    }

    myFile0.close();
    ofstream myFile1("list_solution.csv");
    if (!myFile1.is_open()) {
        cout << "File failed to open." << "\n";
        return 0;
    }

    myFile1 << "Employee_ID;Salary;Department" << "\n";
    for (it; it != Employees.end(); it++){
        myFile1 << it->get_id() << ";" << it->get_salary() << ";" << it->get_department() << "\n";
    }
    myFile1.close();

    return 0;
}
