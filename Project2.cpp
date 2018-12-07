// name: Drew Waller, dgw0015
// partner: Daniel Clayton, dic0001
// group name: _ _proj02 5
// filename: Project2.cpp
// due date: 12/07/2018
/* collaboration: Project 2, in this project we are designing and iplementing a system for a secure teller terminal. The program has an idle state and an active state.
                  Either an administrator or Branch Staff member logins in, causing the system to change from idle to active. The Administrator and Staff member have
                  separate actions that they can execute, thus they have there own classes and methods. Within these classes we will be adding clients, and their account
                  information(e.g. types of accounts and the funds within them). Clients, accounts, and users will be all structs. The program uses a BankDataFiles class
                  to save the users lists, clients info, and accounts info. Once th user is done in the system they will logout or exit
                  write all clients info to a file called (client-info.txt). All account info will be written to a file called (account-info.txt)  */

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <ctime>
using namespace std;

//#define UNIT_TESTING //used for the testing class of the program.

//Global variables.
const int MAX_MEMBERS = 100;

//Structure to house the information of each user when stored into an array of users.
struct User   {
   string username;
   string password;
   string role;
};

//Structure to house the information of each Client when stored into an array of Clients.
struct Client  {
   string name;
   string address;
   string phoneNum;
   string ssn;
   string employer;
   string annual_income;
};

//Structure to house the information of each Account when stored into an array of accounts.
struct Account {
   string acctName;
   string acctNumber;
   string acct_type;
   string balance;
};


// class to control the active and idle state of the teller system.
class System_status  {

   private:
   int s; //represents state of the terminal 
   
   public:
   System_status(); //Constructor
   void P();      //busy.
   void S();      //release signal.
   int check_status();
   };
   
System_status::System_status()   {

   s = 0; 
}

//Changes the status to 1 - representing system in use.   
void System_status:: P() {

   s = s + 1;
}

//Changes the status back to 0 - releasing the system in use.
void System_status:: S()   {

   s = s - 1;
}

//Returns the current system status.
int System_status:: check_status() {

   return s;
}

   
//This class handles all the saved system data and will keep track of the clients information, account information, the system users, deposits,
//withdrawals,
class BankDataFiles  {

private:
   Client clientsList[MAX_MEMBERS];
   Account acctsList[MAX_MEMBERS];
   User usersList[MAX_MEMBERS];
   

public:
   User staffList[MAX_MEMBERS];
   bool check_isAdmin(string username, string pswd);
   bool check_isStaff(string username, string pswd);
   bool empty_line(string line);
   bool check_if_userExists(string nameIn);
   bool check_acctNames_database(string acct_nameIn);
   bool check_clientName_database(string clientNameIn);
   bool check_acctNumber(string acct_numberIn);
   string get_clientsName(string acct_numberIn);
   void make_deposit(string acct_numberIn, int amt);
   void make_withdrawal(string acct_number, int amt);
   void print_usersList();
   void print_clientInfo(string nameIn);
   void update_client(string client_nameIn);
   void load_clientList();
   void load_acctsList();
   void load_usersList();
   void extract_staffList();
   void delete_from_usersList(string usernameIn);
   void update_user_pswd(string usernameIn, string pswdIn);
   void add_to_usersList(User userIn);
   void add_to_clientsList(Client client);
   void add_to_acctsList(Account acct);
   void write_file_clientsList();
   void write_file_acctsList();
   void write_file_usersList();
  
};

//checks if the line from the file is empty when reading in each of the three Structure arrays
bool BankDataFiles:: empty_line(string line) {

   return (line.find_first_not_of(' ') == line.npos); // reads the line when called and find the first instance of a non character type.
}

//loads the saved client-info.txt list.
void BankDataFiles:: load_clientList() {

   ifstream fileIn;
   int i;

   fileIn.open("client_info.txt");
   if (fileIn.fail())   {
      cout << "\nThe current Clients list could not be loaded from a file!" << endl;
      return;
   }

   for (i = 0; i < MAX_MEMBERS; i++)  {
      getline(fileIn, clientsList[i].name);
      
      if (empty_line(clientsList[i].name))   {
         clientsList[i].name = "";
      }
      getline(fileIn, clientsList[i].phoneNum);
      getline(fileIn, clientsList[i].address);
      getline(fileIn, clientsList[i].ssn);
      getline(fileIn, clientsList[i].employer);
      getline(fileIn, clientsList[i].annual_income);
   }
  
   fileIn.close();
}

