#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stack>
#include <vector>
#include <iomanip> // For use setw()

using namespace std;

struct Account {
    int accountHolderID;
	string accountHolderName;
	string gender;
	string phoneNO;
	string gmail;
	string account;
    int accountNumber;
    string password;
    int pin;
    int balance;
};

struct Transaction {
    int transactionID;
    int sourceAccount;
    int destinationAccount;
    double amount;
};

stack<Account> accountsStack;
stack<Transaction> transactionsStack;

const string accountsFile = "accounts.csv";
const string transactionsFile = "transactions.csv";

void loadAccounts() {
    ifstream file(accountsFile);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Account acc;
        string data;

        getline(ss, data, ',');
        acc.accountHolderID = stoi(data);

        getline(ss,data,',');
        acc.accountHolderName = data;

        getline(ss,data,',');
        acc.gender = data;

        getline(ss,data,',');
        acc.phoneNO = data;

        getline(ss, data, ',');
        acc.gmail = data;

        getline(ss,data,',');
        acc.account = data;

        getline(ss,data,',');
        acc.accountNumber = stoi(data);

        getline(ss,data,',');
        acc.password = data;

        getline(ss,data,',');
        acc.pin = stoi(data);

        getline(ss, data, ',');
        acc.balance = stod(data);

        accountsStack.push(acc);
    }


    file.close();
}

void saveAccounts() {
    ofstream file(accountsFile);

    stack<Account> tempStack = accountsStack;

    while (!tempStack.empty()) {
        Account acc = tempStack.top();
        file << acc.accountHolderID << "," << acc.accountHolderName << "," << acc.gender << "," << acc.phoneNO << ","
             << acc.gmail << "," << acc.account << "," << acc.accountNumber << "," << acc.password << ","<< acc.pin << "," << acc.balance << "\n";
        tempStack.pop();
    }

    file.close();
}

void loadTransactions() {
    ifstream file(transactionsFile);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Transaction trans;
        string data;

        getline(ss, data, ',');
        trans.transactionID = stoi(data);

        getline(ss, data, ',');
        trans.sourceAccount = stoi(data);

        getline(ss, data, ',');
        trans.destinationAccount = stoi(data);

        getline(ss, data, ',');
        trans.amount = stod(data);

        transactionsStack.push(trans);
    }

    file.close();
}

void saveTransactions() {
    ofstream file(transactionsFile);

    stack<Transaction> tempStack = transactionsStack;

    while (!tempStack.empty()) {
        Transaction trans = tempStack.top();
        file << trans.transactionID << "," << trans.sourceAccount << "," << trans.destinationAccount << "," << trans.amount << "\n";
        tempStack.pop();
    }

    file.close();
}

