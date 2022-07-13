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
        while ((str.size()>0) && (0 == getPos(str)))
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
public:
    void split(string str)
    {
        words.clear();
        //str = lowerCase(str);
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
public:
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