//loads the saved account-info.txt list.
void BankDataFiles:: load_acctsList()  {

   ifstream fileIn;
   int i;

   fileIn.open("account_info.txt");
   if (fileIn.fail())   {
      cout << "\nThe current Accounts list information could not be loaded from a file!" << endl;
      return;
   }
   
   for (i = 0; i < MAX_MEMBERS; i++)   {
      getline(fileIn, acctsList[i].acctName);
      
      if (empty_line(acctsList[i].acctName))   {
         acctsList[i].acctName = "";
      }
      
      getline(fileIn, acctsList[i].acctNumber);
      getline(fileIn, acctsList[i].acct_type);
      getline(fileIn, acctsList[i].balance);
   }
   
   fileIn.close();
}

//loads the saved users from the staff.txt list.
void BankDataFiles:: load_usersList()  {

   ifstream fileIn;
   int i;

   fileIn.open("staff.txt");

   if (fileIn.fail())   {
      cout << "Error: The current staff members list could not be loaded from file!!" << endl;
   
      return;
   }
   
   for (i = 0; i < MAX_MEMBERS; i++)   {
      getline(fileIn, usersList[i].username);
      
      
      if (empty_line(usersList[i].username))   {
         usersList[i].username = "";
      }
      
      getline(fileIn, usersList[i].password);
      getline(fileIn, usersList[i].role);
   
   }
   fileIn.close();
}

//validates admin users identity.
bool BankDataFiles:: check_isAdmin(string username, string pswd)  {

   bool nameCheck;
   bool pswdCheck;
   string name;
   string pass;
   string r;
   int i;

   for (i = 0; i < MAX_MEMBERS; i++) {
      name = usersList[i].username;
      pass = usersList[i].password;
      r = usersList[i].role;
      
      if ((name == username) && (pass == pswd))  {
         if (r == "System Administrator") {
            nameCheck = true;
            pswdCheck = true;
         }
      }
   }
     
   if (nameCheck == true)   {
      if (pswdCheck == true)  {
      
         return true;
      }
   }
   else  {
      return false;
   }

}

//Takes all of the staff members and creates a list for easy printing elsewhere.
void BankDataFiles:: extract_staffList() {
   int i;
   
   for (i = 0; i < MAX_MEMBERS; i++) {
      if (usersList[i].role == "System Administrator" || usersList[i].role == "Staff") {
         staffList[i].username = usersList[i].username;
         staffList[i].password = usersList[i].password;
         staffList[i].role = usersList[i].role;
      }
   }
}

bool BankDataFiles:: check_isStaff(string username, string pswd)  {

   bool nameCheck = false;
   bool pswdCheck = false;
   int i;

   for (i = 0; i < MAX_MEMBERS; i++) {
      if (usersList[i].username == username) {
         nameCheck = true;
      }
      if (usersList[i].password == pswd)   {
         pswdCheck = true;
      }
   }
     
   if (nameCheck == true)   {
      if (pswdCheck == true)  {
      
         return true;
      }
   }
   else  {
      
      return false;
   }

}

//Check the Users list to see if a new user being added already exists.
bool BankDataFiles:: check_if_userExists(string nameIn)  {

   for (int i = 0; i < MAX_MEMBERS; i++)   {
      if (nameIn == usersList[i].username) {
         return true;
      }
   }
      
   return false;
}

//Adds a User structure to the usersList array.
void BankDataFiles:: add_to_usersList(User userIn)  {

   int list_size = 1; // equals one for the default system administrator.

   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (usersList[i].username != "") {
         list_size++; //will count the number of non-empty elements in the array. 
      }
   }
   //checks to see if the list is full.
   if (list_size >= 99) {
      
      cout << "ERROR: You the users database is full. You must delete old users to add new ones.\n" << endl;
   }
   usersList[list_size] = userIn;
   
}

