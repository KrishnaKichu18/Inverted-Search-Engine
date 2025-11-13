# 🔍 Inverted Search Engine – Project 2

## 📘 Overview
The **Inverted Search Engine** is a C-based indexing and lookup system that maps words to the list of files in which they appear. It constructs an efficient **word → file(s)** database using a **hash table + linked list** architecture, enabling fast lookup and compact storage.

This project focuses on modular design, text parsing, dynamic memory management, and real-world inverted-index behavior.

---

## ⚙️ Features

- ✅ 27-bucket hash table (A–Z + special characters)
- ✅ Tracks:
  - files containing each word  
  - occurrence count of each word per file  
- ✅ Supports:
  - Create Database  
  - Display Database  
  - Search Word  
  - Save Database  
  - Update/Load Database  
- ✅ Validates input files:
  - extension check  
  - duplication check  
  - emptiness check  
  - file availability  
- ✅ Menu-driven UI  
- ✅ Fully modular `.c` + `.h` structure

---

## 🧩 Project Structure

```
Inverted_Search/
├── Main.c                 → Menu controller and program driver
├── Validate.c             → Argument + file validation
├── Create_DataBase.c      → Builds hash table from input files
├── Display_and_Search.c   → Display & search system
├── Save_DataBase.c        → Serializes the database
├── Update_DataBase.c      → Loads database from save file
├── Operations.c           → List utilities and helpers
├── Types.h                → Structs, typedefs, enums
├── Inverted_Search.h      → Prototypes + shared includes
└── Makefile               → Build script
```

---

## 🧠 Core Concepts

### **1. Hash Table (27 buckets)**
- `0–25 → a–z`
- `26 → digits + all non-alphabet characters`

### **2. Linked List Nodes**
- **MAIN_NODE**  
  Stores:
  - word  
  - number of files  
  - pointer to SUB_NODE list
- **SUB_NODE**  
  Stores:
  - filename  
  - occurrence count  
- **LIST**  
  Temporary file list from command-line inputs

### **3. Inverted Index Mapping**
```
word → { filename : count }
```

### **4. Save/Load Format**
```
#index; word; file_count; fname1; count; fname2; count; #
```

---

## 🧪 Usage

### 🔹 Compile
```
make
```

Or manual compilation:
```
gcc Main.c Validate.c Create_DataBase.c Operations.c \
Display_and_Search.c Save_DataBase.c Update_DataBase.c \
-o Inverted
```

### 🔹 Run
```
./Inverted file1.txt file2.txt ...
```

### 🔹 Menu
```
1. Create Database
2. Display Database
3. Search Database
4. Save Database
5. Update Database
6. Exit
```

---

## 🔍 Example Display Output

```
======================================================================
 📊  INVERTED SEARCH DATABASE
======================================================================
| Idx | Word            | Files    | File Name            | Count    |
|-----|-----------------|----------|----------------------|----------|
| 19  | the             |    2     | README.txt           |    4     |
|     |                 |          | Hi.txt               |    2     |
```

---

## 🚨 Error Handling
Handled gracefully:

- Wrong extension  
- File not found  
- Duplicate input file  
- Empty text file  
- Corrupted save file  
- Memory allocation issues  
- Word not found  
- Invalid menu input  

---

## 📂 Future Enhancements

- Alphabetical sorting in display  
- Stop-word filtering  
- JSON / CSV export  
- Multi-database merge  
- Case-insensitive indexing  
- Colourized terminal UI  

---

## 🧑‍💻 Author Notes

Built to practice:

- Hash tables  
- Linked lists  
- File handling  
- Indexing logic  
- Modular C design  
- Serialization techniques  

A compact and well-structured inverted search engine written from scratch in C.

---

## 🪪 License
Free for educational and learning use.  
Enhancements, forks, pull requests welcome.
