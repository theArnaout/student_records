# Student Records Manager (C) – School Project

A menu-driven console application in C (C99) for managing student records stored in a binary file. Developed as part of a university assignment, this program supports Create, Read, Update, Search, and Delete (CRUD) operations with input validation.

---

## Features
- **Add Records:** Create new entries (ID, name, age, GPA)  
- **View Records:** List all students or look up by ID  
- **Update Records:** Modify existing records  
- **Delete Records:** Remove student entries permanently  
- **Search:** Find students by ID or by partial name match  
- **Persistent Storage:** Records saved in whatever `.bin` file name you choose  

---

## Prerequisites
- **Compiler:** GCC or any C99-compatible compiler installed  
- **Shell:** Linux, macOS, or Windows with a Unix-style terminal  

---

## Build & Run

```bash
# Compile
gcc student_records.c -o student_records

# Run
./student_records