//checks to see if the Accounts list to see if the acct name 
bool BankDataFiles:: check_acctNames_database(string acct_nameIn)  {
 
   for (int i = 0; i < MAX_MEMBERS; i++) {
      if (acctsList[i].acctName == acct_nameIn)   {
      
         return true;
      }
      else  {
         return false;
      }
   }
 
}
 
 //checks the client database for to see if the clients name already exists
bool BankDataFiles:: check_clientName_database(string clientNameIn) {
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (clientsList[i].name == clientNameIn) {
      
         return true;
      }
      else  {
         return false;
      }
   }
   
}
   
   //checks the Account list array to see if the account number in already exists.
bool BankDataFiles:: check_acctNumber(string acct_numberIn) {
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (acctsList[i].acctNumber == acct_numberIn)   {
      
         return true;
      }
      else  {
         return false;
      }
   }
   
}
   
   //returns a clients name from the Clients list array
string BankDataFiles:: get_clientsName(string acct_numberIn)   {
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (acctsList[i].acctNumber == acct_numberIn)   {
      
         return acctsList[i].acctName;
      }
      else {
         return "DOES NOT EXIST";
      }
   }
   
}
   
   //method used for naking deposits in a clients Account.
   //We also use the function atoi and c_str(). atoi converts a string to integer and c_str() gives a c string equivalent.
void BankDataFiles:: make_deposit(string acct_numberIn, int amt)  {
   
   int total_balance;
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (acctsList[i].acctNumber == acct_numberIn)   {
         total_balance = atoi(acctsList[i].balance.c_str());
         total_balance = total_balance + amt;
         acctsList[i].balance = static_cast<ostringstream*>(&(ostringstream() << total_balance))->str();
         
         cout << "Account Number: " << acctsList[i].acctNumber << " has a balance of: " << acctsList[i].balance << "\n" << endl;
      }
   } 
   
}
   
   //Function that performs the withdrawals from clients accounts
void BankDataFiles:: make_withdrawal(string acct_numberIn, int amt) {
   
   int total_balance;
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (acctsList[i].acctNumber == acct_numberIn)   {
         total_balance = atoi(acctsList[i].balance.c_str());
         total_balance = total_balance - amt;
         
         if (total_balance < 0)  {  
            cout << "ERROR: This withdrawal amount exceeds the balance." << endl;
         }
           
         acctsList[i].balance = static_cast<ostringstream*>(&(ostringstream() << total_balance))->str();
         cout << "Account Number: " << acctsList[i].acctNumber << " has a balance of: " << acctsList[i].balance << "\n" << endl;
         
      }
   } 

}
   
//Print the list of Users from the Users list array
void BankDataFiles:: print_usersList() {
   
   int number_of_users = 0;
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (empty_line(usersList[i].username) && empty_line(usersList[i].password))   {
         break;
      }
      number_of_users++;
   }
   cout << "The total number of users is the system is: " << number_of_users << endl;
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (i == 0) {
         cout << "\t1) UserName: admin \t Role: System Administrator";
      }
      cout << "\n\t" << (i + 1) << ") Username: " << usersList[i].username << "\t Role: " << usersList[i].role; 
   }
   cout << endl;
}
   
//prints the information from the client information database
void BankDataFiles:: print_clientInfo(string nameIn)   {
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (clientsList[i].name == nameIn)  {
         cout << "\tAddress: " << clientsList[i].address << endl;
         cout << "\tPhone number: " << clientsList[i].phoneNum << endl;
         cout << "\tSocial security number: " << clientsList[i].ssn << endl;
         cout << "\tEmployer: " << clientsList[i].employer << endl;
         cout << "\tAnnual income: " << clientsList[i].annual_income << endl;
      }
   }  
}
   
//updates the clients information in the database.
void BankDataFiles:: update_client(string client_nameIn)   {
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (clientsList[i].name == client_nameIn)   {
         cout << "Please fill out all your new information to be updated: " << endl;
         cout << "Phone number: ";
         cin >> clientsList[i].phoneNum;
         cout << "\nAddress: ";
         cin >> clientsList[i].address;
         cout << "\nSocial security number: ";
         cin >> clientsList[i].ssn;
         cout << "\nEmployer: ";
         cin >> clientsList[i].employer;
         cout << "\nAnnual income: ";
         cin >> clientsList[i].annual_income;
         cout << "Client " << clientsList[i].name << "'s information has been updated!" << endl;
      }
   }
}
   
