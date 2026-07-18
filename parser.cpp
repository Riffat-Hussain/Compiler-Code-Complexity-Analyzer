#include <iostream>
#include "parser.h"
#include "lexer.h"

using namespace std;

extern vector<Token> tokens;

namespace
{
    size_t pos = 0;

    const Token &peek()
    {
        return tokens[pos];
    }

    bool atEnd()
    {
        return peek().type == "EOF_TOKEN";
    }

    const Token &advance()
    {
        const Token &t = tokens[pos];
        if (!atEnd()) pos++;
        return t;
    }

    bool checkType(const string &type)
    {
        return peek().type == type;
    }

    bool checkValue(const string &value)
    {
        return peek().value == value;
    }

    // Consume a token of the given value or raise a ParseError naming
    // what was expected and what was actually found, with a line number.
    Token expectValue(const string &value, const string &context)
    {
        if (checkValue(value)) return advance();
        throw ParseError{
            "Expected '" + value + "' " + context + " but found '" +
                peek().value + "'",
            peek().line};
    }

    bool isType(const string &v)
    {
        return v == "int" || v == "float" || v == "char" ||
               v == "double" || v == "bool" || v == "void";
    }

    ASTNode *parseExpression();
    ASTNode *parseStatement();
    ASTNode *parseBlock();

    // ---------- Expressions (precedence climbing) ----------

    ASTNode *parsePrimary()
    {
        const Token &t = peek();

        if (t.type == "NUMBER")
        {
            advance();
            return new ASTNode(ND_LITERAL, t.value, t.line);
        }
        if (t.type == "IDENTIFIER" || t.value == "true" || t.value == "false")
        {
            advance();
            return new ASTNode(ND_IDENTIFIER, t.value, t.line);
        }
        if (t.value == "(")
        {
            advance();
            ASTNode *inner = parseExpression();
            expectValue(")", "to close '('");
            return inner;
        }

        throw ParseError{"Expected an expression but found '" + t.value + "'",
                          t.line};
    }

    ASTNode *parseUnary()
    {
        if (checkValue("!") || checkValue("-"))
        {
            Token op = advance();
            ASTNode *node = new ASTNode(ND_UNARY, op.value, op.line);
            node->children.push_back(parseUnary());
            return node;
        }
        return parsePrimary();
    }

    // Shared helper for all left-associative binary levels.
    ASTNode *parseBinaryLevel(ASTNode *(*next)(),
                               const vector<string> &ops)
    {
        ASTNode *left = next();
        while (true)
        {
            bool matched = false;
            for (const string &op : ops)
            {
                if (checkValue(op))
                {
                    Token opTok = advance();
                    ASTNode *right = next();
                    ASTNode *node = new ASTNode(ND_BINARY, opTok.value, opTok.line);
                    node->children.push_back(left);
                    node->children.push_back(right);
                    left = node;
                    matched = true;
                    break;
                }
            }
            if (!matched) break;
        }
        return left;
    }

    ASTNode *parseMultiplicative() { return parseBinaryLevel(parseUnary, {"*", "/"}); }
    ASTNode *parseAdditive()       { return parseBinaryLevel(parseMultiplicative, {"+", "-"}); }
    ASTNode *parseRelational()     { return parseBinaryLevel(parseAdditive, {"<", ">", "<=", ">="}); }
    ASTNode *parseEquality()       { return parseBinaryLevel(parseRelational, {"==", "!="}); }
    ASTNode *parseLogicalAnd()     { return parseBinaryLevel(parseEquality, {"&&"}); }
    ASTNode *parseLogicalOr()      { return parseBinaryLevel(parseLogicalAnd, {"||"}); }

    ASTNode *parseExpression()
    {
        return parseLogicalOr();
    }

    // ---------- Statements ----------

    // Type IDENTIFIER ('=' Expression)? ';'
    //   or
    // Type IDENTIFIER '(' ')' Block     -- a parameterless function definition,
    // supported because the term project's sample program wraps everything
    // in `int main() { ... }`. No parameters/return-type checking is done;
    // this language subset still has no notion of calling functions.
    ASTNode *parseDeclaration()
    {
        Token typeTok = advance(); // consume type keyword
        if (!checkType("IDENTIFIER"))
            throw ParseError{"Expected identifier after type '" + typeTok.value + "'",
                              peek().line};
        Token nameTok = advance();

        if (checkValue("("))
        {
            advance();
            expectValue(")", "to close parameter list of function '" + nameTok.value + "'");
            ASTNode *body = parseBlock();
            ASTNode *funcNode = new ASTNode(ND_FUNC_DEF,
                                             typeTok.value + " " + nameTok.value,
                                             typeTok.line);
            funcNode->children.push_back(body);
            return funcNode;
        }

        ASTNode *node = new ASTNode(ND_VAR_DECL, typeTok.value + " " + nameTok.value,
                                     typeTok.line);

        if (checkValue("="))
        {
            advance();
            node->children.push_back(parseExpression());
        }
        expectValue(";", "to end declaration of '" + nameTok.value + "'");
        return node;
    }

