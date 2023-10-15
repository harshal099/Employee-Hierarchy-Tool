#include <bits/stdc++.h>
using namespace std;
//********Question 1********//
//Define a data structure to represent the given dataset.

struct Employee {
    string name;
    string reports_to;
};
//******Question 2*********//
//A. List the direct reports of a manager.
// Function to list direct reports of a manager
void listDirectReports(const vector<Employee>& employees, const string& managerName) {
    for (const auto& employee : employees) {
        if (employee.reports_to == managerName) {
            cout << employee.name << "\n";
        }
    }
}

//B.List the full suborganization that reports to a manager.
// Function to list the full suborganization that reports to a manager
void listSuborganization(const vector<Employee>& employees, const string& managerName, int level = 0) {
    for (const auto& employee : employees) {
        if (employee.reports_to == managerName) {
            for (int i = 0; i < level; ++i) {
                cout << "  "; // Indentation for hierarchy visualization
            }
            cout << employee.name << "\n";
            listSuborganization(employees, employee.name, level + 1);
        }
    }
}

//C.Show the complete management chain of an Employee
//Function to show complete management chain of an Employee.
void showManagementChain(const vector<Employee>& employees, const string& employeeName) {
    for (const auto& employee : employees) {
        if (employee.name == employeeName) {
            cout << employee.name;
            if (!employee.reports_to.empty()) {
                cout << " <- ";
                showManagementChain(employees, employee.reports_to);
            } else {
                cout << " (Top-Level Manager)";
            }
            return;
        }
    }
}

//D.Check for loops in the organization
//Function to check for loops
bool hasLoop(const vector<Employee>& employees) {
    unordered_set<string> visited;
    for (const auto& employee : employees) {
        unordered_set<string> currentChain; // To detect loops in a specific branch
        string currentEmployee = employee.name;

        while (!currentEmployee.empty()) {
            if (visited.count(currentEmployee) > 0) {
                cout << "Loop detected involving employee: " << currentEmployee << std::endl;
                return true; // Loop detected
            }

            if (currentChain.count(currentEmployee) > 0) {
                break; // Loop detected in this branch
            }

            visited.insert(currentEmployee);
            currentChain.insert(currentEmployee);

            for (const auto& emp : employees) {
                if (emp.name == currentEmployee) {
                    currentEmployee = emp.reports_to;
                    break;
                }
            }
        }
    }
    
    cout << "No loops detected in the organization hierarchy.\n";
    return false; // No loop detected
}

//**********Question 3*******//
//A.Add a given employee as a direct report of a given manager. Validate that
//* given employee does not exist
//*given manager does exist
//* the entry will not create a loop
//Methods to execute these tasks.
bool willCreateLoop(const vector<Employee>& employees, const string& managerName, const string& employeeName) {
    unordered_set<string> visited;
    string currentManager = managerName;

    while (!currentManager.empty()) {
        if (visited.count(currentManager) > 0) {
            return true; // Loop detected
        }

        visited.insert(currentManager);

        for (const auto& emp : employees) {
            if (emp.name == currentManager) {
                currentManager = emp.reports_to;
                if (emp.name == employeeName) {
                    return true; // Adding this employee will create a loop
                }
                break;
            }
        }
    }

    return false; // No loop detected
}
bool addEmployeeWithValidation(vector<Employee>& employees, const string& employeeName, const string& managerName) {
    for (const auto& employee : employees) {
        if (employee.name == employeeName) {
            cout << "Invalid operation: Employee " << employeeName << " already exists.\n";
            return false;
        }
    }
    bool managerFound = false;
    for (const auto& employee : employees) {
        if (employee.name == managerName) {
            managerFound = true;
            break;
        }
    }
    if (!managerFound) {
        cout << "Invalid operation: Manager " << managerName << " does not exist.\n";
        return false;
    }
    // Checking for potential loops
    if (willCreateLoop(employees, managerName, employeeName)) {
        cout << "Invalid operation: Adding " << employeeName << " to " << managerName << " will create a loop.\n";
        return false;
    }
    employees.push_back({employeeName, managerName});
    cout << "Added employee " << employeeName << " under manager " << managerName << std::endl;
    return true;
}


