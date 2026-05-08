#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Transaction {
    string type;
    double amount;
    Transaction* next;

    Transaction(string t, double a) {
        type   = t;
        amount = a;
        next   = nullptr;
    }
};

struct Account {
    int    accountNumber;
    string ownerName;
    double balance;
    Transaction* transactionHead;
    Account*     next;

    Account(int num, string name, double initialBalance) {
        accountNumber   = num;
        ownerName       = name;
        balance         = initialBalance;
        transactionHead = nullptr;
        next            = nullptr;
    }
};

class Bank {
private:
    Account* head;
    int      nextAccNum;

    Account* findAccount(int accNum) {
        Account* current = head;
        while (current != nullptr) {
            if (current->accountNumber == accNum)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void addTransaction(Account* acc, string type, double amount) {
        Transaction* newTrans = new Transaction(type, amount);
        if (acc->transactionHead == nullptr) {
            acc->transactionHead = newTrans;
        } else {
            Transaction* t = acc->transactionHead;
            while (t->next != nullptr)
                t = t->next;
            t->next = newTrans;
        }
    }

    void insertAccount(Account* newAcc) {
        if (head == nullptr) {
            head = newAcc;
        } else {
            Account* current = head;
            while (current->next != nullptr)
                current = current->next;
            current->next = newAcc;
        }
    }

public:
    Bank() {
        head       = nullptr;
        nextAccNum = 1001;
    }

    void saveToFile() {
        ofstream file("accounts.txt");
        if (!file.is_open()) {
            cout << "  * Error: Could not open file for saving.\n";
            return;
        }

        file << nextAccNum << "\n";

        Account* current = head;
        while (current != nullptr) {
            file << "ACCOUNT\n";
            file << current->accountNumber << "\n";
            file << current->ownerName     << "\n";
            file << current->balance       << "\n";

            Transaction* t = current->transactionHead;
            while (t != nullptr) {
                file << "TRANSACTION\n";
                file << t->type   << "\n";
                file << t->amount << "\n";
                t = t->next;
            }

            file << "END\n";
            current = current->next;
        }

        file.close();
        cout << "  * Data saved to accounts.txt\n";
    }

    void loadFromFile() {
        ifstream file("accounts.txt");
        if (!file.is_open()) {
            cout << "  * No saved data found. Starting fresh.\n";
            return;
        }

        string line;
        getline(file, line);
        nextAccNum = stoi(line);

        while (getline(file, line)) {
            if (line == "ACCOUNT") {
                string numStr, name, balStr;
                getline(file, numStr);
                getline(file, name);
                getline(file, balStr);

                int    num = stoi(numStr);
                double bal = stod(balStr);

                Account* acc = new Account(num, name, bal);
                insertAccount(acc);

                string next;
                while (getline(file, next) && next != "END") {
                    if (next == "TRANSACTION") {
                        string type, amtStr;
                        getline(file, type);
                        getline(file, amtStr);
                        double amt = stod(amtStr);
                        Transaction* t = new Transaction(type, amt);
                        if (acc->transactionHead == nullptr) {
                            acc->transactionHead = t;
                        } else {
                            Transaction* cur = acc->transactionHead;
                            while (cur->next != nullptr) cur = cur->next;
                            cur->next = t;
                        }
                    }
                }
            }
        }

        file.close();
        cout << "  * Data loaded from accounts.txt\n";
    }

    void createAccount(string name, double initialDeposit) {
        if (initialDeposit < 0) {
            cout << "  * Error: Initial deposit cannot be negative.\n";
            return;
        }

        Account* newAcc = new Account(nextAccNum, name, initialDeposit);
        nextAccNum++;
        insertAccount(newAcc);

        if (initialDeposit > 0)
            addTransaction(newAcc, "Initial Deposit", initialDeposit);

        saveToFile();

        cout << "\n  * Account Created Successfully!\n";
        cout << "  ----------------------------------\n";
        cout << "  Account Number : " << newAcc->accountNumber << "\n";
        cout << "  Owner          : " << newAcc->ownerName     << "\n";
        cout << "  Balance        : $" << newAcc->balance      << "\n";
        cout << "  ----------------------------------\n";
    }

    void deleteAccount(int accNum) {
        if (head == nullptr) {
            cout << "  * Error: No accounts exist.\n";
            return;
        }

        Account* current  = head;
        Account* previous = nullptr;

        while (current != nullptr && current->accountNumber != accNum) {
            previous = current;
            current  = current->next;
        }

        if (current == nullptr) {
            cout << "  * Error: Account " << accNum << " not found.\n";
            return;
        }

        if (previous == nullptr)
            head = current->next;
        else
            previous->next = current->next;

        Transaction* t = current->transactionHead;
        while (t != nullptr) {
            Transaction* temp = t;
            t = t->next;
            delete temp;
        }

        cout << "\n  * Account " << accNum << " (" << current->ownerName << ") deleted.\n";
        cout << "  ----------------------------------\n";

        delete current;
        saveToFile();
    }

    void deposit(int accNum, double amount) {
        if (amount <= 0) {
            cout << "  * Error: Deposit amount must be positive.\n";
            return;
        }

        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "  * Error: Account " << accNum << " not found.\n";
            return;
        }

        acc->balance += amount;
        addTransaction(acc, "Deposit", amount);
        saveToFile();

        cout << "\n  * Deposit Successful!\n";
        cout << "  ----------------------------------\n";
        cout << "  Deposited   : $" << amount       << "\n";
        cout << "  New Balance : $" << acc->balance << "\n";
        cout << "  ----------------------------------\n";
    }

    void withdraw(int accNum, double amount) {
        if (amount <= 0) {
            cout << "  * Error: Withdrawal amount must be positive.\n";
            return;
        }

        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "  * Error: Account " << accNum << " not found.\n";
            return;
        }

        if (acc->balance < amount) {
            cout << "  * Error: Insufficient balance. Current: $" << acc->balance << "\n";
            return;
        }

        acc->balance -= amount;
        addTransaction(acc, "Withdrawal", amount);
        saveToFile();

        cout << "\n  * Withdrawal Successful!\n";
        cout << "  ----------------------------------\n";
        cout << "  Withdrawn   : $" << amount       << "\n";
        cout << "  New Balance : $" << acc->balance << "\n";
        cout << "  ----------------------------------\n";
    }

