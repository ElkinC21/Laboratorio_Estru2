#ifndef INDEX_HPP
#define INDEX_HPP

#include <vector>
#include <string>

// Structure that represents a single row in our index table
struct IndexEntry {
    char account_number[11]; // Primary key (10 digits + '\0')
    long long offset;        // Exact byte position in the .dat file
    int record_size;         // Total size of the record in bytes
};

class Index {
private:
    std::vector<IndexEntry> entries; // RAM container for sorted index entries
    std::string index_path;          // Physical path of the index file (e.g., "students.idx")

public:
    // Constructor
    Index(const std::string& path);

    // Core database operations
    bool load();
    bool save();
    bool insert(const std::string& account, long long offset, int size);
    int search(const std::string& account) const;
    void removeFromMemory(int position);
    void clear();
    
    // Getters / Helper methods
    IndexEntry getEntry(int position) const;
    size_t size() const;
};

#endif