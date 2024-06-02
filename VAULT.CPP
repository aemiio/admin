#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <fstream.h>
#include <conio.h>
#include <string.h>
#include <math.h>

const int MAX_USER = 30;
const int MAX_WORDS = 100;
const int PAGE_SIZE = 20;

struct Words {
    char word[100];
    char definition[100];
    int categories; // 1 - Changed Word, 0 - Acronyms
    char changed_word[100];
    char acronym[100];
    int approved; // 1 - approved, 0 - reject
} words[MAX_WORDS];

struct Users {
    char user_name[100];
    char user_pass[100];
    int is_admin; // 1 - admin, 0 - user
    int banned; //1 - banned
} users[MAX_USER];

struct Feedback {
    int feed_num;
    char feed[100];
} feedback1[50];

int word_count = 0;
int user_count = 0;
int feedback_count = 0;

int choice = 0, i = 0;
int index;

void load_words();

void save();

void view_words(int page);

void add_and_edit();

void categorize();

void approval();

void manage_users();

void backup();

void restore();

void admin_menu();

void user_menu();

void search_words();

void create_feedback();

void load_feedback();


//----- Login & Register

void login() {
    char name[100], pass[100];

    char admin[100] = "admin";
    char admin_pass[100] = "admin";

    int tries;
    int found = 0;

    do {
        clrscr();
        cout << "Username: ";
        gets(name);
        cout << "Password: ";
        gets(pass);

        if (strcmp(admin, name) == 0 && strcmp(admin_pass, pass) == 0) {
            cout << "Welcome Admin!";
            found = 1;
            admin_menu();
        } else {
            for (i = 0; i < user_count; i++) {
                if (strcmp(users[i].user_name, name) == 0 && strcmp(users[i].user_pass, pass) == 0) {
                    cout << "Welcome " << users[i].user_name;
                    index = i;
                    found = 1;
                    user_menu();
                    getch();
                } else {
                    cout << "Invalid login credentials!\n";
                    tries += 1;
                    getch();
                }
            }
        }

        if (tries >= 3) {
            users[i].banned = 1;
            continue;
        }
    } while (tries < 3);

    if (found == 0) {
        cout << "User not found!\n";

        getch();

    }
}

void register_user() {
    clrscr();
    cout << "Enter username: ";
    gets(users[user_count].user_name);
    cout << "Enter password: ";
    gets(users[user_count].user_pass);
    user_count++;
    users[user_count].is_admin = 0;
    users[user_count].banned = 0;
    save();
    user_menu();
    getch();
}

//-----

void display_menu() {
    clrscr();
    cout << "Main Menu" << endl;
    cout << "Login" << endl;
    cout << "Register" << endl;
    cout << "Exit" << endl;
    cout << "Enter Choice: " << endl;
}

void display_admin_menu() {
    clrscr();
    cout << "Admin Menu\n";
    cout << "1. Add and Edit Words\n";
    cout << "2. Categorize Words\n";
    cout << "3. Approval Queue\n";
    cout << "4. Feedback Management\n";
    cout << "5. Manage Users\n";
    cout << "6. Backup Data\n";
    cout << "7. Restore Data\n";
    cout << "8. Logout\n";
    cout << "9. Exit Program\n";
    cout << "Enter Choice: \n";
}

void display_user_menu() {
    clrscr();
    cout << "User Menu\n";
    cout << "1. Search for Gen Z Words\n";
    cout << "2. Create Feedback\n";
    cout << "3. Exit Program\n";
    cout << "Enter Choice: \n";
}

void main_menu() {
    clrscr();
    int continueLoop = 1;
    while (continueLoop == 1) {
        display_menu();
        cin >> choice;
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                register_user();
                break;
            case 4:
                exit(0);
            default:
                cout << "Invalid choice.\n";
                getch();
                break;

        }
    }
}

void admin_menu() {
    clrscr();
    int continueLoop = 1;
    while (continueLoop == 1) {
        display_admin_menu();
        cin >> choice;
        switch (choice) {
            case 1:
                add_and_edit();
                break;
            case 2:
                categorize();
                break;
            case 3:
                approval();
                break;
            case 4:
                load_feedback();
                break;
            case 5:
                manage_users();
                break;
            case 6:
                backup();
                break;
            case 7:
                restore();
                break;
            case 8:
                cout << "Logout" << endl;
                getch();
                login();
                break;
            case 9:
                exit(0);
            default:
                cout << "Invalid choice.\n";
                getch();
                break;
        }
    }
}

