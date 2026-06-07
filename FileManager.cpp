#include "FileManager.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include "nlohmann/json.hpp" 

using json = nlohmann::json;

FileManager::FileManager(const std::string& data_path, const std::string& index_path) {
    this->data_file = data_path;
    this->index = Index(index_path);
    this->index.load();
}

FileManager::~FileManager() {
    this->index.save();
}

Student FileManager::readJson(const std::string& json_path) {
    Student student;
    student.is_active = true; 

    std::ifstream file;
    file.open(json_path);
    if (file.is_open() == false) {
        std::cerr << "Error opening JSON file: " << json_path << "\n";
        student.is_active = false;
        return student;
    }

    json j;
    try {
        file >> j;
        std::string account = j["account_id"];
        std::string phone = j["phone_number"];
        std::string enrollment = j["enrollment_date"];

        std::strncpy(student.account_id, account.c_str(), sizeof(student.account_id) - 1);
        student.account_id[sizeof(student.account_id) - 1] = '\0';

        std::strncpy(student.phone_number, phone.c_str(), sizeof(student.phone_number) - 1);
        student.phone_number[sizeof(student.phone_number) - 1] = '\0';

        std::strncpy(student.enrollment_date, enrollment.c_str(), sizeof(student.enrollment_date) - 1);
        student.enrollment_date[sizeof(student.enrollment_date) - 1] = '\0';
        
        student.age = j["age"];
        student.name = j["name"];
    } catch(const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << "\n";
        student.is_active = false;
    }
    
    file.close();
    return student;
}

bool FileManager::addStudent(const std::string& json_path) {
    Student s = this->readJson(json_path);
    if (s.is_active == false) return false;

    if (this->index.search(s.account_id) != -1) {
        std::cerr << "Error: Account number already exists in index.\n";
        return false;
    }

    std::ofstream file(this->data_file, std::ios::binary | std::ios::app);
    if (!file.is_open()) return false;

    file.seekp(0, std::ios::end);
    long long offset = file.tellp();

    file.write(s.account_id, sizeof(s.account_id));
    file.write(s.phone_number, sizeof(s.phone_number));
    file.write(reinterpret_cast<char*>(&s.age), sizeof(s.age));
    file.write(s.enrollment_date, sizeof(s.enrollment_date));
    file.write(reinterpret_cast<char*>(&s.is_active), sizeof(s.is_active));
    
    int name_len = s.name.length();
    file.write(reinterpret_cast<char*>(&name_len), sizeof(name_len));
    file.write(s.name.c_str(), name_len);

    int total_size = sizeof(s.account_id) + sizeof(s.phone_number) + sizeof(s.age) + 
                     sizeof(s.enrollment_date) + sizeof(s.is_active) + sizeof(name_len) + name_len;

    file.close();
    this->index.insert(s.account_id, offset, total_size);
    std::cout << "Student added successfully.\n";
    return true;
}

bool FileManager::searchStudent(const std::string& account_id) {
    int pos = this->index.search(account_id);
    if (pos == -1) {
        std::cout << "Student not found.\n";
        return false;
    }

    IndexEntry entry = this->index.getEntry(pos);
    std::ifstream file(this->data_file, std::ios::binary);
    if (!file.is_open()) return false;

    file.seekg(entry.offset, std::ios::beg);
    Student s;
    file.read(s.account_id, sizeof(s.account_id));
    file.read(s.phone_number, sizeof(s.phone_number));
    file.read(reinterpret_cast<char*>(&s.age), sizeof(s.age));
    file.read(s.enrollment_date, sizeof(s.enrollment_date));
    file.read(reinterpret_cast<char*>(&s.is_active), sizeof(s.is_active));

    int name_len;
    file.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
    

    s.name.resize(name_len);
    file.read(&s.name[0], name_len);
    

    file.close();
    if (!s.is_active) {
        std::cout << "Student inactive.\n";
        return false;
    }

    std::cout << "--- Student Data ---\nAccount: " << s.account_id << "\nName: " << s.name 
              << "\nPhone: " << s.phone_number << "\nAge: " << s.age 
              << "\nEnrollment Date: " << s.enrollment_date << "\n";
    return true;
}

