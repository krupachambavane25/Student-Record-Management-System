# Student Record Management System 

## 📌 Project Overview

The **Student Record Management System** is a C-based academic project designed to manage student information using fundamental **Data Structures and Algorithms**.

The system provides operations for adding, displaying, searching, updating and deleting student records. It also demonstrates the practical use of a **Linked List, Stack and Queue**.

- **Linked List** → Dynamic student record management
- **Stack** → Recently deleted records and Undo operation
- **Queue** → Admission waiting list using FIFO
- **Searching** → Finding student records
- **Sorting** → CGPA-based student ranking
- **File Handling** → Persistent record storage/report generation

---

## 🎯 Objectives

- Store and manage student records efficiently.
- Implement a dynamic student database using a Linked List.
- Perform insertion, deletion, searching and updating.
- Implement Stack-based Undo functionality.
- Implement Queue-based admission waiting-list management.
- Sort students according to CGPA.
- Calculate academic statistics such as average and highest CGPA.
- Demonstrate practical applications of Data Structures and Algorithms.

---

## 🧠 Data Structures and Algorithms Used

| Concept | Application |
|---|---|
| Linked List | Student record storage |
| Stack | Recently deleted records / Undo |
| Queue | Admission waiting list |
| Linear Search | Search student by roll number/name |
| Sorting | CGPA-based ranking |
| File Handling | Store and retrieve records |
| Dynamic Memory Allocation | Create student nodes dynamically |

---

## ⚙️ Main Features

### Student Management
- Add student
- Display all students
- Search student
- Update student
- Delete student

### Stack Operations
- Push deleted student record
- Pop deleted student record
- Undo last deletion

### Queue Operations
- Add student to waiting list
- Admit student from front
- FIFO-based admission management

### Academic Analysis
- Calculate average CGPA
- Find highest CGPA
- Sort students by CGPA
- Attendance analysis
- Generate academic report

### Validation
- Test valid student records
- Test unsuccessful searches
- Test deletion
- Test Undo operation
- Test Queue operations
- Test CGPA sorting

---

## 🏗️ System Architecture

```text
                 STUDENT RECORD MANAGEMENT SYSTEM
                              |
              +---------------+---------------+
              |               |               |
              v               v               v
       Student Records       Stack           Queue
              |               |               |
              v               v               v
         Linked List       Undo Delete     Waiting List
              |
      +-------+--------+
      |       |        |
      v       v        v
   Search   Update   Delete
              |
              v
       Academic Analysis
              |
       +------+------+
       |             |
       v             v
    CGPA Ranking   Attendance
```

---

## 📂 Project Structure

```text
DSA SLA/
│
├── index.html
├── script.js
├── server.c
├── student.c
├── student.h
└── style.css
├── README.md
└── .gitignore
```

> Keep the source code in `src/`, reports in `docs/`, and actual program screenshots in `screenshots/`.

---

## 💻 Requirements

### Software

- Windows 10/11 or compatible operating system
- Visual Studio Code or another C IDE
- GCC / MinGW C Compiler

### Hardware

- Minimum 4 GB RAM
- Standard computer/laptop
- At least 100 MB free storage

---

## 🚀 How to Run

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR-USERNAME/Student-Record-Management-System.git
```

### 2. Open the Project

```bash
cd Student-Record-Management-System
```

### 3. Compile the Program

```bash
gcc src/student_record_management_system.c -o student_record_system
```

### 4. Run the Program on Windows

```bash
server.exe
```

### Linux/macOS

```bash
./server
```

---


## 📊 Complexity Analysis

| Operation | Time Complexity |
|---|---:|
| Insert at beginning | O(1) |
| Insert at end | O(n) |
| Display | O(n) |
| Search | O(n) |
| Update | O(n) |
| Delete | O(n) |
| Stack Push | O(1) |
| Stack Pop | O(1) |
| Queue Enqueue | O(1) |
| Queue Dequeue | O(1) |
| Find Highest CGPA | O(n) |
| Average CGPA | O(n) |
| Simple Sorting | O(n²) |

### Space Complexity

```text
Overall Space Complexity = O(n)
```

where `n` represents the number of student records.

---

## 🧪 Testing

The system should be tested using the following cases:

| Test Case | Input | Expected Result |
|---|---|---|
| 1 | Valid student details | Student inserted |
| 2 | Existing roll number | Student found |
| 3 | Non-existing roll number | Student not found |
| 4 | Existing student deletion | Student deleted |
| 5 | Undo after deletion | Last deleted student restored |
| 6 | Multiple waiting-list students | Students added to Queue |
| 7 | Admission operation | First student admitted |
| 8 | CGPA sorting | Records sorted correctly |
| 9 | Generate report | Correct statistics displayed |
| 10 | Empty Stack/Queue operation | Appropriate message displayed |



---


## 👥 Team Members

### Krupa Chambavane
- Problem analysis
- Linked List implementation
- Student insertion, deletion and searching
- System integration
- Testing and documentation

### Vidula Gangan
- Stack-based Undo functionality
- Queue-based waiting list
- Sorting and academic analysis
- Testing and debugging
- Documentation

### Faculty

**Prof. Gauri Chavan**  
Department of Electronics and Telecommunication Engineering

### Institution

**Shah and Anchor Kutchhi Engineering College (SAKEC), Mumbai, India**

---

## 📚 Academic Context

This project demonstrates the practical application of:

- Structures in C
- Pointers
- Dynamic Memory Allocation
- Linked Lists
- Stacks
- Queues
- Searching Algorithms
- Sorting Algorithms
- File Handling
- Time and Space Complexity

---

## 🔮 Future Scope

Possible improvements include:

- Graphical User Interface
- Web-based interface
- MySQL/PostgreSQL database integration
- Hash-table-based fast searching
- Merge Sort for improved sorting performance
- Administrator authentication
- Role-based access control
- Cloud storage
- Advanced academic analytics
- Multi-user access

---

## 📜 License

This project was created for academic and educational purposes.

---

## ⭐ Acknowledgement

We would like to thank our faculty members and Shah and Anchor Kutchhi Engineering College for their guidance and support during the development of this project.
