# 🦝 RaccoonDB
### A Mini SQL-Based Database Engine Built from Scratch in C++

RaccoonDB is a lightweight, SQL-like database engine developed entirely in C++.

The project focuses on understanding how database engines work internally by implementing components such as tokenization, parsing, query execution, data storage, indexing, searching, and sorting.

Unlike a typical CRUD management application, RaccoonDB is built as a small database engine. It includes custom commands and manually implemented Data Structures and Algorithms (DSA) that are integrated into actual database operations.

---

## 🎯 Project Objectives

- Build a mini database engine from scratch using C++.
- Understand how SQL commands are processed internally.
- Implement a custom Tokenizer and Parser.
- Develop database and table management operations.
- Implement searching and sorting algorithms from scratch.
- Integrate Hash Table and Binary Search Tree indexing.
- Build a basic persistent storage system using C++ file handling.
- Understand the relationship between DSA and database performance.

---

## ⭐ Key Features

### 1. Custom SEARCH Command

RaccoonDB provides its own `SEARCH` command to find records based on column conditions.

**Syntax:**

```sql
SEARCH table_name BY column_name operator value;
```

**Examples:**

```sql
SEARCH students BY age = 20;
SEARCH students BY age > 20;
SEARCH students BY age >= 18;
SEARCH students BY age <= 25;
```

**How it works:**

RaccoonDB selects a searching algorithm based on the available index:

| Condition | Searching method |
|---|---|
| Equality with Hash Table index | Hash Table |
| Integer search with BST index | Binary Search Tree |
| No suitable index | Linear Search |

The implementation uses:

- Hash Table for exact equality searches.
- BST for integer range searches.
- Linear Search as a fallback.

### 2. Custom SORT Command

RaccoonDB includes a custom `SORT` command that changes the actual row order of a table.

**Syntax:**

```sql
SORT table_name BY column_name ASC;
SORT table_name BY column_name DESC;
```

**Example:**

```sql
SORT students BY age ASC;
```

The command uses Merge Sort to rearrange the table's records.

**Difference between ORDER BY and SORT:**

| ORDER BY | Custom SORT |
|---|---|
| SQL-style query feature | Custom RaccoonDB command |
| Sorts displayed query results | Changes actual table row order |
| Does not change the original table order | Modifies the in-memory table order |
| Uses Merge Sort | Uses Merge Sort |

### 3. Database Utility Commands

| Command | Purpose |
|---|---|
| `HELP` | Displays supported commands |
| `STATUS` | Displays database engine status |
| `STATS` | Displays database statistics |
| `SEARCH` | Searches records using available indexes or Linear Search |
| `SORT` | Sorts actual table rows using Merge Sort |

---

## ✨ Current Features — Version 1

### Database and Table Management

- `CREATE DATABASE`
- `SHOW DATABASES`
- `USE`
- `DROP DATABASE`
- `CREATE TABLE`
- `SHOW TABLES`
- `DROP TABLE`
- `DESCRIBE TABLE`

### Data Operations

- `INSERT INTO`
- `SELECT`
- `UPDATE`
- `DELETE`

### Query Processing

- Custom Tokenizer.
- Custom Parser.
- SQL-like command validation.
- Column selection and `SELECT *`.
- `WHERE` conditions.
- Comparison operators: `=`, `!=`, `<>`, `>`, `<`, `>=`, `<=`.
- `AND` and `OR` conditions.
- Support for `INT`, `TEXT`, and `VARCHAR(n)`.

### Sorting and Searching

- `ORDER BY` using Merge Sort.
- Custom `SORT` using Merge Sort.
- Custom `SEARCH` command.
- Linear Search fallback.
- Hash Table indexing.
- Binary Search Tree indexing for integer columns.
- Equality and range-based indexed searching.

### Persistent Storage

- CSV-based persistent storage.
- Separate storage for databases and tables.
- Database metadata storage.
- Loading saved databases at startup.
- Saving database and table changes to disk.

---

## 🧠 Data Structures and Algorithms

DSA is a core part of RaccoonDB. The goal is to implement algorithms from scratch and use them in actual database operations rather than keeping them as isolated demonstrations.

### Currently Implemented

| Data Structure / Algorithm | Application | Complexity |
|---|---|---|
| Merge Sort | `ORDER BY` and `SORT` | O(n log n) |
| Linear Search | Fallback searching | O(n) |
| Hash Table | Equality-based indexing | Average O(1) lookup |
| Binary Search Tree (BST) | Integer range indexing | Average O(log n) search |

**1. Merge Sort**

Used to sort records based on a selected column. It powers both `ORDER BY` and the custom `SORT` command.

**2. Linear Search**

Checks records one by one when no suitable index is available.

**3. Hash Table**

Implemented using separate chaining to handle collisions. It stores keys and matching row indices and is used for equality-based searches.

**4. Binary Search Tree**

Stores integer column values and their corresponding row indices. Duplicate values are stored in the same node.

It supports equality and range searches using operators such as `>`, `<`, `>=`, and `<=`.

The current implementation is an ordinary, unbalanced BST. Its worst-case search complexity is O(n).

---

## 🏗️ How RaccoonDB Was Developed

RaccoonDB was developed incrementally, with each milestone adding another component to the database engine.

### Step 1 — Tokenizer

The first component was the Tokenizer.

It breaks commands into smaller tokens that the program can understand.

Example:

```sql
SELECT name FROM students;
```

Tokenized output:

```text
[SELECT] [name] [FROM] [students] [;]
```

It recognizes keywords, identifiers, punctuation, comparison operators, and quoted values.

### Step 2 — Parser

The Parser was developed after the Tokenizer.

