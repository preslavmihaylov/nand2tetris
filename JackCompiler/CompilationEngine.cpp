#include <cassert>
#include <sstream>

#include "CompilationEngine.h"

using namespace JackCompiler;
using namespace std;

#define INVALID_KEYWORD_ERR ("Invalid keyword at line ")
#define INVALID_SYMBOL_ERR ("Invalid symbol at line ")
#define EXPECTED_ID_ERR ("Expected identifier at line ")
#define EXPECTED_EXPRESSION_ERR ("Expected expression at line ")
#define EXPECTED_INT_ERR ("Expected integer constant at line ")
#define EXPECTED_STRING_ERR ("Expected string constant at line ")
#define EXPECTED_TERM_ERR ("Expected term at line ")
#define DECLARED_ID_ERR ("Identifier is already declared at line ")

#include <cstdlib>

/* PRIVATE METHODS */
string CompilationEngine::GetIdentifierXMLFormat(const string& id)
{
    ostringstream oss;
    oss << this->symbolTable.GetIndexOf(id);
    return (id + '-' +
            this->symbolTable.GetTypeOf(id) + '-' +
            this->symbolTable.GetKindString(this->symbolTable.GetKindOf(id)) + '-' +
            oss.str());
}

bool CompilationEngine::IsNextTokenOperation()
{
    /* op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '=' */

    bool result = false;
    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
        (this->tokenizer.GetSymbol() == '+' ||
         this->tokenizer.GetSymbol() == '-' ||
         this->tokenizer.GetSymbol() == '*' ||
         this->tokenizer.GetSymbol() == '/' ||
         this->tokenizer.GetSymbol() == '&' ||
         this->tokenizer.GetSymbol() == '|' ||
         this->tokenizer.GetSymbol() == '<' ||
         this->tokenizer.GetSymbol() == '>' ||
         this->tokenizer.GetSymbol() == '='))
    {
        result = true;
    }

    this->tokenizer.PutbackToken();
    return result;
}

bool CompilationEngine::IsNextTokenTerm()
{
    bool result = false;
    this->tokenizer.Advance();

    // integerConstant
    if (this->tokenizer.GetTokenType() == eTokenTypeIntConst)
    {
        result = true;
    }
    // string constant
    else if (this->tokenizer.GetTokenType() == eTokenTypeStringConst)
    {
        result = true;
    }
    // keyword constant
    else if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
             (this->tokenizer.GetKeywordType() == eKeywordTrue ||
              this->tokenizer.GetKeywordType() == eKeywordFalse ||
              this->tokenizer.GetKeywordType() == eKeywordNull ||
              this->tokenizer.GetKeywordType() == eKeywordThis))
    {
        result = true;
    }
    // varName '[' expression ']' | varName | subroutineCall
    else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
    {
        result = true;
    }
    // '(' expression ')'
    else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
             this->tokenizer.GetSymbol() == '(')
    {
        result = true;
    }
    // unaryOp term
    else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
             (this->tokenizer.GetSymbol() == '-' ||
              this->tokenizer.GetSymbol() == '~'))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    this->tokenizer.PutbackToken();
    return result;
}

void CompilationEngine::ExpectKeyword(eKeyword expectedKeyword)
{
    if (this->tokenizer.HasMoreTokens())
    {
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
            this->tokenizer.GetKeywordType() == expectedKeyword)
        {
            this->outputStream << "<keyword>" << this->tokenizer.GetKeyword() << "</keyword>" << endl;
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_KEYWORD_ERR);
        }
    }
    else
    {
        this->tokenizer.ThrowException(INVALID_KEYWORD_ERR);
    }
}

void CompilationEngine::ExpectSymbol(char expectedSymbol)
{
    if (this->tokenizer.HasMoreTokens())
    {
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            this->tokenizer.GetSymbol() == expectedSymbol)
        {
            string escapedSymbol(1, this->tokenizer.GetSymbol());

            // escape symbols in XML
            if (escapedSymbol == ">")
            {
                escapedSymbol = "&gt;";
            }
            else if (escapedSymbol == "<")
            {
                escapedSymbol = "&lt;";
            }
            else if (escapedSymbol == "\"")
            {
                escapedSymbol = "&quot;";
            }
            else if (escapedSymbol == "&")
            {
                escapedSymbol = "&amp;";
            }

            this->outputStream << "<symbol>" << escapedSymbol << "</symbol>" << endl;
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
        }
    }
    else
    {
        this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
    }
}

