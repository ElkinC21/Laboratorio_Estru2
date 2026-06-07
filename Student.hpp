#ifndef STUDENT_H
#define STUDENT_H
#include <string>

struct Student {
    
    char account_id[10];
    char phone_number[12];  
    int age;
    char enrollment_date[9];
    std::string name;
    bool is_active;
    
};

#endif