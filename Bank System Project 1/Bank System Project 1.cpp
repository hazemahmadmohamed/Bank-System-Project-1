// Bank System Project 1.cpp : Main Function.
// Program execution begins and ends here.

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

vector<string> SplitString(string S1, string Delim = "#//#");

sClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
    sClient Client;
    vector<string> vClientData = SplitString(Line, Separator);

    Client.AccountNumber = vClientData[0];
    Client.PIN_Code = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Separator = "#//#")
{
    string Line = "";
    Line += Client.AccountNumber + Separator;
    Line += Client.PIN_Code + Separator;
    Line += Client.Name + Separator;
    Line += Client.Phone + Separator;
    Line += to_string(Client.AccountBalance);
    return Line;
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> Vstring;
    size_t Pos = 0;
    string sWord;

    while ((Pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, Pos);
        Vstring.push_back(sWord);
        S1.erase(0, Pos + Delim.length());
    }

    if (!S1.empty())
        Vstring.push_back(S1);

    return Vstring;
}

vector<sClient> LoadDataFromFile(string FileName)
{
    vector<sClient> vClients;
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

void PrintRecordClient(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PIN_Code;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(15) << left << Client.Phone;
    cout << "| " << setw(15) << left << Client.AccountBalance << endl;
}

bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;

    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        sClient Client;
        string Line;
        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;

}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number ? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "Account Number [ " << Client.AccountNumber << " ] Exist, Enter Account Number again? ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "Enter PIN Code ? ";
    getline(cin, Client.PIN_Code);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Client.AccountBalance;


    return Client;
}

void AddDataLineToFile(string FileName, string DataLine)
{
    fstream MyFile(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

void AddClients()
{
    sClient C;
    char AddMore = 'Y';
    do
    {
        system("cls");
        C = ReadNewClient();
        AddDataLineToFile(ClientsFileName, ConvertRecordToLine(C));
        cout << "\nClient Added Successfully. Add more clients? (Y/N): ";
        cin >> AddMore;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (toupper(AddMore) == 'Y');
}

void PrintClientCard(sClient Client)
{
    cout << "-----------------------------------------\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PIN Code       : " << Client.PIN_Code << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "-----------------------------------------\n";
}

string ReadClientAccountNumber()
{
    string AccountNumber;
    cout << "Please Enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
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
    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);

    cout << "\n______________________________________________________________________________________________________________________\n";
    cout << "\t\t\t\t\tClients List (" << vClients.size() << ") Client(s)\n";
    cout << "______________________________________________________________________________________________________________________\n";
    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(10) << left << "PIN Code";
    cout << "| " << setw(40) << left << "Name";
    cout << "| " << setw(15) << left << "Phone";
    cout << "| " << setw(15) << left << "Balance";
    cout << "\n______________________________________________________________________________________________________________________\n";

    for (sClient& Client : vClients)
        PrintRecordClient(Client);

    cout << "______________________________________________________________________________________________________________________\n";
}

bool MarkForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
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

void SaveClientDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}

bool DeleteClientsByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    char Answer = 'n';
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient Found :-) \n---------------------\n";
        PrintClientCard(Client);

        cout << "Are you sure you want to delete this client? (Y/N): ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientDataToFile(ClientsFileName, vClients);
            cout << "\nClient deleted successfully.\n";
            return true;
        }
        else
        {
            cout << "\nOperation cancelled.\n";
            return false;
        }
    }
    else
    {
        cout << "\nAccount Number (" << AccountNumber << ") Not Found.\n";
        return false;
    }
}

void ShowAddNewClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\t Add New Clients Screen\n";
    cout << "-------------------------------------\n\n";

    AddClients();
}

void ShowAllClientScreen()
{
    cout << "----------------------------------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\t\t" << "\tAll Clients Screen\n";
    cout << "----------------------------------------------------------------------------------------------------------------------\n\n";
    PrintAllDataClients();
}

void GoBackToMainMenue()
{
    cout << "\nPress any key to go back to main menu...\n";
    system("pause>0");
    ShowMainMenue();
}

void ShowFindClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "-------------------------------------\n\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient Found :-)\n";
        PrintClientCard(Client);
    }
    else
    {
        cout << "\nAccount Number (" << AccountNumber << ") not found!\n";
    }
}

void ShowDeleteClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "-------------------------------------\n\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    DeleteClientsByAccountNumber(AccountNumber, vClients);
}

