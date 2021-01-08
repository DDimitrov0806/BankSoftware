
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

const char* FILENAME = "users.txt";

struct User
{
    string username, password;

    float money=0.0;

    const float OVERDRAFT = -10000.0;


    User(string username, string password,fstream& file)
    {
        this->username = username;
        this->password = password;
        money = fixDouble(getUserMoney(file));
    }

    User()
    {

    }

    bool isOverdraft(float amount)
    {
        if (money < (OVERDRAFT+amount))
        {
            return true;
        }

        return false;
    }

    float fixDouble(float number)
    {
       // return (float)((int)(number * 100 )) / 100.0;

        //return (number * 100) / 100.0;

        return floorf(number * 100) / 100;
    }

    void saveChanges(fstream& file)
    {
        string fileLine;

        ofstream edit("edit.txt");

        while (getline(file, fileLine))
        {

            //temp << fileLine << endl;

            if (fileLine.substr(0, fileLine.find(":")) == username)
            {
                size_t pos = fileLine.find_last_of(':');

                edit << fileLine.substr(0,pos)+":"<<money << endl;
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

    void transfer(User& transferUser,float transferAmount)
    {
        money = money - transferAmount;

        transferUser.money = transferUser.money + transferAmount;

    }

    void withdraw(float amount)
    {
       
        amount = fixDouble(amount);

        money = money - amount;
       
    }

    void deposit(float amount)
    {
        money = money + fixDouble(amount);
    }

    float getUserMoney(fstream& file)
    {
        string fileLine;

        while (getline(file, fileLine))
        {

            if (fileLine.compare(0, username.size(), username) == 0)
            {
                size_t pos = fileLine.find_last_of(':')+1;

                float money = stof(fileLine.substr(pos));

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

void mainMenu(fstream& file);

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

    User transferUser = User();

    while (getline(file, fileLine))
    {

        if (fileLine.compare(0, username.size(), username) == 0)
        {
            size_t firstSeparator = fileLine.find(':');

            size_t secondSeparator = fileLine.find_last_of(':');

            transferUser.username = fileLine.substr(0,firstSeparator);

            transferUser.money = transferUser.getUserMoney(file);

            file.clear();
            file.seekg(0, file.beg);

            return transferUser;
        }
    }

    file.clear();
    file.seekg(0, file.beg);

    return transferUser;
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

    cout << "You have "<< user.money << " BGN. Choose one of the following options:" << endl;
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

        cout << "Enter password to delete the account:"<<endl;
        cin >> confirmPassword;

        if (confirmPassword == user.password)
        {
            if (user.money == 0)
            {
                string fileLine;

                ofstream temp("temp.txt");

                while (getline(file, fileLine))
                {
                    if (fileLine.substr(0, fileLine.find(":"))!=user.username)
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
    else if (userInput == 'D' || userInput == 'd')
    {
        float deposit;

        cout << "Enter the amount you want to deposit:" << endl;
        
        cin >> deposit;

        user.deposit(deposit);

        user.saveChanges(file);
    }

    else if (userInput == 'L' || userInput == 'l')
    {
        mainMenu(file);

        return;
    }

    else if (userInput == 'T' || userInput == 't')
    {
        string username;

        float transferAmount;

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

                    user.saveChanges(file);

                    transferUser.saveChanges(file);
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
        float withdrawAmount;

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

            user.saveChanges(file);
        }       
    }
    else
    {
        cout << "Invalid action!" << endl;
    }

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
        cout << "Already exist a user with this username! Please select another username!" << endl;

        regist(file);
    }

    if (isValidPassword(password, confirmPassword) == false)
    {
        cout << "Invalid password!" << endl;

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
    //const string fileName = "users.txt";

    fstream file;

    file.open(FILENAME, ios::in | ios::out | ios::app);

    if (!file.is_open())
    {
       // cout << "Could not find " + FILENAME + " !\nPlease make sure the file is in the correct directory and is named correctly!" << endl;
        return 0;
    }

    mainMenu(file);

    file.close();

    return 0;

}
