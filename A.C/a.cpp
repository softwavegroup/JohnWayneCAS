#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>

class User {
public:
    std::string name;
    std::string role;
    
    User(std::string n, std::string r) : name(n), role(r) {}
    
    void display() const {
        std::cout << "Name: " << name << ", Role: " << role << std::endl;
    }
};

class ManagementSystem {
private:
    std::vector<User> users;
    sqlite3* db;

    void executeSQL(const std::string& sql) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }

public:
    ManagementSystem() {
        int rc = sqlite3_open("management.db", &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        executeSQL("CREATE TABLE IF NOT EXISTS Inventory (UPC TEXT PRIMARY KEY, Units INTEGER, ToOrder INTEGER);");
    }

    ~ManagementSystem() {
        sqlite3_close(db);
    }

    void addUser(const std::string& name, const std::string& role) {
        users.emplace_back(name, role);
    }
    
    void displayUsers() const {
        for (const auto& user : users) {
            user.display();
        }
    }

    void addInventoryItem(const std::string& upc, int units, int toOrder) {
        std::string sql = "INSERT INTO Inventory (UPC, Units, ToOrder) VALUES ('" + upc + "', " + std::to_string(units) + ", " + std::to_string(toOrder) + ");";
        executeSQL(sql);
    }

    void displayInventory() const {
        std::string sql = "SELECT * FROM Inventory;";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        std::cout << "UPC\tUnits\tTo Order\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string upc = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int units = sqlite3_column_int(stmt, 1);
            int toOrder = sqlite3_column_int(stmt, 2);
            std::cout << upc << "\t" << units << "\t" << toOrder << "\n";
        }
        sqlite3_finalize(stmt);
    }

    void menu() {
        int choice;
        do {
            std::cout << "\n1. Add User\n2. Display Users\n3. Add Inventory Item\n4. Display Inventory\n5. Exit\nEnter your choice: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    std::string name, role;
                    std::cout << "Enter name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter role: ";
                    std::getline(std::cin, role);
                    addUser(name, role);
                    break;
                }
                case 2:
                    displayUsers();
                    break;
                case 3: {
                    std::string upc;
                    int units, toOrder;
                    std::cout << "Enter UPC: ";
                    std::getline(std::cin, upc);
                    std::cout << "Enter Units: ";
                    std::cin >> units;
                    std::cout << "Enter To Order: ";
                    std::cin >> toOrder;
                    std::cin.ignore();
                    addInventoryItem(upc, units, toOrder);
                    break;
                }
                case 4:
                    displayInventory();
                    break;
                case 5:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 5);
    }
};

int main() {
    ManagementSystem system;
    system.menu();
    return 0;
}
