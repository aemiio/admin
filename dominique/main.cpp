#include <iostream.h>
#include <iomanip.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>

// CONSTANTS
const int MAX_LENGTH = 100;
const int MAX_USERS = 20;
const int MAX_ENTRIES = 100;

// struct Feedback {
//     char content[MAX_LENGTH];
//     char username[MAX_LENGTH];

// };
// Feedback feedbacks[100]; 
// int feedbackCount = 0;

// STRUCTS
struct WordEntry {
    char word[MAX_LENGTH];
    char definition[MAX_LENGTH];
    char changedWord[MAX_LENGTH];
    char acronym[MAX_LENGTH];
    int approved;
};

struct UserAccount {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    int isAdmin; // Using int instead of bool
    int banned;
};

WordEntry entries[MAX_ENTRIES]; // Now using the struct
int wordCount = 0;

UserAccount users[MAX_USERS]; // Array of UserAccount structs
int userCount = 0;

// FUNCTIONS
void loadDatabase();

void saveDatabase();

void loadUsers();

void saveUsers();

void mainMenu();

void adminMenu();

void userMenu();

void adminLogin();

void userLogin();

void registerUser();

void addAndEdit();

int categorize();

void approval();

// void writeFeedback();
// void readFeedback();
// void deleteFeedback(int);
void manageUsers();

void backup();

void restore();

void search_entries();

// global variables
int choice = 0, i = 0;

// MAIN
void main() {
    clrscr();
    restore();
    title();
}

// DATABASE
void loadDatabase() {
    ifstream inFile("DATABASE.TXT");
    if (!inFile) {
        cout << "Unable to open file for reading.\n";
        return;
    }
    wordCount = 0;
    char line[MAX_LENGTH];
    while (inFile.getline(line, MAX_LENGTH) && !inFile.eof()) {
        char *token = strtok(line, ":"); // Tokenize the line by colon
        strcpy(entries[wordCount].word, token);

        token = strtok(NULL, ":");
        strcpy(entries[wordCount].definition, token);

        token = strtok(NULL, ":");
        strcpy(entries[wordCount].changedWord, token);

        token = strtok(NULL, ":");
        strcpy(entries[wordCount].acronym, token);

        token = strtok(NULL, ":");
        entries[wordCount].approved = atoi(token);
        wordCount++;
    }

    inFile.close();
}

void saveDatabase() {
    ofstream outFile("DATABASE.TXT");
    if (!outFile) {
        cout << "Unable to open file for writing.\n";
        return;
    }

    for (int i = 0; i < wordCount; i++) {
        outFile << entries[i].word << ":"
                << entries[i].definition << ":"
                << entries[i].changedWord << ":"
                << entries[i].acronym << ":"
                << entries[i].approved << "\n";
    }

    outFile.close();
}

void loadUsers() {
    ifstream loadUsers("USERS.TXT");
    userCount = 0;
    while (loadUsers >> users[userCount].username) {
        loadUsers >> users[userCount].password;
        loadUsers >> users[userCount].isAdmin;
        loadUsers >> users[userCount].banned;
        userCount++;
    }
    loadUsers.close();
}

void saveUsers() {
    ofstream saveUsers("USERS.TXT");
    for (i = 0; i < userCount; i++) {
        saveUsers << users[i].username << " " << users[i].password << " " << users[i].isAdmin << " " << users[i].banned
                  << "\n";
    }
    saveUsers.close();
}
// void saveFeedback() {
//     ofstream outFile("feedback.txt");
//     if (!outFile) {
//         cout << "Unable to open file for writing.\n";
//         return;
//     }

//     for (int i = 0; i < feedbackCount; i++) {
//         outFile << feedbacks[i].username << ":" << feedbacks[i].content << "\n";
//     }

//     outFile.close();
// }

// void loadFeedback() {
//     ifstream inFile("feedback.txt");
//     if (!inFile) {
//         cout << "Unable to open file for reading.\n";
//         return;
//     }

