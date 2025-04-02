
# 📦 C++ Inventory & Purchase Management System ⚙️

<div align="center">
  <p>
    <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="Language C++">
    <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" alt="License MIT">
    </p>

  **A robust Inventory and Purchase Management System built with C++, designed as part of an Object-Oriented Programming curriculum. This project demonstrates core OOP principles through a practical, role-based application.**
</div>

---

## ✨ Overview

This system provides functionalities for managing inventory items, suppliers, purchase requisitions, and purchase orders. It features a clear separation of concerns based on user roles, ensuring data integrity and controlled access. The application utilizes fundamental C++ features and OOP concepts to create a modular and maintainable codebase.

---

## 📸 Screenshots

<div align="center">
  <p><em>(Add screenshots of the main menu, different user role interfaces, etc. here)</em></p>
  </div>

---

## 🚀 Features

* **💾 Data Persistence:** Uses text files (`.txt`) for storing and retrieving data related to items, suppliers, users, and orders.
* **👤 Role-Based Access Control:** Distinct interfaces and permissions for different user types:
    * **👑 Admin:**
        * Create & Manage User Accounts (Sales/Purchase Managers).
        * Add/Modify System Data (Items, Suppliers).
        * Full access to all system functionalities.
    * **🛒 Sales Manager:**
        * Manage Items (Add, Modify, Delete).
        * Manage Suppliers (Add, Modify, Delete).
        * Create Purchase Requisitions.
        * View Purchase Orders.
    * **📊 Purchase Manager:**
        * Manipulate Purchase Orders (Create, Generate, potentially Approve/Update).
        * View Item List.
        * View Suppliers List.
* **📝 CRUD Operations:** Functionality to Create, Read (View), Update (Edit), and Delete records for:
    * Inventory Items
    * Suppliers
    * Sales Entries (or Purchase Orders/Requisitions)
* **🖱️ Menu-Driven Interface:** User-friendly console menus for easy navigation and interaction.

---

## 💡 Core OOP Concepts Implemented

This project was specifically designed to apply fundamental Object-Oriented Programming principles:

* **🛡️ Encapsulation:**
    * Data members within classes (e.g., `Item`, `User`, `Supplier`) are kept private or protected.
    * Access and modification are controlled through public member functions (getters/setters), protecting the internal state.
* **🌳 Inheritance:**
    * A potential base class like `User` could be inherited by `Admin`, `SalesManager`, and `PurchaseManager` to share common attributes (like username, password) and define role-specific functionalities.
    * Base classes for data records (e.g., `Record`) might be used for common operations like file I/O.
* **💫 Polymorphism:**
    * Virtual functions are likely used (e.g., `displayMenu()` or `performAction()`) within the user hierarchy.
    * This allows the program to treat different user objects generically (via base class pointers/references) while invoking the correct role-specific behavior at runtime.
* **🧩 Abstraction:**
    * Classes model real-world entities (Item, Supplier, User, Order) focusing on essential characteristics and behaviors.
    * Complex implementation details (like file parsing or specific data validation logic) are hidden within methods, providing a simpler interface to the user of the class.

---

## 🛠️ Tech Stack

* **Language:** C++ (Specify standard if known, e.g., C++11 or later)
* **Core Libraries:**
    * `<iostream>` for console I/O.
    * `<fstream>` for file operations (reading/writing to `.txt` files).
    * `<vector>`, `<string>`, `<sstream>` for data handling and manipulation.
    * (List any other significant standard or external libraries used).

---

## 🔧 Getting Started

### Prerequisites

* A C++ Compiler that supports C++11 or later (e.g., GCC/g++, Clang, MSVC).
* `make` (Optional, but recommended if a `Makefile` is provided).
* Git (for cloning the repository).

### Installation & Setup

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git](https://www.google.com/search?q=https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git)
    cd YOUR_REPO_NAME
    ```
    *(Replace `YOUR_USERNAME/YOUR_REPO_NAME` with your actual details)*

2.  **Compile the source code:**
    * **Using g++ (Example):**
        Compile all necessary `.cpp` files together. Adjust the command based on your source files:
        ```bash
        g++ main.cpp user.cpp item.cpp supplier.cpp file_handler.cpp -o inventory_system -std=c++11
        ```
        *(Make sure to include all your `.cpp` files in the command)*
    * **Using Make (If `Makefile` exists):**
        ```bash
        make
        ```

3.  **Prepare Data Files:**
    * The system might require specific `.txt` files (e.g., `users.txt`, `items.txt`, `suppliers.txt`) to exist before running. Check if sample files are provided or if the program creates them on first run. Ensure the program has permission to read/write in the execution directory.

### Running the Application

1.  Execute the compiled program:
    ```bash
    ./inventory_system
    ```
2.  Follow the on-screen menu prompts to log in and interact with the system based on user roles.

---

*(Adjust this structure to match your actual project layout)*

---

## 📜 License

Distributed under the MIT License. See `LICENSE` file for more information.

*(If this was purely an academic assignment without intent for broader use, you might state "Educational Purposes Only")*

---

## 👤 Author

* **Pedro Fabian Owono** - [Owono2001](https://github.com/Owono2001)

---

<div align="center">
  <em>Happy Coding & Managing Inventory!</em>
</div>