    void transfer(int fromAccNum, int toAccNum, double amount) {
        if (amount <= 0) {
            cout << "  * Error: Transfer amount must be positive.\n";
            return;
        }
        if (fromAccNum == toAccNum) {
            cout << "  * Error: Cannot transfer to the same account.\n";
            return;
        }

        Account* fromAcc = findAccount(fromAccNum);
        Account* toAcc   = findAccount(toAccNum);

        if (fromAcc == nullptr) {
            cout << "  * Error: Source account " << fromAccNum << " not found.\n";
            return;
        }
        if (toAcc == nullptr) {
            cout << "  * Error: Destination account " << toAccNum << " not found.\n";
            return;
        }
        if (fromAcc->balance < amount) {
            cout << "  * Error: Insufficient balance. Current: $" << fromAcc->balance << "\n";
            return;
        }

        fromAcc->balance -= amount;
        toAcc->balance   += amount;

        addTransaction(fromAcc, "Transfer Out to Acc#"  + to_string(toAccNum),   amount);
        addTransaction(toAcc,   "Transfer In from Acc#" + to_string(fromAccNum), amount);

        saveToFile();

        cout << "\n  * Transfer Successful!\n";
        cout << "  ----------------------------------\n";
        cout << "  Amount            : $" << amount             << "\n";
        cout << "  From Acc# " << fromAccNum << " Balance : $"  << fromAcc->balance << "\n";
        cout << "  To   Acc# " << toAccNum   << " Balance : $"  << toAcc->balance   << "\n";
        cout << "  ----------------------------------\n";
    }

    void viewAccount(int accNum) {
        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "  * Error: Account " << accNum << " not found.\n";
            return;
        }