void user_menu() {
    clrscr();
    int continueLoop = 1;
    while (continueLoop == 1) {
        display_user_menu();
        cin >> choice;
        switch (choice) {
            case 1:
                search_words();
                break;
            case 2:
                create_feedback();
                return;
            case 3:
                exit(0);
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}

//-----admin functions
void view_words(int page) {
    int max_items_per_page = 10;
    int page_number = page;

    int start = page * PAGE_SIZE;
    int end = start + PAGE_SIZE;
    int total_pages = ceil((double) word_count / PAGE_SIZE); //convert to double & round off

    if (end > max_items_per_page) {
        end = max_items_per_page;
    }

    cout << "There are " << word_count << " books." << endl;

    cout << "Page " << page_number + 1 << " of " << total_pages << endl;

    for (i = start; i < end; i++) {
        if (words[i].approved == 1) {
            cout << "No." << endl;
            cout << "Word: " << words[i].word << endl;
            cout << "Definition: " << words[i].definition << endl;

            if (words[i].categories == 1) {
                cout << "Changed Word: " << words[i].changed_word << endl;
            } else {
                cout << "Acronym: " << words[i].acronym << endl;

                cout << endl;
            }

            cout << endl;
            char q;
            cout << "Press [N] for next page, [P] for previous page, [Q] to quit..." << endl;
            q = getch();

            switch (q) {
                case 'N':
                case 'n':
                    if (page_number + 1 < total_pages) {
                        view_words(page_number + 1);
                    } else {
                        view_words(page_number);
                    }
                    break;
                case 'P':
                case 'p':
                    if (page_number > 0) {
                        view_words(page_number - 1);
                    } else {
                        view_words(page_number);
                    }
                    break;
                case 'Q':
                case 'q':
                    return;
                default:
                    view_words(page_number);
                    break;
            }
        } else {
            cout << "No approved words.\n";
            getch();
        }
    }
}

void add_word() {
    clrscr();
    char new_word[100];


    if (word_count >= MAX_WORDS) {
        cout << "Max words reached!" << endl;
        return;
    }


    cout << "Enter Word: ";
    gets(new_word);

    for (i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, new_word) == 0) {
            cout << "Word already exists.\n";
            return;
        }
    }

    strcpy(words[i].word, new_word);

    cout << "Enter Definition: ";
    gets(words[word_count].definition);
    cout << "Categories: [1] Changed Words, [2] Acronym\n";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter changed words: " << endl;
        gets(words[word_count].changed_word);
        words[word_count].categories = 1;
    } else {
        cout << "Enter Acronym" << endl;
        gets(words[word_count].acronym);
        words[word_count].categories = 0;
    }

    word_count++;
    words[word_count].approved = 0;
    save();
    cout << "Word added to approval queue.\n";
    getch();
}

void edit_word() {
    clrscr();
    view_words(0);


    if (word_count == 0) {
        cout << "No words available to edit.\n";
        getch();
        return;
    }

    cout << "Enter Index to Edit" << endl;
    cin >> index;

    if (index >= word_count) {
        cout << "Index out of range.\n";
        getch();
        edit_word();
    }

    cout << "[1] Edit Definition" << endl
         << "[2] Edit Categories" << endl;
    cout << "Enter Choice: ";
    cin >> choice;

    char edit[100];
    switch (choice) {
        case 1:
            cout << "Enter new definition: ";
            gets(edit);
            strcpy(words[index].definition, edit);
            cout << "Definition Updated.\n";
            save();
            getch();
            edit_word();
            break;
        case 2:
            cout << "[1] Change Categories" << endl
                 << "[2] Change current changed word/acronym" << endl;
            cout << "Enter Choice: ";
            cin >> choice;

            if (choice == 1) {
                cout << "Current category is " << words[index].categories << endl;
                if (words[index].categories == 1) {
                    cout << "Change to Acronym category" << endl;
                    cout << "Enter new acronym: " << endl;
                    gets(edit);
                    strcpy(words[index].acronym, edit);
                    words[index].categories = 0;
                    save();
                    cout << "Word Updated\n";
                    getch();
                }
            } else {
                cout << "Current category is " << words[index].categories << endl;
                if (words[index].categories == 1) {
                    cout << "Enter new changed word: " << endl;
                    gets(edit);
                    strcpy(words[index].changed_word, edit);
                    save();
                    cout << "Word Updated\n";
                    getch();
                } else {
                    cout << "Enter new acronym: " << endl;
                    gets(edit);
                    strcpy(words[index].acronym, edit);
                    save();
                    cout << "Word Updated\n";
                    getch();
                }
            }
            break;
        default:
            cout << "Invalid Output\n";
            return;
    }
}

