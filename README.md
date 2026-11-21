# 🦝 RaccoonDB

### A Mini SQL-Based Database Engine Built from Scratch in C++

RaccoonDB is a lightweight command-line database engine developed entirely in C++. It is designed to understand how database systems work internally while implementing important Data Structures and Algorithms (DSA) concepts.

Unlike a typical CRUD management application, RaccoonDB focuses on building the internal components of a database engine, including tokenization, parsing, query execution, searching, sorting, and persistent storage.

**What makes RaccoonDB different is its custom command system.** Alongside SQL-style commands, it introduces its own commands such as `SEARCH` and `SORT`, connecting database operations directly with searching and sorting algorithms.

---

## 🎯 Project Objectives

- Build a mini database engine from scratch using C++.
- Understand how SQL commands are processed internally.
- Implement a custom Tokenizer and Parser.
- Create custom database commands using DSA.
- Implement searching and sorting algorithms.
- Manage databases, tables, columns, and records.
- Build a basic persistent storage system using CSV files.
- Develop a foundation for advanced indexing and query processing.

---

## ⭐ What Makes RaccoonDB Special?

RaccoonDB is not just an application that stores and displays records. It includes custom commands and internal implementations designed specifically for this project.

### 1. Custom SEARCH Command

<badge color="success">Implemented</badge>

RaccoonDB introduces its own `SEARCH` command to find records matching a particular column value.

**Syntax:**

```sql
SEARCH table_name BY column_name = value;
```

**Example:**

```sql
SEARCH students BY name = 'Ashish';
```

**How it works:**
- Identifies the requested table and column.
- Uses Linear Search to examine records one by one.
- Returns matching records.

**DSA used:** Linear Search

**Time Complexity:** O(n)

### 2. Custom SORT Command

<badge color="success">Implemented</badge>

RaccoonDB has a custom `SORT` command that sorts the actual records inside a table.

**Syntax:**

```sql
SORT table_name BY column_name ASC;
```

**Example:**

```sql
SORT students BY age ASC;
```

**How it works:**
- Takes the selected table and column.
- Sorts records using Merge Sort.
- Changes the actual row order in the table.

**Important difference:**

| ORDER BY | Custom SORT |
|---|---|
| Standard SQL-style query feature | Custom RaccoonDB command |
| Sorts the displayed query result | Changes actual table row order |
| Does not modify the original table order | Modifies the in-memory table order |
| Uses Merge Sort | Uses Merge Sort |

This distinction is one of the important design decisions in RaccoonDB.

### 3. Custom Database Utility Commands

<badge color="success">Implemented</badge>

RaccoonDB also includes its own utility commands:

| Command | Purpose |
|---|---|
| `HELP` | Displays supported commands |
| `STATUS` | Displays database engine status |
| `STATS` | Displays database statistics |
| `SEARCH` | Searches records using Linear Search |
| `SORT` | Sorts actual table rows using Merge Sort |

These commands make the terminal interface more useful and give RaccoonDB its own identity.

---

## ✨ Current Features — Version 1

### SQL-Style Database Operations

- `CREATE DATABASE`
- `SHOW DATABASES`
- `USE`
- `DROP DATABASE`
- `CREATE TABLE`
- `SHOW TABLES`
- `DROP TABLE`
- `DESCRIBE TABLE`
- `INSERT INTO`
- `SELECT`
- `UPDATE`
- `DELETE`

### Query Processing

- Custom Tokenizer.
- Custom Parser.
- SQL command validation.
- Column selection and `SELECT *`.
- `WHERE` conditions.
- `AND` and `OR` conditions.
- Support for `INT`, `TEXT`, and `VARCHAR(n)`.

### Sorting and Searching

- `ORDER BY` using Merge Sort.
- Custom `SORT` command using Merge Sort.
- Custom `SEARCH` command using Linear Search.

### Persistent Storage

- CSV-based persistent storage.
- Separate directories for databases.
- Separate CSV files for tables.
- Database metadata storage.
- Loading saved databases at startup.
- Saving changes after successful database operations.

---

## 🧠 Data Structures and Algorithms

DSA is a core part of RaccoonDB. The objective is to use algorithms in actual database operations instead of implementing them only for demonstration.

### Currently Implemented

| Algorithm | Where It Is Used | Complexity |
|---|---|---|
| Merge Sort | `ORDER BY` and custom `SORT` | O(n log n) |
| Linear Search | Custom `SEARCH` | O(n) |

**Merge Sort:** Used to arrange records according to a selected column. `ORDER BY` sorts query results, while custom `SORT` changes the actual order of table rows.

**Linear Search:** Checks records one by one to find matching values in a selected column.

### Planned DSA Implementations — Version 2

| Data Structure / Algorithm | Planned Application |
|---|---|
| Hash Table | Fast record lookup and column indexing |
| Binary Search | Searching sorted data |
| Binary Search Tree (BST) | Organizing indexed values |
| AVL Tree | Maintaining balanced search indexes |
| Heap / Priority Queue | Priority-based record processing |
| Linked List | Exploring alternative record and index management |
| Quick Sort | Additional sorting algorithm for comparison |

These are future development goals, not currently implemented features.

---

## 🏗️ How RaccoonDB Was Developed

RaccoonDB was developed incrementally, with each component adding another layer to the database engine.

### Step 1 — Tokenizer

The first component was the Tokenizer.

It breaks an SQL command into smaller tokens that the program can understand.

Input:

```sql
SELECT name FROM students;
```

Output:

```text
[SELECT] [name] [FROM] [students] [;]
```

It recognizes keywords, identifiers, punctuation, comparison operators, and quoted values.

### Step 2 — Parser

The Parser was implemented after the Tokenizer.

It checks whether the tokens follow the expected command structure and identifies invalid commands.

For example, it recognizes the SELECT command, selected columns, and table name.

### Step 3 — Database and Table Management

Classes were introduced to represent databases, tables, columns, and rows in memory.

This established the internal data model of RaccoonDB.

### Step 4 — CREATE TABLE and Data Types

Table creation was implemented with support for `INT`, `TEXT`, and `VARCHAR(n)`.

### Step 5 — INSERT

INSERT was implemented with validation for column names, value counts, and data types.

### Step 6 — SELECT and WHERE

SELECT was developed to retrieve records, and WHERE conditions were introduced to filter records using `AND` and `OR`.

### Step 7 — Merge Sort

Merge Sort was implemented for sorting query results and table records.

This introduced both standard-style `ORDER BY` and the custom `SORT` command.

### Step 8 — Linear Search

A separate searching component was developed.

The custom `SEARCH` command uses Linear Search to locate matching records.

### Step 9 — UPDATE and DELETE

UPDATE modifies existing records, while DELETE removes records matching specified conditions.

### Step 10 — Database Management and Utility Commands

Commands such as `SHOW DATABASES`, `USE`, `DROP DATABASE`, `SHOW TABLES`, and `DESCRIBE TABLE` were implemented.

Custom utility commands `HELP`, `STATUS`, and `STATS` were also introduced.

### Step 11 — Persistent Storage

A storage component was developed using C++ file handling and CSV files.

Database metadata, table schemas, and records are saved to disk and loaded back into memory when RaccoonDB starts.

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

The `data` directory is generated by RaccoonDB and contains saved database information.

---

## 🚀 How to Compile and Run

### Requirements

- C++17-compatible compiler.
- Terminal or command-line environment.
- Compatible editline library for the `-ledit` linker option.

### 1. Clone the Repository

```bash
git clone https://github.com/ashishraj140744/RaccoonDB.git
```

### 2. Open the Project

```bash
cd RaccoonDB
```

### 3. Compile

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Tokenizer.cpp Parser.cpp Database.cpp MergeSort.cpp Search.cpp Storage.cpp -ledit -o raccoondb
```

### 4. Run

```bash
./raccoondb
```

If your system does not have the editline library, install the compatible library or adjust the linking command for your environment.

---

## 💻 Example Usage

```sql
CREATE DATABASE school;

SHOW DATABASES;

USE school;

CREATE TABLE students (
    id INT,
    name VARCHAR(50),
    age INT
);

INSERT INTO students (id, name, age)
VALUES (1, 'Ashish', 20);

INSERT INTO students (id, name, age)
VALUES (2, 'Rahul', 21);

SELECT * FROM students;

SELECT name, age
FROM students
WHERE age > 20;

SELECT * FROM students
ORDER BY age;

SEARCH students BY name = 'Ashish';

SORT students BY age ASC;

UPDATE students
SET age = 22
WHERE id = 1;

DELETE FROM students
WHERE id = 2;

SHOW TABLES;

DESCRIBE TABLE students;
```

---

## 🔮 Version 2 — Future Development Goals

Version 2 will focus on improving database performance through indexing, advanced searching, and more efficient query processing.

### 1. Hash Table-Based Indexing

- Implement a Hash Table from scratch.
- Create indexes for selected columns.
- Handle hash collisions.
- Improve lookup performance compared with scanning every row.

### 2. Tree-Based Indexing

- Implement Binary Search Tree (BST).
- Implement AVL Tree.
- Maintain balanced search indexes.
- Explore efficient record lookup and insertion.

### 3. Advanced Searching

- Binary Search on sorted data.
- Indexed record lookup.
- Compare Linear Search, Binary Search, and Hash Table lookup.

### 4. Advanced Sorting

- Implement Quick Sort.
- Compare Quick Sort and Merge Sort.
- Improve sorting for larger tables.

### 5. Advanced Query Processing

- `LIMIT`
- Aggregate functions: `COUNT`, `SUM`, `AVG`, `MIN`, `MAX`
- `GROUP BY`
- `HAVING`
- Improved filtering and query execution.

### 6. Improved Storage Engine

- More efficient file organization.
- Better storage and loading performance.
- Improved validation and error handling.
- Safer saving and recovery mechanisms.

### 7. Additional Database Features

- Primary key and uniqueness constraints.
- `NOT NULL` and `DEFAULT`.
- Improved data type support.
- Advanced indexing and query optimization.

---

## ⚠️ Current Limitations

RaccoonDB is an educational database engine, not a production database replacement.

- Uses CSV-based persistence rather than a binary page-based storage engine.
- Currently uses Linear Search rather than indexed lookup.
- Hash Table and tree-based indexing are planned for Version 2.
- Advanced SQL features are not yet implemented.
- Does not provide production-grade transaction recovery or crash safety.

---

## 🎓 Project Purpose

The purpose of RaccoonDB is to understand how database engines work internally and apply Data Structures and Algorithms to real database operations.

Its custom commands, particularly `SEARCH` and `SORT`, demonstrate how algorithms can be integrated directly into a database engine.

**Version 1 establishes the core database engine. Version 2 aims to introduce advanced indexing, searching, sorting, and query-processing techniques.**

---

**Developed in C++ | Custom SQL-Based Database Engine | DSA-Focused Project**