        cout << "\n  **********************************\n";
        cout << "  *      ACCOUNT DETAILS          *\n";
        cout << "  **********************************\n";
        cout << "  - Account No : " << acc->accountNumber << "\n";
        cout << "  - Owner      : " << acc->ownerName     << "\n";
        cout << "  - Balance    : $" << acc->balance      << "\n";
        cout << "  **********************************\n";
    }

    void viewTransactionHistory(int accNum) {
        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "  * Error: Account " << accNum << " not found.\n";
            return;
        }

        cout << "\n  **********************************\n";
        cout << "  *    TRANSACTION HISTORY        *\n";
        cout << "  **********************************\n";
        cout << "  - Acc# " << accNum << " (" << acc->ownerName << ")\n";
        cout << "  ----------------------------------\n";

        if (acc->transactionHead == nullptr) {
            cout << "  * No transactions found.\n";
        } else {
            int count = 1;
            Transaction* t = acc->transactionHead;
            while (t != nullptr) {
                cout << "  " << count++ << ". " << t->type << " : $" << t->amount << "\n";
                t = t->next;
            }
        }

        cout << "  ----------------------------------\n";
        cout << "  - Current Balance : $" << acc->balance << "\n";
        cout << "  **********************************\n";
    }

    void viewAllAccounts() {
        if (head == nullptr) {
            cout << "  * No accounts found.\n";
            return;
        }

        cout << "\n  **********************************\n";
        cout << "  *        ALL ACCOUNTS           *\n";
        cout << "  **********************************\n";

        Account* current = head;
        while (current != nullptr) {
            cout << "  - Acc# " << current->accountNumber
                 << " | " << current->ownerName
                 << " | $" << current->balance << "\n";
            current = current->next;
        }

        cout << "  **********************************\n";
    }

    ~Bank() {
        Account* current = head;
        while (current != nullptr) {
            Transaction* t = current->transactionHead;
            while (t != nullptr) {
                Transaction* temp = t;
                t = t->next;
                delete temp;
            }
            Account* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

void showMenu() {
    cout << "\n  **********************************\n";
    cout << "  *  BANKING SYSTEM (Linked List) *\n";
    cout << "  **********************************\n";
    cout << "  - 1. Create Account\n";
    cout << "  - 2. Delete Account\n";
    cout << "  - 3. Deposit\n";
    cout << "  - 4. Withdraw\n";
    cout << "  - 5. Transfer Between Accounts\n";
    cout << "  - 6. View Account Details\n";
    cout << "  - 7. View Transaction History\n";
    cout << "  - 8. View All Accounts\n";
    cout << "  - 9. Exit\n";
    cout << "  **********************************\n";
    cout << "  Enter your choice: ";
}

int main() {
    Bank bank;

    cout << "\n  **********************************\n";
    cout << "  *  BANKING SYSTEM  *\n";
    cout << "  **********************************\n";

    bank.loadFromFile();
 
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {

            case 1: {
                string name;
                double initialDeposit;
                cout << "  Enter owner name          : ";
                cin.ignore();
                getline(cin, name);
                cout << "  Enter initial deposit ($) : ";
                cin >> initialDeposit;
                bank.createAccount(name, initialDeposit);
                break;
            }

            case 2: {
                int accNum;
                cout << "  Enter account number to delete: ";
                cin >> accNum;
                bank.deleteAccount(accNum);
                break;
            }

            case 3: {
                int accNum;
                double amount;
                cout << "  Enter account number  : ";
                cin >> accNum;
                cout << "  Enter deposit amount  : $";
                cin >> amount;
                bank.deposit(accNum, amount);
                break;
            }

            case 4: {
                int accNum;
                double amount;
                cout << "  Enter account number     : ";
                cin >> accNum;
                cout << "  Enter withdrawal amount  : $";
                cin >> amount;
                bank.withdraw(accNum, amount);
                break;
            }

            case 5: {
                int fromAcc, toAcc;
                double amount;
                cout << "  Enter source account number      : ";
                cin >> fromAcc;
                cout << "  Enter destination account number : ";
                cin >> toAcc;
                cout << "  Enter transfer amount            : $";
                cin >> amount;
                bank.transfer(fromAcc, toAcc, amount);
                break;
            }

            case 6: {
                int accNum;
                cout << "  Enter account number: ";
                cin >> accNum;
                bank.viewAccount(accNum);
                break;
            }

            case 7: {
                int accNum;
                cout << "  Enter account number: ";
                cin >> accNum;
                bank.viewTransactionHistory(accNum);
                break;
            }

            case 8: {
                bank.viewAllAccounts();
                break;
            }

            case 9: {
                cout << "\n  **********************************\n";
                cout << "  *  Thank you for banking with us *\n";
                cout << "  *          Goodbye!              *\n";
                cout << "  **********************************\n";
                break;
            }

            default:
                cout << "  * Error: Invalid choice. Enter 1-9.\n";
        }

    } while (choice != 9);

    return 0;
}
