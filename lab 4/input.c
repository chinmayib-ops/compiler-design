#include <stdio.h>

struct Employee {
    int id;
    char name[50];
    float salary;
};

int main() {
    struct Employee emp1;
    emp1.id = 101;
    emp1.salary = 50000.50;

    printf("Employee ID: %d\n", emp1.id);
    printf("Employee Salary: %.2f\n", emp1.salary);

    return 0;
}