//     feedbackCount = 0; // Reset feedback count
//     char line[200]; // Buffer to hold each line
//     while (inFile.getline(line, sizeof(line)) &&!inFile.eof()) {
//         char* token = strtok(line, ":"); // Tokenize the line by colon
//         if (token!= NULL) {
//             strcpy(feedbacks[feedbackCount].username, token);
//         }

//         token = strtok(NULL, ":"); // Get the next token
//         if (token!= NULL) {
//             strcpy(feedbacks[feedbackCount].content, token);
//         }

//         feedbackCount++;
//     }

//     inFile.close();
// }


//
void displayMainMenu() {
    clrscr();
    cout << "Main Menu\n";
    cout << "1. Admin Login\n";
    cout << "2. User Login\n";
    cout << "3. Register\n";
    cout << "4. Exit\n";
    cout << "Enter Choice: \n";
}

void displayAdminMenu() {
    clrscr();
    cout << "Admin Menu\n";
    cout << "1. Dictionary\n";
    cout << "2. Approval Queue\n";
    // cout << "3. Feedback Management\n";
    cout << "3. Manage Users\n";
    cout << "4. Backup Data\n";
    cout << "5. Restore Data\n";
    cout << "6. Logout\n";
    cout << "7. Exit Program\n";
    cout << "Enter Choice: \n";
}

void displayUserMenu() {
    clrscr();
    cout << "User Menu\n";
    cout << "1. Search for Gen Z entries\n";
    cout << "2. Dictionary\n";
    // cout << "3.Write Feedback\n";
    cout << "3. Logout\n";
    cout << "4. Exit Program\n";
    cout << "Enter Choice: \n";
}

void displayDict(int page, int perPage, int category = 4) {

    int start;
    int pages;
    int end;

    clrscr();
    start = perPage * page;
    pages = (wordCount / perPage) + 1;
    end = start + perPage;
    end > wordCount ? end = wordCount : end = end;
    cout << "Page: " << page + 1 << "/" << pages << endl;
    cout << "###########################################################################" << endl;
    cout << setw(3) << "id" << setw(15) << "Word" << setw(15) << "GenZ" << setw(15) << "Acronym" << endl;
    for (start; start < end; start++) {
        if (entries[start].approved) {
            switch (category) {
                case 1:
                    if (strcmp(entries[start].acronym, " ") == 0)
                        cout << setw(3) << start + 1 << setw(15) << entries[start].word << setw(15)
                             << entries[start].changedWord << endl;
                    break;
                case 2:
                    if (strcmp(entries[start].changedWord, " ") == 0)
                        cout << setw(3) << start + 1 << setw(15) << entries[start].word << setw(15)
                             << entries[start].acronym << endl;
                    break;
                case 3:
                    if (strcmp(entries[start].acronym, " ") != 0 && strcmp(entries[start].changedWord, " ") != 0)
                        cout << setw(3) << start + 1 << setw(15) << entries[start].word << setw(15)
                             << entries[start].changedWord << setw(15) << entries[start].acronym << endl;
                    break;
                default:
                    cout << setw(3) << start + 1 << setw(15) << entries[start].word << setw(15)
                         << entries[start].changedWord << setw(15) << entries[start].acronym << endl;
                    break;
            }
        }
    }
    cout << "###########################################################################" << endl;
}