void CompilationEngine::ExpectIdentifier(eIdentifierType expectedIdType, string idType, eVariableKind idKind)
{
    if (expectedIdType == eIdentifierTypeDeclaration &&
        (idType.empty() || idKind == eVariableKindNone))
    {
        // type and kind should be specified upon declaration
        assert(false);
        return;
    }

    if (this->tokenizer.HasMoreTokens())
    {
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
        {
            string id = this->tokenizer.GetIdentifier();
            if (expectedIdType == eIdentifierTypeDeclaration)
            {
                // identifier is not present in the symbol table
                if (this->symbolTable.GetKindOf(id) == eVariableKindNone)
                {
                    this->symbolTable.Define(id, idType, idKind);
                }
                else
                {
                    this->tokenizer.ThrowException(DECLARED_ID_ERR);
                }

                this->outputStream << "<identifier>"
                                   << this->tokenizer.GetIdentifier()
                                   << "</identifier>" << endl;
            }
            else if (expectedIdType == eIdentifierTypeUsage)
            {
                // identifier is not present in the symbol table
                if (this->symbolTable.GetKindOf(id) == eVariableKindNone)
                {
                    this->tokenizer.ThrowException(DECLARED_ID_ERR);
                }

                this->outputStream << "<" << this->GetIdentifierXMLFormat(id) << ">"
                                   << this->tokenizer.GetIdentifier()
                                   << "</" << this->GetIdentifierXMLFormat(id) << ">" << endl;
            }
            else if (expectedIdType == eIdentifierTypeNone)
            {
                // identifier is present in the symbol table
                if (this->symbolTable.GetKindOf(id) != eVariableKindNone)
                {
                    this->outputStream << "<" << this->GetIdentifierXMLFormat(id) << ">"
                                       << this->tokenizer.GetIdentifier()
                                       << "</" << this->GetIdentifierXMLFormat(id) << ">" << endl;
                }
                else
                {
                    this->outputStream << "<identifier>"
                                       << this->tokenizer.GetIdentifier()
                                       << "</identifier>" << endl;
                }
            }
            else
            {
                // this function should be invoked with one of the three possibilities
                assert(false);
            }
        }
        else
        {
            this->tokenizer.ThrowException(EXPECTED_ID_ERR);
        }
    }
    else
    {
        this->tokenizer.ThrowException(EXPECTED_ID_ERR);
    }
}

void CompilationEngine::ExpectIntConst()
{
    if (this->tokenizer.HasMoreTokens())
    {
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeIntConst)
        {
            this->vmWriter.WritePush(eVMSegmentConst, this->tokenizer.GetIntVal());
        }
        else
        {
            this->tokenizer.ThrowException(EXPECTED_INT_ERR);
        }
    }
    else
    {
        this->tokenizer.ThrowException(EXPECTED_INT_ERR);
    }
}

void CompilationEngine::ExpectStringConst()
{
    if (this->tokenizer.HasMoreTokens())
    {
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeStringConst)
        {
            this->outputStream << "<stringConstant>" << this->tokenizer.GetStringVal() << "</stringConstant>" << endl;
        }
        else
        {
            this->tokenizer.ThrowException(EXPECTED_STRING_ERR);
        }
    }
    else
    {
        this->tokenizer.ThrowException(EXPECTED_STRING_ERR);
    }
}

/* PUBLIC METHODS */
void CompilationEngine::CompileClass()
{
    /*
        class: 'class' className '{' classVarDec* subroutineDec* '}'
    */

    bool classVarDecCompiled = false;
    bool subroutineDecCompiled = false;

    // clear symbol table
    this->symbolTable.Clear();

    this->outputStream << "<class>" << endl;
    this->ExpectKeyword(eKeywordClass);
    this->ExpectIdentifier(eIdentifierTypeNone);
    this->ExpectSymbol('{');

    do
    {
        classVarDecCompiled = this->CompileClassVarDec();
    } while (classVarDecCompiled == true);

    do
    {
        subroutineDecCompiled = this->CompileSubroutine();
    } while (subroutineDecCompiled == true);

    this->ExpectSymbol('}');
    this->outputStream << "</class>" << endl;

}