void openAccount() {
    int enteredAccountHolderID;
    cout << "\t- Enter Account Holder ID: ";
    cin >> enteredAccountHolderID;

    // Check if the account holder ID already exists
    stack<Account> tempStack = accountsStack;
    bool accountExists = false;
    
    while (!tempStack.empty()) {
        Account acc = tempStack.top();
        acc.balance =20000;
        tempStack.pop();

        if (acc.accountHolderID == enteredAccountHolderID) {
            accountExists = true;
            cout << "\nAccount Holder already exists.\n";

            // Check if they have the other type of account
            if (acc.account == "S") {
                cout << "\nYou already have a Savings Account. Do you want to open a Credit Account (C)? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y') {
                    cout << "\t - Enter Account Number: ";
                    cin >> acc.accountNumber;
                    cout << "\t - Enter Password: ";
                    cin.ignore(); // Ignore newline character
                    getline(cin, acc.password);
                    acc.account = "C"; // Change the account type to Credit
                    acc.balance = 20000;
                    accountsStack.push(acc);
                    saveAccounts();
                    cout << "\n\t\tCredit Account opened successfully.\n";
                }
            } else if (acc.account == "C") {
                cout << "\n\tYou already have a Credit Account. Do you want to open a Savings Account (S)? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y') {
                    cout << "-Enter Account Number: ";
                    cin >> acc.accountNumber;
                    cout << "- Enter Password: ";
                    cin.ignore(); // Ignore newline character
                    getline(cin, acc.password);
                    acc.account = "S"; // Change the account type to Savings
                    acc.balance -=20000;
                    accountsStack.push(acc);
                    saveAccounts();
                    cout << "\nSavings Account opened successfully.\n";
                }
            }
            break;
        }
    }

    if (!accountExists) {
        // Create a new account
        Account newAccount;
        newAccount.accountHolderID = enteredAccountHolderID;
        cout << "\t- Enter Account Holder Full Name: ";
        cin.ignore(); // Ignore newline character before getline
        getline(cin, newAccount.accountHolderName);
        cout << "\t- Enter Gender (F/M): ";
        getline(cin, newAccount.gender);
        cout << "\t- Enter Phone Number: ";
        getline(cin, newAccount.phoneNO);
        cout << "\t- Enter Gmail: ";
        getline(cin, newAccount.gmail);
        cout << "\t- Enter Account (S/C): ";
        getline(cin, newAccount.account);
        cout << "\t- Enter Account Number: ";
        cin >> newAccount.accountNumber;
        cin.ignore(); // Ignore newline character
        cout << "\t- Enter Password: ";
        getline(cin, newAccount.password);
        cout << "\t- Enter PIN code: ";
        cin >> newAccount.pin;
        cin.ignore(); // Ignore newline character
        newAccount.balance = 0; // Initializing balance

        if (newAccount.account == "C") {
            newAccount.balance = 20000; // Set initial balance for credit account
        } else {
            newAccount.balance = 0; // For other account types, set initial balance to 0
        }

        accountsStack.push(newAccount); // Add the new account to the stack
        saveAccounts(); // Save the updated account details
        cout << "Account opened successfully..... \n";
    }

    char c;
    do {
        cout << "Press '1' to create a new account, or 'e' to exit: ";
        cin >> c;

        if (c == '1') {
            openAccount();
        } else if (c == 'e') {
            break;
        } else {
            cout << "Invalid input. Please try again." << endl;
        }
    } while (true);
    // Rest of your code...
}

void editAccount(int accountNumber) {
    stack<Account> tempStack;
    bool found = false;

    while (!accountsStack.empty()) {
        Account acc = accountsStack.top();
        accountsStack.pop();

        if (acc.accountNumber == accountNumber) {
            found = true;
            int choice;
            cout << "\tAccount found. " <<endl;
			cout << "\nChoose information to edit:" <<endl;
            cout << "\t 1. Phone number" <<endl;
            cout << "\t 2. Gmail:" <<endl;
            cout << "\t 3. Password:" <<endl;
            cout << "\t 4. PIN code:" <<endl;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(); // Clear the newline character from the buffer

            switch (choice) {
                case 1:
                    cout << "- Enter new phone number: ";
                    cin >> acc.phoneNO;
                    break;
                case 2:
                    cout << "- Enter new gmail: ";
                    cin >> acc.gmail;
                    break;
                case 3:
                    cout << "- Enter new password: ";
                    cin >> acc.password;
                    break;
                case 4:
                    cout << "- Enter new PIN code: ";
                    cin >> acc.pin;
                    break;
                default:
                    cout << "Invalid choice. No changes made.\n";
            }
            tempStack.push(acc);
            cout << "\nAccount edited successfully.\n";
        } else {
            tempStack.push(acc);
        }
    }

    if (!found) {
        cout << "\nAccount not found.\n";
    }

    while (!tempStack.empty()) {
        accountsStack.push(tempStack.top());
        tempStack.pop();
    }
    saveAccounts();
}

void deleteAccount(int accountNumber) {
    stack<Account> tempStack;
    bool found = false;

    while (!accountsStack.empty()) {
        Account acc = accountsStack.top();
        accountsStack.pop();

        if (acc.accountNumber != accountNumber) {
            tempStack.push(acc);
        } else {
            found = true;
            cout << "\nAccount found and deleted successfully.\n";
            // Do not push the account onto tempStack
        }
    }

    if (!found) {
        cout << "\nAccount not found.\n";
    }

    while (!tempStack.empty()) {
        accountsStack.push(tempStack.top());
        tempStack.pop();
    }
    saveAccounts();

    char c;
    do{
    cout <<"Press 'e' to exit: ";
    cin >> c;
        if (c == 'e'){
        break;
    }
    }while(c != 'e');
}