void dict() {
    int page = 0;
    int perPage = 10;
    int cat;
    do {
        displayDict(page, perPage, cat);
        cout << "1.Previous" << endl;
        cout << "2.Next" << endl;
        cout << "3.Definition" << endl;
        cout << "4.Add Word" << endl;
        cout << "5.Per Page" << endl;
        cout << "6.Category" << endl;
        cout << "7.Exit" << endl;
        cout << ": ";

        cin >> choice;
        switch (choice) {
            case 1:
                if (page > 0)
                    page--; // Move to previous page
                break;
            case 2:
                if (page < ((wordCount / perPage) + 1) - 1)
                    page++; // Move to next page
                break;
            case 3:
                cout << "Input Id to Get Definition of the word" << endl;
                cin >> choice;
                clrscr();
                cout << setw(10) << "Word" << "\tDefinition" << endl;
                cout << setw(10) << entries[choice - 1].word << "\t" << entries[choice - 1].definition << endl;
                getch();
                break;
            case 4:
                clrscr();
                if (wordCount >= MAX_ENTRIES) {
                    cout << "Database is full.\n";
                    return;
                }
                cout << "Choose Category\n";
                cout << "1.Changed entries" << endl;
                cout << "2.Acronym" << endl;
                cout << "3.Both" << endl;
                cin >> choice;

                WordEntry newEntry; // Create a new WordEntry struct
                cout << "Enter word: ";
                cin >> newEntry.word;
                cout << "Enter definition: ";
                cin.ignore();
                cin.getline(newEntry.definition, MAX_LENGTH);
                switch (choice) {
                    case 1:
                        cout << "Enter the changed word: ";
                        cin.getline(newEntry.changedWord, MAX_LENGTH);
                        strcpy(newEntry.acronym, " ");
                        break;
                    case 2:
                        strcpy(newEntry.changedWord, " ");
                        cout << "Enter the acronym: ";
                        cin.getline(newEntry.acronym, MAX_LENGTH);
                        break;
                    case 3:
                        cout << "Enter the changed word: ";
                        cin.getline(newEntry.changedWord, MAX_LENGTH);
                        cout << "Enter the acronym: ";
                        cin.getline(newEntry.acronym, MAX_LENGTH);
                        break;
                }
                newEntry.approved = 0;
                entries[wordCount++] = newEntry;
                cout << "Word added to approval queue.\n";
                getch();
                break;
            case 5:
                cout << "Input Per Page" << endl;
                cin >> perPage;
                break;
            case 6:
                cat = categorize();
                break;
        }

    } while (choice != 7);
}

void title() {
    clrscr();
    cout << " " << endl;
    cout << "GEN Z LINGO VAULT" << endl;
    cout << " " << endl;
    cout << "Press any key to continue... " << endl;
    getch();
    mainMenu();
}

// MENU
void mainMenu() {
    clrscr();
    int continueLoop = 1;
    while (continueLoop == 1) {
        displayMainMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                userLogin();
                break;
            case 3:
                registerUser();
                break;
            case 4:
                backup();
                exit(0);
            default:
                cout << "Invalid choice.\n";
                getch();
                break;
        }
    }
}

// incomplete
void adminMenu() {
    clrscr();
    int continueLoop = 1;
    while (continueLoop == 1) {
        displayAdminMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                addAndEdit();
                break;
            case 2:
                approval();
                break;
                // case 3:
                //     readFeedback();
                //     break;
            case 3:
                manageUsers();
                break;
            case 4:
                clrscr();
                backup();
                cout << "Backup Success";
                getch();
                break;
            case 5:
                clrscr();
                restore();
                cout << "Restore Success";
                getch();
                break;
            case 6:
                mainMenu();
                break;
            case 7:
                backup();
                exit(0);
            default:
                cout << "Invalid choice.\n";
                getch();
                break;
        }
    }
}

// incomplete
void userMenu() {
    clrscr();
    int continueLoop = 1;
    while (continueLoop == 1) {
        displayUserMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                search_entries();
                break;
            case 2:
                dict();
                break;
                // case 3:
                //     writeFeedback();
                //     break;
            case 3:
                return;
            case 4:
                backup();
                exit(0);
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
    getch();
}

// LOGIN & REGISTER
void adminLogin() {
    clrscr();
    char username[MAX_LENGTH], password[MAX_LENGTH];
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0 && users[i].isAdmin &&
            !users[i].banned) {
            adminMenu();
            return;
        }
    }
    cout << "Invalid login credentials or not an admin.\n";
    getch();
    mainMenu();
}

