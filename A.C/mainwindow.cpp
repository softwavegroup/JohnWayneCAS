#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("management.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Can't open database: " + db.lastError().text());
        return;
    }

    executeSQL("CREATE TABLE IF NOT EXISTS Inventory (UPC TEXT PRIMARY KEY, Units INTEGER, ToOrder INTEGER);");
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::executeSQL(const QString &sql)
{
    QSqlQuery query;
    if (!query.exec(sql)) {
        QMessageBox::critical(this, "SQL error", query.lastError().text());
    }
}

void MainWindow::on_addUserButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add User", "Enter name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString role = QInputDialog::getText(this, "Add User", "Enter role:", QLineEdit::Normal, "", &ok);
    if (!ok || role.isEmpty()) return;

    // Add user to the UI list (not stored in the database in this example)
    ui->userListWidget->addItem("Name: " + name + ", Role: " + role);
}

void MainWindow::on_displayUsersButton_clicked()
{
    // Display users from the UI list (not stored in the database in this example)
    QMessageBox::information(this, "Users", "Users are displayed in the list.");
}

void MainWindow::on_addInventoryButton_clicked()
{
    bool ok;
    QString upc = QInputDialog::getText(this, "Add Inventory Item", "Enter UPC:", QLineEdit::Normal, "", &ok);
    if (!ok || upc.isEmpty()) return;

    int units = QInputDialog::getInt(this, "Add Inventory Item", "Enter Units:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    int toOrder = QInputDialog::getInt(this, "Add Inventory Item", "Enter To Order:", 0, 0, 10000, 1, &ok);
    if (!ok) return;

    QString sql = QString("INSERT INTO Inventory (UPC, Units, ToOrder) VALUES ('%1', %2, %3);")
                  .arg(upc).arg(units).arg(toOrder);
    executeSQL(sql);
}

void MainWindow::on_displayInventoryButton_clicked()
{
    QSqlQuery query("SELECT * FROM Inventory;");
    if (!query.exec()) {
        QMessageBox::critical(this, "SQL error", query.lastError().text());
        return;
    }

    ui->inventoryTableWidget->setRowCount(0);
    while (query.next()) {
        int row = ui->inventoryTableWidget->rowCount();
        ui->inventoryTableWidget->insertRow(row);
        ui->inventoryTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->inventoryTableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->inventoryTableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
    }
}