void listAllAccounts() {
    stack<Account> tempStack = accountsStack; // Assuming accountsStack is declared somewhere in your code

    // Print the header
    cout << left << setw(5) << "ID" << setw(15) << "Customer" << setw(20) << "Gender" << setw(15) << "Phone Number" << setw(20) << "Gmail"
         << setw(15) << "Account Type" << setw(15) << "Account Number" << setw(10) << "Balance" << endl;

    // Print border
    cout << setfill('-') << setw(110) << "" << setfill(' ') << endl;

    // Print data rows
    while (!tempStack.empty()) {
        Account acc = tempStack.top();
        cout << left << setw(5) << acc.accountHolderID << setw(15) << acc.accountHolderName << setw(20) << acc.gender << setw(15) << acc.phoneNO << setw(20) << acc.gmail
             << setw(15) << acc.account << setw(15) << acc.accountNumber << setw(10) << acc.balance << endl;

        tempStack.pop();
    }

    // Ask for user input to exit
    char c;
    do {
        cout << "Press 'e' to exit: ";
        cin >> c;
    } while (c != 'e');
}

void arrangeAccounts() {
    // Sorting accounts based on account number, for example
    // Implement sorting logic here
    // For simplicity, we'll use an inefficient bubble sort
    vector<Account> tempVec;
    while (!accountsStack.empty()) {
        tempVec.push_back(accountsStack.top());
        accountsStack.pop();
    }

    for (size_t i = 0; i < tempVec.size() - 1; ++i) {
        for (size_t j = 0; j < tempVec.size() - i - 1; ++j) {
            if (tempVec[j].accountNumber > tempVec[j + 1].accountNumber) {
                swap(tempVec[j], tempVec[j + 1]);
            }
        }
    }

    for (const auto &acc : tempVec) {
        accountsStack.push(acc);
    }

    cout << "\nAccounts arranged successfully." <<endl;
    cout << "Show sorted list:" <<endl;
    listAllAccounts();

    saveAccounts();


    char c;
    do{
    cout <<"Press 'e' to exit: ";
    cin >> c;
        if (c == 'e'){
        break;
    }
    }while(c != 'e');
}


void searchAccount(int accountHolderID) {
    stack<Account> tempStack = accountsStack;
    bool found = false;

    while (!tempStack.empty()) {
        Account acc = tempStack.top();
        tempStack.pop();

        if (acc.accountHolderID == accountHolderID) {
            cout << "Account Found!\n";
            cout << "\n\t - Account Holder Name: " << acc.accountHolderName << endl;
            cout << "\t - Gender: " << acc.gender << endl;
            cout << "\t - Phone Number: " << acc.phoneNO << endl;
            cout << "\t - Gmail: " << acc.gmail << endl;
            cout << "\t - Account Type: " << acc.account << endl;
            cout << "\t - Account Number: " << acc.accountNumber << endl;
            cout << "\t - Balance: " << acc.balance << endl;
            found = true;
        }
    }

    char c;
    do {
        cout << "Press 'e' to exit: ";
        cin >> c;
        if (c == 'e') {
            break;
        }
    } while (c != 'e');
}

void depositMoney(int accountNumber, string password, double amount) {
    stack<Account> tempStack;

    while (!accountsStack.empty()) {
        Account acc = accountsStack.top();
        accountsStack.pop();

        if (acc.accountNumber == accountNumber) {
            cout <<"\n - Account name: "<< acc.accountHolderName;
            cout <<"\n - Account Id: "<< acc.accountHolderID;
            cout <<"\n - Account Type: "<< acc.account;
            cout <<"\n - Account Balance: "<< acc.balance;
            cout <<"\n";
            cin.ignore();

            acc.password += password;
            acc.balance += amount;
            accountsStack.push(acc);

            Transaction newTransaction;
            newTransaction.transactionID = transactionsStack.size() + 1;
            newTransaction.sourceAccount = accountNumber;
            newTransaction.destinationAccount = accountNumber; // Deposit into own account
            newTransaction.amount = amount;

            transactionsStack.push(newTransaction);
            saveTransactions();

            cout << "\nAmount deposited successfully.\n";
            cout << "\n - Your Account balance is: "<< acc.balance << "\n";
            break;
        } else {
            tempStack.push(acc);
        }
    }

    while (!tempStack.empty()) {
        accountsStack.push(tempStack.top());
        tempStack.pop();
    }

    saveAccounts();

    char c;
    do{
    cout <<"Press 'e' to exit: ";
    cin >> c;
        if (c == 'e'){
        continue;
     }
     }while(c != 'e');
}