bool FileManager::deleteStudent(const std::string& account_id) {
    int pos = this->index.search(account_id);
    if (pos == -1) return false;

    IndexEntry entry = this->index.getEntry(pos);
    std::fstream file(this->data_file, std::ios::binary | std::ios::in | std::ios::out);
    
    long is_active_offset = entry.offset + sizeof(Student::account_id) + 
                            sizeof(Student::phone_number) + sizeof(Student::age) + 
                            sizeof(Student::enrollment_date);

    file.seekp(is_active_offset, std::ios::beg);
    bool is_active = false;
    file.write(reinterpret_cast<char*>(&is_active), sizeof(is_active));
    file.close();
    return true;
}

bool FileManager::updateStudent(const std::string& json_path) {
    Student s = this->readJson(json_path);
    int pos = this->index.search(s.account_id);
    if (pos == -1) return false;

    this->deleteStudent(s.account_id); 
    this->index.removeFromMemory(pos);

    std::ofstream file(this->data_file, std::ios::binary | std::ios::app);
    long long new_offset = file.tellp();

    file.write(s.account_id, sizeof(s.account_id));
    file.write(s.phone_number, sizeof(s.phone_number));
    file.write(reinterpret_cast<char*>(&s.age), sizeof(s.age));
    file.write(s.enrollment_date, sizeof(s.enrollment_date));
    bool active = true;
    file.write(reinterpret_cast<char*>(&active), sizeof(active));
    int name_len = s.name.length();
    file.write(reinterpret_cast<char*>(&name_len), sizeof(name_len));
    file.write(s.name.c_str(), name_len);
    
    file.close();
    this->index.insert(s.account_id, new_offset, sizeof(s.account_id) + sizeof(s.phone_number) + sizeof(s.age) + sizeof(s.enrollment_date) + sizeof(active) + sizeof(name_len) + name_len);
    return true;
}

bool FileManager::cleanUp() {
    std::ifstream file_in(this->data_file, std::ios::binary);
    std::ofstream file_out("temp_students.dat", std::ios::binary);
    Index new_index; 

    file_in.seekg(0, std::ios::end);
    long long file_size = file_in.tellg();
    file_in.seekg(0, std::ios::beg);

    while (file_in.tellg() < file_size) {
        Student s;
        file_in.read(s.account_id, sizeof(s.account_id));
        file_in.read(s.phone_number, sizeof(s.phone_number));
        file_in.read(reinterpret_cast<char*>(&s.age), sizeof(s.age));
        file_in.read(s.enrollment_date, sizeof(s.enrollment_date));
        file_in.read(reinterpret_cast<char*>(&s.is_active), sizeof(s.is_active));

        int name_len;
        file_in.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
        
        
        s.name.resize(name_len);
        file_in.read(&s.name[0], name_len);
        

        if (s.is_active) {
            long long new_offset = file_out.tellp();
            file_out.write(s.account_id, sizeof(s.account_id));
            file_out.write(s.phone_number, sizeof(s.phone_number));
            file_out.write(reinterpret_cast<char*>(&s.age), sizeof(s.age));
            file_out.write(s.enrollment_date, sizeof(s.enrollment_date));
            file_out.write(reinterpret_cast<char*>(&s.is_active), sizeof(s.is_active));
            file_out.write(reinterpret_cast<char*>(&name_len), sizeof(name_len));
            file_out.write(s.name.c_str(), name_len);
            new_index.insert(std::string(s.account_id), new_offset, sizeof(s.account_id) + sizeof(s.phone_number) + sizeof(s.age) + sizeof(s.enrollment_date) + sizeof(s.is_active) + sizeof(name_len) + name_len);
        }
    }
    file_in.close(); file_out.close();
    std::remove(this->data_file.c_str());
    std::rename("temp_students.dat", this->data_file.c_str());
    this->index = new_index;
    return true;
}