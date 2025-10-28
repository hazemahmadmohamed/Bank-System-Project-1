// Bank System Project 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <vector>
using namespace std;

enum enMenuOption
{
    ShowClientList = 1,
    AddNewClient = 2,
    FindClient = 3,
    DeleteClient = 4,
    UpdateClient = 5,
    ResetAllClient = 6,
    Exit = 7,
};

const string ClientsFileName = "Clients.txt";
void ShowMainMenue();

struct sClient
{
    string AccountNumber;
    string PIN_Code;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Delime = "#//#");

sClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
    sClient Client;
    vector <string> vClientData = SplitString(Line, Separator);

    Client.AccountNumber = vClientData[0];
    Client.PIN_Code = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}
string ConvertRecordToLine(sClient Clients, string Separator = "#//#")
{
    string Line = "";

    Line += Clients.AccountNumber + Separator;
    Line += Clients.PIN_Code + Separator;
    Line += Clients.Name + Separator;
    Line += Clients.Phone + Separator;
    Line += to_string(Clients.AccountBalance) + Separator;

    return Line;
}

vector <string> SplitString(string S1, string Delime)
{
    vector<string> Vstring;
    std::string::size_type Pos = 0;
    string sWord;

    while ((Pos = S1.find(Delime)) != std::string::npos)
    {
        sWord = S1.substr(0, Pos);
        Vstring.push_back(sWord);
        S1.erase(0, Pos + Delime.length());
    }

    if (S1 != "")
        Vstring.push_back(S1);

    return Vstring;
}

vector<sClient> LoadDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            sClient Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }

    return vClients;
}

void PrintRecordClients(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PIN_Code;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(15) << left << Client.Phone;
    cout << "| " << setw(15) << left << Client.AccountBalance << endl;
}
sClient ReadNewClient()
{
    sClient Clients;

    cout << "Enter Account Number ? ";
    getline(cin >> ws, Clients.AccountNumber);

    cout << "Enter PIN Code ? ";
    getline(cin, Clients.PIN_Code);

    cout << "Enter Name ? ";
    getline(cin, Clients.Name);

    cout << "Enter Phone ? ";
    getline(cin, Clients.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Clients.AccountBalance;

    return Clients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}
void AddNewClients()
{
    sClient Clients = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Clients));
}
void AddClients()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Add New Client\n\n";
        AddNewClients();
        cout << "\n\nClients Added Successfuly, Do you want add more clients ? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void PrintClientCard(sClient Clients)
{
    cout << "Account Number: " << Clients.AccountNumber << endl;
    cout << "PIN_Code      : " << Clients.PIN_Code << endl;
    cout << "Name          : " << Clients.Name << endl;
    cout << "Phone         : " << Clients.Phone << endl;
    cout << "Account Balanc: " << Clients.AccountBalance << endl;
    cout << "-----------------------------------------\n";
}
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "Please Enter Account Number to Show Details: ";
    cin >> AccountNumber;

    return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
void PrintAllDataClients()
{
    vector <sClient> vClients = LoadDataFromFile(ClientsFileName);

    cout << "\n______________________________________________________________________________________________________________________\n";
    cout << "\t\t\t\t\tClients List (" << vClients.size() << ") Client(s)\n";
    cout << "________________________________________________________________________________________________________________________\n";

    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(10) << left << "PIN Code";
    cout << "| " << setw(40) << left << "Name";
    cout << "| " << setw(15) << left << "Phone";
    cout << "| " << setw(15) << left << "Balance";
    cout << "\n";

    cout << "________________________________________________________________________________________________________________________\n";

    for (sClient& Client : vClients)
    {
        PrintRecordClients(Client);
    }

    cout << "________________________________________________________________________________________________________________________\n";
}

bool MarkForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
void SaveClientDataToFile(string FileName, vector <sClient>vClients)
{
    fstream MyFile;
    string DataLine;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}

bool DeleteClientsByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    char Answer = 'n';
    sClient Clients;

    if (FindClientByAccountNumber(AccountNumber, vClients, Clients))
    {
        cout << "\nClient Found :-) \n---------------------\n";
        PrintClientCard(Clients);

        cout << "are you sure do you delete client data? y/n? ";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            MarkForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientDataToFile(ClientsFileName, vClients);
            vClients = LoadDataFromFile(ClientsFileName);
            cout << "Client deleted succesfuly\n";
            return true;
        }
        else
        {
            cout << "Operation cancelled.\n";
            return false;
        }
    }
    else
    {
        cout << "This Account Number (" << AccountNumber << ") Not Found\n";
        return false;
    }
}

void ShowAddNewClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tAdd New Client Screen\n";
    cout << "-------------------------------------\n\n";

    AddClients();
}
void ShowAllClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tAll Client Screen\n";
    cout << "-------------------------------------\n\n";

    PrintAllDataClients();
}
void GoBackToMainMenue()
{
    cout << "\t..Press any key to back the main menue\n";
    system("pause>0");
    ShowMainMenue();
}
void ShowFindClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "-------------------------------------\n\n";

    vector <sClient> vCLients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, vCLients, Client))
    {
        cout << "\nClient found :-)\n--------------------------\n";
        PrintClientCard(Client);
    }
    else
    {
        cout << "Account Number (" << AccountNumber << ") not found!\n";
    }
}
void ShowdeleteClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "-------------------------------------\n\n";

    vector <sClient> vCLients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

    DeleteClientsByAccountNumber(AccountNumber, vCLients);
}
void ShowEndScreen()
{
    system("color 2F"); // أخضر على أبيض

    cout << "\n\n\n\n\n";
    cout << "\t\t\t=============================================================\n";
    cout << "\t\t\t||                                                         ||\n";
    cout << "\t\t\t||             P R O G R A M   E N D   :-)                 ||\n";
    cout << "\t\t\t||                                                         ||\n";
    cout << "\t\t\t=============================================================\n";
    cout << "\n\n";
}