void withdrawMoney(int accountNumber, string password, double amount) {
    stack<Account> tempStack;

    while (!accountsStack.empty()) {
        Account acc = accountsStack.top();
        accountsStack.pop();

        if (acc.accountNumber == accountNumber) {
            cout << "Account name: " << acc.accountHolderName;
            cout << "\nAccount Id: " << acc.accountHolderID;
            cout << "\nAccount Type: " << acc.account;
            cout << "\nAccount Balance: " << acc.balance << "\n";
            cin.ignore();

            if (acc.account == "S") {
                // Check if it's a Savings account
                cout << "Do you want to withdraw all the money?\n";
                cout << "Press 'y' to withdraw all, or 'n' to cancel: ";
                char choice;
                cin >> choice;
                if (choice == 'y') {
                    int pinAttempts = 3;
                    while (pinAttempts > 0) {
                        int enteredPin;
                        cout << "Enter your PIN: ";
                        cin >> enteredPin;
                    if (enteredPin == acc.pin) {
                    acc.balance = 0;
                    accountsStack.push(acc);

                    Transaction newTransaction;
                    newTransaction.transactionID = transactionsStack.size() + 1;
                    newTransaction.sourceAccount = accountNumber;
                    newTransaction.destinationAccount = accountNumber;
                    newTransaction.amount = -amount;

                    transactionsStack.push(newTransaction);
                    saveTransactions();

                    cout << "All funds successfully withdrawn from Savings Account.\n";
                    cout << "Your Account balance is now 0.\n";
                    break;
                    }else{
                        cout << "Incorrect PIN." << pinAttempts - 1 << "attempt remainign.\n";
                    }
            }

                } else {
                    cout << "Insufficient balance.\n";
                }
            } else if (acc.account == "C") {
                // Checking account
                if (acc.balance < amount) {
                    cout << "Insufficient balance. Cannot withdraw more than the available balance.\n";
                } else {
                    acc.balance -= amount;
                    accountsStack.push(acc);

                    Transaction newTransaction;
                    newTransaction.transactionID = transactionsStack.size() + 1;
                    newTransaction.sourceAccount = accountNumber;
                    newTransaction.destinationAccount = accountNumber;
                    newTransaction.amount = -amount;

                    transactionsStack.push(newTransaction);
                    saveTransactions();

                    cout << "Amount withdrawn successfully.\n";
                    cout << "Your Account balance is now: " << acc.balance << "\n";
                }
            } else {
                // Other account types
                if (acc.balance < amount) {
                    cout << "Insufficient balance.\n";
                } else {
                    acc.balance -= amount;
                    accountsStack.push(acc);

                    Transaction newTransaction;
                    newTransaction.transactionID = transactionsStack.size() + 1;
                    newTransaction.sourceAccount = accountNumber;
                    newTransaction.destinationAccount = accountNumber;
                    newTransaction.amount = -amount;

                    transactionsStack.push(newTransaction);
                    saveTransactions();

                    cout << "Amount withdrawn successfully.\n";
                    cout << "Your Account balance is now: " << acc.balance << "\n";
                }
            }
            break;
        } else {
            tempStack.push(acc);
        }
    }

    while (!tempStack.empty()) {
        accountsStack.push(tempStack.top());
        tempStack.pop();
    }

    saveAccounts();

    char c;
    do {
        cout << "Press 'e' to exit: ";
        cin >> c;
        if (c == 'e') {
            break;
        }
    } while (c != 'e');
}

void listAllTransactions() {
    stack<Transaction> tempStack = transactionsStack;

    while (!tempStack.empty()) {
        Transaction trans = tempStack.top();
        cout << "Transaction ID: " << trans.transactionID << ", Source Account: " << trans.sourceAccount
             << ", Destination Account: " << trans.destinationAccount << ", Amount: " << trans.amount << endl;
        tempStack.pop();
    }
}

void saveAndExit() {
    saveAccounts();
    saveTransactions();
    cout << "Exiting...\n";
}