void userLogin() {
    clrscr();
    char username[MAX_LENGTH], password[MAX_LENGTH];
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0 && !users[i].banned) {
            userMenu();
            return;
        }
    }
    cout << "Invalid login credentials.\n";
    getch();
    mainMenu();
}

void registerUser() {
    clrscr();
    if (userCount >= MAX_USERS) {
        cout << "Maximum number of users reached.\n";
        return;
    }
    char newUsername[MAX_LENGTH], newPassword[MAX_LENGTH];
    int usernameExists = 0; // Simulate boolean with int

    cout << "Enter new username: ";
    cin >> newUsername;
    cin.ignore(256, '\n'); // Clear input buffer

    for (int userIndex = 0; userIndex < userCount; userIndex++) {
        if (strcmp(users[userIndex].username, newUsername) == 0) {
            usernameExists = 1; // Set to true
            break;
        }
    }

    if (usernameExists) {
        cout << "Username already exists.\n";
        getch();
        return;
    }

    cout << "Enter new password: ";
    cin >> newPassword;
    cin.ignore(256, '\n');

    strcpy(users[userCount].username, newUsername);
    strcpy(users[userCount].password, newPassword);
    users[userCount].isAdmin = 0; // Regular user account
    users[userCount].banned = 0;
    userCount++;
    cout << "Registration successful.\n";
    getch();
}

// ADMIN FUNCTIONS
void addAndEdit() {
    clrscr();
    int page = 0;
    int perPage = 10;
    int cat;
    do {
        displayDict(page, perPage, cat);
        cout << "1.Previous" << endl;
        cout << "2.Next" << endl;
        cout << "3.Per Page" << endl;
        cout << "4.Add Word" << endl;
        cout << "5.Edit Word" << endl;
        cout << "6.Category" << endl;
        cout << "7.Exit" << endl;
        cout << ": ";
        cin >> choice;
        switch (choice) {
            case 1:
                if (page > 0)
                    page--; // Move to previous page
                break;
            case 2:
                if (page < ((wordCount / perPage) + 1) - 1)
                    page++; // Move to next page
                break;
            case 3:
                cout << "Input Per Page" << endl;
                cin >> perPage;
                break;
            case 4:
                clrscr();
                if (wordCount >= max_entries) {
                    cout << "Database is full.\n";
                    return;
                }
                cout << "Choose Category\n";
                cout << "1.Changed entries" << endl;
                cout << "2.Acronym" << endl;
                cout << "3.Both" << endl;
                cin >> choice;

                WordEntry newEntry;
                cout << "Enter word: ";
                cin >> newEntry.word;
                cout << "Enter definition: ";
                cin.ignore();
                cin.getline(newEntry.definition, MAX_LENGTH);
                switch (choice) {
                    case 1:
                        cout << "Enter the changed word: ";
                        cin.getline(newEntry.changedWord, MAX_LENGTH);
                        strcpy(newEntry.acronym, " ");
                        break;
                    case 2:
                        strcpy(newEntry.changedWord, " ");
                        cout << "Enter the acronym: ";
                        cin.getline(newEntry.acronym, MAX_LENGTH);
                        break;
                    case 3:
                        cout << "Enter the changed word: ";
                        cin.getline(newEntry.changedWord, MAX_LENGTH);
                        cout << "Enter the acronym: ";
                        cin.getline(newEntry.acronym, MAX_LENGTH);
                        break;
                }

                newEntry.approved = 1;
                entries[wordCount++] = newEntry;
                cout << "Word added to approval queue.\n";
                getch();
                break;
            case 5:
                char wordToEdit[MAX_LENGTH];
                cout << "Enter word to edit: ";
                cin >> wordToEdit;
                cin.ignore();

                for (int index = 0; index < wordCount; index++) {
                    if (strcmp(entries[index].word, wordToEdit) == 0) {
                        clrscr();
                        cout << "Enter new definition: ";
                        cin.getline(entries[index].definition, MAX_LENGTH);
                        cout << "Enter the new changed word: ";
                        cin.getline(entries[index].changedWord, MAX_LENGTH);
                        cout << "Enter the new acronym: ";
                        cin.getline(entries[index].acronym, MAX_LENGTH);
                        cout << "Entry updated.\n";
                        getch();
                        return;
                    }
                }
                cout << "Word not found.\n";
                getch();
                break;

            case 6:
                cat = categorize();
                break;
        }
    } while (choice != 7);
    adminMenu();
}

