#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include "Student.hpp"
#include "Index.hpp"
#include <string>

class FileManager {
private:
    std::string data_file;
    Index index;

    Student readJson(const std::string& json_path);

public:
    FileManager(const std::string& data_path, const std::string& index_path);
    ~FileManager();

    bool addStudent(const std::string& json_path);
    bool deleteStudent(const std::string& account_id);
    bool searchStudent(const std::string& account_id);
    bool updateStudent(const std::string& json_path);
    bool cleanUp();
};

#endif