    // IDENTIFIER '=' Expression   |   IDENTIFIER ('++'|'--')
    // Used both as a full statement and inside a for(...) header.
    ASTNode *parseAssignmentOrIncDec(bool consumeSemicolon)
    {
        Token nameTok = advance(); // IDENTIFIER already confirmed by caller

        static const vector<string> compoundOps = {"=", "+=", "-=", "*=", "/="};

        if (checkValue("++") || checkValue("--"))
        {
            Token op = advance();
            ASTNode *node = new ASTNode(ND_INC_DEC, nameTok.value + op.value, nameTok.line);
            if (consumeSemicolon) expectValue(";", "after increment/decrement");
            return node;
        }

        for (const string &op : compoundOps)
        {
            if (checkValue(op))
            {
                advance();
                ASTNode *node = new ASTNode(ND_ASSIGN, nameTok.value + " " + op, nameTok.line);
                node->children.push_back(parseExpression());
                if (consumeSemicolon) expectValue(";", "after assignment to '" + nameTok.value + "'");
                return node;
            }
        }

        throw ParseError{"Expected assignment or increment/decrement after '" +
                              nameTok.value + "'",
                          peek().line};
    }

    // 'if' '(' Expression ')' Block ('else' (If | Block))?
    ASTNode *parseIf()
    {
        Token ifTok = advance();
        expectValue("(", "after 'if'");
        ASTNode *cond = parseExpression();
        expectValue(")", "to close if-condition");
        ASTNode *thenBlock = parseBlock();

        ASTNode *node = new ASTNode(ND_IF, "", ifTok.line);
        node->children.push_back(cond);
        node->children.push_back(thenBlock);

        if (checkValue("else"))
        {
            advance();
            if (checkValue("if"))
                node->children.push_back(parseIf()); // else-if chain
            else
                node->children.push_back(parseBlock());
        }
        return node;
    }

    // 'while' '(' Expression ')' Block
    ASTNode *parseWhile()
    {
        Token whileTok = advance();
        expectValue("(", "after 'while'");
        ASTNode *cond = parseExpression();
        expectValue(")", "to close while-condition");
        ASTNode *body = parseBlock();

        ASTNode *node = new ASTNode(ND_WHILE, "", whileTok.line);
        node->children.push_back(cond);
        node->children.push_back(body);
        return node;
    }

    // 'for' '(' (Decl | Assign | ε) ';' Expr ';' (Assign|IncDec|ε) ')' Block
    ASTNode *parseFor()
    {
        Token forTok = advance();
        expectValue("(", "after 'for'");

        ASTNode *init = nullptr;
        if (isType(peek().value))
            init = parseDeclaration(); // consumes trailing ';'
        else if (checkValue(";"))
            advance();
        else
        {
            init = parseAssignmentOrIncDec(false);
            expectValue(";", "after for-loop initializer");
        }

        ASTNode *cond = checkValue(";") ? nullptr : parseExpression();
        expectValue(";", "after for-loop condition");

        ASTNode *update = checkValue(")") ? nullptr : parseAssignmentOrIncDec(false);
        expectValue(")", "to close for-loop header");

        ASTNode *body = parseBlock();

        ASTNode *node = new ASTNode(ND_FOR, "", forTok.line);
        node->children.push_back(init   ? init   : new ASTNode(ND_BLOCK, "", forTok.line));
        node->children.push_back(cond   ? cond   : new ASTNode(ND_LITERAL, "true", forTok.line));
        node->children.push_back(update ? update : new ASTNode(ND_BLOCK, "", forTok.line));
        node->children.push_back(body);
        return node;
    }

    // 'return' Expression? ';'
    ASTNode *parseReturn()
    {
        Token retTok = advance();
        ASTNode *node = new ASTNode(ND_RETURN, "", retTok.line);
        if (!checkValue(";"))
            node->children.push_back(parseExpression());
        expectValue(";", "after return statement");
        return node;
    }

    // '{' Statement* '}'
    ASTNode *parseBlock()
    {
        Token openTok = expectValue("{", "to open a block");
        ASTNode *node = new ASTNode(ND_BLOCK, "", openTok.line);

        while (!checkValue("}") && !atEnd())
            node->children.push_back(parseStatement());

        expectValue("}", "to close block opened at line " + to_string(openTok.line));
        return node;
    }

    ASTNode *parseStatement()
    {
        const Token &t = peek();

        if (isType(t.value))                return parseDeclaration();
        if (t.value == "if")                return parseIf();
        if (t.value == "while")             return parseWhile();
        if (t.value == "for")               return parseFor();
        if (t.value == "return")            return parseReturn();
        if (t.value == "{")                 return parseBlock();

        if (t.type == "IDENTIFIER")
        {
            ASTNode *stmt = parseAssignmentOrIncDec(true);
            return stmt;
        }

        // Bare expression statement, e.g. a lone comparison — rare in this
        // language subset but kept so the parser fails gracefully rather
        // than looping forever on unexpected input.
        if (t.type != "EOF_TOKEN")
        {
            ASTNode *exprNode = new ASTNode(ND_EXPR_STMT, "", t.line);
            exprNode->children.push_back(parseExpression());
            expectValue(";", "after expression statement");
            return exprNode;
        }

        throw ParseError{"Unexpected end of file while parsing a statement",
                          t.line};
    }
} // namespace

ASTNode *parse()
{
    pos = 0;
    ASTNode *program = new ASTNode(ND_PROGRAM, "", 1);

    while (!atEnd())
        program->children.push_back(parseStatement());

    return program;
}