//deletes a user from the Users array.
void BankDataFiles:: delete_from_usersList(string usernameIn)   {
      
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (usersList[i].username == usernameIn)  {
         usersList[i] = User();
      
         for (int a = i + 1; a < MAX_MEMBERS; a++) {
            usersList[a - 1].username = usersList[a].username;
            usersList[a - 1].password = usersList[a].password;
            usersList[a - 1].role = usersList[a].role;
         }
      }
   }
      
}
      
//Updated the users password in the database
void BankDataFiles:: update_user_pswd(string usernameIn, string pswdIn) {
   
   if (usernameIn == "admin") {
      usersList[0].password = pswdIn;
   }
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (usersList[i].username == usernameIn) {
         usersList[i].password = pswdIn;
      }
   }
   
}
   
//adds Clients the the clientsList array
void BankDataFiles:: add_to_clientsList(Client client)   {
   
   int list_size;
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      if (clientsList[i].name != "")   {
         list_size++;
      }
   }
      
   if (list_size >= 99) {
      cout << "ERROR: The clients list is full!! You must delete old users to add new ones.\n" << endl;
   }
      
   clientsList[list_size] = client;
   cout << "A new client has been added!\n" << endl;
}
   
//adds a client to the clients list array
void BankDataFiles:: add_to_acctsList(Account acct)   {
   
   int list_size;
   
   for (int i = 0; i < MAX_MEMBERS; i++) {
      if (acctsList[i].acctName != "") {
         list_size++;
      }
   }
   
   if (list_size >= 99) {
      cout << "ERROR: The account list database is full! You must delete old accounts to add new ones.\n" << endl;
   }
   
   acctsList[list_size] = acct;
   cout << "A new account has been added!\n" << endl;
   
}
   
//writes the client list to the (clients-info.txt) file
void BankDataFiles:: write_file_clientsList()   {
   
   ofstream fileWriter;
   fileWriter.open("account-info.txt");
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      fileWriter << clientsList[i].name << endl;
      fileWriter << clientsList[i].phoneNum << endl;
      fileWriter << clientsList[i].address << endl;
      fileWriter << clientsList[i].ssn << endl;
      fileWriter << clientsList[i].employer << endl;
      fileWriter << clientsList[i].annual_income << endl;
   }
      
   fileWriter.close();
   
}
   
//writes the acctsList array to the (account_info.txt) file
void BankDataFiles:: write_file_acctsList()  {
   
   ofstream fileWriter;
   fileWriter.open("account-info.txt");
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      fileWriter << acctsList[i].acctName << endl;
      fileWriter << acctsList[i].acctNumber << endl;
      fileWriter << acctsList[i].acct_type << endl;
      fileWriter << acctsList[i].balance << endl;
   }
   fileWriter.close();
   
}
   
//writes the usersList array to the (staff.txt) file
void BankDataFiles:: write_file_usersList()  {
   
   ofstream fileWriter;
   fileWriter.open("staff.txt");
   
   for (int i = 0; i < MAX_MEMBERS; i++)  {
      fileWriter << usersList[i].username << endl;
      fileWriter << usersList[i].password << endl;
      fileWriter << usersList[i].role << endl;
   }
   fileWriter.close();
   
}



//class for the branch staff members, will show the option they have in the system as well as allow them to edi client information.
class Branch_staff   {

private:
   string username;
   string password;
   string role;
   Client client;
   Account acct;
   BankDataFiles bank_files;
   System_status system;

public:
   Branch_staff(); //Default constructor.
   void staff_options();
   void set_username(string nameIn);
   void set_password(string pswdIn);
   void set_role(string roleIn);
   string get_username();
   string get_password();
   string get_role();
   void acct_management();
   void change_pswd();

};

//default constructor implementation
Branch_staff:: Branch_staff() {

   username = "";
   password = "";
   role = "";
}

//sets the username.
void Branch_staff:: set_username(string nameIn) {

   username = nameIn;
}