void add_and_edit() {
    clrscr();
    cout << "[1] Add Word" << endl;
    cout << "[2] Edit Word" << endl;
    cout << "Enter Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            add_word();
            break;
        case 2:
            edit_word();
            break;
        default:
            cout << "Invalid Input\n";
            return;
    }
}

void categorize() {
    clrscr();
    cout << "Categories: " << endl
         << "[1] Show Changed Words" << endl
         << "[2] Show Acronyms" << endl;
    cin >> choice;


    switch (choice) {
        case 1:
            cout << "Words in Changed Words Category" << endl;

            cout << "No. " << "\t\t" << "Word" << "\t\t" << "Changed Word" << endl;

            for (i = 0; i < word_count; i++) {
                cout << i << " - "
                     << words[i].word << " - "
                     << words[i].changed_word << endl;
            }
            getch();
            break;
        case 2:
            cout << "Words in Acronyms Category" << endl;

            cout << "No. " << "\t\t" << "Word" << "\t\t" << "Acronym" << endl;

            for (i = 0; i < word_count; i++) {
                cout << i << " - "
                     << words[i].word << " - "
                     << words[i].acronym << endl;
            }
            getch();
            break;
        default:
            cout << "Invalid Input\n";
            return;
    }
}

void approval() {
    clrscr();
    cout << "Words awaiting approval:\n";

    for (i = 0; i < word_count; i++) {
        if (words[i].approved == 0) {
            cout << i << " - "
                 << words[i].word << " - "
                 << words[i].definition << " - ";

            if (words[i].categories == 1) {
                cout << words[i].changed_word << endl;
            } else {
                cout << words[i].acronym << endl;
            }

        }
    }

    int index;
    cout << "Enter index of the word to approve or reject (-1 to cancel): ";
    cin >> index;

    if (index == -1) {
        cout << "Operation Canceled\n";
        return;
    }

    cout << "[1] Approve, [2] Reject" << endl;
    cin >> choice;

    if (choice == 1) {
        words[index].approved = 1;
        cout << "Word Approved.\n";
        getch();
    } else if (choice == 2) {
        words[index].approved = 0;

        for (i = index; i < word_count - 1; i++) {
            words[i] = words[i + 1];
        }
        word_count--;
        cout << "Word Rejected.\n";
    } else {
        cout << "Invalid Choice\n";
    }
    save();
    getch();
}

void load_feedback() {
    ifstream feed("feedback_vault.txt");

    if (!feed) {
        cout << "No feedback available\n";
        getch();
        return;
    }

    char line[500];
    while (feed.getline(line, 100)) {
        cout << line << endl;
        cout << "Press any key to go to Menu." << endl;
        getch();
        admin_menu();
    }
    feed.close();
}

void save_feedback() {
    ofstream save_feed("feedback_vault.txt");

    if (!save_feed) {
        cout << "Cannot save words.\n";
        return;
    }

    save_feed << feedback1[feedback_count].feed_num << " - "
              << feedback1[feedback_count].feed << " - ";
}

void manage_users() {
    clrscr();
    cout << "User Accounts: \n";

    for (i = 0; i < user_count; i++) {
        cout << i << " - "
             << users[i].user_name << " - "
             << users[i].is_admin << endl;
    }

    cout << endl;
    cout << "Promote (1), Demote (2), or Ban (3) user (or enter '4' to cancel): ";
    cin >> choice;

    if (choice == 4) {
        cout << "Operation canceled.\n";
        return;
    }

    int index;
    cout << "Enter the index of the user: ";
    cin >> index;

    if (index < 0 || index >= user_count) {
        cout << "Invalid user index.\n";
        return;
    }

    switch (choice) {
        case 1:
            users[index].is_admin = 1;
            save();
            cout << "User promoted.\n";
            break;
        case 2:
            users[index].is_admin = 0;
            save();
            cout << "User demoted.\n";
            break;
        case 3:
            users[index].banned = 1;
            cout << "User banned.\n";
            break;
        default:
            cout << "Invalid choice.\n";
            break;
    }
    getch();
}