bool CompilationEngine::CompileClassVarDec()
{
    /* ('static' | 'field' ) type varName (',' varName)* ';' */
    /* type: 'int' | 'char' | 'boolean' | className */
    this->tokenizer.Advance();

    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword)
    {
        // used for symbol table declaration
        eVariableKind declarationKind;
        string declarationType;

        // expect 'static' or 'field'
        if ((this->tokenizer.GetKeywordType() == eKeywordStatic) ||
            (this->tokenizer.GetKeywordType() == eKeywordField))
        {
            eKeyword keyword = this->tokenizer.GetKeywordType();
            this->outputStream << "<classVarDec>" << endl;
            this->tokenizer.PutbackToken();
            this->ExpectKeyword(keyword);

            // extract variable kind for symbol table declaration
            if (keyword == eKeywordStatic)
            {
                declarationKind = eVariableKindStatic;
            }
            else if (keyword == eKeywordField)
            {
                declarationKind = eVariableKindField;
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            // first token is not classVarDec token
            this->tokenizer.PutbackToken();
            return false;
        }

        this->tokenizer.Advance();

        // expect 'int', 'char', 'boolean' or className
        if ((this->tokenizer.GetTokenType() == eTokenTypeKeyword) &&
            (this->tokenizer.GetKeywordType() == eKeywordInt ||
             this->tokenizer.GetKeywordType() == eKeywordChar ||
             this->tokenizer.GetKeywordType() == eKeywordBoolean))
        {
            declarationType = this->tokenizer.GetKeyword();
            this->outputStream << "<keyword>"
                               << declarationType
                               << "</keyword>" << endl;
        }
        else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
        {
            declarationType = this->tokenizer.GetIdentifier();
            this->tokenizer.PutbackToken();
            this->ExpectIdentifier(eIdentifierTypeNone);
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_KEYWORD_ERR);
        }

        // expect type varName
        this->ExpectIdentifier(eIdentifierTypeDeclaration, declarationType, declarationKind);

        // expect (',', varName)*
        bool hasMoreParameters = true;
        while (hasMoreParameters)
        {
            this->tokenizer.Advance();
            if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
                this->tokenizer.GetSymbol() == ',')
            {
                this->tokenizer.PutbackToken();
                this->ExpectSymbol(',');
                this->ExpectIdentifier(eIdentifierTypeDeclaration, declarationType, declarationKind);
            }
            else
            {
                hasMoreParameters = false;
                this->tokenizer.PutbackToken();
            }
        }

        this->ExpectSymbol(';');
        this->outputStream << "</classVarDec>" << endl;
    }
    else
    {
        // first token is not classVarDec token
        this->tokenizer.PutbackToken();
        return false;

    }

    return true;
}

bool CompilationEngine::CompileSubroutine()
{
    /* ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody  */

    this->tokenizer.Advance();

    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword)
    {
        // expect 'constructor' or 'function' or 'method'
        if ((this->tokenizer.GetKeywordType() == eKeywordConstructor) ||
            (this->tokenizer.GetKeywordType() == eKeywordFunction) ||
            (this->tokenizer.GetKeywordType() == eKeywordMethod))
        {
            this->outputStream << "<subroutineDec>" << endl;
            this->outputStream << "<keyword>";
            this->outputStream << this->tokenizer.GetKeyword();
            this->outputStream << "</keyword>" << endl;
        }
        else
        {
            // first token is not subroutineDec token
            this->tokenizer.PutbackToken();
            return false;
        }

        this->tokenizer.Advance();

        // expect 'void', 'int', 'char', 'boolean' or className
        if ((this->tokenizer.GetTokenType() == eTokenTypeKeyword) &&
            (this->tokenizer.GetKeywordType() == eKeywordInt ||
             this->tokenizer.GetKeywordType() == eKeywordChar ||
             this->tokenizer.GetKeywordType() == eKeywordBoolean ||
             this->tokenizer.GetKeywordType() == eKeywordVoid))
        {
            this->outputStream << "<keyword>"
                               << this->tokenizer.GetKeyword()
                               << "</keyword>" << endl;
        }
        else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
        {
            this->outputStream << "<identifier>"
                               << this->tokenizer.GetIdentifier()
                               << "</identifier>" << endl;
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_KEYWORD_ERR);
        }

        this->ExpectIdentifier(eIdentifierTypeNone);
        this->ExpectSymbol('(');
        this->CompileParameterList();
        this->ExpectSymbol(')');

        // subroutine body: '{' varDec* statement* '}'
        this->outputStream << "<subroutineBody>" << endl;
        this->ExpectSymbol('{');
        bool varDecCompiled = true;

        // varDec*
        do
        {
            varDecCompiled = this->CompileVarDec();
        } while (varDecCompiled == true);

        // statements
        this->CompileStatements();

        this->ExpectSymbol('}');
        this->outputStream << "</subroutineBody>" << endl;

        this->outputStream << "</subroutineDec>" << endl;
    }
    else
    {
        // first token is not subroutineDec token
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

void CompilationEngine::CompileParameterList()
{
    /* parameterList: ( (type varName) (',' type varName)*)? */

    bool hasMoreParameters = true;
    bool typeExpected = false;

    this->outputStream << "<parameterList>" << endl;
    while (hasMoreParameters)
    {
        string declarationType;
        eVariableKind declarationKind = eVariableKindArgument;

        this->tokenizer.Advance();

        // expect 'int', 'char', 'boolean' or className
        if ((this->tokenizer.GetTokenType() == eTokenTypeKeyword) &&
            (this->tokenizer.GetKeywordType() == eKeywordInt ||
             this->tokenizer.GetKeywordType() == eKeywordChar ||
             this->tokenizer.GetKeywordType() == eKeywordBoolean))
        {
            eKeyword keyword = this->tokenizer.GetKeywordType();
            declarationType = this->tokenizer.GetKeyword();

            this->tokenizer.PutbackToken();
            this->ExpectKeyword(keyword);
        }
        else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
        {
            declarationType = this->tokenizer.GetIdentifier();

            this->tokenizer.PutbackToken();
            this->ExpectIdentifier(eIdentifierTypeNone);
        }
        // type is expected after ','
        else if (typeExpected)
        {
            this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
        }
        else
        {
            // empty parameter list
            this->tokenizer.PutbackToken();
            this->outputStream << "</parameterList>" << endl;
            return;
        }

        this->ExpectIdentifier(eIdentifierTypeDeclaration, declarationType, declarationKind);

        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            this->tokenizer.GetSymbol() == ',')
        {
            this->tokenizer.PutbackToken();
            this->ExpectSymbol(',');
            typeExpected = true;
            hasMoreParameters = true;
        }
        else
        {
            hasMoreParameters = false;
            this->tokenizer.PutbackToken();
        }
    }

    this->outputStream << "</parameterList>" << endl;
}