void UpdateClientDataFromFile(string AccountNumber)
{
    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    bool IsFound = false;

    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            // عرض بيانات العميل الحاليه
            cout << "\nClient found :-)\n--------------------------\n";
            cout << "PIN Code        : " << C.PIN_Code << endl;
            cout << "Name            : " << C.Name << endl;
            cout << "Phone           : " << C.Phone << endl;
            cout << "Account Balance : " << C.AccountBalance << endl;

            cout << "\nEnter new data (leave empty to keep current value):\n";

            string input;

            cout << "New PIN Code (" << C.PIN_Code << "): ";
            getline(cin >> ws, input);
            if (!input.empty())
                C.PIN_Code = input;

            cout << "New Name (" << C.Name << "): ";
            getline(cin, input);
            if (!input.empty())
                C.Name = input;

            cout << "New Phone (" << C.Phone << "): ";
            getline(cin, input);
            if (!input.empty())
                C.Phone = input;

            cout << "New Account Balance (" << C.AccountBalance << "): ";
            getline(cin, input);
            if (!input.empty())
            {
                try
                {
                    C.AccountBalance = stod(input);
                }
                catch (...)
                {
                    cout << "Invalid balance input. Keeping old balance.\n";
                }
            }

            IsFound = true;
            break;
        }
    }

    if (IsFound)
    {
        SaveClientDataToFile(ClientsFileName, vClients); // <-- صححنا المعاملات
        cout << "\nClient data updated successfully.\n";
    }
    else
    {
        cout << "\nAccount Number (" << AccountNumber << ") not found :-( \n";
    }
}

void ShowUpdateClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "-------------------------------------\n\n";

    vector<sClient> vClient = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    sClient FoundClient;
    if (!FindClientByAccountNumber(AccountNumber, vClient, FoundClient))
    {
        cout << "\nAccount Number (" << AccountNumber << ") not found!\n";
        return;
    }

    cout << "\nClient found :-)\n--------------------------\n";
    PrintClientCard(FoundClient);

    char ConfirmUpdate = 'N';
    cout << "\nDo you want to update this account? [y/n]? ";
    cin >> ConfirmUpdate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(ConfirmUpdate) == 'Y')
    {
        UpdateClientDataFromFile(AccountNumber);
    }
    else
    {
        cout << "\nUpdate cancelled.\n";
    }
}
short ReadMainMenue()
{
    short Number;
    cout << "Enter a Number From [1 to 7]\n";
    cin >> Number;
    return Number;
}
void ResetAllClients()
{
    char confirm;
    cout << "Are you sure you want to RESET and delete ALL clients data? (y/n): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        ofstream file(ClientsFileName, ios::trunc); // يفتح الملف ويمسحه
        if (file.is_open())
        {
            file.close();
            cout << "\n✅ All clients data has been reset successfully!\n";
        }
        else
        {
            cout << "\n❌ Error: Could not open file.\n";
        }
    }
    else
    {
        cout << "\n❌ Operation canceled.\n";
    }
}
void ShowResetAllClients()
{
    cout << "\n-------------------------------------\n";
    cout << "\Reset All Clients Screen\n";
    cout << "-------------------------------------\n\n";
    ResetAllClients();
}
void PerforMainMenue(enMenuOption MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMenuOption::ShowClientList:
        system("cls");
        ShowAllClientScreen();
        GoBackToMainMenue();
        break;

    case enMenuOption::AddNewClient:
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenue();
        break;

    case enMenuOption::FindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMenuOption::DeleteClient:
        system("cls");
        ShowdeleteClientScreen();
        GoBackToMainMenue();
        break;
    case enMenuOption::UpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    case enMenuOption::ResetAllClient:
        system("cls");
        ShowResetAllClients();
        GoBackToMainMenue();
        break;

    case enMenuOption::Exit:
        system("cls");
        ShowEndScreen();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");

    cout << "=============================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "=============================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Find Client.\n";
    cout << "\t[4] Delete Client.\n";
    cout << "\t[5] Update Client.\n";
    cout << "\t[6] Reset All Client.\n";
    cout << "\t[7] Exit.\n";
    cout << "=============================================\n";
    PerforMainMenue(enMenuOption(ReadMainMenue()));

}
void BankSystem()
{
    system("color 1F");
    cout << "\n\n\n\n\n";
    cout << "\t\t\t=============================================================\n";
    cout << "\t\t\t||                                                         ||\n";
    cout << "\t\t\t||       WELCOM TO THE BANK SYSTEM PROJECT  :-)            ||\n";
    cout << "\t\t\t||                                                         ||\n";
    cout << "\t\t\t=============================================================\n";
    cout << "\n\n";
    system("pause > 0");
    ShowMainMenue();
    
}
int main()
{
    BankSystem();
    system("pause >0");
}