//sets the password
void Branch_staff:: set_password(string pswdIn) {

   password = pswdIn;
}

//Sets the users role.
void Branch_staff:: set_role(string roleIn)   {

   role = roleIn;
}

//Returns the username.
string Branch_staff:: get_username()   {

   return username;
}

//Returns the password.
string Branch_staff:: get_password()   {

   return password;
}

//Returns the users role.
string Branch_staff:: get_role() {

   return role;
}

//Will display the staff options and take in the users input.
void Branch_staff:: staff_options() {
   
   system.P();
   int staff_selection;
   
   cout << "\n\t\t=======================================================" << endl;
   cout << "\t\t|   Teller Terminal System - Branch Staff Member      |\n";
   cout << "\t\t=======================================================" << endl;
   cout << "1)\tClient and Account Management" << endl;
   cout << "2)\tChange password" << endl;
   cout << "3)\tExit" << endl;
   cout << "\t\tPlease choose an option: ";
   cin >> staff_selection;
   
   if (staff_selection == 1)  {
      acct_management();
   }
   else if (staff_selection == 2)   {
      change_pswd();
   }
   else if (staff_selection == 3)   {
      system.S();
      return;
   }
   else  {
      cout << "Your option was invalid! Please select a valid option.\n" << endl;
      
      staff_options();
   }
   
}

//Diplays the options within the account management portal.
void Branch_staff:: acct_management()  {
   system.P();

   int optionIn;
   int subMenuNum;
   int depositAmount;
   int withdrawalAmount;
   string accountNum;
   string clientName;
   bool clientExists;
   bool accountExists;

   cout << "\n\t\t=========================================================" << endl;
   cout << "\t\t| Teller Terminal System - Client and Account Management |" << endl;
   cout << "\t\t=========================================================" << endl;
   cout << "1) Add a client" << endl;
   cout << "2) Add an account" << endl;
   cout << "3) Edit client information" << endl;
   cout << "4) Manage an account" << endl;
   cout << "5) Save client and account information" << endl;
   cout << "6) Exit" << endl;
   cout << "Please choose an option: ";
   cin >> optionIn;
   
   switch (optionIn) {
      case 1:
         cout << "A new client will be added: " << endl;
         cout << "Client name: ";
         cin >> client.name;
         cout << "Address: ";
         cin >> client.address;
         cout << "Phone Number: ";
         cin >> client.phoneNum;
         cout << "Social Security Number: ";
         cin >> client.ssn;
         cout << "Employer: ";
         cin >> client.employer;
         cout << "Annual income: ";
         cin >> client.annual_income;
         
         bank_files.add_to_clientsList(client);
         break;
      case 2:
         cout << "Choose a client: ";
         cin >> clientName;
         clientExists = bank_files.check_clientName_database(clientName);
         if (clientExists) {
            cout << "A new account will be created for " << clientName << "..." << endl;
            cout << "Account number: ";
            cin >> acct.acctNumber;
            acct.acctName = clientName;
            cout << "Account type: ";
            cin >> acct.acct_type;
            cout << "Balance: ";
            cin >> acct.balance;
            
            bank_files.add_to_acctsList(acct);   
         }
         else {
            cout << "Error - The client is not in the system!" << endl;
         }
         break;
      case 3:
         cout << "Choose a client: ";
         cin >> clientName;
         clientExists = bank_files.check_clientName_database(clientName);
         if (clientExists) {
            cout << "Display " << clientName << "'s information" << endl;
            bank_files.print_clientInfo(clientName);
            cout << "Client " << clientName << "'s infomation will be updated." << endl;
            cout << "1) Confirm" << endl;
            cout << "2) Cancel" << endl;
            cout << "Please choose an option: ";
            cin >> subMenuNum;
            if (subMenuNum == 1) {
               bank_files.update_client(clientName);
            }
         }
         else {
            cout << "Error - The client is not in the system!" << endl;
         }
         break;
      case 4:
         cout << "Which account will be managed?";
         cin >> accountNum;
         accountExists = bank_files.check_acctNumber(accountNum);
         if (accountExists) {
            cout << "Manage account " << accountNum << "..." << endl;
            cout << "1) Deposit" << endl;
            cout << "2) Withdraw" << endl;
            cout << "3) Cancel" << endl;
            cout << "Please choose an option: ";
            cin >> subMenuNum;
            if (subMenuNum == 1) {
               cout << "Deposit amount: ";
               cin >> depositAmount;
               bank_files.make_deposit(accountNum, depositAmount);
            }
            else if (subMenuNum == 2) {
               cout << "Withdraw amount: ";
               cin >> withdrawalAmount;
               bank_files.make_withdrawal(accountNum, withdrawalAmount);
            }
         }
         else {
            cout << "Error - The account is not in the system!" << endl;
         }
         break;
      case 5:
         bank_files.write_file_clientsList();
         bank_files.write_file_acctsList();
         cout << "Client information has been saved in the client-info file; account information has been saved in the account-info file!";
         break;
      default:
         break;
   }

   system.S();

}

