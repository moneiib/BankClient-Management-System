
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string FileName = "MyFile.txt";

void MainMenuScreen();
void GoBackToTransactionsMenu();
void ShowTransactionsMenu();

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

vector <string> PrintEachWordInString(string S1, string Delim)
{
	vector <string> vString;

	short pos = 0;
	string sWord;
	while ((pos = S1.find(Delim)) != string::npos)
	{
		sWord = S1.substr(0, pos);
		if (!sWord.empty())
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

string ConvertRecordToLine(stClient Client, string Separator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Separator;
	stClientRecord += Client.PinCode + Separator;
	stClientRecord += Client.Name + Separator;
	stClientRecord += Client.Phone + Separator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

stClient ConvertLineToRecord(string Line, string Seperator)
{
	stClient Client;
	vector <string> vClientData;

	vClientData = PrintEachWordInString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

vector <stClient> LoadClientDataFromFile(string FileName)
{
	vector <stClient> vClient;

	fstream MyFile;
	MyFile.open(FileName, ios::in);


	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line, "#//#");
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return vClient;
}

void PrintClientRecord(stClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector <stClient> vClients)
{

	cout << "\n\t\t\t\t\tClients List (" << vClients.size() << ") Client(s).";
	cout << "\n__________________________________________________________________________________________________" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n__________________________________________________________________________________________________\n";

	for (stClient Clients : vClients)
	{
		PrintClientRecord(Clients);
		cout << endl;
	}

	cout << "\n__________________________________________________________________________________________________" << endl;


}

stClient ReadNewClient()
{
	stClient Client;
	cout << "Enter Account Number ? ";
	getline(cin >> ws, Client.AccountNumber);

	cout << "Enter Pin Code ? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance ? ";
	cin >> Client.AccountBalance;

	return Client;
}

vector <stClient> SaveClientDataToFile(string FileName, vector <stClient> Clients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string Seperator = "#//#";
	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient C : Clients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C, Seperator);
				MyFile << DataLine << endl;
			}
		}
	}
	MyFile.close();
	return Clients;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& Client)
{
	for (stClient& C : Client)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> Clients, stClient& Client)
{
	for (stClient& C : Clients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void PrintStringRecord(stClient Client)
{
	cout << "\n\nThe following is the extracted client record : \n";
	cout << "Account Number  : " << Client.AccountNumber << endl;
	cout << "Pin Code        : " << Client.PinCode << endl;
	cout << "Name            : " << Client.Name << endl;
	cout << "Phone           : " << Client.Phone << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "Enter Account Number : ";
	cin >> AccountNumber;
	return AccountNumber;
}

bool DeleteClientForAccountNumber(string AccountNumber, vector <stClient> vClient)
{
	stClient Client;
	char Answer = 'N';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintStringRecord(Client);
		cout << Client.AccountBalance;

		cout << "\n\nAre you sure you need delete this client ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientDataToFile(FileName, vClient);

			vClient = LoadClientDataFromFile(FileName);

			cout << "\n\n Client Deleted Successfully.";

			return true;
		}
	}

	else
	{
		cout << "\n\nClient with account number " << AccountNumber << " is NOT found.";
	}
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter Pin Code : ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Client Name : ";
	getline(cin >> ws, Client.Name);

	cout << "Enter Phone : ";
	getline(cin >> ws, Client.Phone);

	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientForAccountNumber(string AccountNumber, vector <stClient> vClient)
{
	stClient Client;
	char Answer = 'N';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintStringRecord(Client);

		cout << "\n\nAre you sure you need delete this client ? (Y/N) ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (stClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientDataToFile(FileName, vClient);

			vClient = LoadClientDataFromFile(FileName);

			cout << "\n\n Client Updated Successfully.";

			return true;
		}
	}

	else
	{
		cout << "\n\nClient with account number " << AccountNumber << " is NOT found.";
	}
}

bool FindClientByAccountNumber(string AccountNumber, stClient& Client)
{
	vector <stClient> vClient = LoadClientDataFromFile(FileName);
	for (stClient C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void ShowClientList()
{
	vector <stClient> vClient = LoadClientDataFromFile(FileName);
	PrintAllClientsData(vClient);
}

void AddNewClient()
{
	fstream MyFile;
	string Separator = "#//#";
	char Again;
	MyFile.open("MyFile.txt", ios::out | ios::app);

	if (MyFile.is_open())
	{

		Again = 'Y';
		while (Again == 'y' || Again == 'Y')
		{
			MyFile << ConvertRecordToLine(ReadNewClient(), Separator);
			MyFile << "\n";
			cout << "Do you want to add a new client? (Y/N) ";
			cin >> Again;
			system("cls");
		}

		MyFile.close();
	}


}

void DeleteClientByAccountNumber()
{
	stClient Client;
	vector <stClient> vClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientForAccountNumber(AccountNumber, vClient);


}

void UpdateClientByAccountNumber()
{
	stClient Client;
	vector <stClient> vClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientForAccountNumber(AccountNumber, vClient);


}

void FindClientByAccountNumber()
{
	stClient Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, Client))
	{
		PrintStringRecord(Client);
	}
	else
	{
		cout << "Client with account number " << AccountNumber << " NOT found\n";
	}
}

