# Lab 1 — Variable-Length Records and Simple Primary Index

**Course:** Estructura de Datos II — UNITEC Q2 2026  
**Professor:** Ing. Iván de Jesús Deras Tábora

---

## Overview

This application manages student records stored in a binary file using variable-length records.
A primary index is loaded entirely into memory at startup and persisted to disk on exit.
All operations are performed through the command-line interface.

---

## Record Structure

| Field           | Type         | Size     | Notes                       |
|-----------------|--------------|----------|-----------------------------|
| account_number  | Fixed string | 10 bytes | Primary key (unique)        |
| name            | Var. string  | Variable | 4-byte length prefix + data |
| phone_number    | Fixed string | 12 bytes |                             |
| age             | Integer      | 4 bytes  |                             |
| enrollment_date | Fixed string | 9 bytes  | Format: YYYYMMDD + \0 NULL  |
| is_active       | Bool         | 1 byte   | Soft-delete flag            |

**Total record size:** `10 + 12 + 4 + 9 + 1 + 4 + len(name)` bytes

---

## Index Structure

The primary index is stored as a binary file. Each entry contains:

| Field          | Type      | Description                          |
|----------------|-----------|--------------------------------------|
| account_number | char[10]  | Search key                           |
| offset         | long long | Byte position of record in data file |
| record_size    | int       | Total byte size of the record        |

The index is kept **sorted by account number** at all times using a manual sorted-insertion
algorithm.

---

## Deletion Strategy

This project uses **Soft Delete (Option A)**:

- Deleted records are marked as inactive (`is_active = false`) but remain physically in the data file.
- The index reflects the inactive status of each record.
- The `--clean-up` command rebuilds both the data file and the index from scratch,
  physically removing all soft-deleted records.

---


## How to Compile

### Requirements

- CMake 3.10 or later
- C++17-compatible compiler (g++, clang++)

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The binary will be at `build/LabEstru` (Linux/macOS) or `build\Debug\LabEstru.exe` (Windows).

---

## Commands

```
./LabEstru <command> [argument]
```

| Command                             | Description                                    |
|-------------------------------------|------------------------------------------------|
| `--add_student <json_file>`         | Add a new student from a JSON file             |
| `--delete_student <account_number>` | Soft-delete a student by account number        |
| `--find_student <account_number>`   | Search and display a student by account number |
| `--update_student <json_file>`      | Update an existing student from a JSON file    |
| `--clean-up`                        | Rebuild data file removing all deleted records |

---

## Usage Examples

```bash
# Add three students
./LabEstru --add_student samples/student1.json
./LabEstru --add_student samples/student2.json
./LabEstru --add_student samples/student3.json

# Search for a student
./LabEstru --find_student 202120012

# Update a student (account number must match an existing record)
./LabEstru --update_student samples/student1_update.json

# Delete a student
./LabEstru --delete_student 201920034

# Verify the student shows as inactive
./LabEstru --find_student 201920034

# Rebuild the data file removing all soft-deleted records
./LabEstru --clean-up
```

---

## Sample JSON Files

**samples/student1.json**
```json
{
    "account_id":  "202120012",
    "name":            "Maria Elena Lopez",
    "phone_number":    "50498765432",
    "age":             21,
    "enrollment_date": "20210115"
}
```

**samples/student2.json**
```json
{
    "account_id":  "201920034",
    "name":            "Carlos Andres Mejia Ramos",
    "phone_number":    "50499123456",
    "age":             25,
    "enrollment_date": "20190301"
}
```

**samples/student3.json**
```json
{
    "account_id":  "202320010",
    "name":            "Ana Sofia Reyes",
    "phone_number":    "50487654321",
    "age":             19,
    "enrollment_date": "20230801"
}
```

**samples/student1_update.json**
```json
{
    "account_number":  "202120012",
    "name":            "Maria Elena Lopez Hernandez",
    "phone_number":    "50498765432",
    "age":             22,
    "enrollment_date": "20210115"
}
```
