#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int max = 3;
struct teletype
{
	string name;
	string number;
	teletype* next;
};

void populate(teletype*);
void insert(teletype*);
void modify(teletype*);
void remove(teletype*);
void display(teletype*);
void find(teletype*);
void uppertable();
void lowertable();

int main()
{
	cout << "DONGRYUL KIM (ONLY TAKES 5 DATA MEMBERS)" << endl;
	string again;
	int choice = 0;
	teletype* list, * current, * data;
	list = new teletype;
	current = list;
	do
	{
		populate(current);
		current->next = new teletype;
		current = current->next;
		current->next = NULL;
		data = list;
		cout << "Would you like to input more data? (y/n) : ";
		cin >> again;
	} while (again == "y" || again == "Y");
	cout << "\nThe linked list records :" << endl;

	uppertable();
	display(list);
	lowertable();

	///////////////////DONGRYUL KIM///////////////////////////



	do
	{
		switch (choice)
		{
		case 1:
			populate(current);
			current->next = new teletype;
			current = current->next;
			current->next = NULL;
			data = list;
			cout << "\nThe list after insertion holds the following records:" << endl;
			uppertable();
			display(list);
			lowertable();
			break;

		case 2:
			modify(data);
			uppertable();
			display(list);
			lowertable();
			break;


		case 3:
			remove(data);
			uppertable();
			display(list);
			lowertable();
			break;

		case 4:
			find(data);
			uppertable();
			display(list);
			lowertable();
			break;

		}
		cout << "\nSelect from the menu";
		cout << "\n1. Insert new structure in the linked list";
		cout << "\n2. Modify an existing structure in the linked list";
		cout << "\n3. Delete an existing structure from the list";
		cout << "\n4. Find an existing structure from the list";
		cout << "\n5. Exit from the program";
		cout << "\n\nEnter the number : ";

		try
		{
			cin >> choice;
			if (cin.fail()) throw runtime_error("Input is no unavailable\n");
		}
		catch (const runtime_error& e)
		{
			cout << "Error : " << e.what();
			system("pause");
			return 1;
		}
	} while (choice != 5);






	system("pause");
	return 0;
}

void populate(teletype* record)
{
	cin.ignore();
	cout << "Enter a name : ";
	getline(cin, record->name);
	cout << "Enter phone number :";
	getline(cin, record->number);
}

void display(teletype* contents)
{
	while (contents != NULL)
	{
		cout << endl << setiosflags(ios::left)
			<< setw(30) << contents->name
			<< setw(20) << contents->number;
		contents = contents->next;
	}
	return;
}
void remove(teletype* preaddr)
{
	teletype* temp;
	string num;
	cout << "Which record do you want to delete? (2~5) :";
	cin >> num;

	if (num == "2")
	{
		temp = preaddr->next;
		preaddr->next = temp->next;
		cout << "\nThe list after deleting a record holds the following records" << endl;
		return;
	}
	else if (num == "3")
	{
		temp = preaddr->next->next;
		preaddr->next->next = temp->next;
		cout << "\nThe list after deleting a record holds the following records" << endl;
		return;
	}
	else if (num == "4")
	{
		temp = preaddr->next->next->next;
		preaddr->next->next->next = temp->next;
		cout << "\nThe list after deleting a record holds the following records" << endl;
		return;
	}
	else if (num == "5")
	{
		temp = preaddr->next->next->next->next;
		preaddr->next->next->next->next = temp->next;
		cout << "\nThe list after deleting a record holds the following records" << endl;
		return;
	}
	else
	{
		cout << "THIS PROGRAM ONLY TAKES BETWEEN 2~5" << endl << endl << endl;
		system("pause");
	}


}

void insert(teletype* prev)
{
	if (prev == NULL)
	{
		cout << "Error. Try again" << endl;
		return;
	}

	teletype* another = new teletype;
	if (another = NULL)
		cout << "Not enough space !" << endl;
	else
		populate(another);
	another->next = prev->next;
	prev->next = another;
}


void modify(teletype* saved)
{
	teletype* temp;
	temp = saved;
	string record;
	cin.ignore();
	cout << "Enter a name or number that you want to find : ";
	getline(cin, record);
	if (temp->name == record || temp->number == record)
	{
		cout << "Enter a new name: ";
		string nname;
		getline(cin, nname);
		cout << "Enter new phone number :";
		string nnumber;
		getline(cin, nnumber);

		temp->name = nname;
		temp->number = nnumber;
	}
	else if (temp->next->name == record || temp->next->number == record)
	{
		cout << "Enter a new name: ";
		string nname;
		getline(cin, nname);
		cout << "Enter new phone number :";
		string nnumber;
		getline(cin, nnumber);
		temp->next->name = nname;
		temp->next->number = nnumber;
	}
	else if (temp->next->next->name == record || temp->next->next->number == record)
	{
		cout << "Enter a new name: ";
		string nname;
		getline(cin, nname);
		cout << "Enter new phone number :";
		string nnumber;
		getline(cin, nnumber);
		temp->next->next->name = nname;
		temp->next->next->number = nnumber;
	}
	else if (temp->next->next->next->name == record || temp->next->next->next->number == record)
	{
		cout << "Enter a new name: ";
		string nname;
		getline(cin, nname);
		cout << "Enter new phone number :";
		string nnumber;
		getline(cin, nnumber);
		temp->next->next->next->name = nname;
		temp->next->next->next->number = nnumber;
	}
	else if (temp->next->next->next->next->name == record || temp->next->next->next->next->number == record)
	{
		cout << "Enter a new name: ";
		string nname;
		getline(cin, nname);
		cout << "Enter new phone number :";
		string nnumber;
		getline(cin, nnumber);
		temp->next->next->next->next->name = nname;
		temp->next->next->next->next->number = nnumber;
	}
	else
	{
		cout << "DOES NOT EXIST ON THE LIST !" << endl;
	}
	return;
}

void find(teletype* list)
{
	teletype* temp;
	temp = list;
	string record;
	cin.ignore();
	cout << "Enter a name or phone number that you want to find : ";
	getline(cin, record);

	if (temp->name == record || temp->number == record)
	{
		cout << "\n\nEXIST ON THE LIST\n\n" << endl;
	}
	else if (temp->next->name == record || temp->next->number == record)
	{
		cout << "\n\nEXIST ON THE LIST\n\n" << endl;
	}
	else if (temp->next->next->name == record || temp->next->next->number == record)
	{
		cout << "\n\nEXIST ON THE LIST\n\n" << endl;
	}
	else if (temp->next->next->next->name == record || temp->next->next->next->number == record)
	{
		cout << "\n\nEXIST ON THE LIST\n\n" << endl;
	}
	else if (temp->next->next->next->next->name == record || temp->next->next->next->next->number == record)
	{
		cout << "\n\nEXIST ON THE LIST\n\n" << endl;
	}
	else
	{
		cout << "\n\nDOES NOT EXIST ON THE LIST\n\n" << endl;
	}
}

void uppertable()
{
	cout << "///////////////////////////////////////" << endl;
}
void lowertable()
{
	cout << "\n///////////////////////////////////////" << endl;
}