It validates the structure of commands and detects invalid syntax before execution.

### Step 3 — Database and Table Management

Classes were introduced to represent databases, tables, columns, and rows in memory.

### Step 4 — CREATE TABLE and Data Types

Table creation and data type validation were implemented with support for `INT`, `TEXT`, and `VARCHAR(n)`.

### Step 5 — INSERT

INSERT was implemented with validation for column names, value counts, and data types.

### Step 6 — SELECT and WHERE

SELECT was developed to retrieve records, followed by filtering using comparison operators and `AND` / `OR`.

### Step 7 — Merge Sort

Merge Sort was integrated into `ORDER BY` and the custom `SORT` command.

### Step 8 — Linear Search

A separate searching implementation was developed and integrated into the custom `SEARCH` command.

### Step 9 — UPDATE and DELETE

UPDATE modifies existing records, while DELETE removes records matching specified conditions.

### Step 10 — Database Utility Commands

Commands such as `SHOW DATABASES`, `USE`, `DROP DATABASE`, `SHOW TABLES`, and `DESCRIBE TABLE` were implemented.

Custom commands `HELP`, `STATUS`, and `STATS` were also introduced.

### Step 11 — Persistent Storage

A storage component was developed using C++ file handling and CSV files.

Database metadata, table schemas, and records are saved to disk and loaded when RaccoonDB starts.

### Step 12 — Hash Table Indexing

A Hash Table was implemented from scratch using separate chaining.

It was integrated into `SEARCH` to perform equality-based indexed searches.

### Step 13 — Binary Search Tree Indexing

A Binary Search Tree was implemented and integrated into RaccoonDB.

It supports integer range searches and stores row indices for matching records.

---

## 📂 Project Structure

```text
RaccoonDB/
│
├── main.cpp
│
├── Tokenizer.h
├── Tokenizer.cpp
│
├── Parser.h
├── Parser.cpp
│
├── Database.h
├── Database.cpp
│
├── MergeSort.cpp
│
├── Search.h
├── Search.cpp
│
├── HashTable.cpp
├── BST.cpp
│
├── Storage.h
├── Storage.cpp
│
├── data/
│   ├── databases.txt
│   ├── default/
│   └── school/
│
├── .gitignore
└── README.md
```

The `data` directory is generated by RaccoonDB and stores database metadata and table records.

---

## 🚀 How to Compile and Run

### Requirements

- C++17-compatible compiler.
- Terminal or command-line environment.
- Editline library for the `-ledit` linker option.

### 1. Clone the Repository

```bash
git clone https://github.com/ashishraj140744/RaccoonDB.git
```

### 2. Open the Project Directory

```bash
cd RaccoonDB
```

### 3. Compile

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Tokenizer.cpp Parser.cpp Database.cpp MergeSort.cpp Search.cpp Storage.cpp HashTable.cpp BST.cpp -ledit -o raccoondb
```

### 4. Run

```bash
./raccoondb
```

---

## 💻 Example Usage

```sql
CREATE DATABASE school;

SHOW DATABASES;

USE school;

CREATE TABLE students (
    id INT,
    name VARCHAR(50),
    age INT,
    city TEXT
);

INSERT INTO students (id, name, age, city)
VALUES (1, 'Ashish', 20, 'Shimla');

INSERT INTO students (id, name, age, city)
VALUES (2, 'Rahul', 21, 'Delhi');

SELECT * FROM students;

SELECT name, age
FROM students
WHERE age > 20;

SELECT * FROM students
ORDER BY age;

SEARCH students BY age > 20;

SORT students BY age ASC;

UPDATE students
SET age = 22
WHERE id = 1;

DELETE FROM students
WHERE id = 2;

SHOW TABLES;

DESCRIBE TABLE students;
```

### Creating Indexes

**Hash Table index:**

```sql
CREATE INDEX students BY age;
```

**BST index:**

```sql
CREATE BST INDEX students BY age;
```

**Searching with indexes:**

```sql
SEARCH students BY age = 20;
SEARCH students BY age > 20;
SEARCH students BY age <= 25;
```

---

## 🔮 Future Development — Version 2

Version 2 will focus on improving the storage engine, indexing, and query execution.

### Planned Improvements

- B-Tree indexing.
- More advanced disk-based indexing.
- Index persistence across database restarts.
- Automatic index rebuilding after INSERT, UPDATE, and DELETE.
- Page-based storage instead of basic CSV-only storage.
- Buffer pool and memory management.
- Improved storage and recovery mechanisms.
- Better query execution and index selection.

These are future goals, not currently implemented features.

---

## ⚠️ Current Limitations

RaccoonDB is an educational database engine and is not intended to replace production database systems.

- Storage currently uses CSV files rather than a binary page-based storage engine.
- Hash Table and BST indexes are maintained in memory.
- Index persistence across restarts is not implemented.
- Automatic index rebuilding after INSERT, UPDATE, and DELETE is deferred to a future version.
- The BST is unbalanced and can degrade to O(n) in the worst case.
- Advanced SQL features such as `GROUP BY`, `HAVING`, `LIMIT`, and aggregate functions are not part of the current Version 1 scope.
- Production-grade transaction handling and crash recovery are not implemented.

---

## 🎓 Project Purpose

The purpose of RaccoonDB is to understand how database engines work internally and apply Data Structures and Algorithms to real database operations.

It combines C++ programming, database concepts, file handling, searching, sorting, hashing, and tree-based indexing into one project.

**Version 1 establishes the core database engine. Future versions will focus on advanced indexing and a more efficient storage engine.**

---

**Developed in C++ | Mini Database Engine | Data Structures and Algorithms | SQL-Like Query Processing**