void predisplay() {
cout << "\t\t\t         .*.*.                       .*.*." << endl;
cout << "\t\t\t        |     |                     |     |" << endl;
cout << "\t\t\t        | ... |*.*.*.*.*.*.*.*.*.*.*| ... |" << endl;
cout << "\t\t\t        | ||| |  o NATIONAL BANK o  | ||| |" << endl;
cout << "\t\t\t        | \"\"\" |  \"\"\"    \"\"\"    \"\"\"  | \"\"\" |" << endl;
cout << "\t\t\t   ())  |[-|-]| [-|-]  [-|-]  [-|-] |[-|-]|  ())" << endl;
cout << "\t\t\t  (())) |     |---------------------|     | (()))" << endl;
cout << "\t\t\t (())())| \"\"\" |  \"\"\"    \"\"\"    \"\"\"  | \"\"\" |(())())" << endl;
cout << "\t\t\t (()))()|[-|-]|  ---   --o--   ---  |[-|-]|(()))()" << endl;
cout << "\t\t\t ()))(()|     | |~|~|  |*|*|  |~|~| |     |()))(()" << endl;
cout << "\t\t\t   ||   |_____| ||_||  |*|*|  ||_|| |_____|  ||" << endl;
}

void clearscreen() {
    // For Windows
    #ifdef _WIN32
        system("cls");
    #endif
}

