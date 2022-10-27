#include<iostream>
#include<stdlib.h>
#include<string.h>
using namespace std;
#define MAX 1000 //Setting the size of the hash table
//Function to generate random hash value. Function multiplies each character by its
//index to ensure that chances of collisions are minimized.
int hash_fn(char *str)
{
	int sum = 0;
	sum = str[0] * str[0] + str[strlen(str) - 1] * str[strlen(str) - 1];
	return sum % MAX;
}
//Holds the patient name and department in one struct.
typedef struct Person_data
{
	char name[25];
	char dept[25];
} Person_data;
//Creating a linked list for chaining in case same hash value is obtained for
//different names.
typedef struct Chain
{
	Person_data* item;
	Chain* next;
} Chain;
//Creating a hash table. Size sets the max size
typedef struct HashTable
{
	Chain** data;
	int size;
} HashTable;
//Inserting into the chain
Chain* Chain_insert(Chain* head, Person_data* item)
{
	Chain* current;
	current = head;
	if (head == NULL) {
		head = (Chain*) malloc (sizeof(Chain));
		current = head;
	}
	else
	{
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = (Chain*) malloc (sizeof(Chain));
		current = current->next;
	}
	current->item = item;
	current->next = NULL;
	return head;
}
// Creates person item with name and department
Person_data* create_item(char * name, char * dept)
{
	Person_data* item = (Person_data*) malloc (sizeof(Person_data));
	strcpy(item->name, name);
	strcpy(item->dept, dept);
	return item;
}
//Creates hash table
HashTable* create_table(int size)
{
	HashTable* table = (HashTable*) malloc (sizeof(HashTable));
	table->size = size;
	table->data = (Chain**) malloc (table->size * sizeof(Chain*));
// Initializing all the Chains to NULL
	for (int i = 0; i < table->size; i++)
		table->data[i] = NULL;
	return table;
}
// Insert data into the hash table based on hash value as index.
void Table_insert(HashTable* table, char * name, char * dept )
{
	Person_data* item = create_item(name, dept);
	int index = hash_fn(name);
	table->data[index] = Chain_insert(table->data[index], item);
}
// Searching the hash table for the patient's department.
char * Table_Search(HashTable* table, char * name)
{
	int index = hash_fn(name);
	Chain* current;
	current = table->data[index];
	int f = 0;
	while (current != NULL) {
		if (strcmp((current->item)->name, name) == 0)
		{
			return (current->item)->dept;
			f = 1;
			break;
		}
		current = current->next;
	}
	if (f == 0)
	{
		return NULL;
	}
	return NULL;
}
// Graph implementation starts
class person // Node person, used for both doctor and patient
{
public:
	string name;
	string type;
	bool present;
	int age;
	string gender;
	person* parent; // pointer to parent node
	person* left; //pointer to left child
	person* right_sibling; //pointer to right sibling
	person(string str, string ty)
	{
		name = str;
		type = ty;
		left = NULL;
		right_sibling = NULL;
		parent = NULL;
		present = true;
	}
};
class hospital_database
{
public:
	person* root = new person("Hospital", ""); // creating a root
	void insert_doc(string str, string ty)
	{
		person* n = new person(str, ty);
		if (root->left == NULL) // initial condition, no children
		{
			root->left = n;
			n->parent = root;
		}
		else
		{
			person* temp = root->left; // moving to depth 1
			while (temp->right_sibling != NULL) // traversing depth 1
			{
				temp = temp->right_sibling;
			}
			n->parent = root;
			temp->right_sibling = n;
		}
		cout << endl;
		cout << "------------Doctor Added----------" << endl;
	}
	void insert_patient(string str, string ty, int age, string gen)
	{
		person* n = new person(str, ty);
		n->age = age;
		n->gender = gen;
		person* temp = root->left; // moving to depth 1
		if (temp == NULL) // if no doctors present
		{
			cout << "------No Doctor For Disease, Please Add Doctor------" << endl;
			return;
		}
		while (temp->type != ty)
		{
			temp = temp->right_sibling; // traversing depth 1
			if (temp == NULL)
			{
				cout << "------No Doctor For Disease, Please Add Doctor------" << endl;
				return;
			}
			if (temp->right_sibling == NULL && temp->type != ty)
			{
				cout << "------No Doctor For Disease, Please Add Doctor------" << endl;
				return;
			}
		}
		if (temp->left == NULL) // base condition, no children
		{
			temp->left = n;
			n->parent = temp;
		}
		else
		{
			person* ttemp = temp->left; // moving to depth 2
			while (ttemp->right_sibling != NULL)
			{
				ttemp = ttemp->right_sibling; //traversing depth 2
			}
			ttemp->right_sibling = n;
			n->parent = temp;
		}
		cout << endl;
		cout << "----------Patient Added-----------" << endl;
	}
	void find_patient(string str, string ty)
	{
		person* temp = root->left; //moving to depth 1
		if (temp == NULL) // base condition, no children
		{
			cout << "--------No Records-------" << endl;
			return;
		}
		while (temp->type != ty)
		{
			if (temp->right_sibling == NULL && temp->type != ty)
			{
				cout << "-----No Doctor with Matching Specialization-------" << endl;
				return;
			}
			temp = temp->right_sibling; // traversing depth 1
		}
		person* ttemp = temp->left; // moving to depth 2
		if (ttemp == NULL)
		{
			cout << "-------No Matching Patients--------" << endl;
			return;
		}
		while (ttemp->name != str)
		{
			if (ttemp->right_sibling == NULL && ttemp->name != str)
			{
				cout << "-------No Matching Patients--------" << endl;
				return;
			}
			ttemp = ttemp->right_sibling; // traversing depth 2
		}
		cout << endl;
		cout << "------Patient found-----" << endl;
		if (ttemp->present == true) // checking if discharged
			cout << "Patient currently in Hospital" << endl;
		else
			cout << "Patient already Discharged" << endl;
		cout << "Patient name: " << str << endl;
		cout << "Patient diseased organ: " << ty << endl;
		cout << "Patient age: " << ttemp->age << endl;
		cout << "Patient gender: " << ttemp->gender << endl;
		cout << "Administered by doctor: " << ttemp->parent->name << endl;
	}
	void find_doctor(string s)
	{
		person* temp = root->left; //moving to depth 1
		if (temp == NULL) // base condition, no children
		{
			cout << "--------No Records-------" << endl;
			return;
		}
		while (temp->name != s)
		{
			if (temp->right_sibling == NULL && temp->name != s)
			{
				cout << "------Doctor Not Found-------" << endl;
				return;
			}
			temp = temp->right_sibling; // traversing depth 1
		}
		cout << endl;
		cout << "-------Doctor found-------" << endl;
		if (temp->present == false) // checking if already removed
			cout << "Doctor not in practice" << endl;
		else
			cout << "Doctor in practice" << endl;
		cout << "Doctor name: " << s << endl;
		cout << "Doctor department: " << temp->type << endl;
		person*ttemp = temp->left; // moving to depth 2
		if (temp->present == true)
		{
			cout << "Patients under doctor: " << endl;
			while (ttemp != NULL)
			{
				cout << ttemp->name;
				if (ttemp->present == false) // checking if already discharged
					cout << " (Discharged)" << endl;
				else
					cout << endl;
				ttemp = ttemp->right_sibling;
			}
		}
	}
	void discharge(string str, string ty)
	{
		person* temp = root->left; // moving to depth 1
		if (temp == NULL) // base condition, no children
		{
			cout << endl;
			cout << "--------No Records-------" << endl;
			return;
		}
		while (temp->type != ty)
		{
			if (temp->right_sibling == NULL && temp->type != ty)
			{
				cout << endl;
				cout << "-----No Doctor with Matching Department------" << endl;
				return;
			}
			temp = temp->right_sibling; // traversing depth 1
		}
		person* ttemp = temp->left; // moving to depth 2
		if (ttemp == NULL)
		{
			cout << "-------No Matching Patients--------" << endl;
			return;
		}
		while (ttemp->name != str)
		{
			if (ttemp->right_sibling == NULL && ttemp->name != str)
			{
				cout << "-------No Matching Patients--------" << endl;
				return;
			}
			ttemp = ttemp->right_sibling; // traversing depth 2
		}
		cout << endl;
		cout << "------Patient Discharged-----" << endl;
		ttemp->present = false;
	}
	void remove_doc(string s)
	{
		person* temp = root->left; // moving to depth 1
		if (temp == NULL) // base case, no children
		{
			cout << "--------No Records-------" << endl;
			return;
		}
		while (temp->name != s)
		{
			if (temp->right_sibling == NULL && temp->name != s)
			{
				cout << "------Doctor Not Found-------" << endl;
				return;
			}
			temp = temp->right_sibling; // traversing depth 1
		}
		cout << endl;
		person* ttemp = temp->left; // moving to depth 2
		if (ttemp == NULL || ttemp->present == false)
		{
			temp->present = false;
			cout << "--------Doctor Removed--------" << endl;
		}
		else
			cout<<endl << "Warning, trying to remove Doctor with patients. Discharge patients first.$
	int ch;
there: // label for goto, keeps menu updated
	cout << endl;
// Menu driven program starts
	cout << "--------Select Login---------" << endl;
	cout << "1.Standard Login" << endl;
	cout << "2.Admin Login" << endl;
	cout << "3.End Database" << endl;
	cin >> ch;
	if (ch == 3)
		exit(0); //terminate
	else if (ch == 1)
	{
		int choice = 0;
		int check = 0;
		while (check != 4)
		{
			cout << endl;
			cout << "--------Open Clinic Database----------" << endl;
			cout << "Pick an operation: " << endl;
			cout << "1.Find Doctor Details" << endl;
			cout << "2.Find Patient Details" << endl;
			cout << "3.Back to login page" << endl;
			cout << "4.End database" << endl;
			int choice;
			cin >> choice;
			if (choice == 1)
			{
				string s;
				cout << endl;
				cout << "Enter doctor name to find: ";
				cin >> s;
				database_1.find_doctor(s); // search in graph
			}
			else if (choice == 2)
			{
				string s, t;
				cout << endl;
				cout << "Enter Patient name to find: " << endl;
				cin >> s;
				char*ts = database_1.convertChar(s);
				if (Table_Search(ht, ts) == NULL) // search in hash table
					cout << "-------No Matching Patients--------" << endl;
				else
				{
					t = database_1.convertStr(Table_Search(ht, ts)); // search in hash table
					database_1.find_patient(s, t);
				}
			}
			else if (choice == 3)
				goto there; // back to start of menu
			else if (choice == 4)
				check = 4;
		}
	}
	else
	{
		cout << endl;
		cout << "--------Password Protected--------" << endl;
		cout << "Enter The password" << endl;
		cout << "(warning end database if fraudulent attempt)" << endl;
		string pass;
		cin >> pass;
		if (pass == password)
		{
			int check = 0;
			while (check != 5)
			{
				cout << endl;
				cout << "--------Protected Clinic Database----------" << endl;
				cout << "Pick an operation: " << endl;
				cout << "1.Add Doctor" << endl;
				cout << "2.Add Patient" << endl;
				cout << "3.Find Doctor Details" << endl;
				cout << "4.Find Patient Details" << endl;
				cout << "5.Discharge Patient" << endl;
				cout << "6.Remove Doctor" << endl;
				cout << "7.Back to Login page" << endl;
				cout << "8.End database" << endl;
				int choice;
				cin >> choice;
				if (choice == 1)
				{
					string n, t;
					cout << endl;
					cout << "-----Doctor Entry------" << endl;
					cout << "Enter Doctor name: ";
					cin >> n;
					cout << "Enter Doctor Department: ";
					cin >> t;
					database_1.insert_doc(n, t); // insert in graph
				}
				else if (choice == 2)
				{
					string n, t, g;
					int a;
					cout << endl;
					cout << "------Patient Entry------" << endl;
					cout << "Enter Patient name: ";
					cin >> n;
					cout << "Enter diseased organ: ";
					cin >> t;
					cout << "Enter age: ";
					cin >> a;
					cout << "Enter gender: ";
					cin >> g;
					char* tn = database_1.convertChar(n);
					char* tt = database_1.convertChar(t);
					Table_insert(ht, tn, tt); // insert in hash table
					database_1.insert_patient(n, t, a, g); // insert in graph
				}
				else if (choice == 3)
				{
					string s;
					cout << endl;
					cout << "Enter doctor name to find: ";
					cin >> s;
					database_1.find_doctor(s);
				}
				else if (choice == 4)
				{
					string s, t;
					cout << endl;
					cout << "Enter Patient name to find: " << endl;
					cin >> s;
					char*ts = database_1.convertChar(s);
					if (Table_Search(ht, ts) == NULL)
						cout << "-------No Matching Patients--------" << endl;
					else
					{
						t = database_1.convertStr(Table_Search(ht, ts)); // search in hash table
						database_1.find_patient(s, t);
					}
				}
				else if (choice == 5)
				{
					cout << endl;
					cout << "Enter patient name to discharge" << endl;
					string s, t;
					cin >> s;
					char*ts = database_1.convertChar(s);
					t = database_1.convertStr(Table_Search(ht, ts)); // search in hash table
					database_1.discharge(s, t);
				}
				else if (choice == 6)
				{
					cout << endl;
					cout << "Enter the doctor name to remove" << endl;
					string s;
					cin >> s;
					database_1.remove_doc(s);
				}
				else if (choice == 7)
					goto there; // back to start of menu
				else if (choice == 8)
					check = 5;
			}
		}
		else
		{
			cout << endl;
			cout << "-------WRONG PASSWORD DATABASE ENDED----------" << endl;
			exit(0); //terminate
		}
	}
}
