
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
            return true;
        }
    }

    file.clear();
    file.seekg(0, file.beg);

    return false;
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

void actionMenu(fstream& file)
{
    cout << "ActionMenu";
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
            actionMenu(file);
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
    string username, password, confirmPassword;

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
        
        file << username+":"+hashPassword(password)+":0" <<endl;

        actionMenu(file);
    }

}

void quit()
{

}

void mainMenu(fstream& file)
{
    char action;

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

    cin >> action;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (action == 'L' || action == 'l')
    {
        
        login(file);
        return;
    }

    if (action == 'R' || action == 'r')
    {
        regist(file);
        return;
    }

    if (action == 'Q' || action == 'q')
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
