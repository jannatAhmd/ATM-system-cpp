

#include <iostream>
#include<cctype>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>

using namespace std;

enum enMainMenueOptions { quickWithdraw = 1, normalWithdraw = 2, Deposit = 3, checkBalance = 4, Exit = 5 };

struct stClient
{
    string accountNumber = "";
    string PINCode = "";
    string Name = "";
    string phone = "";
    double accountBalance = 0.0;
    bool markForDelete = false;
};

stClient currentClient;

const string clientFileName = "Clients.txt";
void ShowMainMenueScreen();
void GoBackToMainMenue();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void LogIn();


string Taps(unsigned short number)
{
    string tap = "";
    for (unsigned int i = 1; i <= number; i++)
    {
        tap += "\t";
    }
    return tap;
}

vector <string> SliptString(string s1, string delim /*sperator*/)
{
    vector <string> vString;
    short pos = 0;
    string sWord = "";

    while ((pos = s1.find(delim)) != std::string::npos)
    {
        sWord = s1.substr(0, pos);

        if (sWord != "")
            vString.push_back(sWord);

        s1.erase(0, pos + delim.length());
    }

    if (s1 != "")
        vString.push_back(s1);


    return vString;
}

stClient ConvertLineToRecord(string sLine, string sperator = "#//#")
{
    stClient ClintInfo;
    vector<string> vClient = SliptString(sLine, sperator);

    ClintInfo.accountNumber = vClient[0];
    ClintInfo.PINCode = vClient[1];
    ClintInfo.Name = vClient[2];
    ClintInfo.phone = vClient[3];
    ClintInfo.accountBalance = stof(vClient[4]);

    return ClintInfo;
}

vector <stClient> LoadDataFromFileToVector(string FileNmae)
{
    vector <stClient> vFileContent;
    fstream File;

    File.open(FileNmae, ios::in);


    string Line = "";
    stClient Cleint;
    if (File.is_open())
    {
        while (getline(File, Line))
        {
            Cleint = ConvertLineToRecord(Line);
            vFileContent.push_back(Cleint);
        }

        File.close();
    }

    return vFileContent;
}

bool FindCleintByAccountNumberAndPinCode(string accountNumber, string pinCode, stClient& cleint)
{
    vector<stClient> vCleints = LoadDataFromFileToVector(clientFileName);

    for (stClient& c : vCleints) {
        if (c.accountNumber == accountNumber && c.PINCode == pinCode)
        {
            cleint = c;
            return true;
        }
    }
    return false;
}

string ConvertRecordToLine(stClient Client, string sperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.accountNumber + sperator;
    stClientRecord += Client.PINCode + sperator;
    stClientRecord += Client.Name + sperator;
    stClientRecord += Client.phone + sperator;
    stClientRecord += to_string(Client.accountBalance);


    return stClientRecord;
}

void AddDataLineToFile(string fileName, string sDataLine)
{
    fstream NewFile;
    NewFile.open(fileName, ios::out | ios::app);

    if (NewFile.is_open())
    {

        NewFile << sDataLine << endl;
        NewFile.close();
    }
}


void ShowCheckBalanceScreen()
{
    cout << "=======================================================\n";
    cout << "\t\tCheck Balance Screen \n";
    cout << "=======================================================\n";

    cout << "Your Balance is " << currentClient.accountBalance;

}

vector <stClient> SaveCleintDataToFile(string fileName, vector<stClient>vCleints)
{
    fstream myFile;
    string dataLine;

    myFile.open(fileName, ios::out);

    if (myFile.is_open())
    {
        for (stClient& C : vCleints)
        {
            if (C.markForDelete == false)
            {
                dataLine = ConvertRecordToLine(C);
                myFile << dataLine << endl;
            }
        }
        myFile.close();
    }
    return vCleints;
}

bool DepositBalanceToClientAccountByAccountNumber(string accountNumber, double amount, vector <stClient>& vClients)
{
    char answer = 'n';

    cout << "\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> answer;

    if (tolower(answer) == 'y')
    {
        for (stClient& C : vClients)
        {
            if (C.accountNumber == currentClient.accountNumber)
            {
                C.accountBalance += amount;
                SaveCleintDataToFile(clientFileName, vClients);
                cout << "\nDone successfully,New balance is = " << C.accountBalance << endl;
                return true;
            }
        }



    }
    return false;
}

unsigned short ReadMainMenueOption()
{
    unsigned short choise = 0;
    do
    {
        cout << "\nChoose what you want to do [1 to 5]? ";
        cin >> choise;
    } while (choise < 1 || choise>5);

    return choise;
}

double ReadDepositAmount()
{
    double depositAmount = 0;

    cout << "Please enter a positive deposite amount? ";
    cin >> depositAmount;

    while (depositAmount <= 0)
    {
        cout << "Please enter a positive deposite amount? ";
        cin >> depositAmount;
    }

    return depositAmount;
}

