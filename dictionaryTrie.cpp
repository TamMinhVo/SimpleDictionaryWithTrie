#include <iostream>
#include <cstring>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int numberOfWord = 0;
const int ALPHABET_SIZE = 26;


struct Node
{
	int enOfWord;
	struct Node *children[ALPHABET_SIZE];
};

Node* InitializeNode()
{
	Node *pNode = new Node;
	pNode->enOfWord = 0;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;

	return pNode;
}

int Search(Node* root, string word)
{
	Node *tem = root;

	for (int i = 0; i < word.length(); i++)
	{
		int index = word[i] - 'a';
		if (!tem->children[index])
			return 0;
		tem = tem->children[index];
	}

	return tem->enOfWord;
}

void InsertIntoTrie(Node* root, string word)
{
	Node *tem = root;
	int inW;
	for (int i = 0; i < word.length(); i++)
	{
		inW = word[i] - 'a';
		if (!tem->children[inW])
			tem->children[inW] = InitializeNode();
		tem = tem->children[inW];
	}

	tem->enOfWord = ++numberOfWord;
}

void ContructTrie(Node* root)
{
	int index;
	string word;
	int numberOfWord = 0;
	ifstream readFile;

	readFile.open("words.txt"); if (readFile.fail()) { std::cout << "Fail to open file word"; exit(0); }

	//xây dựng cây từ file đã có sẵn.
	while (!readFile.eof())
	{
		readFile >> index >> word;
		++numberOfWord;
		InsertIntoTrie(root, word);
	}
	readFile.close();
}

void LookupWord(Node* root)
{
	string lookupWord;
	cout << endl << "\nNhap tu can tra: ";
	fflush(stdin);
	cin.ignore();
	getline(cin, lookupWord);

	int pos = Search(root, lookupWord);

	if (pos != 0)
	{
		ifstream readFile;
		string description;
		int index;
		int place = 0;

		readFile.open("description.txt"); if (readFile.fail()) { std::cout << "Fail to open file description"; return; }
		while (!readFile.eof())
		{
			readFile >> index;
			fflush(stdin);
			getline(readFile, description);

			if (index == pos)
			{
				cout << lookupWord << ":\n\t" << description << endl;
				break;
			}
		}
		readFile.close();
	}
	else
		cout << "\nTu " << lookupWord << " chua duoc them vao tu dien";
}

void AddWord(Node* root)
{
	string word;
	cin.ignore();
	cout << "\nNhap tu can them vao tu dien: ";
	cin >> word;

	Node *tem = root;
	int inW;
	bool isNew = false;
	int i = 0;//chỉ số của từ ký tự trong từ đó.
	for (;i < word.length(); i++)
	{
		inW = word[i] - 'a';
		if (!tem->children[inW])
		{
			isNew = true;
			break;
		}
		tem = tem->children[inW];
	}
	if (isNew)
	{
		//thêm vào cây
		int inW;
		for (i; i < word.length(); i++)
		{
			inW = word[i] - 'a';
			tem->children[inW] = InitializeNode();
			tem = tem->children[inW];
		}
		tem->enOfWord = ++numberOfWord;

		//thêm vào  file tập tin words, và description
		string descrip;
		cout << "\nNhap mo ta cho tu: ";
		fflush(stdin);
		//fflush(stdin);
		cin.ignore();
		getline(cin, descrip);
		
		ofstream writeFile;
		writeFile.open("words.txt", ios::app);
		writeFile << endl << numberOfWord << " " << word;
		writeFile.close();

		writeFile.open("description.txt", ios::app);
		writeFile << endl << numberOfWord << " " << descrip;
		cout << "\nThem thanh cong";
		writeFile.close();
	}
	else
		cout << "\nTu nay da co trong tu dien!"; //xử lý update?
}

static std::vector<char> ReadAllBytes(char const* filename)
{
	ifstream ifs(filename, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);

	return result;
}
//string toString(int n)
//{
//	string s = "";
//	char c;
//	while (n != 0)
//	{
//		c = n % 10 - '0';
//		s += c;
//		n /= 10;
//	}
//	return s;
//}
void UpdateWord(Node* root)
{
	string descrip, word;
	cout << "\nNhap tu can cap nhat mo ta: ";
	cin.ignore();
	getline(cin, word);
	short update = 2;
	int pos = Search(root, word);
	if(pos > 0)
		cout<< "\nTu nay da co trong tu dien!\nBan co muon cap nhat lai tu nay?\n1. Co.\nKhac. Khong.";
	
	cin >> update;
	if (update != 1)
		return;
	//Nhập mô tả mới
	cout << "\nNhap mo ta moi: ";
	cin.ignore();
	getline(cin, descrip);

	//đọc dữ liệu từ file description lên theo từng dòng, đến dòng cần cập nhật (dòng có chỉ số trùng với pos vừa tìm ở trên) thì chỉ đọc dòng đó lên mà không ghi vào, thay thế bằng descrip vừa nhập
	ifstream readFile;
	int index;
	string description;
	int place = 0;
	//biến result lưu kết quả của file description sau khi đã cập nhật xong, sau đó ghi lại vào file
	string result = "";
	readFile.open("description.txt"); if (readFile.fail()) { std::cout << "Fail to open file description"; return; }
	bool flag = 0;
	while (!readFile.eof())
	{
		if (flag == 1)
			result += "\n";
		flag = 1;
		readFile >> index;
		fflush(stdin);
		getline(readFile, description);

		if (index == pos)
		{
			result += to_string(pos);
			result += " ";
			result += descrip;
		}
		else
		{
			result += to_string(index);
			result += " ";
			result += description;
		}
	}
	readFile.close();
	//ghi du lieu vao lai file
	ofstream writeFile;
	writeFile.open("description.txt");
	writeFile << result;
	cout << "\nThem thanh cong";
	writeFile.close();
}
int main()
{
	Node *root = InitializeNode();

	ContructTrie(root);

	int x;
	while(true)
	{
		cout << "\n1. Them tu vung.";
		cout << "\n2. Tra tu.";
		cout << "\n3. Cap nhat phan mo ta cua tu.";
		cout << "\nKhac. Thoat.";
		cout << "\nNhap lua chon: ";
		cin >> x;
		cout << "\n";
		if (x == 1)
			AddWord(root);
		else if (x == 2)
			LookupWord(root);
		else if (x == 3)
			UpdateWord(root);
		else
			break;
	}
	
	system("pause");
	return 0;
}