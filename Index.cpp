#include "Index.hpp"
#include <fstream>
#include <cstring>
#include <iostream>


Index::Index(const std::string& path) {
    this->index_path = path;
}


bool Index::load() {
    this->entries.clear();
    std::ifstream file;
    file.open(this->index_path, std::ios::binary);
    
    if (file.is_open() == false) {
        return false; 
    }

    IndexEntry entry;
   
    while (file.read(reinterpret_cast<char*>(&entry), sizeof(IndexEntry))) {
        this->entries.push_back(entry);
    }
    
    file.close();
    return true;
}


bool Index::save() {
    std::ofstream file;
    file.open(this->index_path, std::ios::binary | std::ios::trunc);
    
    if (file.is_open() == false) {
        return false;
    }


    for (const IndexEntry& current_entry : this->entries) {
        file.write(reinterpret_cast<const char*>(&current_entry), sizeof(IndexEntry));
    }
    
    file.close();
    return true;
}

int Index::search(const std::string& account) const {
    if (this->entries.empty() == true) {
        return -1;
    }

    int left = 0;
    int right = this->entries.size() - 1;

    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        IndexEntry mid_entry = this->entries.at(mid);
        std::string mid_account = std::string(mid_entry.account_number);

        if (mid_account == account) {
            return mid; 
        }
        
        if (mid_account < account) {
            left = mid + 1; 
        } else {
            right = mid - 1; 
        }
    }
    
    return -1;
}


bool Index::insert(const std::string& account, long long offset, int size) {
    
    int existing_position = this->search(account);
    if (existing_position != -1) {
        std::cerr << "Error: Account number already exists." << std::endl;
        return false; 
    }

   
    IndexEntry new_entry;
    std::strncpy(new_entry.account_number, account.c_str(), sizeof(new_entry.account_number) - 1);
    new_entry.account_number[sizeof(new_entry.account_number) - 1] = '\0';
    new_entry.offset = offset;
    new_entry.record_size = size;

    int target_position = 0;
    
    
    for (const IndexEntry& current_entry : this->entries) {
        std::string current_account = std::string(current_entry.account_number);
        
        if (current_account > account) {
            break; 
        }
        target_position = target_position + 1;
    }


    this->entries.insert(this->entries.begin() + target_position, new_entry);
    return true;
}


void Index::removeFromMemory(int position) {
    if (position >= 0) {
        if (position < this->entries.size()) {
            this->entries.erase(this->entries.begin() + position);
        }
    }
}


void Index::clear() {
    this->entries.clear();
}


size_t Index::size() const { 
    return this->entries.size(); 
}


IndexEntry Index::getEntry(int position) const { 
    return this->entries.at(position); 
}