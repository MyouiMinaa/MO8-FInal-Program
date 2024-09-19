/* Program name: kpop_paradise.cpp
 *  Author: Ximena Carlos
 *  Date last updated: 9/20/2024
 * Purpose: Creating a C++ code that Kpop Paradise can use to conduct all functions that need to be done within the business. This code will create queries to input all necessary information into kpopparadise's database that can be used to create a report of all the information that the business has.
 */

#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

// all functions included in the database
int connectToDatabase(sqlite3 **db);
void addCustomer(sqlite3 *db);
void addProduct(sqlite3 *db);
void updateCustomer(sqlite3 *db);
void updateProduct(sqlite3 *db);
void deleteCustomer(sqlite3 *db);
void processTransaction(sqlite3 *db);
void displayCustomerOrders(sqlite3 *db);
void displayProductSuppliers(sqlite3 *db);
void displayMainMenu(sqlite3 *db);

int main() {
  sqlite3 *db;

  // Connecting to the database...
  if (connectToDatabase(&db) != SQLITE_OK) {
    return -1;
  }

  displayMainMenu(db);

  // database connection complete! add on message to display
  sqlite3_close(db);
  return 0;
}

int connectToDatabase(sqlite3 **db) {
  int rc = sqlite3_open("kpop_paradise.sql", db);
  if (rc) {
    cerr << "Can't open database: " << sqlite3_errmsg(*db) << endl;
    return rc;
  }
  return SQLITE_OK;
}

// menu /
void displayMainMenu(sqlite3 *db) {
  int choice;
  do {
    cout << "\nWelcome to K-Pop Paradise Database Management System" << endl;
    cout << "1. Create Customer Record" << endl;
    cout << "2. Update Customer Information" << endl;
    cout << "3. Remove CustomerRecord" << endl;
    cout << "4. Process Transaction" << endl;
    cout << "5. Generate Report Of The Day" << endl;
    cout << "6. Close Application" << endl;

    cout << "Select an option (1-6): ";
    cin >> choice;

    switch (choice) {
    case 1:
      // adding a customer into the records(typcially based on transaction being completed the keypad would say something along the lines of sign up today for $5 off your purchase!)
      int addChoice;
      cout << "Add Menu:\n1. Customer\n2. Product\n";
      cin >> addChoice;
      cin.ignore();
      if (addChoice == 1)
        addCustomer(db);
      else if (addChoice == 2)
        addProduct(db);
      break;

    case 2:
      // updadting customer information/change of phone number and such 
      int updateChoice;
      cout << "Update Menu:\n1. Customer\n2. Product\n";
      cin >> updateChoice;
      cin.ignore();
      if (updateChoice == 1)
        updateCustomer(db);
      else if (updateChoice == 2)
        updateProduct(db);
      break;

    case 3:
      // remove customer account
      deleteCustomer(db);
      break;

    case 4:
      // completing a sale
      processTransaction(db);
      break;

    case 5:
      // insights of the day
      displayCustomerOrders(db);
      displayProductSuppliers(db);
      break;

    case 6:
      cout << "Sailing away from your paradise.\n";
      break;

    default:
      cout << "ERROR! Invalid option.\n";
    }
  } while (choice != 6);
}

// Function to add a new customer record
void addCustomer(sqlite3 *db) {
  string name, email, phone;

  cout << "Enter Customer Name: ";
  cin.ignore(); 
  getline(cin, name);

  cout << "Enter Customer Email: ";
  getline(cin, email);

  cout << "Enter Customer Phone: ";
  getline(cin, phone);

  // SQL query being made to insert a new customer record
  string sql = "INSERT INTO Customers (Name, Email, Phone) VALUES ('" + name +
               "', '" + email + "', '" + phone + "');";

  char *errMsg = nullptr;
  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "ERROR!Cannot add customer because " << errMsg << endl;
    sqlite3_free(errMsg);
  } else {
    cout << "Customer added successfully." << endl;
  }
}

// add a new product information
void addProduct(sqlite3 *db) {
  string name, description;
  double price;
  int stock;

  cout << "Enter Product Name: ";
  cin.ignore();
  getline(cin, name);

  cout << "Enter Product Description: ";
  getline(cin, description);

  cout << "Enter Product Price: ";
  cin >> price;

  cout << "Enter Product Stock: ";
  cin >> stock;

  //  SQL query to insert a new product and all of its information 
  string sql =
      "INSERT INTO Products (Name, Prod_descrip, Price, Stock) VALUES ('" +
      name + "', '" + description + "', " + std::to_string(price) + ", " +
      std::to_string(stock) + ");";

  char *errMsg = nullptr;
  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "ERROR! Cannot add product because " << errMsg << endl;
    sqlite3_free(errMsg);
  } else {
    cout << "Product added successfully." << endl;
  }
}