//B.Remove a given employee. Validate that
// *Removal will not create orphans (employees whose management chain is broken/disconnected)
//Method for the above task
bool employeeExistsInHierarchy(const vector<Employee>& employees, const string& employeeName) {
    for (const auto& employee : employees) {
        if (employee.reports_to == employeeName) {
            return true;
        }
    }
    return false;
}
bool removeEmployeeWithValidation(vector<Employee>& employees, const string& employeeName) {
    // Find the employee to remove
    auto it = remove_if(employees.begin(), employees.end(),
                            [&employeeName](const Employee& employee) {
                                return employee.name == employeeName;
                            });

    // If the employee was found and removed
    if (it != employees.end()) {
        string removedEmployeeName = it->name;
        employees.erase(it, employees.end());
        cout << "Removed employee " << removedEmployeeName << endl;

        // Check if removal created orphans
        for (const auto& employee : employees) {
            if (!employeeExistsInHierarchy(employees, employee.name)) {
                cout << "Orphan detected: " << employee.name << endl;
                return false;
            }
        }
        return true;
    }

    // Employee not found
    cout << "Invalid operation: Employee " << employeeName << " not found.\n";
    return false;
}


//C.Move an employee from one manager to another, along with the employee's suborganization if any
// Method for the above task.
bool moveEmployeeWithSubordinates(vector<Employee>& employees, const string& employeeName, const string& newManagerName) {
    // Find the employee to move
    auto it = find_if(employees.begin(), employees.end(),
        [&employeeName](const Employee& employee) {
            return employee.name == employeeName;
        });

    if (it != employees.end()) {
        Employee employeeToMove = *it;

        // Check if moving the employee will create a loop
        if (willCreateLoop(employees, newManagerName, employeeName)) {
            cout << "Invalid operation: Moving " << employeeName << " to " << newManagerName << " will create a loop.\n";
            return false;
        }

        // Move the employee and update their manager
        it->reports_to = newManagerName;

        // Move subordinates
        for (auto& subordinate : employees) {
            if (subordinate.reports_to == employeeName) {
                subordinate.reports_to = employeeToMove.reports_to;
            }
        }

        cout << "Moved employee " << employeeName << " to manager " << newManagerName << " along with subordinates.\n";
        return true;
    }

    // Employee not found
    cout << "Invalid operation: Employee " << employeeName << " not found.\n";
    return false;
}


int main() {
    // Create the hierarchy starting from the top-level manager.
    Employee sandeep = {"sandeep", ""};
    Employee shyam = {"shyam", "sandeep"};
    Employee peter = {"peter", "sandeep"};
    Employee mohan = {"mohan", "shyam"};
    Employee abdul = {"abdul", "shyam"};
    Employee ram = {"ram", "shyam"};
    Employee vikram = {"vikram", "peter"};
    Employee baljeet = {"baljeet", "peter"};
    Employee sundar = {"sundar", "baljeet"};

vector<Employee> employees = {
        {"sandeep", "", },
        {"shyam", "sandeep", },
        {"peter", "sandeep", },
        {"mohan", "shyam", },
        {"abdul", "shyam", },
        {"ram", "shyam", },
        {"vikram", "peter", },
        {"baljeet", "peter", },
        {"sundar", "baljeet", }
    };


    //for example lets print the direct reports of a manager, e.g., Shyam's direct reports:
    cout << "Direct Reports of Shyam:\n";
    listDirectReports(employees, "shyam");

     cout<<"*************"<<endl;
    
    //for example printing the full Suborganizationunder sandeep.
    cout << "Suborganization under Sandeep:\n";
    listSuborganization(employees, "sandeep");
    
    cout<<"*************"<<endl;
     
    //for printing management chain of an employee
    std::cout << "Management Chain of Sundar:\n";
    showManagementChain(employees, "sundar");
    cout<<endl;
    
    cout<<"***************"<<endl;
    
    //Checking for loops in the organization hierarchy
    hasLoop(employees);

    cout<<"***************"<<endl;
    
    //Methods to add an employee and simultaneously checking wether he is already in the organization or not.    
    removeEmployeeWithValidation(employees, "john");  // Valid
    removeEmployeeWithValidation(employees, "abdul");  // Removes the top-level manager, which should be avoided
    // removeEmployeeWithValidation(employees, "mohan");  // Orphan creation detected 
    
    cout<<"*****************"<<endl;
    
    moveEmployeeWithSubordinates(employees, "mohan", "peter"); // Valid
    moveEmployeeWithSubordinates(employees, "invalid_employee", "new_manager"); // Employee not found
    moveEmployeeWithSubordinates(employees, "sandeep", "peter"); // Move the top-level manager
    
    cout<<"*******************"<<endl<<endl<<endl;
    cout<<"End of Program";
    return 0;
    
}