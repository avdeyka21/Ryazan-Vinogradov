#include "test_runner.h"
#include <fstream>
#include <list>
#include <cctype>
#include <sstream>
#include <algorithm>

using namespace std;

// Функции из вашей программы (скопированы для тестирования)
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

// Функция, которая выполняет логику вашей программы и возвращает результат
unordered_map<string, int> processFile(const string& filename) {
    ifstream infile(filename);
    string s;
    list<string> vec;
    list<string> pali;

    if (infile.is_open()) {
        while (infile >> s) {
            vec.push_back(s);
        }
        infile.close();
    } else {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return {};
    }

    // Преобразование в нижний регистр
    for (string& word1 : vec) {
        for (char& c : word1) {
            c = tolower(static_cast<unsigned char>(c));
        }
    }

    // Удаление пунктуации
    list<string> array;
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

    return umap;
}

// Тестовые функции для 3 разных файлов
void TestFile1() {
    // Создаем первый тестовый файл
    ofstream outfile("test1.txt");
    outfile << "level radar kayak racecar madam";
    outfile.close();

    auto result = processFile("test1.txt");

    unordered_map<string, int> expected = {
            {"level", 1},
            {"radar", 1},
            {"kayak", 1},
            {"racecar", 1},
            {"madam", 1}
    };

    ASSERT_UNORDERED_MAP_EQUAL(result, expected);

    // Удаляем временный файл
    remove("test1.txt");
}

void TestFile2() {
    // Создаем второй тестовый файл с повторениями
    ofstream outfile("test2.txt");
    outfile << "level level level radar radar wow noon noon";
    outfile.close();

    auto result = processFile("test2.txt");

    unordered_map<string, int> expected = {
            {"level", 3},
            {"radar", 2},
            {"wow", 1},
            {"noon", 2}
    };

    ASSERT_UNORDERED_MAP_EQUAL(result, expected);

    // Удаляем временный файл
    remove("test2.txt");
}

void TestFile3() {
    // Создаем третий тестовый файл с пунктуацией и разным регистром
    ofstream outfile("test3.txt");
    outfile << "Madam, I'm Adam! Level test. RACECAR? Bob, Anna. Civic!";
    outfile.close();

    auto result = processFile("test3.txt");

    unordered_map<string, int> expected = {
            {"madam", 1},
            {"level", 1},
            {"racecar", 1},
            {"anna", 1},
            {"civic", 1},
            {"bob", 1}
    };

    ASSERT_UNORDERED_MAP_EQUAL(result, expected);

    // Удаляем временный файл
    remove("test3.txt");
}

// Дополнительные тесты для проверки разных случаев
void TestEmptyFile() {
    ofstream outfile("test_empty.txt");
    outfile.close();

    auto result = processFile("test_empty.txt");
    ASSERT_EQUAL(result.size(), 0);

    remove("test_empty.txt");
}

void TestNoPalindromes() {
    ofstream outfile("test_no_pal.txt");
    outfile << "hello world programming computer";
    outfile.close();

    auto result = processFile("test_no_pal.txt");
    ASSERT_EQUAL(result.size(), 0);

    remove("test_no_pal.txt");
}

void TestOneLetterWords() {
    ofstream outfile("test_one_letter.txt");
    outfile << "a b c aa bb cc aba abcba";
    outfile.close();

    auto result = processFile("test_one_letter.txt");

    // Однобуквенные слова должны быть удалены
    unordered_map<string, int> expected = {
            {"aa", 1},
            {"bb", 1},
            {"cc", 1},
            {"aba", 1},
            {"abcba", 1}
    };

    ASSERT_UNORDERED_MAP_EQUAL(result, expected);

    remove("test_one_letter.txt");
}

// Функция для запуска всех тестов
void TestAll() {
    TestRunner runner;

    // Основные тесты с тремя разными файлами
    RUN_TEST(runner, TestFile1);
    RUN_TEST(runner, TestFile2);
    RUN_TEST(runner, TestFile3);

    // Дополнительные тесты
    RUN_TEST(runner, TestEmptyFile);
    RUN_TEST(runner, TestNoPalindromes);
    RUN_TEST(runner, TestOneLetterWords);
}

int main() {
    try {
        TestAll();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}