//Allows the branch or admin users to change their passwords.
void Branch_staff:: change_pswd()   {
   string passIn;
   cout << "What would you like to change the password to? ";
   cin >> passIn;
   set_password(passIn);
}



// The admin class inherits everything from branch staff member and then has addition function that only a administrator can perform. 
class Admin : public Branch_staff   {

private:
   BankDataFiles bank_files;
   User user;
   System_status system;
   string username;
   string password;
   string role;


public:
   Admin(); //Constructor
   void admin_options();
   void add_user();
   void delete_user();
   void print_staffList();

};

//implementation of constructor.
Admin::Admin() {

   username = "";
   password = "";
   role = "";
}

//Displays the admin optionsand reads in the users selection.
void Admin:: admin_options()  {
    
   system.P();      
   int admin_selection;

   cout << "\n\t\t=======================================================" << endl;
   cout << "\t\t|   Teller Terminal System - System Administrator      |\n";
   cout << "\t\t=======================================================" << endl;
   cout << "1)\tClient and Account Management" << endl;
   cout << "2)\tAdd a branch staff member" << endl;
   cout << "3)\tDelete a branch staff member" << endl;
   cout << "4)\tDisplay branch staff" << endl;
   cout << "5)\tChange password" << endl;
   cout << "6)\tExit" << endl;
   cout << "\t\tPlease choose and option: ";
   cin >> admin_selection;
   
   if (admin_selection == 1) {
      acct_management();
   }
   else if (admin_selection == 2)   {
      add_user();
   }
   else if (admin_selection == 3) {
      delete_user();
   }
   else if (admin_selection == 4)   {
      print_staffList();
   }
   else if (admin_selection == 5)   {
      change_pswd();
   }
   else if (admin_selection == 6) {
      system.S();
      return;
   }
   else {
      cout << "Your option was invalid! Please select a valid option.\n" << endl;
      admin_options();
   }
         

}
 //Adds a new user to the teller terminal system.
void Admin:: add_user() {
   
   string nameIn;
   string pswdIn;
   string roleIn;
   int role;
   int confirm;
   
   cout << "Please enter the new Users username: ";
   cin >> nameIn;
   
   if (bank_files.check_if_userExists(nameIn) == true)   {
      cout << "ERROR: This User already exists in the system. Re-routing to main option screen\n" << endl;
      admin_options();
   }
   
   cout << "Enter password: ";
   cin >> pswdIn;
   cout << "User Role\t(1 - Administrator; 2 - Branch Staff): ";
   cin >> role;
   
   while ((role != 1) && (role != 2))  {
      cout << "Invalid selection! Please select 1 for Administrator or 2 for Branch staff: "; 
      cin >> role;
   } 
   
   if (role == 1) {
      roleIn = "System Administrator";
   }
   if (role == 2) {
      roleIn = "Branch Staff";
   }   
   user.username = nameIn;
   user.password = pswdIn;
   user.role = roleIn;
   cout << "Press 1 to Confirm and add the User, 0 to cancel: ";
   cin >> confirm;
   
   if (confirm == 1) {
      if (role == 1) {
         bank_files.add_to_usersList(user);
         cout << " A new system administrator is added!" << endl;
         cout << "User name: " << user.username << "Role: " << user.role << endl;
      }
      if (role == 2) {
         bank_files.add_to_usersList(user);
         cout << " A new branch staff member is added!" << endl;
         cout << "User name: " << user.username << "Role: " << user.role << endl;
      }
        
      cout << "\nPress any key to continue...";
      cin.ignore().get(); 
   }
   else  {
      cout << "Cancelled! Re-routing to the administrator options screen." << endl;
      admin_options();
   }

}
   
   //Delets a current user from the teller terminal system.