bool checkAccountExists(int accountNumberToCheck) {
    stack<Account> tempStack = accountsStack;
    bool exists = false;

    while (!tempStack.empty()) {
        Account acc = tempStack.top();
        tempStack.pop();

        if (acc.accountNumber == accountNumberToCheck) {
            exists = true;
            break;
        }
    }

    return exists;
}
void adminMenu() {
            clearscreen();
            int adminChoice;
            bool returnToMainMenu = false;
            do {
            	cout << "\t\t\t\t======================================" <<endl;
                cout << "\t\t\t\t=         ***ADMIN MENU***           =" <<endl;
                cout << "\t\t\t\t=     1. Create Account              =" <<endl;
                cout << "\t\t\t\t=     2. Search Account              =" <<endl;
                cout << "\t\t\t\t=     3. Show List of All Accounts   =" <<endl;
                cout << "\t\t\t\t=     4. Update Account Information  =" <<endl;
                cout << "\t\t\t\t=     5. Sort datas                  =" <<endl;
                cout << "\t\t\t\t=     6. Delete Account              =" <<endl;
                cout << "\t\t\t\t=     7. Return to Main Menu         =" <<endl;
                cout << "\t\t\t\t======================================"<<endl;
                cout << "\n\t\t\tEnter your choice: ";
            cin >> adminChoice;
            clearscreen();
            switch (adminChoice) {
                case 1: {
                    openAccount();
                    clearscreen();
                    break;
                }
                case 2: {
                    int AccountID;
                    cout << "Enter Account Holder ID to search: ";
                    cin >> AccountID;
                    searchAccount(AccountID);
                    clearscreen();
                    break;
                    }
                case 3: {
                    listAllAccounts();
                    clearscreen();
                    break;
                }
                case 4: {
                    int accountNum;
                    cout << "Enter Account Number to update: ";
                    cin >> accountNum;
                    editAccount(accountNum);
                    clearscreen();
                    break;
                }
                case 5: {
                	arrangeAccounts();
                	clearscreen();
					break;
				}
                case 6: {
                    int accountNum;
                    cout << "Enter Account Number to delete: ";
                    cin >> accountNum;
                    deleteAccount(accountNum);

                    char c;
                    do{
                    cout <<"Press 'e' to exit: ";
                    cin >> c;
                        if (c == 'e'){
                        break;
                    }
                    }while(c != 'e');
                    clearscreen();
                }
                case 7: {
                    returnToMainMenu = true;
                    clearscreen();
                    break;
                }
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }while (!returnToMainMenu);
}

void staffMenu(){
    clearscreen();
    int staffChoice;
    bool returnToMainMenu = false;
    do{
        cout << "\t\t\t\t=====================================" <<endl;
        cout << "\t\t\t\t=         ***STAFF MENU***          =" <<endl;
        cout << "\t\t\t\t=    1. Check Account Information   =" <<endl;
        cout << "\t\t\t\t=    2. Deposit Cash                =" <<endl;
        cout << "\t\t\t\t=    3. Withdraw Cash               =" <<endl;
        cout << "\t\t\t\t=    4. Return to Main Menu         =" <<endl;
        cout << "\t\t\t\t=====================================" <<endl;
        cout << "\n\t\t\tEnter your choice: ";
        cin >> staffChoice;
        clearscreen();



        switch (staffChoice) {
            case 1: {
                int AccountID;
                cout << "Enter Account Holder Name to check information: ";
                cin >> AccountID;
                searchAccount(AccountID);
                clearscreen();
                break;
                }
            case 2: {
                int accountNum;
                string password;
                double amount;
                cout << "Enter Account Number to deposit into: ";
                cin >> accountNum;
                cout << "Enter your password: ";
                cin >> password;
                cout << "Enter amount to deposit: ";
                cin >> amount;
                depositMoney(accountNum, password, amount);
                clearscreen();
                break;
            }
            case 3: {
                int accountNum;
                string password;
                double amount;
                cout << "Enter Account Number to withdraw from: ";
                cin >> accountNum;
                cout << "Enter your password: ";
                cin >> password;
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                withdrawMoney(accountNum,password, amount);
                clearscreen();
                break;
            }
            case 4: {
                // Return to main menu
                return;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }while (!returnToMainMenu);
}
void atmMenu(bool& loggedIn, int& accountNumber) {
            clearscreen();
            int enteredPassword;
            cout << "\nEnter Password to login (or 0 to return to the main menu): ";
            cin >> enteredPassword;
            if (enteredPassword == 0) {
                // Return to main menu
                return;
            }

            if (checkAccountExists(enteredPassword)) {
                loggedIn = true;
                accountNumber = enteredPassword;
                cout << "Logged in successfully!!!!" << endl;

                // Add ATM service logic here based on loggedIn status
                int atmChoice;
                cout << "\t\t\t\t============================" <<endl;
                cout << "\t\t\t\t=  ***ATM SERVICE MENU***  =" <<endl;
                cout << "\t\t\t\t=  1. Deposit Money        =" <<endl;
                cout << "\t\t\t\t=  2. Withdraw Money       =" <<endl;
                cout << "\t\t\t\t=  3. Return to Main Menu  =" <<endl;
                cout << "\t\t\t\t============================" <<endl;
                cout << "\n\t\t\tEnter your choice: ";
                cin >> atmChoice;
                clearscreen();

                switch (atmChoice) {
                    case 1: {
                        string password;
                        double depositAmount;
                        cout << "Enter your password: ";
                        cin >> password;
                        cout << "Enter amount to deposit: ";
                        cin >> depositAmount;
                        depositMoney(accountNumber,password, depositAmount);
                        clearscreen();
                        break;
                    }
                    case 2: {
                        string password;
                        double withdrawAmount;
                        cout << "Enter your password: ";
                        cin >> password;
                        cout << "Enter amount to withdraw: ";
                        cin >> withdrawAmount;
                        withdrawMoney(accountNumber, password, withdrawAmount);
                        clearscreen();
                        break;
                    }
                    case 3:
                        break;
                    default:
                        cout << "Invalid choice for ATM Service.\n";
                }

            } else {
                cout << "Account number does not exist. Please try again." << endl;
            }

            loggedIn = false; // Logout after completing ATM service

            char c;
            do{
            cout <<"Press 'e' to exit: ";
            cin >> c;
                if (c == 'e'){
                break;
            }
            }while(c != 'e');
        }

void case4(){
    saveAndExit();
}

int main() {
    loadAccounts();
    loadTransactions();

    int choice;
    bool loggedIn = false;
    int accountNumber = 0;

    do {
        predisplay();
        cout << "\n";
        cout << "\t\t\t\tBANK MANAGEMENT SYSTEM MENU\n";
        cout << "\t\t\t\t     1. Admin Login\n";
        cout << "\t\t\t\t     2. Staff Login\n";
        cout << "\t\t\t\t     3. ATM Service\n";
        cout << "\t\t\t\t     4. Exit Program\n";
        cout << "\n\t\t\tEnter your choice: ";
        cin >> choice;
        clearscreen();

        switch (choice) {
            case 1: {
                adminMenu(); // Enter the admin menu
                break;
            }
            case 2: {
                clearscreen();
                staffMenu(); // Enter the staff menu
                break;
            }
            case 3: {
                clearscreen();
                atmMenu(loggedIn, accountNumber); // Enter the ATM menu
                break;
            }
            case 4: {
                return 0; // Exit the program
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true); // Loop until the user chooses to exit the program

    return 0;
}
