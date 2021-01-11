/**
*
* Solution to course project # 3
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2020/2021
*
* @author Danail Dimitrov
* @idnumber 62605
* @compiler VC
*
* main cpp file
*
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const char* FILENAME = "users.txt";

void mainMenu(fstream& file);

string hashPassword(string password);

bool hasExistingUsername(fstream& file, string username);

bool isValidPassword(string password, string confirmPassword);

void login(fstream& file);

void regist(fstream& file);

struct User
{
    string username, password;

    double money=0.0;

    const double OVERDRAFT = -10000.0;

    User(string username, string password, fstream& file)
    {
        this->username = username;

        this->password = password;

        money = fixDouble(getUserMoney(file));
    }

    User()
    {

    }

    bool isOverdraft(const double amount)
    {
        if (money < (OVERDRAFT + amount))
        {
            return true;
        }

        return false;
    }

    double fixDouble(double number)
    {
        return (double)((int)(number * 100 )) / 100.0;
    }

    void saveChanges(fstream& file)
    {
        if (file.peek() == EOF)
        {
            file << username + ":" + hashPassword(password) + ":" << money << endl;
        }

        else
        {
            string fileLine;

            ofstream edit("edit.txt");

            while (getline(file, fileLine))
            {
                if (fileLine.substr(0, fileLine.find(":")) == username)
                {
                    size_t pos = fileLine.find_last_of(':');

                    edit << fileLine.substr(0, pos) + ":" << money << endl;
                }
                else
                {
                    edit << fileLine << endl;
                }
            }

            edit.close();
            file.close();

            remove(FILENAME);

            rename("edit.txt", FILENAME);

            file.open(FILENAME);
        }
    }

    void saveChanges(fstream& file, User secondUser)
    {
            string fileLine;

            ofstream edit("edit.txt");

            while (getline(file, fileLine))
            {
                string fileUsername = fileLine.substr(0, fileLine.find(":"));

                if (fileUsername == username)
                {
                    size_t pos = fileLine.find_last_of(':');

                    edit << fileLine.substr(0, pos) + ":" << money << endl;
                }

                else if (fileUsername == secondUser.username)
                {
                    size_t pos = fileLine.find_last_of(':');

                    edit << fileLine.substr(0, pos) + ":" << secondUser.money << endl;
                }

                else
                {
                    edit << fileLine << endl;
                }
            }

            edit.close();
            file.close();

            remove(FILENAME);

            rename("edit.txt", FILENAME);

            file.open(FILENAME);
    }

    double getUserMoney(fstream& file)
    {
        string fileLine;

        while (getline(file, fileLine))
        {
            if (fileLine.compare(0, username.size(), username) == 0)
            {
                size_t pos = fileLine.find_last_of(':') + 1;

                double money = stod(fileLine.substr(pos));

                file.clear();
                file.seekg(0, file.beg);

                return money;
            }
        }

        file.clear();
        file.seekg(0, file.beg);

        return 0.0;
    }
};

void actionMenu(fstream& file, User& user);

void deleteUser(fstream& file, User& user);

string hashPassword(string password)
{
    hash<string> hashedPassword;

    return to_string(hashedPassword(password));
}

bool hasExistingUsername(fstream& file, const string username)
{
    string fileLine;

    while (getline(file, fileLine))
    {
        if (fileLine.compare(0, username.size(), username) == 0)
        {
            file.clear();
            file.seekg(0, file.beg);

            return true;
        }
    }

    file.clear();
    file.seekg(0, file.beg);

    return false;
}

User getUser(fstream& file, const string username)
{
    string fileLine;

    User transferUser = User();

    while (getline(file, fileLine))
    {
        if (fileLine.compare(0, username.size(), username) == 0)
        {
            file.clear();
            file.seekg(0, file.beg);

            size_t firstSeparator = fileLine.find(':');

            transferUser.username = fileLine.substr(0, firstSeparator);

            transferUser.money = transferUser.getUserMoney(file);

            return transferUser;
        }
    }

    file.clear();
    file.seekg(0, file.beg);

    return transferUser;
}

double validDoubleInput()
{
    double input;

    while (!(cin >> input))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid input.  Try again: ";
    }

    return input;
}

bool isValidPassword(const string password, const string confirmPassword)
{
    bool capitalLetter = false, smallLetter = false, containsSymbol = false;

    if (password != confirmPassword)
    {
        cout << "The two passwords doesn't match!" << endl;

        return false;
    }

    if (password.size() < 5)
    {
        cout << "Password should be atleast 5 characters long!" << endl;

        return false;
    }

    for (unsigned int i = 0; i < password.size(); i++)
    {
        if (password[i] >= 'A' && password[i] <= 'Z')
        {
            capitalLetter = true;
        }
        else if (password[i] >= 'a' && password[i] <= 'z')
        {
            smallLetter = true;
        }
        else if (password[i] < '0' && password[i] > '9')
        {
            return false;
        }
    }

    if (password.find('!') != string::npos
        || password.find('@') != string::npos
        || password.find('#') != string::npos
        || password.find('$') != string::npos
        || password.find('%') != string::npos
        || password.find('^') != string::npos
        || password.find('&') != string::npos
        || password.find('*') != string::npos)
    {
        containsSymbol = true;
    }

    if (smallLetter == true && capitalLetter == true && containsSymbol == true)
    {
        return true;
    }
    else
    {
        cout << "Invalid password!" << endl;

        return false;
    }
}

void deleteUser(fstream& file, User& user)
{
    string confirmPassword;

    cout << "Enter password to delete the account: ";
    cin >> confirmPassword;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirmPassword == user.password)
    {
        if (user.money == 0)
        {
            string fileLine;

            ofstream temp("temp.txt");

            while (getline(file, fileLine))
            {
                if (fileLine.substr(0, fileLine.find(":")) != user.username)
                {
                    temp << fileLine << endl;
                }
            }

            file.close();
            temp.close();

            remove(FILENAME);

            rename("temp.txt", FILENAME);

            file.open(FILENAME);

            mainMenu(file);

            return;
        }
        else
        {
            cout << "The bank account should be empty before it can be deleted!" << endl;

            actionMenu(file, user);
        }
    }
    else
    {
        cout << "Password is incorect" << endl;

        actionMenu(file, user);
    }
}

void actionMenu(fstream& file, User& user)
{
    char userInput;

    cout << "You have "<< user.money << " BGN. Choose one of the following options:" << endl;
    cout << string(50, '-') << endl;
    cout << "C-cancel account" << endl;
    cout << "D-deposit" << endl;
    cout << "L-logout" << endl;
    cout << "T-transfer" << endl;
    cout << "W-withdraw" << endl;
    cout << string(50, '-') << endl;
    cout << "Select option:";

    cin >> userInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << endl;

    if (userInput == 'C' || userInput == 'c')
    {
        deleteUser(file, user);
    }

    else if (userInput == 'D' || userInput == 'd')
    {
        double depositAmount;

        cout << "Enter the amount you want to deposit: " ;
        
        depositAmount = validDoubleInput();

        cout << endl;

        if (depositAmount <= 0)
        {
            cout << "Please enter a valid amount!" << endl;

            actionMenu(file, user);
        }

        else
        {
            user.money = user.money + user.fixDouble(depositAmount);

            user.saveChanges(file);
        }
    }

    else if (userInput == 'L' || userInput == 'l')
    {
        mainMenu(file);

        return;
    }

    else if (userInput == 'T' || userInput == 't')
    {
        string username;

        double transferAmount;

        cout << "Enter user's username: ";

        cin >> username;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter the amount you want to transfer: ";

        transferAmount = validDoubleInput();

        cout << endl;

        if (hasExistingUsername(file, username) == true)
        {
            if (transferAmount <= 0)
            {
                cout << "Please enter a valid amount!" << endl;

                actionMenu(file, user);
            }

            else
            {
                if (user.isOverdraft(transferAmount) == true)
                {
                    cout << "Insuficient funds!"<<endl;

                    actionMenu(file, user);
                }

                else
                {
                    User transferUser = getUser(file, username);

                    user.money = user.money - transferAmount;

                    transferUser.money = transferUser.money + transferAmount;

                    user.saveChanges(file, transferUser);
                }
            }
        }

        else
        {
            cout << "User does not exist!" << endl;
        }       
    }

    else if (userInput == 'W' || userInput == 'w')
    {
        double withdrawAmount;

        cout << "Enter the amount you want to withdraw: ";

        withdrawAmount = validDoubleInput();

        cout << endl;

        if (withdrawAmount <= 0)
        {
            cout << "Please enter a valid amount!" << endl;

            actionMenu(file, user);
        }

        else if (user.isOverdraft(withdrawAmount) == true)
        {
            cout << "Insuficient funds!" << endl;

            actionMenu(file, user);
        }

        else
        {
            user.money = user.money - user.fixDouble(withdrawAmount);

            user.saveChanges(file);
        }       
    }

    else
    {
        cout << "Invalid action!" << endl << endl;
    }

    actionMenu(file, user);
}

void login(fstream& file)
{
    string fileLine, username, password, compareString;

    cout << "Username: ";
    cin >> username;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Password: ";
    cin >> password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << endl ;

    compareString.append(username);
    compareString.append(":");
    compareString.append(hashPassword(password));

    while (getline(file,fileLine))
    {
        if (fileLine.compare(0, compareString.size(), compareString) == 0)
        {
            file.clear();
            file.seekg(0, file.beg);

            User user = User(username,password,file);

            actionMenu(file,user);

            return;
        }
    }

    cout << "Your username or/and password is incorrect!"<<endl;

    file.clear();
    file.seekg(0, file.beg);

    login(file);
}

void regist(fstream& file)
{
    
    string username,password,confirmPassword;

    cout << "Username: ";
    cin >> username;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Password: ";
    cin >> password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Confirm password: ";
    cin >> confirmPassword;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << endl;

    for (unsigned int i = 0; i < username.size(); i++)
    {
        if (username[i] < 33 || (username[i] >= 48 && username[i] <= 57) || username[i] > 126)
        {
            cout << "Username contains invalid characters!" << endl;

            regist(file);
        }
    }

    if (hasExistingUsername(file, username) == true)
    {
        cout << "Already exist a user with this username! Please select another username!" << endl;

        regist(file);
    }

    if (isValidPassword(password, confirmPassword) == false)
    {
        regist(file);
    }

    else
    {
        User user = User(username, password ,file);

        file.seekg(0, file.end);

        file << user.username+ ":" + hashPassword(user.password) + ":0" << endl;

        file.clear();
        file.seekg(0, file.beg);

        actionMenu(file, user);
    }

}

void mainMenu(fstream& file)
{
    char userInput;

    cout << "Please select one of the following actions" << endl;

    cout << string(50, '-') << endl;

    cout << "L-login" << endl;
    cout << "R-register" << endl;
    cout << "Q-quit" << endl;

    cout << string(50, '-') << endl;

    cout << "Select action: ";

    cin >> userInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << endl;

    if (userInput == 'L' || userInput == 'l')
    {       
        login(file);

        return;
    }

    if (userInput == 'R' || userInput == 'r')
    {
        regist(file);

        return;
    }

    if (userInput == 'Q' || userInput == 'q')
    {
        return;
    }

    cout << "Please select valid action!" << endl << endl;

    mainMenu(file);
}

int main()
{
    fstream file;

    file.open(FILENAME, ios::in | ios::out | ios::app);

    if (file.fail())
    {
        cout << "An error has occurred!" << endl;

        return 0;
    }

    mainMenu(file);

    file.close();

    return 0;
}
