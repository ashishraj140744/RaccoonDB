# 🦝 RaccoonDB

**A lightweight SQL-like database engine built from scratch in C++.**

RaccoonDB is a personal project focused on understanding how database engines work internally while implementing fundamental Data Structures and Algorithms (DSA) in C++.

Instead of building another CRUD management system, the goal is to develop a small terminal-based database engine with its own query language, parsing system, data storage, and searching mechanisms.

The project is inspired by relational database systems such as MySQL, but aims to keep the implementation simple, understandable, and built from scratch.

---

## 🎯 Project Goals

- Build a basic database engine using C++.
- Understand how SQL queries are tokenized, parsed, and executed.
- Implement useful Data Structures and Algorithms in a real-world-style project.
- Develop a simple SQL-like query language.
- Implement custom commands to make database operations easier.
- Understand how databases organize, search, sort, and store records.
- Practice problem-solving and core DSA concepts through practical implementation.

## ⚙️ Planned Architecture

```text
             User
              |
         Terminal CLI
              |
         Tokenizer
              |
           Parser
              |
       Query Executor
              |
     +--------+--------+
     |        |        |
   Search   Sorting  Indexing
     |        |        |
     +--------+--------+
              |
        Storage Engine
              |
          Disk Files
```

Each component will be implemented incrementally as the project develops.

## 🛠️ Technology Stack

| Technology   | Purpose                        |
| ------------ | ------------------------------ |
| C++          | Core implementation            |
| STL          | Basic containers and utilities |
| g++          | Compilation                    |
| Git & GitHub | Version control                |

No external database engine is used for the core implementation.

## 📌 Features and Development Roadmap

### 1. Query Processing

- [x] Terminal-based command interface
- [x] Tokenizer implementation
- [x] Basic SELECT command parsing
- [x] Multiple-column SELECT parsing
- [ ] WHERE clause parsing
- [ ] Query execution engine

### 2. SQL-like Commands

Planned standard commands:

- `CREATE` – Create databases and tables.
- `INSERT` – Insert records.
- `SELECT` – Retrieve records.
- `UPDATE` – Modify records.
- `DELETE` – Remove records.
- `WHERE` – Filter records using conditions.
- `GROUP BY` – Group records.
- `ORDER BY` – Sort query results.
- `HAVING` – Filter grouped results.

### 3. Custom Commands

RaccoonDB will also introduce additional commands that go beyond traditional SQL syntax.

| Command  | Purpose                                         |
| -------- | ----------------------------------------------- |
| `SEARCH` | Provide a simplified way to search for records. |
| `SORT`   | Sort records using selected sorting algorithms. |

The aim is to make common database operations easier while demonstrating how different algorithms work internally.

### 4. Data Structures and Algorithms

DSA will be an important part of RaccoonDB rather than just an additional feature.

Planned implementations include:

| Concept              | Application                                       |
| -------------------- | ------------------------------------------------- |
| Arrays / Vectors     | Record and column storage                         |
| Linked Lists         | Managing collections of records where appropriate |
| Hash Tables          | Fast record lookup and indexing                   |
| Binary Search Trees  | Organizing searchable data                        |
| AVL Trees            | Balanced indexing                                 |
| Merge Sort           | Sorting records                                   |
| Quick Sort           | Efficient sorting                                 |
| Searching Algorithms | Finding records based on conditions               |
| Heaps                | Priority-based operations and heap sorting        |

The focus is on understanding why a particular data structure or algorithm is useful and implementing it where it makes sense.

### 5. Storage Engine

Planned storage functionality:

- Persistent file-based storage.
- Creating and managing databases and tables.
- Saving records to disk.
- Loading records when the database starts.
- Maintaining data between program executions.

## 🚀 Building and Running

RaccoonDB uses a simple g++ compilation workflow without CMake.

Clone the repository:

```bash
git clone https://github.com/ashishraj140744/RaccoonDB.git
cd RaccoonDB
```

Compile:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp Tokenizer.cpp Parser.cpp -o raccoondb
```

Run:

```bash
./raccoondb
```

## 📂 Project Structure

```text
RaccoonDB/
│
├── main.cpp
├── Tokenizer.h
├── Tokenizer.cpp
├── Parser.h
├── Parser.cpp
├── .gitignore
└── README.md
```

Additional modules will be introduced as the project grows.

## 📈 Development Approach

RaccoonDB is being developed incrementally.

The priority is to understand each component before moving to the next one. Features will be implemented, tested, and integrated step by step instead of building the entire database engine at once.

The project prioritizes practical DSA implementation, database fundamentals, and clean C++ code over unnecessary complexity.

---

**Status:** Under Development 🚧

**Language:** C++

**Project:** RaccoonDB 
