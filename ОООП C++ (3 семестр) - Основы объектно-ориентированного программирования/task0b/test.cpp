#include "pch.h"

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class FileReader
{
    string finput;
    string str;
    fstream file;
    bool error;
    string getPath(string path)
    {
        int k = path.size();
        while (path[k] != '\\')
        {
            k--;
        }
        return path.erase(k);
    }
public:
    FileReader(string path, string finput)
    {
        error = false;
        if (finput != "")
        {
            this->finput = getPath(path) + '\\' + finput;
            file.open(this->finput);
            if (!file.is_open())
            {
                cout << "Error open in input file!\n";
                error = true;
            }
        }
        else
        {
            error = true;
            cout << "No input file!\n";
        }
    }
    bool checkError()
    {
        return error;
    }
    bool nextLine()
    {
        return (bool)getline(file, str);
    }
    string getLine()
    {
        return str;
    }
    ~FileReader()
    {
        file.close();
    }
};

class TokenService
{
    vector <string> words;
public:
    string lowerCase(string str)
    {
        int i = 0;
        while (i < str.size())
        {
            if ((str[i] >= 'A') && (str[i] <= 'Z'))
            {
                str[i] += 32;
            }
            i++;
        }
        return str;
    }
    int getPos(string str)
    {
        int i = 0;
        while ((i < str.size()) && (((str[i] >= 'A') && (str[i] <= 'Z')) || ((str[i] >= 'a') && (str[i] <= 'z')) || ((str[i] >= 'А') && (str[i] <= 'Я')) || ((str[i] >= 'а') && (str[i] <= 'я')) || ((str[i] >= '0') && (str[i] <= '9'))))
        {
            i++;
        }
        return i;
    }
    int getShift(string str)
    {
        int i = 0;
        while ((str.size() > 0) && (0 == getPos(str)))
        {
            i++;
            str = str.substr(1);
        }
        if (str.size() == 0)
        {
            return -1;
        }
        return i;
    }
    void split(string str)
    {
        words.clear();
        str = lowerCase(str);
        int pos = getPos(str);
        while (pos != -1)
        {
            if (pos == 0)
            {
                pos = getShift(str);
                if (pos == -1)
                {
                    break;
                }
                str = str.substr(pos);
                pos = getPos(str);
            }
            string word = str.substr(0, pos);
            words.push_back(word);
            str = str.substr(pos);
            pos = getPos(str);
        }
    }
    vector<string> getWords()
    {
        return words;
    }
};

class WordStatistic
{
    vector<pair<string, int>> words_;
    vector<double> frequencies;
    vector<vector<string>>stat;
    map <string, int> words;
    map <string, int> ::iterator it;
public:
    int getSumElements()
    {
        int k = 0;
        it = words.begin();
        for (int i = 0; it != words.end(); i++, it++)
        {
            k += it->second;
        }
        return k;
    }
    void frequency()
    {
        int max_frequency = words_[0].second;
        frequencies.resize(max_frequency + 1);
        frequencies[0] = 0;
        int sum_elements = getSumElements();
        for (int i = 1; i < frequencies.size(); i++)
        {
            frequencies[i] = (double)i / (double)sum_elements * 100;
        }
    }
    void sorting()
    {
        it = words.begin();
        pair <string, int> word_pair;
        for (int i = 0; i < words.size(); it++, i++)
        {
            word_pair.first = it->first;
            word_pair.second = it->second;
            words_.push_back(word_pair);
        }
        sort(words_.begin(), words_.end(), [](auto& left, auto& right)
            {
                return left.second > right.second;
            });
    }
    void addWord(string word)
    {
        it = words.find(word);
        if (it == words.end())
        {
            words[word] = 1;
        }
        else
        {
            it->second = it->second + 1;
        }
    }
    vector<vector<string>> getStat()
    {
        sorting();
        frequency();
        stat.resize(words.size());
        for (int i = 0; i < words.size(); i++)
        {
            stat[i].push_back(words_[i].first);
            stat[i].push_back(to_string(words_[i].second));
            stat[i].push_back(to_string(frequencies[words_[i].second]) + '%');
        }
        return stat;
    }
};

class ReportService
{
    string foutput;
    ofstream file;
    bool error;
    string getPath(string path)
    {
        int k = path.size();
        while (path[k] != '\\')
        {
            k--;
        }
        return path.erase(k);
    }
public:
    ReportService(string path, string foutput)
    {
        error = false;
        if (foutput != "")
        {
            this->foutput = getPath(path) + '\\' + foutput;
            file.open(this->foutput);
            if (!file.is_open())
            {
                cout << "Error open in output file!\n";
                error = true;
            }
        }
        else
        {
            error = true;
            cout << "No output file!\n";
        }
    }
    bool checkError()
    {
        return error;
    }
    void printSVG(vector<vector<string>> stat)
    {
        for (int i = 0; i < stat.size(); i++)
        {
            for (int j = 0; j < stat[i].size(); j++)
            {
                file << stat[i][j];
                if (j != (stat[i].size() - 1))
                {
                    file << ";";
                }
                else
                {
                    file << "\n";
                }
            }
        }
    }
    ~ReportService()
    {
        file.close();
    }
};