bool CompilationEngine::CompileVarDec()
{
    /* varDec: 'var' type varName (',' varName)* ';' */

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordVar)
    {
        string declarationType;
        eVariableKind declarationKind = eVariableKindLocal;

        this->outputStream << "<varDec>" << endl;
        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordVar);
        this->tokenizer.Advance();

        // expect 'int', 'char', 'boolean' or className
        if ((this->tokenizer.GetTokenType() == eTokenTypeKeyword) &&
            (this->tokenizer.GetKeywordType() == eKeywordInt ||
             this->tokenizer.GetKeywordType() == eKeywordChar ||
             this->tokenizer.GetKeywordType() == eKeywordBoolean))
        {
            declarationType = this->tokenizer.GetKeyword();

            eKeyword keyword = this->tokenizer.GetKeywordType();
            this->tokenizer.PutbackToken();
            this->ExpectKeyword(keyword);
        }
        else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
        {
            declarationType = this->tokenizer.GetIdentifier();

            this->tokenizer.PutbackToken();
            this->ExpectIdentifier(eIdentifierTypeNone);
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_KEYWORD_ERR);
        }

        this->ExpectIdentifier(eIdentifierTypeDeclaration, declarationType, declarationKind);

        // expect (',', varName)*
        bool hasMoreParameters = true;
        while (hasMoreParameters)
        {
            this->tokenizer.Advance();
            if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
                this->tokenizer.GetSymbol() == ',')
            {
                this->tokenizer.PutbackToken();
                this->ExpectSymbol(',');
                this->ExpectIdentifier(eIdentifierTypeDeclaration, declarationType, declarationKind);
            }
            else
            {
                hasMoreParameters = false;
                this->tokenizer.PutbackToken();
            }
        }

        this->ExpectSymbol(';');
        this->outputStream << "</varDec>" << endl;
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

void CompilationEngine::CompileStatements()
{
    bool statementCompiled;

    this->outputStream << "<statements>" << endl;
    do {
        // When one of these executes, the others will not
        if (this->CompileDo() ||
            this->CompileLet() ||
            this->CompileWhile() ||
            this->CompileIf() ||
            this->CompileReturn())
        {
            statementCompiled = true;
        }
        else
        {
            statementCompiled = false;
        }
    } while (statementCompiled == true);

    this->outputStream << "</statements>" << endl;
}