void ShowDeleteClientByAccountNumber()
{
	system("cls");

	cout << "================================\n";
	cout << "Delete Client By Account Number\n";
	cout << "================================\n";

	DeleteClientByAccountNumber();
}

void ShowAddNewClient()
{
	system("cls");

	cout << "================================\n";
	cout << "\tAdd New Client\n";
	cout << "================================\n";

	AddNewClient();
}

void ShowUpdateClientByAccountNumber()
{
	system("cls");

	cout << "================================\n";
	cout << "Update Client By Account Number\n";
	cout << "================================\n";

	UpdateClientByAccountNumber();
}

void ShowFindClientByAccountNumber()
{
	system("cls");

	cout << "================================\n";
	cout << " Find Client By Account Number\n";
	cout << "================================\n";

	FindClientByAccountNumber();
}

int ReadNumber()
{
	int Number;

	do
	{
		cout << "Choose what do you want to do : ";
		cin >> Number;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max() << '\n');

			cout << "Invalid Number, Plz enter number : ";
			cin >> Number;
		}
	} while (Number < 1 && Number >> 6);
	
	return Number;
}

void MainMenuScreen();

void GoBackToMainMenu()
{
	cout << "\n\n\nEnter any key to back to main menu ...\n";
	system("pause>0");
	MainMenuScreen();
}

void GoBackToTransactionsMenu()
{
	cout << "\n\nPress any key to back to transactions menu ... ";
	system("pause>0");
	ShowTransactionsMenu();
}

void PerfromMainMenuOption()
{
	switch (ReadNumber())
	{
	case 1:
	{
		system("cls");
		ShowClientList();
		GoBackToMainMenu();
		break;
	}
	case 2:
	{
		system("cls");
		ShowAddNewClient();
		GoBackToMainMenu();
		break;
	}
	case 3:
	{
		system("cls");
		ShowDeleteClientByAccountNumber();
		GoBackToMainMenu();
		break;
	}
	case 4:
	{
		system("cls");
		ShowUpdateClientByAccountNumber();
		GoBackToMainMenu();
		break;
	}
	case 5:
	{
		system("cls");
		ShowFindClientByAccountNumber();
		GoBackToMainMenu();
		break;
	}
	case 6 :
	{
		system("cls");
		ShowTransactionsMenu();
		break;
	}
	case 7 :
	{
		system("pause>0");
		break;
	}
	}
}

bool DepositBalanceToClientAccountNumber(string AccountNumber, double Amount, vector <stClient> vClients)
{
	char Answer = 'N';

	cout << "\nAre you sure you want perfrom this transactions ? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (stClient C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientDataToFile(FileName, vClients);
				cout << "\nDone Successfully, New Balance is : " << C.AccountBalance << endl;
				return true;
			}
		}
		return false;
	}
}

void ShowDepositScreen()
{
	cout << "____________________________\n";
	cout << "\tDepositScreen\n";
	cout << "____________________________\n";

	stClient Client;
	vector <stClient> vClients = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintStringRecord(Client);

	double Amount = 0;
	cout << "Plz Enter Deposit Amount : ";
	cin >> Amount;

	DepositBalanceToClientAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
	cout << "____________________________\n";
	cout << "\tDepositScreen\n";
	cout << "____________________________\n";

	stClient Client;
	vector <stClient> vClients = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintStringRecord(Client);

	double Amount = 0;
	cout << "Plz Enter Deposit Amount : ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exeeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Plz enter another amount ? ";
		cin >> Amount;
	}

	DepositBalanceToClientAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalances()
{
	vector <stClient> vClient = LoadClientDataFromFile(FileName);
	PrintAllClientsData(vClient);

	double TotalBalances = 0;

	if (vClient.size() == 0)
	{
		cout << "\t\tNo Client Balance in the System.\n";
	}
	else
	{
		for (stClient Client : vClient)
		{
			TotalBalances += Client.AccountBalance;
			cout << "\n";
		}
	}

	cout << "______________________________________\n";
	cout << "\tTotal Balances = " << TotalBalances << endl;

}

void PerfromTransactionsMenuOption()
{
	

	switch (ReadNumber())
	{
	case 1 :
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenu();
	}
	case 2 :
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
	}
	case 3 :
	{
		system("cls");
		ShowTotalBalances();
		GoBackToTransactionsMenu();	
	}
	case 4 :
	{
		MainMenuScreen();
	}
	}
}

void ShowTransactionsMenu()
{
	system("cls");
	cout << "====================================\n";
	cout << "\tTransactions Menu Screen\t\t\n";
	cout << "====================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "====================================\n";

	PerfromTransactionsMenuOption();
}

void MainMenuScreen()
{
	system("cls");
	cout << "====================================\n";
	cout << "\tMain Menu Screen\t\t\n";
	cout << "====================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "====================================\n";

	PerfromMainMenuOption();
}

int main()
{
	MainMenuScreen();
}