int categorize() {
    int opt;
    clrscr();
    cout << "Categories:\n";
    cout << "1.GenZ Word" << endl;
    cout << "2.Acronym" << endl;
    cout << "3.Both" << endl;
    cout << ": ";
    cin >> opt;
    return opt;
}

void approval() {
    clrscr();
    cout << "entries awaiting approval:\n";
    for (int i = 0; i < wordCount; i++) {
        if (!entries[i].approved) {
            cout << i << ". " << entries[i].word << " - " << entries[i].definition << " - " << entries[i].changedWord
                 << " - " << entries[i].acronym << "\n";
        }
    }

    int index;
    cout << "Enter the index of the word to approve or reject (-1 to go back): ";
    cin >> index;
    index;
    if (index == -1) {
        return;
    }
    if (index < 0 || index >= wordCount) {
        cout << "Invalid index.\n";
        getch();
        return;
    }

    cout << "Approve (1) or Reject (2): ";
    cin >> choice;

    if (choice == 1) {
        entries[index].approved = 1;
        cout << "Word approved.\n";
    } else if (choice == 2) {
        // Option to remove the word from the database
        // This is a simplistic approach; consider implementing a more sophisticated removal strategy
        for (int i = index; i < wordCount - 1; ++i) {
            entries[i] = entries[i + 1];
        }
        wordCount--;
        cout << "Word rejected.\n";
    } else {
        cout << "Invalid choice.\n";
    }
    getch();
    approval();
}
// incomplete need user to create feedback
// void writeFeedback() {
//     if (feedbackCount >= 100) {
//         cout << "Feedback array is full. Cannot accept more feedback.\n";
//         return;
//     }

//     cout << "Enter feedback content: ";
//     cin.getline(feedbacks[feedbackCount].content, MAX_LENGTH);

//     cout << "Enter your username: ";
//     cin.getline(feedbacks[feedbackCount].username, MAX_LENGTH);

//     feedbackCount++; // Increment the count of feedback entries
//     cout << "Feedback successfully submitted.\n";
// }
// void readFeedback() {
//     int ind;
//     for (int i = 0; i < feedbackCount; i++) {
//         cout << i<<". Feedback from " << feedbacks[i].username << ": " << feedbacks[i].content << endl;
//     }
//     cout << "Choose index to delete: " ;
//     cin>>ind;
//     deleteFeedback(ind);
//     getch();
// }
// void deleteFeedback(int index) {
//     if (index < 0 && index < feedbackCount) {
//         for (int i = index; i < feedbackCount - 1; i++) {
//             feedbacks[i] = feedbacks[i + 1];
//         }
//         feedbackCount--;
//     } else {
//         cout << "Invalid index.\n";
//     }
// }
// complete??
void manageUsers() {
    clrscr();
    cout << "User Accounts:\n";
    cout << setw(15) << "Username" << setw(10) << "Position" << setw(10) << "Status" << endl;
    for (i = 0; i < userCount; ++i) {
        cout << i << setw(14) << users[i].username;
        users[i].isAdmin ? cout << setw(10) << "Admin" : cout << setw(10) << "User";
        users[i].banned ? cout << setw(10) << "Banned" : cout << setw(10) << "";
        cout << "\n";
    }
    getch();

    cout << "Promote (1), Demote (2), or Ban/Unbanned (3) user (or enter '4' to cancel): ";
    cin >> choice;

    if (choice == 4) {
        cout << "Operation canceled.\n";
        return;
    }

    int index;
    cout << "Enter the index of the user: ";
    cin >> index;

    if (index < 0 || index >= userCount) {
        cout << "Invalid user index.\n";
        return;
    }

    switch (choice) {
        case 1:
            users[index].isAdmin = 1;
            cout << "User promoted.\n";
            break;
        case 2:
            users[index].isAdmin = 0;
            cout << "User demoted.\n";
            break;
        case 3:
            users[index].banned = !users[index].banned;
            users[index].banned ? cout << "User banned.\n" : cout << "User Unbanned.";
            break;
        default:
            cout << "Invalid choice.\n";
            break;
    }
    getch();
}


