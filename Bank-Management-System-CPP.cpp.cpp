#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct stUser
{
    string userName;
    string password;
    int permissions;
    bool markForDelete = false;
};

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enManageUsersMenueOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
    eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3,
    eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6,
    eManageUsers = 7, eExit = 8
};

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

const string clientsFileName= "Clients.txt";
const string usersFileName = "Users.txt";

stUser currentUser;


void showMainMenue();
void showTransactionsMenue();
void showManageUsersMenue();
bool checkAccessPermission(enMainMenuePermissions permission);
void login();


struct stClient
{
    string accountNumber;
    string pinCode;
    string name;
    string phone;
    double accountBalance;
    bool markForDelete= false;
};

vector<string> splitString(string s1, string delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord;  

     
    while ((pos = s1.find(delim)) != std::string::npos)
    {
        sWord = s1.substr(0, pos); 
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        s1.erase(0, pos + delim.length());  
    }

    if (s1 != "")
    {
        vString.push_back(s1);  
    }

    return vString;

}

stUser convertUserLinetoRecord(string line, string seperator = "#//#")
{

    stUser user;
    vector<string> vUserData;

    vUserData = splitString(line, seperator);

    user.userName = vUserData[0];
    user.password = vUserData[1];
    user.permissions = stoi(vUserData[2]);

    return user;

}

stClient convertLinetoRecord(string line, string seperator = "#//#")
{

    stClient client;
    vector<string> vClientData;

    vClientData = splitString(line, seperator);

    client.accountNumber = vClientData[0];
    client.pinCode = vClientData[1];
    client.name = vClientData[2];
    client.phone = vClientData[3];
    client.accountBalance = stod(vClientData[4]); 


    return client;

}

stUser convertUserLinetoRecord2(string line, string seperator = "#//#")
{
    stUser user;
    vector<string> vUserData;

    vUserData = splitString(line, seperator);

    user.userName = vUserData[0];
    user.password = vUserData[1];
    user.permissions = stoi(vUserData[2]);

    return user;

}

string convertRecordToLine(stClient client, string seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += client.accountNumber + seperator;
    stClientRecord += client.pinCode + seperator;
    stClientRecord += client.name + seperator;
    stClientRecord += client.phone + seperator;
    stClientRecord += to_string(client.accountBalance);

    return stClientRecord;

}

string convertUserRecordToLine(stUser user, string seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += user.userName + seperator;
    stClientRecord += user.password + seperator;
    stClientRecord += to_string(user.permissions);

    return stClientRecord;

}

bool clientExistsByAccountNumber(string accountNumber, string fileName)
{

    vector <stClient> vClients;

    fstream myFile;
    myFile.open(fileName, ios::in); 

    if (myFile.is_open())
    {

        string line;
        stClient client;

        while (getline(myFile, line))
        {

            client = convertLinetoRecord(line);
            if (client.accountNumber == accountNumber)
            {
                myFile.close();
                return true;
            }


            vClients.push_back(client);
        }

        myFile.close();

    }

    return false;


}

bool userExistsByUsername(string username, string fileName)
{


    fstream myFile;
    myFile.open(fileName, ios::in); 

    if (myFile.is_open())
    {

        string line;
        stUser user;

        while (getline(myFile, line))
        {

            user = convertUserLinetoRecord(line);
            if (user.userName == username)
            {
                myFile.close();
                return true;
            }

        }

        myFile.close();

    }

    return false;


}

stClient readNewClient()
{
    stClient client;

    cout << "Enter Account Number? ";

     
    getline(cin >> ws, client.accountNumber);

    while (clientExistsByAccountNumber(client.accountNumber, clientsFileName))
    {
        cout << "\nClient with [" << client.accountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, client.accountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, client.pinCode);

    cout << "Enter Name? ";
    getline(cin, client.name);

    cout << "Enter Phone? ";
    getline(cin, client.phone);

    cout << "Enter AccountBalance? ";
    cin >> client.accountBalance;

    return client;

}

