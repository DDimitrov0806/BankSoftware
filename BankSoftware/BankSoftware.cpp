
#include <iostream>

using namespace std;

void login()
{

}

void regist()
{

}

void quit()
{

}

void mainMenu()
{
    char action;

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
        cout << "login";
        login();
    }

    if (action == 'R' || action == 'r')
    {
        cout << "register";
        regist();
    }

    if (action == 'Q' || action == 'q')
    {
        cout << "quit";
        quit();
    }

    cout << "Please select valid action!"<<endl;

    mainMenu();

}

int main()
{
    mainMenu();

}