/*
int main(int argc, char* argv[])
{
    FileReader FR((string)argv[0], (string)argv[1]);// без _
    if (FR.checkError())
    {
        return 0;
    }
    //отдельно ращбить на компоненты cpp

    TokenService TS;
    WordStatistic WS;
    while (FR.nextLine())
    {
        TS.split(FR.getLine());
        vector<string>v = TS.getWords();
        for (int i = 0; i < v.size(); i++)
        {
            WS.addWord(v[i]);
        }
    }

    ReportService RS((string)argv[0], (string)argv[2]);
    if (RS.checkError())
    {
        return 0;
    }
    else
    {
        RS.printSVG(WS.getStat());
    }

    return 0;
}
*/

TEST(FileReader_test, check1)
{
    FileReader FR("D:\\Files\\Visual Studio\\Proga for 2nd course\\task0b\\Debug\\code.exe", "input.txt");
    EXPECT_FALSE(FR.checkError());
}

TEST(FileReader_test, check2)
{
    FileReader FR("D:\\Files\\Visual Studio\\Proga for 2nd course\\task0b\\Debug\\code.exe", "fbhdfb.txt");
    EXPECT_TRUE(FR.checkError());
}

TEST(FileReader_test, check3)
{
    FileReader FR("D:\\Files\\Visual Studio\\Proga for 2nd course\\task0b\\Debug\\code.exe", "");
    EXPECT_TRUE(FR.checkError());
}

TEST(FileReader_test, check4)
{
    FileReader FR("D:\\Files\\Visual Studio\\Proga for 2nd course\\task0b\\Debug\\code.exe", "input.txt");
    EXPECT_TRUE(FR.nextLine());
    EXPECT_EQ("hill key keep key", FR.getLine());
    EXPECT_TRUE(FR.nextLine());
    EXPECT_EQ("heap coap cheap cheap", FR.getLine());
    EXPECT_TRUE(FR.nextLine());
    EXPECT_EQ("key hill milk", FR.getLine());
    EXPECT_TRUE(FR.nextLine());
    EXPECT_EQ("down up key milk", FR.getLine());
    EXPECT_TRUE(FR.nextLine());
    EXPECT_EQ("key heap", FR.getLine());
    EXPECT_FALSE(FR.nextLine());
}

TEST(TokenService_test, check1)
{
    TokenService TS;
    EXPECT_EQ("qwertyuiop", TS.lowerCase("QwErTyUiOp"));
    EXPECT_EQ("asdfghjkl", TS.lowerCase("ASDFGHJKL"));
    EXPECT_EQ("zxcvbnm,./", TS.lowerCase("zxcvbnm,./"));
}

TEST(TokenService_test, check2)
{
    TokenService TS;
    EXPECT_EQ(0, TS.getPos(".,!.,.qwert"));
    EXPECT_EQ(5, TS.getPos("qwert qwert"));
    EXPECT_EQ(0, TS.getPos("    qwert qwert"));
}

TEST(TokenService_test, check3)
{
    TokenService TS;
    EXPECT_EQ(6, TS.getShift(".,!.,.qwert"));
    EXPECT_EQ(0, TS.getShift("qwert qwert"));
    EXPECT_EQ(4, TS.getShift("    qwert qwert"));
}

TEST(TokenService_test, check4)
{
    TokenService TS;
    vector <string> words;
    words.push_back("key");
    words.push_back("lift");
    words.push_back("split");
    string str = "key lift split";
    TS.split(str);
    EXPECT_EQ(words, TS.getWords());
    words.clear();
    words.push_back("key");
    words.push_back("lift");
    words.push_back("split");
    str = "Key, !/lift- _ - ==&# //split";
    TS.split(str);
    EXPECT_EQ(words, TS.getWords());
}

TEST(Word_Static_test, check1)
{
    WordStatistic WS;
    vector<vector<string>>stat;
    stat.resize(4);
    double a = 40.0;
    double b = 30.0;
    double c = 20.0;
    double d = 10.0;
    stat[0].push_back("keep");
    stat[0].push_back(to_string(4));
    stat[0].push_back(to_string(a) + '%');
    stat[1].push_back("cheap");
    stat[1].push_back(to_string(3));
    stat[1].push_back(to_string(b) + '%');
    stat[2].push_back("pick");
    stat[2].push_back(to_string(2));
    stat[2].push_back(to_string(c) + '%');
    stat[3].push_back("pix");
    stat[3].push_back(to_string(1));
    stat[3].push_back(to_string(d) + '%');
    WS.addWord("keep");
    WS.addWord("cheap");
    WS.addWord("keep");
    WS.addWord("pick");
    WS.addWord("cheap");
    WS.addWord("keep");
    WS.addWord("pick");
    WS.addWord("pix");
    WS.addWord("keep");
    WS.addWord("cheap");
    EXPECT_EQ(stat, WS.getStat());
}

TEST(ReportService_test, check1)
{
    ReportService RS("D:\\Files\\Visual Studio\\Proga for 2nd course\\task0b\\Debug\\code.exe", "output.csv");
    EXPECT_FALSE(RS.checkError());
}

TEST(ReportService_test, check2)
{
    ReportService RS("D:\\Files\\Visual Studio\\Proga for 2nd course\\task0b\\Debug\\code.exe", "");
    EXPECT_TRUE(RS.checkError());
}