// modifying an existing customer (perhaps joining a new fan club)
void updateCustomer(sqlite3 *db) {
  int customerId;
  string name, email, phone;

  cout << "Enter Customer ID to update: ";
  cin >> customerId;
  cin.ignore();

  cout << "Enter new Customer Name: ";
  getline(cin, name);

  cout << "Enter new Customer Email: ";
  getline(cin, email);

  cout << "Enter new Customer Phone: ";
  getline(cin, phone);

  // SQL query to update the customer's details
  string sql = "UPDATE Customers SET Name = '" + name + "', Email = '" + email +
               "', Phone = '" + phone +
               "' WHERE CustomerID = " + std::to_string(customerId) + ";";

  char *errMsg = nullptr;
  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "ERROR! Cannot update customer because " << errMsg << endl;
    sqlite3_free(errMsg);
  } else {
    cout << "Customer updated successfully." << endl;
  }
}

// modifying a product's information
void updateProduct(sqlite3 *db) {
  int productId;
  string name, description;
  double price;
  int stock;

  cout << "Enter Product ID to update: ";
  cin >> productId;
  cin.ignore();

  cout << "Enter new Product Name: ";
  getline(cin, name);

  cout << "Enter new Product Description: ";
  getline(cin, description);

  cout << "Enter new Product Price: ";
  cin >> price;

  cout << "Enter new Product Stock: ";
  cin >> stock;

  //SQL query to update the product's details
  string sql = "UPDATE Products SET Name = '" + name + "', Prod_descrip = '" +
               description + "', Price = " + std::to_string(price) +
               ", Stock = " + std::to_string(stock) +
               " WHERE ProductID = " + std::to_string(productId) + ";";

  char *errMsg = nullptr;

  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "ERROR! cannot update product because" << errMsg << endl;
    sqlite3_free(errMsg);
  } else {
    cout << "Product updated successfully!" << endl;
  }
}

// deleting customer ID
void deleteCustomer(sqlite3 *db) {
  int customerId;

  cout << "Enter Customer ID to delete:";

  cin >> customerId;

  // SQL query to delete the customer ID
  string sql =
      "DELETE FROM Customers WHERE CustomerID = " + std::to_string(customerId) +
      ";";

  char *errMsg = nullptr;

  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
    cerr << "ERROR! Could not delete customer because" << errMsg << endl;
    sqlite3_free(errMsg);
  } else {
    cout << "Customer with ID " << customerId << " deleted successfully!"
         << endl;
  }
}

// processing transactions 
void processTransaction(sqlite3 *db) {
    
  

  cout << "Processing transaction... "
       << endl;
}

// Display employee orders using JOINs (they are customers of the suppliers)
void displayCustomerOrders(sqlite3 *db) {
  string sql =
      "SELECT Customers.Name AS CustomerName ,Orders.OrderID ,Orders.Quantity "
      "FROM Orders JOIN Customers ON Orders.CustomerID=Customers.CustomerID ;";

  sqlite3_stmt *stmt;


  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << "ERROR! Cannot prepare statement because " << sqlite3_errmsg(db) << endl;
    return;
  }

  // Fetch and display results of customer orders
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char *customerName = sqlite3_column_text(stmt, 0);
    int orderID = sqlite3_column_int(stmt, 1);
    int quantity = sqlite3_column_int(stmt, 2);

    printf("Customer Name:%s ,Order ID:%d ,Quantity:%d\n", customerName,
           orderID, quantity);
  }

  // make sure everything is organized before finalizing
  sqlite3_finalize(stmt);
}

// Display products and their suppliers - insights use
void displayProductSuppliers(sqlite3 *db) {
  string sql = “SELECT Products.Name AS ProductName,
         Suppliers
             .supplierInfo AS SupplierInfo FROM Products JOIN Suppliers ON
                 Products.supplierID = Suppliers.supplierID;
  " ;

      sqlite3_stmt *stmt;


  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << "ERROR! Cannot prepare statement because " << sqlite3_errmsg(db) << endl;
    return;
  }


  while (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char *productName = sqlite3_column_text(stmt, 0);
    const unsigned char *supplierInfo = sqlite3_column_text(stmt, 1);

    printf("Product Name: %s | Supplier Info: %s\n", productName, supplierInfo);
  }


  sqlite3_finalize(stmt);
}
