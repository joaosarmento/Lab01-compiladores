#include "lexer.h"
#include <iostream>
#include <sstream>
using std::cin;
using std::cout;
using std::stringstream;

// construtor 
Lexer::Lexer()
{
    // insere as palavras-reservadas na tabela de id's
    id_table["true"] = Id{ Tag::TRUE, "true" };
    id_table["false"] = Id{ Tag::FALSE, "false" };
}

// ignora comentários
void Lexer::ignoreComments()
{
    if (peek == '/') {
        char next = cin.get();
        if (next == '/') { // single-line comment
            while (peek != '\n' && peek != EOF) {
                peek = cin.get();
            }
        } else if (next == '*') { // multi-line comment
            while (true) {
                peek = cin.get();
                if (peek == EOF) break; // End of file
                if (peek == '*') {
                    if ((peek = cin.get()) == '/') break; // End of comment
                }
            }
        } else {
            cin.unget(); // Not a comment, put back the character
        }
    }
}

// retorna tokens para o analisador sintático
Token Lexer::Scan()
{
    // salta espaços em branco, tabulações e novas linhas
    while (isspace(peek)) {
        if (peek == '\n')
            line += 1;
        peek = cin.get();
    }

    // Ignorar comentários antes de processar o token
    ignoreComments();

    // Se peek é EOF após ignorar comentários, retornar um token EOF
    if (peek == EOF) return Token{EOF_TAG};

    // retorna números
    if (isdigit(peek)) {
        int v = 0;

        do {
            // converte caractere 'n' para o dígito numérico n
            int n = peek - '0';
            v = 10 * v + n;
            peek = cin.get();
        } while (isdigit(peek));

        // DEBUG: exibe o token reconhecido
        cout << "<NUM," << v << "> ";

        // retorna o token NUM
        return Num{v};
    }

    // retorna palavras-chave e identificadores
    if (isalpha(peek)) {
        stringstream ss;

        do {
            ss << peek;
            peek = cin.get();
        } while (isalpha(peek));

        string s = ss.str();
        auto pos = id_table.find(s);

        // se o lexema já está na tabela
        if (pos != id_table.end()) {
            // DEBUG: exibe o token reconhecido
            switch (pos->second.tag) {
                case TRUE: cout << "<TRUE," << pos->second.name << "> "; break;
                case FALSE: cout << "<FALSE," << pos->second.name << "> "; break;
                default: cout << "<ID," << pos->second.name << "> "; break;
            }

            // retorna o token ID
            return pos->second;
        }

        // se o lexema ainda não está na tabela
        Id new_id {Tag::ID, s};
        id_table[s] = new_id;

        // DEBUG: exibe o token reconhecido
        cout << "<ID," << new_id.name << "> ";

        // retorna o token ID
        return new_id;
    }

    // operadores (e caracteres não alphanuméricos isolados)
    Token t {peek};
    peek = ' ';

    // DEBUG: exibe o token para o caractere
    cout << "<" << char(t.tag) << "> ";

    // retorna o token para o caractere isolado
    return t;
}