int readPermissionsToSet()
{

    int permissions = 0;
    char answer = 'n';


    cout << "\nDo you want to give full access? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {


        permissions += enMainMenuePermissions::pListClients;
    }


    cout << "\nAdd New Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        permissions += enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        permissions += enMainMenuePermissions::pDeleteClient;
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        permissions += enMainMenuePermissions::pUpdateClients;
    }

    cout << "\nFind Client? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        permissions += enMainMenuePermissions::pFindClient;
    }

    cout << "\nTransactions? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        permissions += enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage Users? y/n? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        permissions += enMainMenuePermissions::pManageUsers;
    }


    return permissions;

}

stUser readNewUser()
{
    stUser user;

    cout << "Enter Username? ";

    
    getline(cin >> ws, user.userName);

    while (userExistsByUsername(user.userName, usersFileName))
    {
        cout << "\nUser with [" << user.userName << "] already exists, Enter another Username? ";
        getline(cin >> ws, user.userName);
    }

    cout << "Enter Password? ";
    getline(cin, user.password);

    user.permissions = readPermissionsToSet();

    return user;

}

vector <stUser> loadUsersDataFromFile(string fileName)
{

    vector <stUser> vUsers;

    fstream myFile;
    myFile.open(fileName, ios::in); 

    if (myFile.is_open())
    {

        string line;
        stUser user;

        while (getline(myFile, line))
        {

            user = convertUserLinetoRecord(line);

            vUsers.push_back(user);
        }

        myFile.close();

    }

    return vUsers;

}

vector <stClient> loadCleintsDataFromFile(string fileName)
{

    vector <stClient> vClients;

    fstream myFile;
    myFile.open(fileName, ios::in); 

    if (myFile.is_open())
    {

        string line;
        stClient client;

        while (getline(myFile, line))
        {

            client = convertLinetoRecord(line);

            vClients.push_back(client);
        }

        myFile.close();

    }

    return vClients;

}

void printClientRecordLine(stClient client)
{

    cout << "| " << setw(15) << left << client.accountNumber;
    cout << "| " << setw(10) << left << client.pinCode;
    cout << "| " << setw(40) << left << client.name;
    cout << "| " << setw(12) << left << client.phone;
    cout << "| " << setw(12) << left << client.accountBalance;

}

void printUserRecordLine(stUser user)
{

    cout << "| " << setw(15) << left << user.userName;
    cout << "| " << setw(10) << left << user.password;
    cout << "| " << setw(40) << left << user.permissions;
}

void printClientRecordBalanceLine(stClient client)
{

    cout << "| " << setw(15) << left << client.accountNumber;
    cout << "| " << setw(40) << left << client.name;
    cout << "| " << setw(12) << left << client.accountBalance;

}

void showAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void showAllClientsScreen()
{


    if (!checkAccessPermission(enMainMenuePermissions::pListClients))
    {
        showAccessDeniedMessage();
        return;
    }

    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (stClient client : vClients)
        {

            printClientRecordLine(client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void showAllUsersScreen()
{


    vector <stUser> vUsers = loadUsersDataFromFile(usersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else

        for (stUser user : vUsers)
        {

            printUserRecordLine(user);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void showTotalBalances()
{

    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double totalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (stClient client : vClients)
        {

            printClientRecordBalanceLine(client);
            totalBalances += client.accountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << totalBalances;

}

void printClientCard(stClient client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << client.accountNumber;
    cout << "\nPin Code     : " << client.pinCode;
    cout << "\nName         : " << client.name;
    cout << "\nPhone        : " << client.phone;
    cout << "\nAccount Balance: " << client.accountBalance;
    cout << "\n-----------------------------------\n";

}

void printUserCard(stUser user)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << user.userName;
    cout << "\nPassword    : " << user.password;
    cout << "\nPermissions : " << user.permissions;
    cout << "\n-----------------------------------\n";

}

bool findClientByAccountNumber(string accountNumber, vector <stClient> vClients, stClient& client)
{

    for (stClient C : vClients)
    {

        if (C.accountNumber == accountNumber)
        {
            client = C;
            return true;
        }

    }
    return false;

}

bool findUserByUsername(string username, vector <stUser> vUsers, stUser& user)
{

    for (stUser u : vUsers)
    {

        if (u.userName == username)
        {
            user = u;
            return true;
        }

    }
    return false;

}

bool findUserByUsernameAndPassword(string username, string password, stUser& user)
{

    vector <stUser> vUsers = loadUsersDataFromFile(usersFileName);

    for (stUser u : vUsers)
    {

        if (u.userName == username && u.password == password)
        {
            user = u;
            return true;
        }

    }
    return false;

}

stClient changeClientRecord(string accountNumber)
{
    stClient client;

    client.accountNumber = accountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, client.pinCode);

    cout << "Enter Name? ";
    getline(cin, client.name);

    cout << "Enter Phone? ";
    getline(cin, client.phone);

    cout << "Enter AccountBalance? ";
    cin >> client.accountBalance;

    return client;

}

stUser changeUserRecord(string username)
{
    stUser user;

    user.userName = username;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, user.password);

    user.permissions = readPermissionsToSet();

    return user;

}

bool markClientForDeleteByAccountNumber(string accountNumber, vector <stClient>& vClients)
{
    for (stClient& c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            c.markForDelete = true;
            return true;
        }
    }
    return false;
}

bool markUserForDeleteByUsername(string username, vector <stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.userName == username)
        {
            U.markForDelete = true;
            return true;
        }
    }
    return false;
}

vector <stClient> saveCleintsDataToFile(string fileName, vector <stClient> vClients)
{

    fstream myFile;
    myFile.open(fileName, ios::out); 

    string dataLine;

    if (myFile.is_open())
    {

        for (stClient c : vClients)
        {

            if (c.markForDelete == false)
            {
                 
                dataLine = convertRecordToLine(c);
                myFile << dataLine << endl;

            }

        }

        myFile.close();

    }

    return vClients;

}

vector <stUser> saveUsersDataToFile(string fileName, vector <stUser> vUsers)
{

    fstream myFile;
    myFile.open(fileName, ios::out); 

    string dataLine;

    if (myFile.is_open())
    {

        for (stUser u : vUsers)
        {

            if (u.markForDelete == false)
            {
                  
                dataLine = convertUserRecordToLine(u);
                myFile << dataLine << endl;

            }

        }

        myFile.close();

    }

    return vUsers;

}

void addDataLineToFile(string fileName, string  stDataLine)
{
    fstream myFile;
    myFile.open(fileName, ios::out | ios::app);

    if (myFile.is_open())
    {

        myFile << stDataLine << endl;

        myFile.close();
    }

}

void addNewClient()
{
    stClient client;
    client = readNewClient();
    addDataLineToFile(clientsFileName, convertRecordToLine(client));

}

void addNewUser()
{
    stUser user;
    user = readNewUser();
    addDataLineToFile(usersFileName, convertUserRecordToLine(user));

}

void addNewClients()
{
    char addMore = 'Y';
    do
    {
         
        cout << "Adding New Client:\n\n";

        addNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> addMore;

    } while (toupper(addMore) == 'Y');

}

void addNewUsers()
{
    char addMore = 'Y';
    do
    {
        
        cout << "Adding New User:\n\n";

        addNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";


        cin >> addMore;

    } while (toupper(addMore) == 'Y');

}

bool deleteClientByAccountNumber(string accountNumber, vector <stClient>& vClients)
{



    stClient client;
    char answer = 'n';

    if (findClientByAccountNumber(accountNumber, vClients, client))
    {

        printClientCard(client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            markClientForDeleteByAccountNumber(accountNumber, vClients);
            saveCleintsDataToFile(clientsFileName, vClients);

             
            vClients = loadCleintsDataFromFile(clientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << accountNumber << ") is Not Found!";
        return false;
    }

}

bool deleteUserByUsername(string username, vector <stUser>& vUsers)
{

    if (username == "Admin")
    {
        cout << "\n\nYou cannot Delete This User.";
        return false;

    }

    stUser user;
    char answer = 'n';

    if (findUserByUsername(username, vUsers, user))
    {

        printUserCard(user);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {

            markUserForDeleteByUsername(username, vUsers);
            saveUsersDataToFile(usersFileName, vUsers);

             
            vUsers = loadUsersDataFromFile(usersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Username (" << username << ") is Not Found!";
        return false;
    }

}

bool updateClientByAccountNumber(string accountNumber, vector <stClient>& vClients)
{

    stClient client;
    char answer = 'n';

    if (findClientByAccountNumber(accountNumber, vClients, client))
    {

        printClientCard(client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {

            for (stClient& c : vClients)
            {
                if (c.accountNumber == accountNumber)
                {
                    c = changeClientRecord(accountNumber);
                    break;
                }

            }

            saveCleintsDataToFile(clientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << accountNumber << ") is Not Found!";
        return false;
    }

}

bool updateUserByUsername(string username, vector <stUser>& vUsers)
{

    stUser user;
    char answer = 'n';

    if (findUserByUsername(username, vUsers, user))
    {

        printUserCard(user);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {

            for (stUser& U : vUsers)
            {
                if (U.userName == username)
                {
                    U = changeUserRecord(username);
                    break;
                }

            }

            saveUsersDataToFile(usersFileName, vUsers);

            cout << "\n\nUser Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nUser with Account Number (" << username << ") is Not Found!";
        return false;
    }

}

bool depositBalanceToClientByAccountNumber(string accountNumber, double amount, vector <stClient>& vClients)
{
    char answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {

        for (stClient& c : vClients)
        {
            if (c.accountNumber == accountNumber)
            {
                c.accountBalance += amount;
                saveCleintsDataToFile(clientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << c.accountBalance;

                return true;
            }

        }


        return false;
    }

}

string readClientAccountNumber()
{
    string accountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> accountNumber;
    return accountNumber;

}

string readUserName()
{
    string username = "";

    cout << "\nPlease enter Username? ";
    cin >> username;
    return username;

}

void showListUsersScreen()
{

    showAllUsersScreen();

}

void showAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    addNewUsers();


}

void showDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = loadUsersDataFromFile(usersFileName);

    string username = readUserName();
    deleteUserByUsername(username, vUsers);

}

void showUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = loadUsersDataFromFile(usersFileName);
    string username = readUserName();

    updateUserByUsername(username, vUsers);
}

void showDeleteClientScreen()
{


    if (!checkAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        showAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);
    string accountNumber = readClientAccountNumber();
    deleteClientByAccountNumber(accountNumber, vClients);

}

void showUpdateClientScreen()
{
    if (!checkAccessPermission(enMainMenuePermissions::pUpdateClients))
    {
        showAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);
    string accountNumber = readClientAccountNumber();
    updateClientByAccountNumber(accountNumber, vClients);

}

void showAddNewClientsScreen()
{

    if (!checkAccessPermission(enMainMenuePermissions::pUpdateClients))
    {
        showAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    addNewClients();

}

void showFindClientScreen()
{


    if (!checkAccessPermission(enMainMenuePermissions::pFindClient))
    {
        showAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);
    stClient client;
    string accountNumber = readClientAccountNumber();
    if (findClientByAccountNumber(accountNumber, vClients, client))
        printClientCard(client);
    else
        cout << "\nClient with Account Number[" << accountNumber << "] is not found!";

}

void showFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = loadUsersDataFromFile(usersFileName);
    stUser user;
    string username = readUserName();
    if (findUserByUsername(username, vUsers, user))
        printUserCard(user);
    else
        cout << "\nUser with Username [" << username << "] is not found!";

}
 
void showDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    stClient client;

    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);
    string accountNumber = readClientAccountNumber();


    while (!findClientByAccountNumber(accountNumber, vClients, client))
    {
        cout << "\nClient with [" << accountNumber << "] does not exist.\n";
        accountNumber = readClientAccountNumber();
    }


    printClientCard(client);

    double amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> amount;

    depositBalanceToClientByAccountNumber(accountNumber, amount, vClients);

}

void showWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    stClient client;

    vector <stClient> vClients = loadCleintsDataFromFile(clientsFileName);
    string accountNumber = readClientAccountNumber();


    while (!findClientByAccountNumber(accountNumber, vClients, client))
    {
        cout << "\nClient with [" << accountNumber << "] does not exist.\n";
        accountNumber = readClientAccountNumber();
    }

    printClientCard(client);

    double amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> amount;

    
    while (amount > client.accountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << client.accountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> amount;
    }

    depositBalanceToClientByAccountNumber(accountNumber, amount * -1, vClients);

}

void showTotalBalancesScreen()
{

    showTotalBalances();

}

bool checkAccessPermission(enMainMenuePermissions Permission)
{
    if (currentUser.permissions == enMainMenuePermissions::eAll)
        return true;

    if ((Permission & currentUser.permissions) == Permission)
        return true;
    else
        return false;

}

void goBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    showMainMenue();
}

void goBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    showTransactionsMenue();

}

void goBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    showManageUsersMenue();

}

short readTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short choice = 0;
    cin >> choice;

    return choice;
}

