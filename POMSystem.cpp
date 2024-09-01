 #include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>
#include <string>
#include <regex>

using namespace std;

// Base Class
class FileManager {
public:
    // Check if a value already exists in a file
    bool isDuplicate(const string& value, const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string fieldValue;
            iss >> fieldValue;
            if (value == fieldValue) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    // Delete an entry from a file
    bool deleteEntry(const string& code, const string& filename) {
        vector<string> entries;
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            entries.push_back(line);
        }
        file.close();

        bool entryFound = false;
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            string savedCode;
            istringstream iss(*it);
            iss >> savedCode;

            if (code == savedCode) {
                entryFound = true;
                entries.erase(it);
                break;
            }
        }

        if (entryFound) {
            ofstream outfile(filename);
            for (int i = 0; i < entries.size(); i++) {
                outfile << entries[i] << endl;
            }
            outfile.close();
            cout << "Entry deleted successfully." << endl;
            return true;
        } else {
            cout << "Entry not found." << endl;
            return false;
        }
    }

};

// User class
class User {
private:
    string username;
    string password;
    string accessLevel;

public:
    User(string username, string password, string accessLevel) {
        this->username = username;
        this->password = password;
        this->accessLevel = accessLevel;
    }

    // Register a new user
    void RegisterUser() {
        // Validate access level
        if (accessLevel != "SM" && accessLevel != "PM" && accessLevel != "Admin") {
            cout << "Error: Invalid access level. Access level should be SM/PM/Admin." << endl;
            return;
        }

        // Generate unique user ID
        string userId = generateUserId(accessLevel);

        // Save user details to file
        ofstream file("users.txt", ios::app);
        file << userId << " " << " " << password << " " << accessLevel << endl;
        file.close();
        cout << "User registration successful. User ID: " << userId << endl;

    }