void ShowEndScreen()
{
    system("color 2F");
    cout << "\n\n\n\n";
    cout << "\t\t=============================================================\n";
    cout << "\t\t||                                                         ||\n";
    cout << "\t\t||            (-:  P R O G R A M   E N D   :-)             ||\n";
    cout << "\t\t||                                                         ||\n";
    cout << "\t\t=============================================================\n\n";
}

void UpdateClientDataFromFile(string AccountNumber)
{
    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    bool IsFound = false;

    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            cout << "\nClient found :-)\n--------------------------\n";
            PrintClientCard(C);

            string input;

            cout << "New PIN Code (" << C.PIN_Code << "): ";
            getline(cin >> ws, input);
            if (!input.empty()) C.PIN_Code = input;

            cout << "New Name (" << C.Name << "): ";
            getline(cin, input);
            if (!input.empty()) C.Name = input;

            cout << "New Phone (" << C.Phone << "): ";
            getline(cin, input);
            if (!input.empty()) C.Phone = input;

            cout << "New Account Balance (" << C.AccountBalance << "): ";
            getline(cin, input);
            if (!input.empty()) C.AccountBalance = stod(input);

            IsFound = true;
            break;
        }
    }

    if (IsFound)
    {
        SaveClientDataToFile(ClientsFileName, vClients);
        cout << "\nClient data updated successfully.\n";
    }
    else
    {
        cout << "\nAccount Number (" << AccountNumber << ") not found :-(\n";
    }
}

void ShowUpdateClientScreen()
{
    cout << "\n-------------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "-------------------------------------\n\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    sClient FoundClient;
    if (!FindClientByAccountNumber(AccountNumber, vClients, FoundClient))
    {
        cout << "\nAccount Number (" << AccountNumber << ") not found!\n";
        return;
    }

    PrintClientCard(FoundClient);

    char ConfirmUpdate;
    cout << "\nDo you want to update this account? (Y/N): ";
    cin >> ConfirmUpdate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(ConfirmUpdate) == 'Y')
        UpdateClientDataFromFile(AccountNumber);
    else
        cout << "\nUpdate cancelled.\n";
}

short ReadMainMenue()
{
    short Number;
    cout << "\nEnter a Number From [1 to 7]: ";
    cin >> Number;
    return Number;
}

void ResetAllClients()
{
    char confirm;
    cout << "Are you sure you want to RESET all client data? (Y/N): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(confirm) == 'Y')
    {
        ofstream file(ClientsFileName, ios::trunc);
        if (file.is_open())
        {
            file.close();
            cout << "\nAll client data has been reset successfully!\n";
        }
        else
        {
            cout << "\nError: Could not open file.\n";
        }
    }
    else
    {
        cout << "\nOperation canceled.\n";
    }
}

void ShowResetAllClients()
{
    cout << "\n-------------------------------------\n";
    cout << "\tReset All Clients Screen\n";
    cout << "-------------------------------------\n\n";
    ResetAllClients();
}

void PerformMainMenue(enMenuOption MainMenueOption)
{
    switch (MainMenueOption)
    {
    case ShowClientList:
        system("cls");
        system("color 1F");
        ShowAllClientScreen();
        GoBackToMainMenue();
        break;

    case AddNewClient:
        system("cls"); system("color 2F");
        ShowAddNewClientScreen();
        GoBackToMainMenue();
        break;

    case FindClient:
        system("cls"); system("color 3F");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case DeleteClient:
        system("cls"); system("color 4F");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case UpdateClient:
        system("cls"); system("color 5F");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case ResetAllClient:
        system("cls"); system("color 6F");
        ShowResetAllClients();
        GoBackToMainMenue();
        break;

    case Exit:
        system("cls");
        ShowEndScreen();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");

    cout << "=============================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "=============================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Find Client.\n";
    cout << "\t[4] Delete Client.\n";
    cout << "\t[5] Update Client.\n";
    cout << "\t[6] Reset All Clients.\n";
    cout << "\t[7] Exit.\n";
    cout << "=============================================\n";

    PerformMainMenue((enMenuOption)ReadMainMenue());
}

void BankSystem()
{
    

    cout << "\n\n\n";
    cout << "\t\t=============================================================\n";
    cout << "\t\t||                                                         ||\n";
    cout << "\t\t||      (-:  WELCOME TO THE BANK SYSTEM PROJECT :-)        ||\n";
    cout << "\t\t||                                                         ||\n";
    cout << "\t\t=============================================================\n\n";
    system("pause > 0");
    ShowMainMenue();
}

int main()
{
    BankSystem();
    system("pause >0");
}