void PerformDepositOption()
{
    double depositAmount = ReadDepositAmount();

    vector <stClient> vClients = LoadDataFromFileToVector(clientFileName);

    if (DepositBalanceToClientAccountByAccountNumber(currentClient.accountNumber, depositAmount, vClients))
        currentClient.accountBalance += depositAmount;

}
void ShowDepositScreen()
{
    system("cls");
    cout << "=======================================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "=======================================================\n";

    PerformDepositOption();

}

unsigned short GetQuickWithdarwAmount(short option)
{
    switch (option)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;

    }
}

unsigned short ReadQuickWithdrawOption()
{
    unsigned short choise = 0;
    do
    {
        cout << "\nChoose what do you want to do from [1] to [9]? ";
        cin >> choise;
    } while (choise < 1 || choise>9);

    return choise;
}

void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short withdrawBalance = GetQuickWithdarwAmount(QuickWithdrawOption);

    if (currentClient.accountBalance < withdrawBalance)
    {
        cout << "\n Amount Exceeds the balance,Make another choise ";
        system("cls");
        ShowQuickWithdrawScreen();
    }

    vector <stClient> vClients = LoadDataFromFileToVector(clientFileName);

    if (DepositBalanceToClientAccountByAccountNumber(currentClient.accountNumber, -1 * withdrawBalance, vClients))
        currentClient.accountBalance -= withdrawBalance;

}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "=======================================================\n";
    cout << "\t\tQuick Withdraw \n";
    cout << "=======================================================\n";
    cout << "\t[1] 20   \t\t[2] 50\n";
    cout << "\t[3] 100  \t\t[4] 200\n";
    cout << "\t[5] 400  \t\t[6] 600\n";
    cout << "\t[7] 800  \t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "=======================================================\n";
    cout << "Your Balance is " << currentClient.accountBalance;

    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadNormalWithdrawOption()
{
    int withdrawBalance = 0;
    cout << "Enter an amount multiple of 5`s ?";
    cin >> withdrawBalance;

    while (withdrawBalance % 5 != 0)
    {
        cout << "Enter an amount multiple of 5`s ?";
        cin >> withdrawBalance;
    }

    return withdrawBalance;
}
void PerformNormalWithdrawOption()
{
    int withdrawBalance = ReadNormalWithdrawOption();

    if (currentClient.accountBalance < withdrawBalance || withdrawBalance <= 0)
    {
        cout << "\n Amount Exceeds the balance,Make another choise ";
        system("cls");
        ShowNormalWithdrawScreen();
    }

    vector <stClient> vClients = LoadDataFromFileToVector(clientFileName);

    if (DepositBalanceToClientAccountByAccountNumber(currentClient.accountNumber, -1 * withdrawBalance, vClients))
        currentClient.accountBalance -= withdrawBalance;


}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "=======================================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "=======================================================\n";

    PerformNormalWithdrawOption();
}

void GoBackToMainMenue()
{

    cout << "\n\nPress any key to continue... \n";
    system("pause>0");
    ShowMainMenueScreen();
}

void PerformMainMenueOption(enMainMenueOptions mainMenueOptions)
{
    switch (mainMenueOptions)
    {
    case enMainMenueOptions::quickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::normalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::Deposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::checkBalance:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::Exit:
    {
        system("cls");
        LogIn();
        break;
    }
    }

}


void ShowMainMenueScreen()
{
    system("cls");
    cout << "=======================================================\n";
    cout << Taps(2) << "ATM Main Menue Screen \n";
    cout << "=======================================================\n";
    cout << Taps(1) << "[1] Quick Withdraw.\n";
    cout << Taps(1) << "[2] Normal Withdraw.\n";
    cout << Taps(1) << "[3] Deposit.\n";
    cout << Taps(1) << "[4] Check Balance.\n";
    cout << Taps(1) << "[5] Logout.\n";

    cout << "=======================================================\n";

    PerformMainMenueOption(enMainMenueOptions(ReadMainMenueOption()));
}

bool LoadClientInfo(string accountNumber, string pinCode)
{

    if (FindCleintByAccountNumberAndPinCode(accountNumber, pinCode, currentClient))
        return true;

    return false;
}

void LogIn()
{
    string accountNumber = "";
    string pinCode = "";
    bool loginFailed = false;

    do
    {
        system("cls");
        cout << "\n-------------------------------------";
        cout << "\n\t\tLogin Screen";
        cout << "\n-------------------------------------";

        if (loginFailed)
        {
            cout << "\nInvalid account Number/pinCode, Please try again ";

        }

        cout << "\nEnter account Number? ";
        cin >> accountNumber;

        cout << "\nEnter Client pin? ";
        cin >> pinCode;


        loginFailed = !LoadClientInfo(accountNumber, pinCode);

    } while (loginFailed);


    ShowMainMenueScreen();
}


int main()
{

    LogIn();

}