bool CompilationEngine::CompileDo()
{
    /* doStatement: 'do' subroutineCall ';' */
    /* subroutineCall:
        subroutineName '(' expressionList ')' | ( className | varName) '.' subroutineName '(' expressionList ')'  */

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordDo)
    {
        this->outputStream << "<doStatement>" << endl;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordDo);
        this->ExpectIdentifier(eIdentifierTypeNone);

        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol)
        {
            if (this->tokenizer.GetSymbol() == '(')
            {
                this->tokenizer.PutbackToken();

                this->ExpectSymbol('(');
                this->CompileExpressionList();
                this->ExpectSymbol(')');
            }
            else if (this->tokenizer.GetSymbol() == '.')
            {
                this->tokenizer.PutbackToken();
                this->ExpectSymbol('.');
                this->ExpectIdentifier(eIdentifierTypeNone);
                this->ExpectSymbol('(');
                this->CompileExpressionList();
                this->ExpectSymbol(')');
            }
            else
            {
                this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
            }
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
        }

        this->ExpectSymbol(';');
        this->outputStream << "</doStatement>" << endl;
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

bool CompilationEngine::CompileLet()
{
    /* letStatement: 'let' varName ('[' expression ']')? '=' expression ';'  */

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordLet)
    {
        this->outputStream << "<letStatement>" << endl;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordLet);
        this->ExpectIdentifier(eIdentifierTypeUsage);

        // ('[' expression ']')?
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            this->tokenizer.GetSymbol() == '[')
        {
            this->tokenizer.PutbackToken();

            this->ExpectSymbol('[');
            // Expression is mandatory
            if (this->CompileExpression() == false)
            {
                this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
            }

            this->ExpectSymbol(']');
        }
        else
        {
            this->tokenizer.PutbackToken();
        }

        this->ExpectSymbol('=');

        // Expression is mandatory
        if (this->CompileExpression() == false)
        {
            this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
        }

        this->ExpectSymbol(';');

        this->outputStream << "</letStatement>" << endl;
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

bool CompilationEngine::CompileWhile()
{
    /* whileStatement: 'while' '(' expression ')' '{' statements '}' */

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordWhile)
    {
        this->outputStream << "<whileStatement>" << endl;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordWhile);
        this->ExpectSymbol('(');

        // Expression is mandatory
        if (this->CompileExpression() == false)
        {
            this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
        }

        this->ExpectSymbol(')');
        this->ExpectSymbol('{');

        // statements
        this->CompileStatements();
        this->ExpectSymbol('}');

        this->outputStream << "</whileStatement>" << endl;
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

bool CompilationEngine::CompileReturn()
{
    /* ReturnStatement 'return' expression? ';' */

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordReturn)
    {
        this->outputStream << "<returnStatement>" << endl;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordReturn);

        // Expression is optional
        this->CompileExpression();
        this->ExpectSymbol(';');

        this->outputStream << "</returnStatement>" << endl;
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

bool CompilationEngine::CompileIf()
{
    /* ifStatement: 'if' '(' expression ')' '{' statements '}' ( 'else' '{' statements '}' )? */

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordIf)
    {
        this->outputStream << "<ifStatement>" << endl;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordIf);
        this->ExpectSymbol('(');

        // Expression is mandatory
        if (this->CompileExpression() == false)
        {
            this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
        }

        this->ExpectSymbol(')');
        this->ExpectSymbol('{');

        // statements
        this->CompileStatements();

        this->ExpectSymbol('}');

        this->tokenizer.Advance();

        // (else '{' statements '}')?
        if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
            this->tokenizer.GetKeywordType() == eKeywordElse)
        {
            this->tokenizer.PutbackToken();

            this->ExpectKeyword(eKeywordElse);
            this->ExpectSymbol('{');
            this->CompileStatements();
            this->ExpectSymbol('}');
        }
        else
        {
            this->tokenizer.PutbackToken();
        }

        this->outputStream << "</ifStatement>" << endl;
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

void CompilationEngine::CompileExpressionList()
{
    // expressionList: (expression (',' expression)* )?

    bool hasMoreExpressions = false;

    this->outputStream << "<expressionList>" << endl;
    if (this->CompileExpression() == true)
    {
        hasMoreExpressions = true;
    }

    while (hasMoreExpressions)
    {
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            this->tokenizer.GetSymbol() == ',')
        {
            this->tokenizer.PutbackToken();

            this->ExpectSymbol(',');
            // Expression is mandatory
            if (this->CompileExpression() == false)
            {
                this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
            }

            hasMoreExpressions = true;
        }
        else
        {
            hasMoreExpressions = false;
            this->tokenizer.PutbackToken();
        }
    }

    this->outputStream << "</expressionList>" << endl;
}

