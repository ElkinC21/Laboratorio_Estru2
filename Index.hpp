#ifndef INDEX_HPP
#define INDEX_HPP

#include <vector>
#include <string>


struct IndexEntry {
    char account_number[11]; 
    long long offset;        
    int record_size;        
};

class Index {
private:
    std::vector<IndexEntry> entries; 
    std::string index_path;          

public:
    Index();
    Index(const std::string& path);
 
    
    bool load();
    bool save();
    bool insert(const std::string& account, long long offset, int size);
    int search(const std::string& account) const;
    void removeFromMemory(int position);
    void clear();
    
    
    IndexEntry getEntry(int position) const;
    size_t size() const;
};

#endif