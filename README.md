# Banking System in C++

A simple **Banking Management System** built using **C++**, **Object-Oriented Programming (OOP)**, and **Linked Lists**.

This project allows users to:
- Create bank accounts
- Deposit and withdraw money
- Transfer funds
- View transaction history
- Manage accounts through a menu-driven console application

---

# Features

- Create new bank accounts
- Delete existing accounts
- Deposit money
- Withdraw money
- Transfer money between accounts
- View account details
- View transaction history
- View all accounts
- Automatic account number generation
- Linked list implementation for accounts and transactions
- Input validation and error handling

---

# Technologies Used

- C++
- Object-Oriented Programming (OOP)
- Linked Lists
- Console-based User Interface

---

# Data Structures Used

## 1. Account Linked List
Stores all bank accounts dynamically.

Each account contains:
- Account Number
- Owner Name
- Balance
- Transaction History

## 2. Transaction Linked List
Stores transaction records for each account.

Each transaction contains:
- Transaction Type
- Amount
- Pointer to the next transaction

---

# System Functionalities

## Create Account
Creates a new account with:
- Auto-generated account number
- Owner name
- Initial deposit

## Deposit
Adds money to an account balance.

## Withdraw
Removes money from an account if sufficient balance exists.

## Transfer
Transfers money between two accounts.

## Delete Account
Deletes an account and clears all associated transactions.

## View Account
Displays:
- Account number
- Owner name
- Current balance

## Transaction History
Displays all transactions performed on an account.

---

# Sample Menu

```text
**********************************
*        BANKING SYSTEM         *
**********************************
- 1. Create Account
- 2. Delete Account
- 3. Deposit
- 4. Withdraw
- 5. Transfer Between Accounts
- 6. View Account Details
- 7. View Transaction History
- 8. View All Accounts
- 9. Exit
**********************************
```

---

# Concepts Demonstrated

This project demonstrates:
- Classes and Objects
- Constructors and Destructors
- Dynamic Memory Allocation
- Pointers
- Linked Lists
- Encapsulation
- Menu-Driven Programming
- Basic Banking Operations

---

# How to Run

## Using Code::Blocks

1. Open Code::Blocks
2. Create a new C++ project
3. Copy and paste the code into `main.cpp`
4. Build and Run

## Using g++

Compile the program:

```bash
g++ main.cpp -o bank
```

Run the program:

```bash
./bank
```

---

# Example Output

```text
* Account Created Successfully!

Account Number : 1001
Owner          : John
Balance        : $500
```

---

# Future Improvements

Possible future upgrades:
- File handling for permanent storage
- Password protection
- Admin panel
- Interest calculation
- GUI version
- Database integration

---

# Author

**Bisrat Tibebu**,
**Tsyon Yohannes**,
**Rihanna Abrham**,
**Kidus Yared**

---

# License

This project is open-source and free to use for educational purposes.