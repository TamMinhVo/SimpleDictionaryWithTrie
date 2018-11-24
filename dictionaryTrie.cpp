#include <iostream>
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

	tem->enOfWord = numberOfWord;
}

void ContructTrie(Node* root)
{
	int index;
	string word;
	//int numberOfWord = 0;
	ifstream readFile;

	readFile.open("words.txt"); if (readFile.fail()) { std::cout << "\nFail to open file word"; exit(0); }

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
	cout << "\nNhap tu can tra: ";
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

		readFile.open("description.txt"); if (readFile.fail()) { std::cout << "\nFail to open file description"; return; }
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
		cout << "\nTu ---" << lookupWord << "--- chua duoc them vao tu dien";
}

void AddWord(Node* root)
{
	string word;
	cin.ignore();
	std::cout << "\nNhap tu can them vao tu dien: ";
	std::cin >> word;

	Node *tem = root;
	int inW;
	bool isNew = false;
	int i = 0;//chỉ số của từ ký tự trong từ đó.
	for (;i < word.length(); i++)
	{
		inW = word[i] - 'a';
		if (!tem->children[inW] && tem->enOfWord == 0)
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
			if (!tem->children[inW])
				tem->children[inW] = InitializeNode();
			tem = tem->children[inW];
		}
		tem->enOfWord = ++numberOfWord;

		//thêm vào  file tập tin words, và description
		string descrip;
		std::cout << "\nNhap mo ta cho tu: ";
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
		std::cout << "\nThem thanh cong";
		writeFile.close();
	}
	else
		std::cout << "\nTu nay da co trong tu dien!";
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

void UpdateWord(Node* root)
{
	string descrip, word;
	std::cout << "\nNhap tu can cap nhat mo ta: ";
	cin.ignore();
	getline(std::cin, word);
	short update = 2;
	int pos = Search(root, word);
	if(pos > 0)
		std::cout<< "\nTu nay da co trong tu dien! Ban co muon cap nhat lai tu nay?\n1. Co.\nKhac. Khong. ";
	
	std::cin >> update;
	if (update != 1)
		return;
	//Nhập mô tả mới
	std::cout << "\nNhap mo ta moi: ";
	std::cin.ignore();
	getline(std::cin, descrip);

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
		//fflush(stdin);
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
			//result += " ";
			result += description;
		}
	}
	readFile.close();
	//ghi du lieu vao lai file
	ofstream writeFile;
	writeFile.open("description.txt");
	writeFile << result;
	std::cout << "\nCap nhat thanh cong";
	writeFile.close();
}

void DeleteWord(Node* root)
{
	string word;
	std::cout << "\nNhap tu can xoa: ";
	cin.ignore();
	getline(std::cin, word);

	int pos;
	Node *tem = root;
	for (int i = 0; i < word.length(); i++)
	{
		pos = word[i] - 'a';
		if (!tem->children[pos])
		{
			cout << "\nTu nay KHONG co trong tu dien!";
			exit(1);
		}

		tem = tem->children[pos];
	}
	pos = tem->enOfWord;
	//đặt lại dấu hiệu kết thúc của từ này = 0 tức là đây không còn là từ đã có trong từ điển.
	tem->enOfWord = 0;
	numberOfWord--;
	//đọc dữ liệu từ file description lên theo từng dòng, đến dòng cần cập nhật (dòng có chỉ số trùng với pos vừa tìm ở trên) thì chỉ đọc dòng đó lên mà không ghi vào, thay thế bằng descrip vừa nhập
	ifstream readFile;
	ofstream writeFile;
	int index;
	//biến result lưu kết quả của file words/description sau khi đã cập nhật xong, sau đó ghi lại vào file
	string result = "", temp;
	bool flag = 0;

	readFile.open("description.txt"); if (readFile.fail()) { std::cout << "\nFail to open file description"; return; }
		
	while (!readFile.eof())
	{
		if (flag == 1 && pos != index)
			result += "\n";
		flag = 1;

		readFile >> index;
		getline(readFile, temp);

		if (index < pos)
		{
			result += to_string(index);
			result += temp;
		}
		if(index > pos)
		{
			result += to_string(index-1);
			result += temp;
		}
			
	}
	readFile.close();
	//ghi du lieu vao lai file
	writeFile.open("description.txt");
	writeFile << result;
	writeFile.close();
	//---------------------------------------------------------------------------------------------------------------------//
	//đọc dữ liệu từ file words lên theo từng dòng, đến dòng cần cập nhật (dòng có chỉ số trùng với pos vừa tìm ở trên) thì chỉ đọc dòng đó lên mà không ghi vào, thay thế bằng descrip vừa nhập

	flag = 0;
	result = "";
	readFile.open("words.txt"); if (readFile.fail()) { std::cout << "Fail to open file words"; return; }

	while (!readFile.eof())
	{
		if (flag == 1 && pos!=index)
			result += "\n";
		flag = 1;

		readFile >> index;
		getline(readFile, temp);

		if (index < pos)
		{
			result += to_string(index);
			//result += " ";
			result += temp;
		}
		else if (index > pos)
		{
				result += to_string(index - 1);
				result += temp;
		}
	}
	readFile.close();
	//ghi du lieu vao lai file
	writeFile.open("words.txt");
	writeFile << result;
	writeFile.close();
	std::cout << "\nXoa thanh cong";
}
int main()
{
	Node *root = InitializeNode();

	ContructTrie(root);

	int x;
	while(true)
	{
		std::cout << "\n1. Them tu vung.";
		std::cout << "\n2. Tra tu.";
		std::cout << "\n3. Cap nhat phan mo ta cua tu.";
		std::cout << "\n4. Xoa tu.";
		std::cout << "\nKhac. Thoat.";
		std::cout << "\nNhap lua chon: ";
		std::cin >> x;
		std::cout << "\n";
		if (x == 1)
			AddWord(root);
		else if (x == 2)
			LookupWord(root);
		else if (x == 3)
			UpdateWord(root);
		else if(x==4)
			DeleteWord(root);
		else
			break;
	}
	
	system("pause");
	return 0;
}