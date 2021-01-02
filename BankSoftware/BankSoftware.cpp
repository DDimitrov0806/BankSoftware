
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct User
{
    string username, password;

    double money;

    const double OVERDRAFT = -10000;


    User(string username, string password,fstream& file)
    {
        this->username = username;
        this->password = password;
        money = getUserMoney(file);
    }

    User()
    {

    }

    bool isOverdraft(double amount)
    {
        if (money < (OVERDRAFT+amount))
        {
            return true;
        }

        return false;
    }

    double fixDouble(double number)
    {
        int fixDouble = number * 100;

        return fixDouble / 100;
    }

    void transfer(User transferUser,double transferAmount)
    {
        money = money - transferAmount;

        transferUser.money = transferUser.money + transferAmount;

        //transferUser.SaveChanges();

    }

    void withdraw(double amount)
    {
       
        amount = fixDouble(amount);

        money = money - amount;
       
    }

    void deposit(double amount)
    {
        money = money + fixDouble(amount);
    }

    double getUserMoney(fstream& file)
    {
        string fileLine;

        while (getline(file, fileLine))
        {

            if (fileLine.compare(0, username.size(), username) == 0)
            {
                size_t pos = fileLine.find_last_of(':');

                double money = stod(fileLine.substr(pos));

                return money;
            }
        }

        file.clear();
        file.seekg(0, file.beg);

        return 0;
    }

};

string hashPassword(string password)
{
    hash<string> hashedPassword;

    return to_string(hashedPassword(password));
}

bool hasExistingUsername(fstream& file,string username)
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

User getUser(fstream& file,string username)
{
    string fileLine;

    User user = User();

    while (getline(file, fileLine))
    {

        if (fileLine.compare(0, username.size(), username) == 0)
        {
            size_t firstSeparator = fileLine.find(':');
            size_t secondSeparator = fileLine.find(":");

            user.username = username;

            user.money = user.getUserMoney(file);

            file.clear();
            file.seekg(0, file.beg);

            return user;
        }
    }

    file.clear();
    file.seekg(0, file.beg);

    return user;
}

bool isValidPassword(string password, string confirmPassword)
{
    bool capitalLetter = false, smallLetter = false,containsSymbol=false;

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

    for (int i = 0; i < password.size(); i++)
    {
        if (password[i] >= 'A' && password[i] <= 'Z')
        {
            capitalLetter = true;
        }
        else if (password[i] >= 'a' && password[i] <= 'z')
        {
            smallLetter = true;
        }
        else if (password[i] < '0' && password[i]>'9')
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

void actionMenu(fstream& file, User& user)
{
    char userInput;

    cout << "You have " + to_string(user.money) + " BGN. Choose one of the following options:" << endl;
    cout << "C-cancel account" << endl;
    cout << "D-deposit" << endl;
    cout << "L-logout" << endl;
    cout << "T-transfer" << endl;
    cout << "W-withdraw" << endl;

    cin >> userInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (userInput == 'C' || userInput == 'c')
    {
        string confirmPassword;

        cout << "Enter password to delete the account";
        cin >> confirmPassword;

        if (confirmPassword == user.password)
        {
            if (user.money == 0)
            {

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

    if (userInput == 'D' || userInput == 'd')
    {
        double deposit;

        cout << "Enter the amount you want to deposit:" << endl;
        
        cin >> deposit;

        user.deposit(deposit);
    }

    if (userInput == 'L' || userInput == 'l')
    {
        //user.saveChanges();

        mainMenu(file);
    }

    if (userInput == 'T' || userInput == 't')
    {
        string username;

        double transferAmount;

        cout << "Enter user's username: " << endl;

        cin >> username;

        cout << "Enter the amount you want to transfer:" << endl;

        cin >> transferAmount;

        if (hasExistingUsername(file, username) == true)
        {
            if (transferAmount <= 0)
            {
                cout << "Please enter a valid amount" << endl;

                actionMenu(file, user);
            }
            else
            {
                if (user.isOverdraft(transferAmount) == true)
                {
                    cout << "Insuficient funds!";

                    actionMenu(file, user);
                }
                else
                {
                    User transferUser = getUser(file, username);

                    user.transfer(transferUser, transferAmount);
                }
            }
        }
        else
        {
            cout << "User does not exist!" << endl;
        }

        
    }

    if (userInput == 'W' || userInput == 'w')
    {
        double withdrawAmount;

        cout << "Enter the amount you want to withdraw:" << endl;

        cin >> withdrawAmount;

        if (withdrawAmount <= 0)
        {
            cout << "Please enter a valid amount" << endl;
            actionMenu(file, user);
        }
        else if (user.isOverdraft(withdrawAmount) == true)
        {
            cout << "Insuficient funds!";
            actionMenu(file, user);
        }
        else
        {
            user.withdraw(withdrawAmount);
        }

        
    }


    
    cout << "Invalid action!" << endl;

    actionMenu(file, user);
}

void login(fstream& file)
{
    string fileLine, username, password,compareString;

    cout << "Username:" << endl;
    cin >> username;
    
    cout << "Password:" << endl;
    cin >> password;

    compareString.append(username);
    compareString.append(":");
    compareString.append(hashPassword(password));

    while (getline(file,fileLine))
    {

        if (fileLine.compare(0, compareString.size(), compareString) == 0)
        {
            User user=User(username,password,file);

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

    cout << "Username: " << endl;
    cin >> username;
    
    cout << "Password:" << endl;
    cin >> password;

    cout << "Confirm password: " << endl;
    cin >> confirmPassword;

    for (int i = 0; i < username.size(); i++)
    {
        if (username[i] < 33 || (username[i] >= 48 && username[i] <= 57) || username[i]>126)
        {
            cout << "Username contains invalid characters!" << endl;

            regist(file);
        }
    }

    if (hasExistingUsername(file, username) == true)
    {
        cout << "Already exist a user with this username!\nPlease select another username!" << endl;

        regist(file);
    }

    if (isValidPassword(password, confirmPassword) == false)
    {
        regist(file);
    }
    else
    {
        User user = User(username, password ,file);

        file << user.username+":"+hashPassword(user.password)+":0" <<endl;

        actionMenu(file,user);
    }

}

void quit()
{

}

void mainMenu(fstream& file)
{
    char userInput;

    /*string fileName = "users.txt";
    fstream file;
    file.open(fileName, ios::in | ios::out | ios::app);

    if (!file.is_open())
    {
        cout << "Could not find " + fileName + " !\nPlease make sure the file is in the correct directory and is named correctly!"<<endl;
        return;
    }*/

    cout << "\tPlease select one of the following actions: " << endl;
    cout << "==========================================================" << endl;
    cout << "L-login" << endl;
    cout << "R-register" << endl;
    cout << "Q-quit" << endl;
    cout << "==========================================================" << endl;

    cin >> userInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
        cout << "quit";
        quit();
        return;
    }

    cout << "Please select valid action!"<<endl<<endl;

    mainMenu(file);

}



int main()
{
    string fileName = "users.txt";
    fstream file;
    file.open(fileName, ios::in | ios::out | ios::app);

    if (!file.is_open())
    {
        cout << "Could not find " + fileName + " !\nPlease make sure the file is in the correct directory and is named correctly!" << endl;
        return 0;
    }

    mainMenu(file);

    file.close();

    return 0;

}
