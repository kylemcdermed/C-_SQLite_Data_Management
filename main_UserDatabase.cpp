#include <iostream>
#include <stdio.h>
#include <iomanip>
#include "sqlite3.h"

using namespace std;

sqlite3* db;
sqlite3_stmt* stmt;
int result, roll;
string query, name, email;

void connection();
void insertR();
void retrieve();
void deleteR();
void update();

int main() {

    // Call connection function
    connection();
    int menu;
    cout << "\n-----------------------------\n";
    cout << "Press [1] to insert new Data\n";
    cout << "Press [2] to read all Data\n";
    cout << "Press [3] to update a record\n";
    cout << "Press [4] to delete a record\n";
    cout << "Press [5] to exit";
    cout << "\n-----------------------------\n";
    cin  >> menu;
    getchar();
    if (menu == 1) {
        insertR();
        main();
    } else if (menu == 2){
        retrieve();
        main();
    }
    else if (menu == 3){
        update();
        main();
    } else if (menu == 4){
        deleteR();
        main();
    } else if (menu == 5){
        return 0;
    } else {
        cout << "Invalid command\n";
    }
    //insertR();
    //retrieve();
    sqlite3_close(db);
    return 0;

}

void connection() {

    if (sqlite3_open("demo.db", &db) == SQLITE_OK) {
        result = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS user(name VARCHAR(50), roll INT, email VARCHAR(80));", -1, &stmt, NULL);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (result != SQLITE_OK) {
            cout << "Error: " << sqlite3_errmsg(db) << '\n';
        }
    }

}

void insertR() {

    cout << "\n-> Enter your name: \n";
    getline(cin, name);
    cout << "\n-> Enter your roll: \n";
    cin >> roll;
    getchar();
    cout << "\n-> Enter your email: \n";
    getline(cin, email);

    string query = "INSERT INTO user(name, roll, email) VALUES (?, ?, ?);";

    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, roll);
    sqlite3_bind_text(stmt, 3, email.c_str(), email.length(), SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        cout << "\n-> Error: " << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "\n-> Data inserted successfully \n";
    }

}

void retrieve() {

    //query = "SELECT name, roll, email FROM user";
    query = "SELECT rowid, * FROM user";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cout << "Error: " << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "\n----------------------------------------------------------------------------------\n";
        cout << left << "\t" << setw(15) << "ID" <<
        setw(25) << " Name" <<
        setw(20) << " Roll" <<
        setw(25) << " Email\n";
        cout << "\n----------------------------------------------------------------------------------\n";
        while ( (result = sqlite3_step(stmt)) == SQLITE_ROW ) {
            cout << left << "\t" << setw(15) << sqlite3_column_int(stmt, 0)
                         << setw(25) << sqlite3_column_text(stmt, 1)
                         << setw(20) << sqlite3_column_int(stmt, 2)
                         << setw(25) << sqlite3_column_text(stmt, 3) << "\n";
        }
        cout << "\n----------------------------------------------------------------------------------\n";
    }

}

void deleteR() {

    int id;
    char confirm;
    cout << "\nWhich record do you want to delete? Type [id] number: \n";
    cin >> id;
    getchar();
    cout << "\nAre you sure? Type [y] for Yes, and [n] for No: \n";
    cin >> confirm;
    getchar();

    if (confirm == 'y') {
        query = "DELETE FROM user WHERE rowid=?";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (result == SQLITE_OK) {
            cout << "Data deleted successfully\n";
        } else {
            cout << "\nFailed: " << sqlite3_errmsg(db) << '\n';
        }
    } else {
        cout << "\nDelete operation aborted successfully\n";
    }

}

void update() {

    int id;
    cout << "\nWhich record do you want to update? Type [id] number: \n";
    cin >> id;
    getchar();
    cout << "Type new name: \n";
    getline(cin, name);
    cout << "Type new roll: \n";
    cin >> roll;
    getchar();
    cout << "Type new email: \n";
    getline(cin, email);

    query = "UPDATE user SET name=?, roll=?, email=? WHERE rowid=?;";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, roll);
    sqlite3_bind_text(stmt, 3, email.c_str(), email.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
        cout << "\nFailed: " << sqlite3_errmsg(db) << '\n';
    } else {
        cout << "\nData updated successfully\n";
    }
}
