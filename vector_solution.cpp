#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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

    vector<Employee> Employees;
    Employee* ptr = nullptr;

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
    if (!myFile0.is_open()) {
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
            error = false;
            i = 0;
            
            getline(ss, str_id, ';');
            getline(ss, str_salary, ';');
            getline(ss, str_department, '\r');
            id = stoi(str_id);
            salary = stoi(str_salary);
            department = stoi(str_department);

            while (Employees.at(i).get_id() != id){
                i++;
                if (Employees.begin() + i == Employees.end()){
                    cout << "ERROR: An invalid ID to update" << '\n';
                    error = true;
                    break;
                }
            }
            if (error == true){
                continue;
            }

            Employees.at(i).set_salary(salary);
            Employees.at(i).set_department(department);
        } else {
            error = false;
            i = 0;
            if (Employees.size() == 0){
                cout << "ERROR: There is no Employee" << '\n';
            } else {
                getline(ss, str_id, '\r');
                id = stoi(str_id);

                while (Employees.at(i).get_id() != id){
                    i++;
                    if (Employees.begin() + i == Employees.end()){
                       cout << "ERROR: An invalid ID to delete" << '\n';
                       error = true;
                       break;
                    }
                }
                if (error){
                    continue;
                }
                Employees.erase(Employees.begin() + i);
            }
        }
    }

    myFile0.close();
    ofstream myFile1("vector_solution.csv");
    if (!myFile1.is_open()) {
        cout << "File failed to open." << "\n";
        return 0;
    }

    myFile1 << "Employee_ID;Salary;Department" << "\n";
    for (int i = 0; i < Employees.size(); i++){
        myFile1 << Employees.at(i).get_id() << ";" << Employees.at(i).get_salary() << ";" << Employees.at(i).get_department() << '\n';
    }
    
    myFile1.close();

    return 0;
}