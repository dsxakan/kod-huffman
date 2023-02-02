#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

// Требуется установить C++17 в настройках проекта

// Структура для представления узла дерева
struct Node {
    char symbol;
    int freq;
    Node* left;
    Node* right;

    // Конструктор для создания листьев дерева
    Node(char symbol, int freq) : symbol(symbol), freq(freq), left(nullptr), right(nullptr) {}

    // Конструктор для создания внутренних узлов дерева
    Node(Node* left, Node* right) : symbol('\0'), freq(left->freq + right->freq),
        left(left), right(right) {}

    // Функция для сравнения узлов по частоте
    bool operator<(const Node& other) const {
        return freq > other.freq;
    }
};

// Функция для создания дерева Хаффмана
Node* BuildHuffmanTree(unordered_map<char, int>& freq_table) {
    // Создание кучи из узлов
    priority_queue<Node> nodes;
    for (const auto& [symbol, freq] : freq_table) {
        nodes.emplace(symbol, freq);
    }

    // Слияние узлов в дерево
    while (nodes.size() > 1) {
        Node* left = new Node(nodes.top());
        nodes.pop();
        Node* right = new Node(nodes.top());
        nodes.pop();
        nodes.emplace(left, right);
    }

    return new Node(nodes.top());
}

// Функция для создания таблицы кодирования
unordered_map<char, string> BuildEncodingTable(Node* root) {
    unordered_map<char, string> encoding_table;
    function<void(Node*, string)> dfs = [&](Node* node, string code) {
        if (!node) return;
        if (node->symbol) {
            encoding_table[node->symbol] = code;
            return;
        }
        dfs(node->left, code + '0');
        dfs(node->right, code + '1');
    };
    dfs(root, "");
    return encoding_table;
}

// Функция для кодирования текста
string Encode(const string& text, const unordered_map<char, string>& encoding_table) {
    string encoded_text;
    for (const char symbol : text) {
        encoded_text += encoding_table.at(symbol);
    }
    return encoded_text;
}

// Функция для декодирования текста
string Decode(const string& encoded_text, unordered_map<string, char>& encoding_table){
    string cur = "";
    string res = "";
    for (char c : encoded_text) {
        cur += c;
        if (encoding_table.count(cur)) {
            res+=encoding_table[cur];
            cur = "";
        }
    }
    return res;
}

void read_table(ifstream& in, unordered_map<string, char>& table) {
    char ch;
    string code;
    while (in >> code >> ch) {
        table[code] = ch;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    cout << "Введи режим работы (1 - кодирование, 2 - декодирование): ";
    int mode;
    cin >> mode;
    if (mode == 1) {
        // Чтение текста из файла
        cout << "Введи название входного файла: ";
        string input_filename;
        cin >> input_filename;
        ifstream input_file(input_filename);
        string text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
        cout << text << endl;
        // Подсчет частот символов
        unordered_map<char, int> freq_table;
        for (const char symbol : text) {
            ++freq_table[symbol];
        }

        // Запись закодированного текста в файл
        cout << "Введи название файла для сохранения кода: ";
        string output_filename;
        cin >> output_filename;

        // Запись таблицы в файл
        cout << "Введи название файла для сохранения таблицы: ";
        string table_filename;
        cin >> table_filename;

        // Создание дерева Хаффмана и таблицы кодирования
        Node* root = BuildHuffmanTree(freq_table);
        unordered_map<char, string> encoding_table = BuildEncodingTable(root);

        // Кодирование текста
        string encoded_text = Encode(text, encoding_table);

        // Сохранение закодированного
        ofstream output_file(output_filename);
        output_file << encoded_text;

        // Сохранение таблицы кодирования в файл
        ofstream table_file(table_filename);
        for (const auto& pair : encoding_table) {
            table_file << pair.second << " " << pair.first << endl;
        }
    }
    
    else if (mode == 2) 
    {
        // Чтение закодированного текста из файла
        cout << "Введи название входного файла: ";
        string input_filename;
        cin >> input_filename;
        ifstream input_file(input_filename);
        string encoded_text((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());

        cout << "Введи название файла с таблицей: ";
        string table_filename;
        cin >> table_filename;
        ifstream input_table(table_filename);

        // Чтение таблицы кодирования
        unordered_map<string, char> encoding_table;
        read_table(input_table, encoding_table);

        // Декодирование текста
        string decoded_text = Decode(encoded_text, encoding_table);
        cout << decoded_text << endl;
        // Запись декодированного текста в файл
        cout << "Введи название выходного файла: ";
        string output_filename;
        cin >> output_filename;
        ofstream output_file(output_filename);
        output_file << decoded_text;
    }
    else {
        cout << "Неправильный режим" << endl;
        return 1;
    }
    
    return 0;
}