void perfromTranactionsMenueOption(enTransactionsMenueOptions transactionMenueOption)
{
    switch (transactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        showDepositScreen();
        goBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        showWithDrawScreen();
        goBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        showTotalBalancesScreen();
        goBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        showMainMenue();

    }
    }

}

void showTransactionsMenue()
{

    if (!checkAccessPermission(enMainMenuePermissions::pTranactions))
    {
        showAccessDeniedMessage();
        goBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    perfromTranactionsMenueOption((enTransactionsMenueOptions)readTransactionsMenueOption());
}

short readMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short choice = 0;
    cin >> choice;

    return choice;
}

void perfromManageUsersMenueOption(enManageUsersMenueOptions manageUsersMenueOption)
{
    switch (manageUsersMenueOption)
    {
    case enManageUsersMenueOptions::eListUsers:
    {
        system("cls");
        showListUsersScreen();
        goBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eAddNewUser:
    {
        system("cls");
        showAddNewUserScreen();
        goBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eDeleteUser:
    {
        system("cls");
        showDeleteUserScreen();
        goBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eUpdateUser:
    {
        system("cls");
        showUpdateUserScreen();
        goBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eFindUser:
    {
        system("cls");

        showFindUserScreen();
        goBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eMainMenue:
    {
        showMainMenue();
    }
    }

}

short readManageUsersMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short choice = 0;
    cin >> choice;

    return choice;
}

void showManageUsersMenue()
{

    if (!checkAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        showAccessDeniedMessage();
        goBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";


    perfromManageUsersMenueOption((enManageUsersMenueOptions)readManageUsersMenueOption());
}

void perfromMainMenueOption(enMainMenueOptions mainMenueOption)
{
    switch (mainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        showAllClientsScreen();
        goBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        showAddNewClientsScreen();
        goBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        showDeleteClientScreen();
        goBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        showUpdateClientScreen();
        goBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        showFindClientScreen();
        goBackToMainMenue();
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        showTransactionsMenue();
        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        showManageUsersMenue();
        break;

    case enMainMenueOptions::eExit:
        system("cls");

        login();

        break;
    }

}

void showMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";


    perfromMainMenueOption((enMainMenueOptions)readMainMenueOption());
}

bool  loadUserInfo(string username, string password)
{

    if (findUserByUsernameAndPassword(username, password, currentUser))
        return true;
    else
        return false;

}

void login()
{
    bool loginFaild = false;

    string username, password;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (loginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> username;

        cout << "Enter Password? ";
        cin >> password;

        loginFaild = !loadUserInfo(username, password);

    } while (loginFaild);

    showMainMenue();

}

int main()

{
    login();

    system("pause>0");
    return 0;
}