void Admin:: delete_user()   {
   string userIn;
   bool userExists;
   
   cout << "Please enter a username to delete: ";
   cin >> userIn;
   userExists = bank_files.check_if_userExists(userIn);
   if (userExists) {
      bank_files.delete_from_usersList(userIn);
      cout << "User " << userIn << " successfully removed from database." << endl;
      cout << "\nPress any key to continue...";
      cin.ignore().get(); 
   }
   else {
      cout << "Provided user does not exist!  Returning to main menu. " << endl;
      admin_options();
   }
   
   
}
   
   //Prints the currents staff/user list.
void Admin:: print_staffList()  {
   int i = 0;
   
   cout << "Printing staff list..." << endl;
   for (i = 0; i < MAX_MEMBERS; i++) {
      cout << bank_files.staffList[i].username << " " << bank_files.staffList[i].password << " " << bank_files.staffList[i].role << endl;
   }
   
   
}


    
class LoginMenu   {

private:
   BankDataFiles bank_files;
   System_status system;
   Branch_staff staff;
   Admin admin;

public:
   LoginMenu(); //Constructor.
   void displayHome();
   void accessTerminal();

};

//Constructor implementation.
LoginMenu::LoginMenu()  {

// initializes the array lists from multiple txt files.
   bank_files.load_usersList(); 
   bank_files.load_acctsList();
   bank_files.load_clientList(); 
   displayHome();
}


//Main home page to the teller terminal.
void LoginMenu:: displayHome()   {

   int userIn;
   
   cout << "\n\t\t=======================================================" << endl;
   cout << "\t\t|    Welcome to the Auburn Branch of Tiger Bank!      |\n";
   cout << "\t\t=======================================================" << endl;
   cout << "1)\tLogin" << endl;
   cout << "2)\tQuit" << endl;
   cout << "\t\tPlease select from options above: ";
   cin >> userIn;
   
   if (userIn == 1)  {
      accessTerminal();
   }
   else if (userIn == 2)  {
      exit(0);
   }
   else {
      displayHome();
   }
 
   
}

//Handles the username and password proccesses.
void LoginMenu:: accessTerminal()   {

   //local variables.
   string usernameIn;
   string pswdIn;
   
   cout << "\n\t\t=======================================================" << endl;
   cout << "\t\t|    Login to Access the Teller Terminal System      |\n";
   cout << "\t\t=======================================================" << endl;
   cout << "Username: ";
   cin >> usernameIn;
   cout << "Password: ";
   cin >> pswdIn;
   
   if (bank_files.check_isAdmin(usernameIn, pswdIn)) {
      admin = Admin();
      admin.set_username(usernameIn);
      admin.set_password(pswdIn);
      admin.admin_options();
   }
   else if (bank_files.check_isStaff(usernameIn, pswdIn)) {
      staff = Branch_staff();
      staff.set_username(usernameIn);
      staff.set_password(pswdIn);
      staff.staff_options();
   }
   else  {
      cout << "Username or password was incorrect, please try again." << endl;
      displayHome(); //recursively loops to allow user to try again from front screen.
   }

}


//Testing class, used to test the class functions of the program.
class Testing  {

   private:
   LoginMenu menu;
   
   public:
   bool result;

};


//PROGRAM DRIVER.
int main()  {
   
   #ifdef UNIT_TESTING
   Testing test;
   #endif
   System_status system;
   LoginMenu menu;
   int idle = 2;
   
   while (idle = 2)  {
   
      if (system.check_status() == 0) {
         menu.displayHome();
      }
      if (system.check_status() == 1) {
         cout << "Sorry this terminal is currently already logged into." << endl;
      }
      
   }



   return 0;

}