void backup() {
    // saveFeedback();
    saveDatabase();
    saveUsers();
}

void restore() {
    // loadFeedback();
    loadDatabase();
    loadUsers();
}

void search_entries() {
    clrscr();
    char search[100];
    int found = 1;

    cout << "Search by: [1] Word, [2] Changed Word, or [3] Acronym" << endl;
    cout << ": ";
    cin >> choice;
    cout << "Search: ";
    gets(search);

    cout << setw(15) << "Word" << setw(15) << "GenZ" << setw(15) << "Acronym" << endl;
    switch (choice) {
        case 1:
            for (i = 0; i < wordCount; i++) {
                if (strcmp(entries[i].word, search) == 0) {
                    cout << setw(15) << entries[i].word;
                    if (strcmp(entries[i].changedWord, " ") != 0 && strcmp(entries[i].acronym, " ") != 0) {
                        cout << setw(15) << entries[i].changedWord << setw(15) << entries[i].acronym;
                    } else if (!strcmp(entries[i].changedWord, " ") == 0) {
                        cout << setw(15) << entries[i].changedWord;
                    } else if (!strcmp(entries[i].acronym, " ") == 0) {
                        cout << setw(30) << entries[i].acronym;
                    }

                    cout << endl;
                    found = 0;
                }
            }
            if (found) {
                cout << "Word not found.\n";
                getch();
            }
            getch();
            break;
        case 2:
            for (i = 0; i < wordCount; i++) {
                if (strcmp(entries[i].changedWord, search) == 0) {
                    cout << setw(15) << entries[i].word;
                    if (strcmp(entries[i].changedWord, " ") != 0 && strcmp(entries[i].acronym, " ") != 0) {
                        cout << setw(15) << entries[i].changedWord << setw(15) << entries[i].acronym;
                    } else if (!strcmp(entries[i].changedWord, " ") == 0) {
                        cout << setw(15) << entries[i].changedWord;
                    } else if (!strcmp(entries[i].acronym, " ") == 0) {
                        cout << setw(30) << entries[i].acronym;
                    }

                    cout << endl;
                    found = 0;
                }
            }
            if (found) {
                cout << "Word not found.\n";
                getch();
            }
            getch();
            break;
        case 3:
            for (i = 0; i < wordCount; i++) {
                if (strcmp(entries[i].acronym, search) == 0) {
                    cout << setw(15) << entries[i].word;
                    if (strcmp(entries[i].changedWord, " ") != 0 && strcmp(entries[i].acronym, " ") != 0) {
                        cout << setw(15) << entries[i].changedWord << setw(15) << entries[i].acronym;
                    } else if (!strcmp(entries[i].changedWord, " ") == 0) {
                        cout << setw(15) << entries[i].changedWord;
                    } else if (!strcmp(entries[i].acronym, " ") == 0) {
                        cout << setw(30) << entries[i].acronym;
                    }

                    cout << endl;
                    found = 0;
                }
            }
            if (found) {
                cout << "Word not found.\n";
                getch();
            }
            getch();
            break;
        default:
            cout << "Invalid Input.\n";
            return;
    }
}
