#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstring>
#include <vector>
#include <bitset>
#include <algorithm>
#include <fstream>
#include <map>

#define VAR_ARRAY_SIZE 4

void Zhegalkin_prepare (std::vector<std::string> &variables) {
    std::bitset<VAR_ARRAY_SIZE> variable = 0;
    uint max_number = static_cast<uint>(pow(2, VAR_ARRAY_SIZE));
    std::string index = "";
    bool first_number;

    for (uint i = 0; i < max_number; ++i) {
        variable = i;
        first_number = true;
        for (uint j = 0; j < VAR_ARRAY_SIZE; ++j) {
            if (variable[j] == true) {
                if (first_number) {
                    index = std::to_string(j + 1);
                    first_number = false;
                }
                else index += ":" + std::to_string(j + 1);
            }
        }
        if (index == "") {
            variables.emplace_back("0");
        } else {
            variables.push_back(index);
        }
        index = "";
    }
    for (auto i = variables.begin(); i != variables.end();) {
        *i = std::to_string(std::count(i->begin(), i->end(), ':')) + ")" + *i;
        ++i;
    }
    std::sort(variables.begin(), variables.end());
    for (auto i = variables.begin(); i != variables.end();) {
        i->erase(0, i->find(")") + 1);
        ++i;
    }
}

void Zhegalkin_solve (std::map<std::string, bool> values, std::vector<std::string> variables, std::string function) {
    std::bitset<VAR_ARRAY_SIZE> variable = 0;
    values["0"] = true;
    std::pair<uint, std::vector<std::string>> current_koef;
    uint current_koef_result = 0;
    int dividor_position = -1;


    for (auto i = variables.begin() + 1; i != variables.end(); ++i) {
        //current coefficient form
        current_koef.first = std::count(i->begin(), i->end(), ':') + 1;
        for (uint j = 0; j < current_koef.first; ++j) {
            if (current_koef.first == 1) {
                current_koef.second.push_back(i->substr(0, i->size()));
            }
            else if (i->find(':', dividor_position + 1) == std::string::npos) {
                current_koef.second.push_back(i->substr(dividor_position + 1, i->size() - dividor_position));
            } else {
                current_koef.second.push_back(i->substr(dividor_position + 1,  i->find(':', dividor_position + 1) - (dividor_position + 1)));
                dividor_position = i->find(':', dividor_position + 1);
            }
        }
        dividor_position = -1;

        //current coefficient calculation
        std::string sum = "";
        for (auto l = variables.begin(); l != i; ++l) {
            if (*l == "0") {
                current_koef_result = (current_koef_result + 1) % 2;
            } else if (std::count(l->begin(), l->end(), ':') == current_koef.first - 1) {
                break;
            } else {
                uint m = 0;
                for (uint u = 0; u < current_koef.first; ++u) {
                    if (l->find(current_koef.second[u]) != std::string::npos)
                        ++m;
                }
                if (m == std::count(l->begin(), l->end(), ':') + 1) {
                    current_koef_result = (current_koef_result + values[*l]) % 2;
                }
            }
        }
            for (uint j = 0; j < current_koef.first; ++j) {
                variable[stoi(current_koef.second[j]) - 1] = true;
            }
            if (function[variable.to_ulong()] == '1') {
                if (current_koef_result == 1) {
                    values[*i] = false;
                } else values[*i] = true;
            } else {
                if (current_koef_result == 0) {
                    values[*i] = false;
                } else values[*i] = true;
            }
        current_koef.second.clear();
        current_koef_result = 0;
        variable = 0;
    }

    //outputting the result
    uint end_circle = static_cast<uint>(pow(2, VAR_ARRAY_SIZE) - 1);
    std::pair<uint, std::vector<std::string>> to_string_convert;
    to_string_convert.first = 0;
    std::string key = "";
    for (variable = 0;; variable = variable.to_ulong() + 1) {
        if (variable.to_ulong() == 0) {
            key = "0";
        } else {
            for (uint i = 0; i < VAR_ARRAY_SIZE; ++i) {
                if (variable[i] == true) {
                    key += std::to_string(i + 1) + ":";
                }
            }
            key = key.substr(0, key.size() - 1);
        }
        std::cout << values[key];
        if (variable.to_ulong() == end_circle)
            break;
        key = "";
    }
}

int main() {
    std::map<std::string, bool> values;
    std::vector<std::string> variables;
    Zhegalkin_prepare(variables);
    char buffer[16];
    char *function = new char [16];
    std::cout << std::endl;
    std::cin >> buffer;
    if (!variables.empty())
        strcpy(function, buffer);
    Zhegalkin_solve(values, variables, std::to_string(*function));
    *function = ' ';
    return 0;
}
