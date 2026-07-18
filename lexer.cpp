#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include "lexer.h"

using namespace std;

vector<Token> tokens;

static const string KEYWORDS[] = {
    "int", "float", "char", "double", "bool", "void",
    "if", "else", "for", "while", "return",
    "true", "false"
};

static bool isKeyword(const string &word)
{
    for (const string &k : KEYWORDS)
        if (word == k) return true;
    return false;
}

static bool isIdentStart(char c) { return isalpha((unsigned char)c) || c == '_'; }
static bool isIdentChar(char c)  { return isalnum((unsigned char)c) || c == '_'; }

static void addToken(const string &type, const string &value, int line)
{
    tokens.push_back(Token{type, value, line});
}

// Two-character operators must be checked before their single-char prefixes.
static const string TWO_CHAR_OPS[] = {
    "==", "!=", "<=", ">=", "&&", "||",
    "++", "--", "+=", "-=", "*=", "/="
};

void lexicalAnalysis(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        throw LexError{"Could not open source file: " + filename, 0};
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string src = buffer.str();
    file.close();

    tokens.clear();

    size_t i = 0;
    int line = 1;
    size_t n = src.size();

    while (i < n)
    {
        char c = src[i];

        // Whitespace / newlines
        if (c == '\n') { line++; i++; continue; }
        if (isspace((unsigned char)c)) { i++; continue; }

        // Line comment
        if (c == '/' && i + 1 < n && src[i + 1] == '/')
        {
            while (i < n && src[i] != '\n') i++;
            continue;
        }

        // Block comment
        if (c == '/' && i + 1 < n && src[i + 1] == '*')
        {
            int startLine = line;
            i += 2;
            bool closed = false;
            while (i + 1 < n)
            {
                if (src[i] == '\n') line++;
                if (src[i] == '*' && src[i + 1] == '/') { i += 2; closed = true; break; }
                i++;
            }
            if (!closed)
                throw LexError{"Unterminated block comment", startLine};
            continue;
        }

        // Identifier / keyword
        if (isIdentStart(c))
        {
            string word;
            int startLine = line;
            while (i < n && isIdentChar(src[i])) word += src[i++];

            if (isKeyword(word))
                addToken("KEYWORD", word, startLine);
            else
                addToken("IDENTIFIER", word, startLine);
            continue;
        }

        // Number (integer or float, e.g. 42 or 3.14)
        if (isdigit((unsigned char)c))
        {
            string num;
            int startLine = line;
            bool sawDot = false;
            while (i < n && (isdigit((unsigned char)src[i]) ||
                              (src[i] == '.' && !sawDot)))
            {
                if (src[i] == '.') sawDot = true;
                num += src[i++];
            }
            addToken("NUMBER", num, startLine);
            continue;
        }

        // String literal
        if (c == '"')
        {
            int startLine = line;
            string s;
            i++;
            while (i < n && src[i] != '"')
            {
                if (src[i] == '\n')
                    throw LexError{"Unterminated string literal", startLine};
                s += src[i++];
            }
            if (i >= n)
                throw LexError{"Unterminated string literal", startLine};
            i++; // closing quote
            addToken("STRING", s, startLine);
            continue;
        }

        // Two-character operators
        bool matchedTwo = false;
        if (i + 1 < n)
        {
            string two = src.substr(i, 2);
            for (const string &op : TWO_CHAR_OPS)
            {
                if (two == op)
                {
                    addToken("OPERATOR", two, line);
                    i += 2;
                    matchedTwo = true;
                    break;
                }
            }
        }
        if (matchedTwo) continue;

        // Single-character operators
        static const string SINGLE_OPS = "+-*/=<>!";
        if (SINGLE_OPS.find(c) != string::npos)
        {
            addToken("OPERATOR", string(1, c), line);
            i++;
            continue;
        }

        // Delimiters
        static const string DELIMS = "(){};,";
        if (DELIMS.find(c) != string::npos)
        {
            addToken("DELIMITER", string(1, c), line);
            i++;
            continue;
        }

        // Anything else is a lexical error, not silently dropped.
        throw LexError{string("Unrecognized character '") + c + "'", line};
    }

    addToken("EOF_TOKEN", "", line);
}