    // Login a user
    static bool Login(string username, string password, string& accessLevel) {
        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            string savedUsername, savedPassword, savedAccessLevel;
            istringstream iss(line);
            iss >> savedUsername >> savedPassword >> savedAccessLevel;

            // Check if username and password are not empty
            if (username.empty() || password.empty()){
                cout<< "Error: Username and password cannot be empty."<<endl;
                file.close();
                return false;
            }

            // Validate username and password
            if (username == savedUsername && password == savedPassword) {
                accessLevel = savedAccessLevel;
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
private:
    // Generate a unique user ID based on access level
    string generateUserId(const string& accessLevel) {
        ifstream file("users.txt");
        string line;
        int maxId = 0;
        string prefix;

        if (accessLevel == "SM")
            prefix = "SM";
        else if (accessLevel == "PM")
            prefix = "PM";
        else if (accessLevel == "Admin")
            prefix = "Admin";

        while (getline(file, line)) {
            istringstream iss(line);
            string fieldValue;
            iss >> fieldValue;
            if (fieldValue.find(prefix) == 0) {
                int id = stoi(fieldValue.substr(prefix.length()));//'prefix' from the value and converting the remaining string to an integer using stoi.
                if (id > maxId) {
                    maxId = id;
                }
            }
        }
        file.close();
        return prefix + to_string(maxId + 1);
        //The function then returns the unique user ID by combining the access level prefix (SM, PM, Admin) with the incremented ID.
    }

};

// Item class
class Item : public FileManager {
    static vector<Item> items;
protected:
    string itemCode;
    string itemName;
    string supplierID;

public:
    Item(string itemCode, string itemName, string supplierID) {
        this->itemCode = itemCode;
        this->itemName = itemName;
        this->supplierID = supplierID;
    }

    // Add an item to the inventory
    void AddItem() {
        if (!isItemCodeTaken(itemCode)) {
            if (isSupplierIDValid(supplierID)) {
                // Add the item to a vector or any other data structure to hold the items temporarily
                items.push_back(Item(itemCode, itemName, supplierID));
                cout << "Item added to inventory. Press 2 to Save items." << endl;
            } else {
                cout << "Invalid supplier ID. Item not added." << endl;
                return;
            }
        }else {
            cout << "Item code already exists. Item not added." << endl;
            return;
        }
    }

    static void SaveItems() {
        ofstream file("items.txt", ios::app);
        for (const auto& item : items) {
            file << item.itemCode << " " << item.itemName << " " << item.supplierID << endl;
        }
        file.close();
        cout << "Items saved successfully." << endl;
    }

    // List all items in the inventory
    static void ListItems() {
        ifstream file("items.txt");
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    // Edit an item in the inventory
    static void EditItem() {
    string itemCode, newItemName, newSupplierID;
    cout << "Enter item code to edit: ";
    getline(cin, itemCode);
    if(itemCode.empty()){
        cout<<"please the itemCode cannot be empty yah"<<endl;
        return;
    }
    if (!isItemCodeTaken(itemCode)) {
        cout << "Invalid item code. Item not updated." << endl;
        return;
    }

    vector<string> items;
    ifstream file("items.txt");
    if (!file) {
        cerr << "Unable to open file items.txt";
        exit(1); // terminate with error
    }
    string line;
    while (getline(file, line)) {
        items.push_back(line);
    }
    file.close();

    bool itemFound = false;
    for (int i = 0; i < items.size(); i++) {
        string savedItemCode, itemName, supplierID;
        istringstream iss(items[i]);
        iss >> savedItemCode >> itemName >> supplierID;

        if (itemCode == savedItemCode) {
            itemFound = true;
            cout << "Enter new item name: ";
            getline(cin, newItemName);
            if (newItemName.empty()) {
                cout << "Item name cannot be empty. Item not updated." << endl;
                return;
            }

            cout << "Enter new supplier ID: ";
            getline(cin, newSupplierID);
            if(newSupplierID.empty()){
                cout<<"newSupplierID cannot be empty. Item not updated."<<endl;
                return;
            }
            if (!isSupplierIDValid(newSupplierID)) {
                cout << "Invalid supplier ID. Item not updated." << endl;
                return;
            }

            items[i] = savedItemCode + " " + newItemName + " " + newSupplierID;
            break;
        }
    }

    if (itemFound) {
        ofstream outfile("items.txt");
        if (!outfile) {
            cerr << "Unable to open file items.txt";
            exit(1); // terminate with error
        }
        for (const auto& item : items) {
            outfile << item << endl;
        }
        outfile.close();
        cout << "Item updated successfully." << endl;
    } else {
        cout << "Item not found." << endl;
    }
}


    // Delete an item from the inventory
    static void DeleteItem() {
        string itemCode;
        cout << "Enter item code to delete: ";
        getline(cin, itemCode);
        if(itemCode.empty()){
            cout<<"ItemCode cannot be empty"<<endl;
            return;
        }
        FileManager fileManager;
        fileManager.deleteEntry(itemCode, "items.txt");
    }

private:
    // Check if an item code is already taken
    static bool isItemCodeTaken(const std::string& itemCode) {
        FileManager fileManager;
        return fileManager.isDuplicate(itemCode, "items.txt");
    }

    // Check if a supplier ID is valid
    static bool isSupplierIDValid(const std::string& supplierID) {
        FileManager fileManager;
        return fileManager.isDuplicate(supplierID, "suppliers.txt");
    }
};

// Definition of the static member variable 'items'
vector<Item> Item::items;

// Supplier class
class Supplier : public FileManager {

    static vector<Supplier> suppliers;

protected:
    string supplierCode;
    string supplierName;
    string itemID;

public:
    Supplier(string supplierCode, string supplierName, string itemID) {
        this->supplierCode = supplierCode;
        this->supplierName = supplierName;
        this->itemID = itemID;
    }

    // Add a supplier to the list
void AddSupplier() {
    if (!isSupplierCodeTaken(supplierCode)) {
        if (supplierName.empty() || itemID.empty()) {
            cout << "Supplier name or item ID cannot be empty. Supplier not added." << endl;
            return;
        }
        // Add the supplier to a vector or any other data structure to hold the suppliers temporarily
        suppliers.push_back(Supplier(supplierCode, supplierName, itemID));
        cout << "Supplier entry added to inventory. Press 2 to Save supplier entry." << endl;
    } else {
        cout << "Supplier code already exists. Supplier not added." << endl;
    }
}

// Save the suppliers to the file
static void SaveSuppliers() {
    ofstream file("suppliers.txt", ios::app);
    if (!file) {
        cerr << "Unable to open file suppliers.txt";
        exit(1); // terminate with error
    }
    for (const auto& supplier : suppliers) {
        file << supplier.supplierCode << " " << supplier.supplierName << " " << supplier.itemID << endl;
    }
    file.close();
    cout << "Suppliers saved successfully." << endl;
}


    // List all suppliers
    static void ListSuppliers() {
        ifstream file("suppliers.txt");
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    // Edit a supplier in the list
static void EditSupplier() {
    string supplierCode, newSupplierName, newItemID;
    cout << "Enter supplier code to edit: ";
    getline(cin, supplierCode);
    if(supplierCode.empty()){
        cout<<"SupplierCode cannot be empty"<<endl;
        return;
    }

    vector<string> suppliers;
    ifstream file("suppliers.txt");
    if (!file) {
        cerr << "Unable to open file suppliers.txt";
        exit(1); // terminate with error
    }
    string line;
    while (getline(file, line)) {
        suppliers.push_back(line);
    }
    file.close();

    bool supplierFound = false;
    for (int i = 0; i < suppliers.size(); i++) {
        string savedSupplierCode, supplierName, itemID;
        istringstream iss(suppliers[i]);
        iss >> savedSupplierCode >> supplierName >> itemID;

        if (supplierCode == savedSupplierCode) {
            supplierFound = true;
            cout << "Enter new supplier code: ";
            getline(cin, supplierCode);  // Reuse the supplierCode variable for the new code
            cout << "Enter new supplier name: ";
            getline(cin, newSupplierName);
            cout << "Enter new item ID: ";
            getline(cin, newItemID);

            if (supplierCode.empty() || newSupplierName.empty() || newItemID.empty()) {
                cout << "Supplier code, name, or item ID cannot be empty. Supplier not updated." << endl;
                return;
            }

            if (isSupplierCodeTaken(supplierCode)) {
                cout << "Supplier code already exists. Supplier not updated." << endl;
                return;
            }

            suppliers[i] = supplierCode + " " + newSupplierName + " " + newItemID;
            break;
        }
    }

    if (supplierFound) {
        ofstream outfile("suppliers.txt");
        if (!outfile) {
            cerr << "Unable to open file suppliers.txt";
            exit(1); // terminate with error
        }
        for (int i = 0; i < suppliers.size(); i++) {
            outfile << suppliers[i] << endl;
        }
        outfile.close();
        cout << "Supplier updated successfully." << endl;
    } else {
        cout << "Supplier not found." << endl;
    }
}


    // Delete a supplier from the list
    static void DeleteSupplier() {
        string supplierCode;
        cout << "Enter supplier code to delete: ";
        getline(cin, supplierCode);
        if(supplierCode.empty()){
            cout<<"SupplierCode cannot be empty"<<endl;
            return;
    }
        FileManager fileManager;
        fileManager.deleteEntry(supplierCode, "suppliers.txt");
    }

private:
    // Check if a supplier code is already taken
    static bool isSupplierCodeTaken(const std::string& supplierCode) {
        FileManager fileManager;
        return fileManager.isDuplicate(supplierCode, "suppliers.txt");
    }
    // Check if an item code is already taken
    static bool isItemCodeTaken(const std::string& itemCode) {
        FileManager fileManager;
        return fileManager.isDuplicate(itemCode, "items.txt");
    }
};

// Definition of the static member variable 'suppliers'
vector<Supplier> Supplier::suppliers;

// SalesEntry class
class SalesEntry : public FileManager {

    static vector<SalesEntry> salesEntry;

protected:
    string itemCode;
    int quantity;
    string salesDate;

public:
    SalesEntry(string itemCode, int quantity, string salesDate) {
        this->itemCode = itemCode;
        this->quantity = quantity;
        this->salesDate = salesDate;
    }

    // Add a sales entry
    void AddSalesEntry() {
        if (isItemCodeTaken(itemCode)) {
            bool isValidQuantity = false;

            while(!isValidQuantity){
                cout<< "Enter the quantity: ";
                cin>>quantity;

                if(cin.fail() || quantity<=0){
                    cout << "Invalid quantity. Please enter a positive integer." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }else{
                     isValidQuantity = true;
                }
            }
            // Add the saleEntry to a vector or any other data structure to hold the suppliers temporarily
            salesEntry.push_back(SalesEntry(itemCode, quantity, salesDate));


            cout << "Sales entry added to inventory. Press 2 to Save sales entry." << endl;
        } else {

            cout << "Item code does not exist. Item not added." << endl;
            return;
        }
    }

    // Save the salesEntry to the file
    static void SaveSalesEntry() {
        ofstream file("sales.txt", ios::app);
        for (const auto& salesEntry : salesEntry) {
            file << salesEntry.itemCode << " " << salesEntry.quantity << " " << salesEntry.salesDate << endl;
        }
        file.close();
        cout << "Sales Entry saved successfully." << endl;
    }

    // List all sales entries
    static void ListSalesEntries() {
        ifstream file("sales.txt");
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    // Edit a sales entry
    static void EditSaleEntry() {
        string itemCode;
        cout << "Enter item code of the sale entry to edit: ";
        getline(cin, itemCode);
        if(itemCode.empty()){
            cout<<"itemCode cannot be empty."<<endl;
        }

        vector<string> salesEntries;
        ifstream file("sales.txt");
        string line;
        while (getline(file, line)) {
            salesEntries.push_back(line);
        }
        file.close();

        bool saleEntryFound = false;
        for (int i = 0; i < salesEntries.size(); i++) {
            string savedItemCode;
            int savedQuantity;
            string savedSalesDate;
            istringstream iss(salesEntries[i]);
            iss >> savedItemCode >> savedQuantity >> savedSalesDate;

            if (itemCode == savedItemCode) {
                saleEntryFound = true;

                string newSalesDate;
                int newQuantity;
                cout << "Enter new sales date: ";
                getline(cin, newSalesDate);
                cout << "Enter new quantity: ";
                cin >> newQuantity;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                salesEntries[i] = savedItemCode + " " + to_string(newQuantity) + " " + newSalesDate;
                break;
            }
        }

        if (saleEntryFound) {
            ofstream outfile("sales.txt");
            for (int i = 0; i < salesEntries.size(); i++) {
                outfile << salesEntries[i] << endl;
            }
            outfile.close();
            cout << "Sale entry updated successfully." << endl;
        } else {
            cout << "Sale entry not found." << endl;
        }
    }

    // Delete a sales entry
    static void DeleteSaleEntry() {
        string itemCode;
        cout << "Enter item code to delete sales entry: ";
        getline(cin, itemCode);
        if(itemCode.empty()){
            cout<<"itemCode cannot be empty."<<endl;
            return;
        }
        FileManager fileManager;
        fileManager.deleteEntry(itemCode, "sales.txt");
    }

private:
    // Check if an item code exists
    static bool isItemCodeTaken(const std::string& itemCode) {
        FileManager fileManager;
        return fileManager.isDuplicate(itemCode, "items.txt");
    }
};

// Definition of the static member variable 'salesEntry'
vector<SalesEntry> SalesEntry::salesEntry;

// PurchaseRequisition class
class PurchaseRequisition : public FileManager {
    static vector<PurchaseRequisition> PR;
protected:
    string PRID;
    string itemCode;
    int quantity;
    string requiredDate;
    string supplierCode;
    string salesManager;

public:
    PurchaseRequisition(string PRID, string itemCode, int quantity, string requiredDate, string supplierCode, string salesManager) {
        this->PRID = PRID;
        this->itemCode = itemCode;
        this->quantity = quantity;
        this->requiredDate = requiredDate;
        this->supplierCode = supplierCode;
        this->salesManager = salesManager;
    }

    // Create a purchase requisition
    void CreatePR() {
        if (!isPRIDTaken(PRID)) {
            if (isSupplierCodeValid(supplierCode) && isSalesManagerValid(salesManager)) {
                if (isValidDateFormat(requiredDate)) {
                    // Check if quantity is a positive number
                    if (quantity > 0) {
                        PR.push_back(PurchaseRequisition(PRID, itemCode, quantity, requiredDate, supplierCode, salesManager));
                        cout << "Purchase requisition added to inventory. Press 2 to Save purchase requisition." << endl;
                    }else {
                        cout << "Invalid quantity. Quantity must be a positive number." << endl;
                    return;
                    }
                    PR.push_back(PurchaseRequisition(PRID, itemCode, quantity, requiredDate, supplierCode, salesManager));
                    cout << "Purchase requisition added to inventory. Press 2 to Save purchase requisition." << endl;
                }else {
                    cout << "Invalid date format. Please enter the date in the format YYYY-MM-DD." << endl;
                    return;
                }
            }else {
                cout << "Supplier code or Sales Manager exist in the file(use a different one). Purchase requisition not created." << endl;
            }
        } else {
            cout << "PRID already exists. Purchase requisition not created." << endl;
        }
    }

     // Save the PR to the file
    static void SavePREntry() {
        ofstream file("purchase_requisitions.txt", ios::app);
        for (const auto& PR : PR) {
            file << PR.PRID << " " << PR.itemCode << " " << PR.quantity << " " << PR.requiredDate << " " << PR.supplierCode << " " << PR.salesManager << endl;
        }
        file.close();
        cout << "Purchase requisitions saved successfully." << endl;
    }

    // Display all purchase requisitions
    static void DisplayPRs() {
        ifstream file("purchase_requisitions.txt");
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    // Edit a purchase requisition
    static void EditPR() {
        string PRID;
        cout << "Enter PRID of the purchase requisition to edit: ";
        getline(cin, PRID);
        if(PRID.empty()){
            cout<<"PRID cannot be empty."<<endl;
            return;
        }

        vector<string> purchaseRequisitions;
        ifstream file("purchase_requisitions.txt");
        string line;
        while (getline(file, line)) {
            purchaseRequisitions.push_back(line);
        }
        file.close();

        bool purchaseRequisitionFound = false;
        for (int i = 0; i < purchaseRequisitions.size(); i++) {
            string savedPRID, savedItemCode, savedRequiredDate, savedSupplierCode, savedSalesManager;
            int savedQuantity;
            istringstream iss(purchaseRequisitions[i]);
            iss >> savedPRID >> savedItemCode >> savedQuantity >> savedRequiredDate >> savedSupplierCode >> savedSalesManager;

            if (PRID == savedPRID) {
                purchaseRequisitionFound = true;

                string newItemCode, newRequiredDate, newSupplierCode, newSalesManager;
                int newQuantity;
                cout << "Enter new item code: ";
                getline(cin, newItemCode);

                bool validInput = false;
                while(!validInput){
                    cout << "Enter new quantity: ";
                    cin >> newQuantity;
                    if(cin.fail() || newQuantity < 0) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid quantity. Please enter a non-negative integer value." << endl;
                    }else{
                        validInput = true;
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                validInput = false;
                regex datePattern("\\d{4}-\\d{2}-\\d{2}");
                while (!validInput) {
                    cout << "Enter new required date (YYYY-MM-DD): ";
                    getline(cin, newRequiredDate);
                    if (!regex_match(newRequiredDate, datePattern)) {
                        cout << "Invalid date format. Please enter the date in the format YYYY-MM-DD." << endl;
                    } else {
                        validInput = true;
                }
                }
                cout << "Enter new supplier code: ";
                getline(cin, newSupplierCode);
                cout << "Enter new sales manager: ";
                getline(cin, newSalesManager);


                    if (isSupplierCodeValid(newSupplierCode) && isSalesManagerValid(newSalesManager)) {
                        purchaseRequisitions[i] = savedPRID + " " + newItemCode + " " + to_string(newQuantity) + " " + newRequiredDate + " " + newSupplierCode + " " + newSalesManager;
                        break;
                    } else {
                        cout << "Invalid supplier code or sales manager. Purchase requisition not updated." << endl;
                        return;
                    }

            }
        }

        if (purchaseRequisitionFound) {
            ofstream outfile("purchase_requisitions.txt");
            for (int i = 0; i < purchaseRequisitions.size(); i++) {
                outfile << purchaseRequisitions[i] << endl;
            }
            outfile.close();
            cout << "Purchase requisition updated successfully." << endl;
        } else {
            cout << "Purchase requisition not found." << endl;
        }
    }

    // Delete a purchase requisition
    static void DeletePR() {
        string PRID;
        cout << "Enter PRID of the purchase requisition to delete: ";
        getline(cin, PRID);
        if(PRID.empty()){
            cout<<"PRID cannot be empty."<<endl;
            return;
        }
        FileManager fileManager;
        fileManager.deleteEntry(PRID, "purchase_requisitions.txt");
    }

private:
    // Check if PRID already exists
    static bool isPRIDTaken(const string& PRID) {
        FileManager fileManager;
        return fileManager.isDuplicate(PRID, "purchase_requisitions.txt");
    }

    // Check if supplier code is valid
    static bool isSupplierCodeValid(const string& supplierCode) {
        FileManager fileManager;
        return fileManager.isDuplicate(supplierCode, "suppliers.txt");
    }

    // Check if sales manager is valid
    static bool isSalesManagerValid(const string& salesManager) {
        FileManager fileManager;
        return fileManager.isDuplicate(salesManager, "users.txt");
    }

    static bool isValidDateFormat(const std::string& date) {
        // Define the pattern for the expected date format
        std::regex pattern(R"(\d{4}-\d{2}-\d{2})");

        // Check if the input date matches the pattern
        return std::regex_match(date, pattern);
    }
};

// Definition of the static member variable 'PR'
vector<PurchaseRequisition> PurchaseRequisition::PR;

// PurchaseOrder class
class PurchaseOrder : public FileManager {
    static vector<PurchaseOrder> POs;
protected:
    string POID;
    string PRID;
    string purchaseManager;

public:
    PurchaseOrder(string POID, string PRID, string purchaseManager) {
        this->POID = POID;
        this->PRID = PRID;
        this->purchaseManager = purchaseManager;
    }

    // Generate a purchase order
    void GeneratePO() {
        if (!isPOIDTaken(POID)) {
            if (!isPRIDTaken(PRID)) {
                if (isPMIDValid(purchaseManager)) {
                    POs.push_back(PurchaseOrder(POID, PRID, purchaseManager));
                    cout << "Purchase order generated to inventory. Press 2 to Save purchase order." << endl;
                } else {
                cout << "Purchase Manager not exists. Purchase order not created." << endl;
                }
            } else {
                cout << "PRID already exists. Purchase order not created." << endl;
            }
        } else {
            cout << "POID already exists. Purchase order not created." << endl;
        }
    }

    // Save the PO to the file
    static void SavePOEntry() {
        ofstream file("purchase_orders.txt", ios::app);
        for (const auto& PO : POs) {
            file << PO.POID << " " << PO.PRID << " " << PO.purchaseManager << endl;
        }
        file.close();
        cout << "Purchase order saved successfully." << endl;
    }

    // List all purchase orders
    static void ListPOs() {
        ifstream file("purchase_orders.txt");
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    // Edit a purchase order
    static void EditPO() {
        string POID;
        cout << "Enter POID of the purchase order to edit: ";
        getline(cin, POID);
        if(POID.empty()){
            cout<<"POID cannot be empty."<<endl;
            return;
        }

        vector<string> purchaseOrders;
        ifstream file("purchase_orders.txt");
        string line;
        while (getline(file, line)) {
            purchaseOrders.push_back(line);
        }
        file.close();

        bool purchaseOrderFound = false;
        for (int i = 0; i < purchaseOrders.size(); i++) {
            string savedPOID, savedPRID, savedPurchaseManager;
            istringstream iss(purchaseOrders[i]);
            iss >> savedPOID >> savedPRID >> savedPurchaseManager;

            if (POID == savedPOID) {
                purchaseOrderFound = true;

                string newPRID, newPurchaseManager;
                cout << "Enter new PRID: ";
                getline(cin, newPRID);
                cout << "Enter new purchase manager: ";
                getline(cin, newPurchaseManager);

                if (isPOIDTaken(POID) && isPRIDTaken(POID)) {
                    purchaseOrders[i] = savedPOID + " " + newPRID + " " + newPurchaseManager;
                    break;
                } else {
                    cout << "Invalid POID/PRID. Purchase order not updated." << endl;
                    return;
                }
            }
        }

        if (purchaseOrderFound) {
            ofstream outfile("purchase_orders.txt");
            for (int i = 0; i < purchaseOrders.size(); i++) {
                outfile << purchaseOrders[i] << endl;
            }
            outfile.close();
            cout << "Purchase order updated successfully." << endl;
        } else {
            cout << "Purchase order not found." << endl;
        }
    }

    // Delete a purchase order
    static void DeletePO() {
        string POID;
        cout << "Enter POID of the purchase order to delete: ";
        getline(cin, POID);
        FileManager fileManager;
        fileManager.deleteEntry(POID, "purchase_orders.txt");
    }
private:
    // Check if POID already exists
    static bool isPOIDTaken(const string& POID) {
        FileManager fileManager;
        return fileManager.isDuplicate(POID, "purchase_orders.txt");
    }
    // Check if PRID already exists
    static bool isPRIDTaken(const string& PRID) {
        FileManager fileManager;
        return fileManager.isDuplicate(PRID, "purchase_orders.txt");
    }
    // Check if PM ID exists
    static bool isPMIDValid(const string& purchaseManager) {
        FileManager fileManager;
        return fileManager.isDuplicate(purchaseManager, "users.txt");
    }
};

// Definition of the static member variable 'PO'
vector<PurchaseOrder> PurchaseOrder::POs;

// Application class
class Application {
public:
    static void DisplayMenu(string accessLevel) {
        cout << "\n==================== Main Menu ===================" << endl;
        if (accessLevel == "Admin") {
            cout << "1.  Item Entry (Add/Save/Edit/Delete)" << endl;
            cout << "2.  Supplier Entry (Add/Save/Edit/Delete)" << endl;
            cout << "3.  Daily Item-wise Sales Entry (Add/Save/Edit/Delete)" << endl;
            cout << "4.  Create a Purchase Requisition (Add/Save/Edit/Delete)" << endl;
            cout << "5.  Display Requisition (View)" << endl;
            cout << "6.  List of Purchaser Orders (View)" << endl;
            cout << "7.  Generate Purchase Order (Add/Save/Delete/Edit)" << endl;
            cout << "8.  List of Items (View)" << endl;
            cout << "9.  List of Suppliers (View)" << endl;
            cout << "10. List of Daily Item-wise Sales Entry (View)" << endl;
            cout << "11. User Registration" << endl;
        }

        else if (accessLevel == "SM") {
            cout << "1. Item Entry (Add/Save/Edit/Delete)" << endl;
            cout << "2. Supplier Entry (Add/Save/Edit/Delete)" << endl;
            cout << "3. Daily Item-wise Sales Entry (Add/Save/Edit/Delete)" << endl;
            cout << "4. Create a Purchase Requisition (Add/Save/Edit/Delete)" << endl;
            cout << "5. Display Requisition (View)" << endl;
            cout << "6. List of Purchaser Orders (View)" << endl;
        }

        else if (accessLevel == "PM") {
            cout << "1. List of Items (View)" << endl;
            cout << "2. List of Suppliers (View)" << endl;
            cout << "3. Display Requisition (View)" << endl;
            cout << "4. Generate Purchase Order (Add/Save/Delete/Edit)" << endl;
            cout << "5. List of Purchaser Orders (View)" << endl;
        }

        cout << "0. Exit" << endl;
        cout << "==================================================\n" << endl;
    }

    static void HandleUserInput(string accessLevel) {
        string choice;
        while (true) {
            DisplayMenu(accessLevel);
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == "0") {
                cout << "Exiting to user access..." << endl;
                return;
            } else if (choice == "1") {
                if (accessLevel == "SM" || accessLevel == "Admin")
                    HandleItemMenu();
                else if (accessLevel == "PM")
                    listItems();
            } else if (choice == "2") {
                if (accessLevel == "SM" || accessLevel == "Admin")
                    HandleSupplierMenu();
                else if (accessLevel == "PM")
                    listSuppliers();
            } else if (choice == "3") {
                if (accessLevel == "SM" || accessLevel == "Admin")
                    HandleSaleEntryMenu();
                else if (accessLevel == "PM")
                    displayPRs();
            } else if (choice == "4") {
                if (accessLevel == "SM" || accessLevel == "Admin")
                    HandlePRMenu();
                else if (accessLevel == "PM")
                    HandlePOMenu();
            } else if (choice == "5") {
                if (accessLevel == "SM" || accessLevel == "Admin")
                    displayPRs();
                else if (accessLevel == "PM")
                    listPOs();
            } else if (choice == "6") {
                if (accessLevel == "SM" || accessLevel == "Admin")
                    listPOs();
            } else if (choice == "7") {
                if (accessLevel == "Admin")
                    HandlePOMenu();
                else if (accessLevel == "SM" || accessLevel == "PM")
                    cout << "Invalid choice. Please try again." << endl;
            } else if (choice == "8") {
                if (accessLevel == "Admin")
                    listItems();
                else if (accessLevel == "SM" || accessLevel == "PM")
                    cout << "Invalid choice. Please try again." << endl;
            } else if (choice == "9") {
                if (accessLevel == "Admin")
                    listSuppliers();
                else if (accessLevel == "SM" || accessLevel == "PM")
                    cout << "Invalid choice. Please try again." << endl;
            } else if (choice == "10") {
                if (accessLevel == "Admin")
                    listSalesEntries();
                else if (accessLevel == "SM" || accessLevel == "PM")
                    cout << "Invalid choice. Please try again." << endl;
            } else if (choice == "11") {
                if (accessLevel == "Admin")
                    RegisterUser();
                else if (accessLevel == "SM" || accessLevel == "PM")
                    cout << "Invalid choice. Please try again." << endl;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    static void RegisterUser() {
        string username, password, accessLevel;
        cout << "Enter access level (SM/PM/Admin): ";
        getline(cin, accessLevel);

        // Check if the access level is valid
        if (accessLevel != "SM" && accessLevel != "PM" && accessLevel != "Admin") {
            cout << "Invalid access level. Redirecting to Admin user for user creation..." << endl;
        }
        cout << "Enter password: ";
        getline(cin, password);

        User user(username, password, accessLevel);
        user.RegisterUser();
    }

    static void HandleItemMenu() {
        string choice;
        while (true) {
            cout << "\n\t==== Item Entry Menu ====" << endl;
            cout << "\t1. Add Item" << endl;
            cout << "\t2. Save Item" << endl;
            cout << "\t3. Edit Item" << endl;
            cout << "\t4. Delete Item" << endl;
            cout << "\t0. Exit" << endl;
            cout << "\t===========================\n" << endl;

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == "0") {
                cout << "Returning to the main menu..." << endl;
                return;
            } else if (choice == "1") {
                addItem();
            } else if (choice == "2") {
                saveItems();
            } else if (choice == "3") {
                editItem();
            } else if (choice == "4") {
                deleteItem();
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    static void addItem() {
        string itemCode, itemName, supplierID;
        cout << "Enter item code: ";
        getline(cin, itemCode);
        cout << "Enter item name: ";
        getline(cin, itemName);
        cout << "Enter supplier ID: ";
        getline(cin, supplierID);

        Item item(itemCode, itemName, supplierID);
        item.AddItem();
    }

    static void saveItems() {
        Item::SaveItems();
    }

    static void listItems() {
        Item::ListItems();
    }

    static void deleteItem() {
        Item::DeleteItem();
    }

    static void editItem() {
        Item::EditItem();
    }

    static void HandleSupplierMenu() {
        string choice;
        while (true) {
            cout << "\n\t==== Supplier Entry Menu ====" << endl;
            cout << "\t1. Add Supplier" << endl;
            cout << "\t2. Save Supplier List" << endl;
            cout << "\t3. Edit Supplier" << endl;
            cout << "\t4. Delete Supplier" << endl;
            cout << "\t0. Exit" << endl;
            cout << "\t===============================\n" << endl;

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == "0") {
                cout << "Returning to main menu..." << endl;
                return;
            } else if (choice == "1") {
                addSupplier();
            } else if (choice == "2") {
                saveSuppliers();
            } else if (choice == "3") {
                editSupplier();
            } else if (choice == "4") {
                deleteSupplier();
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }


    static void addSupplier() {
        string supplierCode, supplierName, itemID;
        cout << "Enter supplier code: ";
        getline(cin, supplierCode);
        cout << "Enter supplier name: ";
        getline(cin, supplierName);
        cout << "Enter item ID: ";
        getline(cin, itemID);

        Supplier supplier(supplierCode, supplierName, itemID);
        supplier.AddSupplier();
    }

    static void saveSuppliers() {
        Supplier::SaveSuppliers();
    }

    static void listSuppliers() {
        Supplier::ListSuppliers();
    }

    static void editSupplier() {
        Supplier::EditSupplier();
    }

    static void deleteSupplier() {
        Supplier::DeleteSupplier();
    }

    static void HandleSaleEntryMenu() {
    string choice;
    while (true) {
        cout << "\n\t==== Sale Entry Menu ====" << endl;
        cout << "\t1. Add Sale Entry" << endl;
        cout << "\t2. Save Sale Entry" << endl;
        cout << "\t3. Edit Sale Entry" << endl;
        cout << "\t4. Delete Sale Entry" << endl;
        cout << "\t0. Exit" << endl;
        cout << "\t=========================\n" << endl;

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "0") {
            cout << "Returning to the main menu..." << endl;
            return;
        } else if (choice == "1") {
            addSalesEntry();
        } else if (choice == "2") {
            saveSalesEntry();
        } else if (choice == "3") {
            editSaleEntry();
        } else if (choice == "4") {
            deleteSaleEntry();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

static void addSalesEntry() {
    string itemCode, salesDate;
    int quantity;

    cout << "Enter item code: ";
    getline(cin, itemCode);

    while (true) {
        cout << "Enter quantity: ";
        if (cin >> quantity && quantity > 0) {
            break;
        } else {
            cout << "Invalid quantity. Please enter a positive integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter sales date: ";
    getline(cin, salesDate);

    // Validate item code and sales date if needed

    SalesEntry salesEntry(itemCode, quantity, salesDate);
    salesEntry.AddSalesEntry();
}

static void saveSalesEntry() {
    SalesEntry::SaveSalesEntry();
}

static void listSalesEntries() {
    SalesEntry::ListSalesEntries();
}

static void editSaleEntry() {
    SalesEntry::EditSaleEntry();
}

static void deleteSaleEntry() {
    SalesEntry::DeleteSaleEntry();
}


    static void HandlePRMenu() {
        string choice;
        while (true) {
            cout << "\n\t==== Purchase Requisition Menu ====" << endl;
            cout << "\t1. Create Purchase Requisition" << endl;
            cout << "\t2. Save Purchase Requisitions" << endl;
            cout << "\t3. Edit Purchase Requisition" << endl;
            cout << "\t4. Delete Purchase Requisition" << endl;
            cout << "\t0. Exit" << endl;
            cout << "\t==================================\n" << endl;

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == "0") {
                cout << "Returning to main menu..." << endl;
                return;
            } else if (choice == "1") {
                createPR();
            } else if (choice == "2") {
                savePREntry();
            } else if (choice == "3") {
                editPR();
            } else if (choice == "4") {
                deletePR();
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }


    static void savePREntry() {
        PurchaseRequisition::SavePREntry();
    }

    static void displayPRs() {
        PurchaseRequisition::DisplayPRs();
    }

    static void editPR() {
        PurchaseRequisition::EditPR();
    }

    static void deletePR() {
        PurchaseRequisition::DeletePR();
    }

    static void createPR() {
    string PRID, itemCode, requiredDate, supplierCode, salesManager;
    int quantity;
    bool validInput = false;

    cout << "Enter PR ID: ";
    getline(cin, PRID);

    cout << "Enter item code: ";
    getline(cin, itemCode);

    while(!validInput) {
        cout << "Enter quantity: ";
        cin >> quantity;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid quantity. Please enter an integer value." << endl;
        } else {
            validInput = true;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    validInput = false;
    regex datePattern("\\d{4}-\\d{2}-\\d{2}");
    while(!validInput) {
        cout << "Enter required date (YYYY-MM-DD): ";
        getline(cin, requiredDate);
        if(!regex_match(requiredDate, datePattern)) {
            cout << "Invalid date. Please enter in the format YYYY-MM-DD." << endl;
        } else {
            validInput = true;
        }
    }

    cout << "Enter supplier code: ";
    getline(cin, supplierCode);

    cout << "Enter sales manager: ";
    getline(cin, salesManager);

    PurchaseRequisition PR(PRID, itemCode, quantity, requiredDate, supplierCode, salesManager);
    PR.CreatePR();
}


    static void HandlePOMenu() {
        string choice;
        while (true) {
            cout << "\n\t==== Purchase Order Menu ====" << endl;
            cout << "\t1. Generate Purchase Order" << endl;
            cout << "\t2. Save Purchase Orders" << endl;
            cout << "\t3. Edit Purchase Order" << endl;
            cout << "\t4. Delete Purchase Order" << endl;
            cout << "\t0. Exit" << endl;
            cout << "\t=============================\n" << endl;

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == "0") {
                cout << "Returning to main menu..." << endl;
                return;
            } else if (choice == "1") {
                generatePO();
            } else if (choice == "2") {
                savePOEntry();
            } else if (choice == "3") {
                editPO();
            } else if (choice == "4") {
                deletePO();
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
}

    static void generatePO() {
        string POID, PRID, purchaseManager;
        cout << "Enter PO ID: ";
        getline(cin, POID);
        cout << "Enter PR ID: ";
        getline(cin, PRID);
        cout << "Enter purchase manager: ";
        getline(cin, purchaseManager);

        PurchaseOrder PO(POID, PRID, purchaseManager);
        PO.GeneratePO();
    }

    static void savePOEntry() {
        PurchaseOrder::SavePOEntry();
    }

    static void listPOs() {
        PurchaseOrder::ListPOs();
    }


    static void editPO() {
        PurchaseOrder::EditPO();
    }

    static void deletePO() {
        PurchaseOrder::DeletePO();
    }
};

int main() {
    string username, password, accessLevel;
    string choice;
    while (true) {
        cout << "\n======= User Access =======" << endl;
        cout << "1. Admin" << endl;
        cout << "2. Sales Manager (SM)" << endl;
        cout << "3. Purchase Manager (PM)" << endl;
        cout << "0. Exit" << endl;
        cout << "===========================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == "0") {
            cout << "Exiting the system..." << endl;
            return 0;
        } else if (choice == "1") {
            accessLevel = "Admin";
        } else if (choice == "2") {
            accessLevel = "SM";
        } else if (choice == "3") {
            accessLevel = "PM";
        } else {
            cout << "Invalid choice. Try again!" << endl;
            continue;
        }

        cout<< "Access level = " << accessLevel << endl;
        cout << "\n==== Login ====" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (User::Login(username, password, accessLevel)) {
            cout << "Login successful. "<< endl;
            Application::HandleUserInput(accessLevel);
        }
        else {
            cout << "Invalid username or password. Try again!" << endl;
        }
    }

    return 0;
}