bool CompilationEngine::CompileExpression()
{
    /* expression: term (op term)* */
    /* term:
        integerConstant | stringConstant | keywordConstant | varName |
        varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
    */

    if (this->IsNextTokenTerm())
    {
        this->outputStream << "<expression>" << endl;

        bool hasNextTerm = true;
        while (hasNextTerm)
        {
            this->CompileTerm();
            if (this->IsNextTokenOperation())
            {
                char expectedSymbol;
                this->tokenizer.Advance();

                if (this->tokenizer.GetTokenType() == eTokenTypeSymbol)
                {
                    expectedSymbol = this->tokenizer.GetSymbol();
                    this->tokenizer.PutbackToken();
                }
                else
                {
                    // next token should be symbol after IsNextTokenOperation
                    assert(false);
                    return false;
                }

                this->ExpectSymbol(expectedSymbol);
                hasNextTerm = true;
            }
            else
            {
                hasNextTerm = false;
            }
        }

        this->outputStream << "</expression>" << endl;
    }

    return true;
}

void CompilationEngine::CompileTerm()
{
    /* term:
        integerConstant | stringConstant | keywordConstant | varName |
        varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
    */

    /* subroutineCall:
        subroutineName '(' expressionList ')' |
        ( className | varName) '.' subroutineName '(' expressionList ')'
    */

    this->outputStream << "<term>" << endl;

    this->tokenizer.Advance();

    // integerConstant
    if (this->tokenizer.GetTokenType() == eTokenTypeIntConst)
    {
        this->tokenizer.PutbackToken();
        this->ExpectIntConst();
    }
    // string constant
    else if (this->tokenizer.GetTokenType() == eTokenTypeStringConst)
    {
        this->tokenizer.PutbackToken();
        this->ExpectStringConst();
    }
    // keyword constant
    else if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
             (this->tokenizer.GetKeywordType() == eKeywordTrue ||
              this->tokenizer.GetKeywordType() == eKeywordFalse ||
              this->tokenizer.GetKeywordType() == eKeywordNull ||
              this->tokenizer.GetKeywordType() == eKeywordThis))
    {
        eKeyword expectedKeyword = this->tokenizer.GetKeywordType();
        this->tokenizer.PutbackToken();
        this->ExpectKeyword(expectedKeyword);
    }
    // varName '[' expression ']' | varName | subroutineCall
    else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
    {
        this->tokenizer.PutbackToken();
        // TODO: Fix. Not working correctly
        this->ExpectIdentifier(eIdentifierTypeNone);

        this->tokenizer.Advance();

        // varName '[' expression ']'
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            (this->tokenizer.GetSymbol() == '['))
        {
            this->tokenizer.PutbackToken();

            this->ExpectSymbol('[');
            // Expression is mandatory
            if (this->CompileExpression() == false)
            {
                this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
            }

            this->ExpectSymbol(']');
        }
        // ( className | varName) '.' subroutineName '(' expressionList ')'
        else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
                 (this->tokenizer.GetSymbol() == '.'))
        {
            this->tokenizer.PutbackToken();

            this->ExpectSymbol('.');
            this->ExpectIdentifier(eIdentifierTypeNone);
            this->ExpectSymbol('(');
            this->CompileExpressionList();
            this->ExpectSymbol(')');
        }
        // subroutineName '(' expressionList ')'
        else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
                 (this->tokenizer.GetSymbol() == '('))
        {
            this->tokenizer.PutbackToken();

            this->ExpectSymbol('(');
            this->CompileExpressionList();
            this->ExpectSymbol(')');
        }
        // varName
        else
        {
            this->tokenizer.PutbackToken();
        }
    }
    // '(' expression ')'
    else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
             this->tokenizer.GetSymbol() == '(')
    {
        this->tokenizer.PutbackToken();

        this->ExpectSymbol('(');
        this->CompileExpression();
        this->ExpectSymbol(')');
    }
    // unaryOp term
    else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
             (this->tokenizer.GetSymbol() == '-' ||
              this->tokenizer.GetSymbol() == '~'))
    {
        char expectedSymbol = this->tokenizer.GetSymbol();
        this->tokenizer.PutbackToken();

        this->ExpectSymbol(expectedSymbol);
        this->CompileTerm();
    }
    else
    {
        this->tokenizer.ThrowException(EXPECTED_TERM_ERR);
    }

    this->outputStream << "</term>" << endl;
}

void CompilationEngine::CompileFile()
{
    this->CompileClass();
}
