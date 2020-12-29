
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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
    compareString.append(password);

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

}

void quit()
{

}

void mainMenu()
{
    char action;

    string fileName = "users.txt";
    fstream file;
    file.open(fileName, ios::out | ios::in);

    if (!file.is_open())
    {
        cout << "Could not find " + fileName + " !\nPlease make sure the file is in the correct directory and is named correctly!"<<endl;
        return;
    }

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
        cout << "register";
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

    mainMenu();

}



int main()
{
    mainMenu();

}