void load_words() {
    ifstream load("words_vault.txt");

    if (!load) {
        cout << "No words to load." << endl;
        return;
    }

    char *token;
    char line[500];

    while (load.getline(line, 500) && word_count < MAX_WORDS) {
        token = strtok(line, " - ");
        strcpy(words[word_count].word, token);

        token = strtok(NULL, " - ");
        strcpy(words[word_count].definition, token);

        token = strtok(NULL, " - ");
        words[word_count].categories = atoi(token);

        token = strtok(NULL, " - ");
        strcpy(words[word_count].changed_word, token);

        token = strtok(NULL, " - ");
        strcpy(words[word_count].acronym, token);
    }
}

void save() {
    ofstream save("words_vault.txt");

    if (!save) {
        cout << "Cannot save words.\n";
        return;
    }

    save << words[word_count].word << " - "
         << words[word_count].definition << " - "
         << words[word_count].categories << " - "
         << words[word_count].changed_word << " - "
         << words[word_count].acronym << endl;
}

void load_users() {
    ifstream load_users("users_vault.txt");

    if (!load_users) {
        cout << "No users found!" << endl;
        return;
    }

    char *tok;
    char line[200];

    while (load_users.getline(line, 200) && user_count < MAX_USER) {
        tok = strtok(line, " - ");
        strcpy(users[user_count].user_name, tok);

        tok = strtok(NULL, " - ");
        strcpy(users[user_count].user_pass, tok);

        tok = strtok (line, " - ");
        users[user_count].is_admin = atoi(tok);

        tok = strtok (line, " - ");
        users[user_count].banned = atoi(tok);

    }
}

void save_users() {
    ofstream save("users_vault.txt");

    if (!save) {
        cout << "Cannot save user" << endl;
    }

    save << users[user_count].user_name << " - "
         << users[user_count].user_pass << " - "
         << users[user_count].is_admin << " - "
         << users[user_count].banned
         << endl;
}

void backup() {
    ifstream source("words_vault.txt");
    ofstream backup("words_vault_backup.txt");

    if (!source) {
        cout << "No source to backup.\n";
        return;
    }

    backup << source.rdbuf(); //copy contents ng source to backup file

    cout << "Backup created successfully." << endl;
}

void restore() {
    ifstream source("words_vault_backup.txt");
    ofstream restore("words_vault.txt");

    if (!source) {
        cout << "No backup to restore.\n";
        return;
    }

    restore << source.rdbuf();

    cout << "Restore Completed successfully.\n";
}

void main() {
    clrscr();
    load_words();
    load_users();
    cout << "Gen Z Lingo Vault " << endl;
    cout << "Press 1 to login or Press 2 to register";
    cin >> choice;

    if (choice == 1) {
        login();
    } else if (choice == 2) {
        register_user();
    } else {
        cout << "Invalid Input!" << endl;
        exit(0);
    }
    getch();
}

//---- user functions;

void search_words() {
    clrscr();
    char search[100];

    cout << "Search by: [1] Word, [2] Changed Word, or [3] Acronym" << endl;
    cin >> choice;
    cout << "Search: ";
    gets(search);

    switch (choice) {
        case 1:
            for (i = 0; i < word_count; i++) {
                if (strcmp(words[i].word, search) == 0) {
                    cout << i << " - "
                         << words[i].word << " - "
                         << words[i].definition << " - ";

                    if (words[i].categories == 1) {
                        cout << words[i].changed_word;
                    } else {
                        cout << words[i].acronym;
                    }
                } else {
                    cout << "Word not found.\n";
                    getch();
                }
            }
            getch();
            break;
        case 2:
            for (i = 0; i < word_count; i++) {
                if (strcmp(words[i].changed_word, search) == 0) {
                    cout << i << " - "
                         << words[i].word << " - "
                         << words[i].definition << " - ";

                    if (words[i].categories == 1) {
                        cout << words[i].changed_word;
                    }
                } else {
                    cout << "Changed Word not found.\n";
                    getch();
                }
            }
            break;
        case 3:
            for (i = 0; i < word_count; i++) {
                if (strcmp(words[i].acronym, search) == 0) {
                    cout << i << " - "
                         << words[i].word << " - "
                         << words[i].definition << " - ";

                    if (words[i].categories == 0) {
                        cout << words[i].acronym;
                    }
                } else {
                    cout << "Acronym not found.\n";
                    getch();
                }
            }
            break;
        default:
            cout << "Invalid Input.\n";
            return;
    }
}

void create_feedback() {
    clrscr();
    char feed[100];
    cout << "Create Feedback\n";
    cout << "Feedback: ";
    gets(feed);

    feedback1[feedback_count].feed_num += 1;
    feedback_count++;
    save_feedback();
    cout << "Feedback successful.\n";
    getch();
    user_menu();
}
