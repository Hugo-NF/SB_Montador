#ifndef MONTADOR_PREPROCESSOR_HPP
#define MONTADOR_PREPROCESSOR_HPP

#include <deque>
#include <map>
#include <regex>
#include <string>
#include "console.hpp"

#define IF_TRUE_EVAL "1"

#define COMMENTS ";(.*)"
#define TABS_SPACES "(\t|\\s)+"
#define LINE_BEGIN "^ "
#define LABEL_DIV "(\t|\\s)+:"

#define LABEL_REGEX "^[A-Za-z](\\w+)?$"
#define NUMBER_REGEX "^[0-9]+$"
#define LINE_REGEX "^([A-Za-z](\\w+)?):(.*)?"

#define DIR_EQU_REGEX "(.*): EQU (.*)"
#define DIR_IF_REGEX "^IF (.*)$"

#define SEC_TEXT_REGEX "^(\t|\\s)*SECTION TEXT(\t|\\s)*$"
#define SEC_DATA_REGEX "^(\t|\\s)*SECTION DATA(\t|\\s)*$"
#define SEC_BSS_REGEX  "^(\t|\\s)*SECTION BSS(\t|\\s)*$"

using namespace std;
using namespace console;

class preprocessor{
private:
    deque<pair<int, string>> text;              // Holds the text with it's original line number

    deque<regex> sections;                      // Regular expressions to catalog sections
    deque<regex> symbols;                       // Regular expressions to catalog symbols
    deque<regex> formatting;                    // Regular expressions to format input file
    deque<regex> directives;                    // Regular expressions to preprocessor directives
    map<string, string> equ_definitions;        // Holds EQUs definitions
    bool if_eval;

    bool is_section(int line);
    void clear_line(string& line);
    bool found_section();
    bool valid_label(string& label);
    bool has_label(int line);
    bool is_equ(int line);
    bool is_if(int line);

public:
    int text_section, data_section, bss_section;
    map<string, int> labels_addresses;          // Holds the line for each valid label

    explicit preprocessor(deque<string>& file_text){
        text_section = data_section = bss_section = -1;
        if_eval = false;

        sections.emplace_back(regex(SEC_TEXT_REGEX, regex::ECMAScript|regex::icase));
        sections.emplace_back(regex(SEC_DATA_REGEX, regex::ECMAScript|regex::icase));
        sections.emplace_back(regex(SEC_BSS_REGEX, regex::ECMAScript|regex::icase));

        symbols.emplace_back(regex(LABEL_REGEX, regex::ECMAScript|regex::icase));
        symbols.emplace_back(regex(NUMBER_REGEX, regex::ECMAScript|regex::icase));
        symbols.emplace_back(regex(LINE_REGEX, regex::ECMAScript));

        formatting.emplace_back(regex(COMMENTS, regex::ECMAScript));
        formatting.emplace_back(regex(TABS_SPACES, regex::ECMAScript));
        formatting.emplace_back(regex(LINE_BEGIN, regex::ECMAScript));
        formatting.emplace_back(regex(LABEL_DIV, regex::ECMAScript));

        directives.emplace_back(regex(DIR_EQU_REGEX, regex::ECMAScript|regex::icase));
        directives.emplace_back(regex(DIR_IF_REGEX, regex::ECMAScript|regex::icase));

        for(int line = 0; line < file_text.size(); line++) {
            clear_line(file_text[line]);
            if(!file_text[line].empty())
                text.emplace_back(line + 1, file_text[line]);
        }
    }

    virtual ~preprocessor() = default;

    deque<pair<int, string>>& process_file();
};

#endif //MONTADOR_PREPROCESSOR_HPP
