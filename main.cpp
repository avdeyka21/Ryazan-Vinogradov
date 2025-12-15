#include <iostream>
#include <fstream>
#include <list>
#include <cctype>
#include <unordered_map>
#include <string>

using namespace std;

string removePunctuation(const string& str) {
    string result;
    remove_copy_if(str.begin(), str.end(),
                   back_inserter(result),
                   [](unsigned char c){ return ispunct(c); });
    return result;
}

bool check_polindrom(string word) {
    int len = word.length();
    for(int i = 0; i < len/2; ++i) {
        if(word[i] != word[len-i-1]) {
            return false;
        }
    }
    return true;
}

int main() {
    string filename = "file.txt";

    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл '" << filename << "'" << endl;
        return 1;
    }

    string s;
    list<string> vec;
    list<string> pali;
    list<string> array;

    // Чтение файла
    while (infile >> s) {
        vec.push_back(s);
    }
    infile.close();

    // Проверка размера файла
    ifstream check_file(filename, ios::ate);
    streamsize size = check_file.tellg();
    check_file.close();

    // Преобразование в нижний регистр
    for (string& word1 : vec) {
        for (char& c : word1) {
            c = tolower(static_cast<unsigned char>(c));
        }
    }

    // Удаление пунктуации
    for (auto &text : vec) {
        string new_text = removePunctuation(text);
        array.push_back(new_text);
    }

    // Поиск палиндромов
    for (auto &s : array) {
        if (check_polindrom(s)) {
            pali.push_back(s);
        }
    }

    // Удаление однобуквенных "палиндромов"
    pali.remove_if([](const string& str) {
        return str.length() == 1;
    });

    // Подсчет частоты
    unordered_map<string, int> umap;
    for (const string& word : pali) {
        umap[word]++;
    }
    umap.erase("");

    // Вывод результатов
    if (umap.empty()) {
        cout << "В файле не найдено слов-палиндромов." << endl;
    } else {
        cout << "\nСлова-палиндромы и их количество:" << endl;
        cout << "==================================" << endl;
        for (const auto& pair : umap) {
            cout << "'" << pair.first << "': " << pair.second << " раз(а)" << endl;
        }
        cout << "==================================" << endl;
        cout << "Всего найдено " << umap.size() << " уникальных слов-палиндромов." << endl;
    }

    return 0;
}