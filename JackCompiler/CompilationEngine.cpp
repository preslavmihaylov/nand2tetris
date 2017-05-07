#include <cassert>
#include <sstream>
#include <unordered_set>

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
#define UNDECLARED_ID_ERR ("Undeclared identifier at line ")
#define INVALID_CLASSNAME_ERR ("Class name should match filename at line ")
#define RETURN_EXPECTED_ERR ("Expected return statement at line ")

unordered_set<char> validOperations =
{
    '+',
    '-',
    '*',
    '/',
    '&',
    '|',
    '<',
    '>',
    '='
};

eVMSegment GetSegmentOf(eVariableKind varKind)
{
    eVMSegment varSegment;
    switch (varKind)
    {
    case eVariableKindArgument:
        varSegment = eVMSegmentArg;
        break;
    case eVariableKindField:
        varSegment = eVMSegmentThis;
        break;
    case eVariableKindLocal:
        varSegment = eVMSegmentLocal;
        break;
    case eVariableKindStatic:
        varSegment = eVMSegmentStatic;
        break;
    default:
        assert(false);
        varSegment = eVMSegmentNone;
    }

    return varSegment;
}

/* PRIVATE METHODS */
string CompilationEngine::GetFullSubroutineName(string subroutineName)
{
    return (this->outputFilename + "." + subroutineName);
}

string CompilationEngine::GenerateLabel()
{
    ostringstream oss;
    oss << "LABEL" << this->autoIncrementCounter++;
    return oss.str();
}

void CompilationEngine::WriteOperation(char operationSymbol, bool isUnary)
{
    switch (operationSymbol)
    {
        case '+':
            this->vmWriter.WriteArithmetic(eVMOperationAdd);
            break;
        case '-':
            if (!isUnary)
            {
                this->vmWriter.WriteArithmetic(eVMOperationSub);
            }
            else
            {
                this->vmWriter.WriteArithmetic(eVMOperationNot);
            }
            break;
        case '*':
            this->vmWriter.WriteCall("Math.multiply", 2);
            break;
        case '/':
            this->vmWriter.WriteCall("Math.divide", 2);
            break;
        case '&':
            this->vmWriter.WriteArithmetic(eVMOperationAnd);
            break;
        case '|':
            this->vmWriter.WriteArithmetic(eVMOperationOr);
            break;
        case '<':
            this->vmWriter.WriteArithmetic(eVMOperationLt);
            break;
        case '>':
            this->vmWriter.WriteArithmetic(eVMOperationGt);
            break;
        case '=':
            this->vmWriter.WriteArithmetic(eVMOperationEq);
            break;
        default:
            // invalid operation
            assert(false);
    }
}

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
    eTokenType tokenType = this->tokenizer.GetTokenType();
    if (tokenType == eTokenTypeSymbol)
    {
        char symbol = this->tokenizer.GetSymbol();
        if (validOperations.find(symbol) != validOperations.end())
        {
            result = true;
        }
        else
        {
            result = false;
        }
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

string CompilationEngine::ExpectIdentifier(eIdentifierType expectedIdType, string idType, eVariableKind idKind)
{
    string result;

    if (expectedIdType == eIdentifierTypeDeclaration &&
        (idType.empty() || idKind == eVariableKindNone))
    {
        // type and kind should be specified upon declaration
        assert(false);
        return string();
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
                return string();
            }

            result = id;
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

    return result;
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

    this->ExpectKeyword(eKeywordClass);
    string identifier = this->ExpectIdentifier(eIdentifierTypeNone);

    // class name must match output filename
    if (identifier != this->outputFilename)
    {
        this->tokenizer.ThrowException(INVALID_CLASSNAME_ERR);
    }

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

    string subroutineName;
    eKeyword funcType;

    this->symbolTable.StartSubroutine();
    this->tokenizer.Advance();

    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword)
    {
        // expect 'constructor' or 'function' or 'method'
        if ((this->tokenizer.GetKeywordType() == eKeywordConstructor) ||
            (this->tokenizer.GetKeywordType() == eKeywordFunction) ||
            (this->tokenizer.GetKeywordType() == eKeywordMethod))
        {
            funcType = this->tokenizer.GetKeywordType();
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
            eKeyword expectedKeyword = this->tokenizer.GetKeywordType();
            this->tokenizer.PutbackToken();
            this->ExpectKeyword(expectedKeyword);
        }
        else if (this->tokenizer.GetTokenType() == eTokenTypeIdentifier)
        {
            this->tokenizer.PutbackToken();
            this->ExpectIdentifier(eIdentifierTypeNone);
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_KEYWORD_ERR);
        }

        if (funcType == eKeywordMethod)
        {
            // define this as first argument
            this->symbolTable.Define("this", "int", eVariableKindArgument);
        }

        subroutineName = this->ExpectIdentifier(eIdentifierTypeNone);
        this->ExpectSymbol('(');
        this->CompileParameterList();
        this->ExpectSymbol(')');

        // subroutine body: '{' varDec* statement* '}'
        this->ExpectSymbol('{');

        int totalDeclarationsCnt = 0;
        int currentDeclarationsCnt;

        // varDec*
        do
        {
            currentDeclarationsCnt = this->CompileVarDec();
            totalDeclarationsCnt += currentDeclarationsCnt;

        } while (currentDeclarationsCnt != 0);


        this->vmWriter.WriteFunction(this->GetFullSubroutineName(subroutineName), totalDeclarationsCnt);

        if (funcType == eKeywordConstructor)
        {
            int localVarsCnt = this->symbolTable.VarCount(eVariableKindField);
            // call Memory.Alloc(localVarsCnt);
            this->vmWriter.WritePush(eVMSegmentConst, localVarsCnt);
            this->vmWriter.WriteCall("Memory.alloc", 1);
            // set this = (returned value from memory alloc)
            this->vmWriter.WritePop(eVMSegmentPointer, 0);
        }
        else if (funcType == eKeywordMethod)
        {
            // set this = arg 0
            this->vmWriter.WritePush(eVMSegmentArg, 0);
            this->vmWriter.WritePop(eVMSegmentPointer, 0);
        }
        else // if (funcType == eKeywordFunction)
        {
            // do nothing
        }

        // statements
        this->CompileStatements();
        this->ExpectSymbol('}');
    }
    else
    {
        // first token is not subroutineDec token
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

int CompilationEngine::CompileParameterList()
{
    /* parameterList: ( (type varName) (',' type varName)*)? */

    bool hasMoreParameters = true;
    bool typeExpected = false;
    int parametersCnt = 0;

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
            return parametersCnt;
        }

        this->ExpectIdentifier(eIdentifierTypeDeclaration, declarationType, declarationKind);
        parametersCnt++;

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

    return parametersCnt;
}

// returns the number of declarations
int CompilationEngine::CompileVarDec()
{
    /* varDec: 'var' type varName (',' varName)* ';' */
    int declarationsCnt = 0;

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordVar)
    {
        string declarationType;
        eVariableKind declarationKind = eVariableKindLocal;

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
        declarationsCnt++;

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
                declarationsCnt++;
            }
            else
            {
                hasMoreParameters = false;
                this->tokenizer.PutbackToken();
            }
        }

        this->ExpectSymbol(';');
    }
    else
    {
        this->tokenizer.PutbackToken();
    }

    return declarationsCnt;
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
        string identifier;
        int argsCnt;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordDo);

        identifier = this->ExpectIdentifier(eIdentifierTypeNone);

        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol)
        {
            // subroutineName '(' expressionList ')'
            if (this->tokenizer.GetSymbol() == '(')
            {
                string subroutineName = this->GetFullSubroutineName(identifier);
                int thisArgumentCnt = 0;

                this->tokenizer.PutbackToken();

                // first argument is *this
                // pointer 0 is this in current class
                this->vmWriter.WritePush(eVMSegmentPointer, 0);
                thisArgumentCnt++;

                this->ExpectSymbol('(');
                argsCnt = this->CompileExpressionList();
                this->ExpectSymbol(')');

                this->vmWriter.WriteCall(subroutineName, argsCnt + thisArgumentCnt);
            }
            // ( className | varName) '.' subroutineName '(' expressionList ')'
            else if (this->tokenizer.GetSymbol() == '.')
            {
                string objectName;
                string subroutineName;
                int thisArgumentCnt = 0;
                eVariableKind varKind = this->symbolTable.GetKindOf(identifier);

                // id not found --> static method called
                if (varKind == eVariableKindNone)
                {
                    // identifier is a name of a class
                    objectName = identifier;
                }
                // id found --> class method called
                else
                {
                    // identifier is a variable --> the object is the variable's type
                    objectName = this->symbolTable.GetTypeOf(identifier);

                    // first argument is *this
                    this->vmWriter.WritePush(GetSegmentOf(varKind), this->symbolTable.GetIndexOf(identifier));
                    thisArgumentCnt++;
                }

                this->tokenizer.PutbackToken();
                this->ExpectSymbol('.');
                subroutineName = this->ExpectIdentifier(eIdentifierTypeNone);
                this->ExpectSymbol('(');
                argsCnt = this->CompileExpressionList();
                this->ExpectSymbol(')');

                this->vmWriter.WriteCall((objectName + "." + subroutineName), (argsCnt + thisArgumentCnt));
            }
            else
            {
                this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
            }

            // throw unused returned value
            this->vmWriter.WritePop(eVMSegmentTemp, 0);
        }
        else
        {
            this->tokenizer.ThrowException(INVALID_SYMBOL_ERR);
        }

        this->ExpectSymbol(';');
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
    bool isArray = false;

    this->tokenizer.Advance();
    if (this->tokenizer.GetTokenType() == eTokenTypeKeyword &&
        this->tokenizer.GetKeywordType() == eKeywordLet)
    {
        string identifier;

        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordLet);
        identifier = this->ExpectIdentifier(eIdentifierTypeUsage);

        eVariableKind varKind = this->symbolTable.GetKindOf(identifier);
        int index = this->symbolTable.GetIndexOf(identifier);

        // ('[' expression ']')?
        this->tokenizer.Advance();
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            this->tokenizer.GetSymbol() == '[')
        {
            isArray = true;

            this->tokenizer.PutbackToken();

            this->vmWriter.WritePush(GetSegmentOf(varKind), index);

            this->ExpectSymbol('[');
            // Expression is mandatory
            if (this->CompileExpression() == false)
            {
                this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
            }

            this->ExpectSymbol(']');

            this->vmWriter.WriteArithmetic(eVMOperationAdd);
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

        if (isArray)
        {
            this->vmWriter.WritePop(eVMSegmentTemp, 0);
            this->vmWriter.WritePop(eVMSegmentPointer, 1);
            this->vmWriter.WritePush(eVMSegmentTemp, 0);
            this->vmWriter.WritePop(eVMSegmentThat, 0);
        }
        else
        {
            this->vmWriter.WritePop(GetSegmentOf(varKind), index);
        }
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
        string beginLabel = this->GenerateLabel();
        string endLabel = this->GenerateLabel();

        this->tokenizer.PutbackToken();

        this->ExpectKeyword(eKeywordWhile);

        // BEGIN_LABEL
        this->vmWriter.WriteLabel(beginLabel);

        this->ExpectSymbol('(');
        // Expression is mandatory
        if (this->CompileExpression() == false)
        {
            this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
        }
        this->ExpectSymbol(')');

        // not
        this->vmWriter.WriteArithmetic(eVMOperationNot);
        // if-goto END_LABEL
        this->vmWriter.WriteIfGoto(endLabel);

        this->ExpectSymbol('{');
        // statements
        this->CompileStatements();
        this->ExpectSymbol('}');

        // goto BEGIN_LABEL
        this->vmWriter.WriteGoto(beginLabel);
        // END_LABEL
        this->vmWriter.WriteLabel(endLabel);
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
        this->tokenizer.PutbackToken();
        this->ExpectKeyword(eKeywordReturn);

        // Expression is optional
        if (this->CompileExpression() == false)
        {
            // no returned expression --> void function
            this->vmWriter.WritePush(eVMSegmentConst, 0);
        }

        this->ExpectSymbol(';');

        this->vmWriter.WriteReturn();
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
        string elseLabel = this->GenerateLabel();
        string endLabel = this->GenerateLabel();

        this->tokenizer.PutbackToken();

        this->ExpectKeyword(eKeywordIf);
        this->ExpectSymbol('(');
        // Expression is mandatory
        if (this->CompileExpression() == false)
        {
            this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
        }
        this->ExpectSymbol(')');

        // not
        this->vmWriter.WriteArithmetic(eVMOperationNot);
        // if-goto ELSE_LABEL
        this->vmWriter.WriteIfGoto(elseLabel);

        this->ExpectSymbol('{');
        this->CompileStatements();
        this->ExpectSymbol('}');

        // goto END_LABEL
        this->vmWriter.WriteGoto(endLabel);

        this->tokenizer.Advance();

        // ELSE_LABEL
        this->vmWriter.WriteLabel(elseLabel);
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

        this->vmWriter.WriteLabel(endLabel);
    }
    else
    {
        this->tokenizer.PutbackToken();
        return false;
    }

    return true;
}

int CompilationEngine::CompileExpressionList()
{
    // expressionList: (expression (',' expression)* )?
    int argsCnt = 0;
    bool hasMoreExpressions = false;

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

        argsCnt++;
    }

    return argsCnt;
}

bool CompilationEngine::CompileExpression()
{
    /* expression: term (op term)* */
    /* term:
        integerConstant | stringConstant | keywordConstant | varName |
        varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
    */

    char currentOperation;

    if (this->IsNextTokenTerm())
    {
        this->CompileTerm();
        while (this->IsNextTokenOperation())
        {
            this->tokenizer.Advance();
            if (this->tokenizer.GetTokenType() == eTokenTypeSymbol)
            {
                currentOperation = this->tokenizer.GetSymbol();
            }
            else
            {
                // next token should be symbol after IsNextTokenOperation
                assert(false);
                return false;
            }

            this->CompileTerm();
            this->WriteOperation(currentOperation, false);
        }

        this->outputStream << "</expression>" << endl;
    }
    else
    {
        return false;
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

    this->tokenizer.Advance();
    eTokenType tokenType = this->tokenizer.GetTokenType();

    // integerConstant
    if (tokenType == eTokenTypeIntConst)
    {
        this->vmWriter.WritePush(eVMSegmentConst, this->tokenizer.GetIntVal());
    }
    // string constant
    else if (tokenType == eTokenTypeStringConst)
    {
        string stringConst = this->tokenizer.GetStringVal();
        // push constant SIZE
        this->vmWriter.WritePush(eVMSegmentConst, stringConst.size());
        // call String.new 1
        this->vmWriter.WriteCall("String.new", 1);
        for (char ch : stringConst)
        {
            // push constant CHAR
            this->vmWriter.WritePush(eVMSegmentConst, ch);
            // call String.appendChar 2
            // 2 args because this is included
            this->vmWriter.WriteCall("String.appendChar", 2);
        }
    }
    // keyword constant
    else if (tokenType == eTokenTypeKeyword &&
             (this->tokenizer.GetKeywordType() == eKeywordTrue ||
              this->tokenizer.GetKeywordType() == eKeywordFalse ||
              this->tokenizer.GetKeywordType() == eKeywordNull ||
              this->tokenizer.GetKeywordType() == eKeywordThis))
    {
        eKeyword keywordType = this->tokenizer.GetKeywordType();
        if (keywordType == eKeywordFalse ||
            keywordType == eKeywordNull)
        {
            // false and null are equivalent to 0 constant
            this->vmWriter.WritePush(eVMSegmentConst, 0);
        }
        else if (keywordType == eKeywordTrue)
        {
            this->vmWriter.WritePush(eVMSegmentConst, 0);
            this->vmWriter.WriteArithmetic(eVMOperationNot);
        }
        else if (keywordType == eKeywordThis)
        {
            // push pointer 0
            this->vmWriter.WritePush(eVMSegmentPointer, 0);
        }
        else
        {
            assert(false);
        }
    }
    // varName '[' expression ']' | varName | subroutineCall
    else if (tokenType == eTokenTypeIdentifier)
    {
        string identifier = this->tokenizer.GetIdentifier();
        eVariableKind varKind = this->symbolTable.GetKindOf(identifier);
        int varIndex = this->symbolTable.GetIndexOf(identifier);
        /*
        this->tokenizer.PutbackToken();
        this->ExpectIdentifier(eIdentifierTypeNone);
        */
        this->tokenizer.Advance();

        // varName '[' expression ']'
        if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
            (this->tokenizer.GetSymbol() == '['))
        {
            this->tokenizer.PutbackToken();

            if (varKind == eVariableKindNone)
            {
                this->tokenizer.ThrowException(EXPECTED_ID_ERR);
            }

            this->vmWriter.WritePush(GetSegmentOf(varKind), varIndex);

            this->ExpectSymbol('[');
            // Expression is mandatory
            if (this->CompileExpression() == false)
            {
                this->tokenizer.ThrowException(EXPECTED_EXPRESSION_ERR);
            }
            this->ExpectSymbol(']');
            this->vmWriter.WriteArithmetic(eVMOperationAdd);
            this->vmWriter.WritePop(eVMSegmentPointer, 1);
            this->vmWriter.WritePush(eVMSegmentThat, 0);
        }
        // ( className | varName) '.' subroutineName '(' expressionList ')'
        else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
                 (this->tokenizer.GetSymbol() == '.'))
        {
            string objectName;
            string subroutineName;
            int argsCnt;
            int thisArgumentCnt = 0;
            this->tokenizer.PutbackToken();

            // id not found --> static method called
            if (varKind == eVariableKindNone)
            {
                // identifier is a name of a class
                objectName = identifier;
            }
            // id found --> class method called
            else
            {
                // identifier is a variable --> the object is the variable's type
                objectName = this->symbolTable.GetTypeOf(identifier);

                // first argument is *this
                this->vmWriter.WritePush(GetSegmentOf(varKind), varIndex);
                thisArgumentCnt++;
            }

            this->ExpectSymbol('.');
            subroutineName = this->ExpectIdentifier(eIdentifierTypeNone);
            this->ExpectSymbol('(');
            argsCnt = this->CompileExpressionList();
            this->ExpectSymbol(')');

            this->vmWriter.WriteCall((objectName + "." + subroutineName), (argsCnt + thisArgumentCnt));
        }
        // subroutineName '(' expressionList ')'
        else if (this->tokenizer.GetTokenType() == eTokenTypeSymbol &&
                 (this->tokenizer.GetSymbol() == '('))
        {
            int argsCnt;
            int thisArgumentCnt = 0;
            string className = this->GetFullSubroutineName(identifier);
            this->tokenizer.PutbackToken();

            // first argument is *this
            // pointer 0 is this in current class
            this->vmWriter.WritePush(eVMSegmentPointer, 0);
            thisArgumentCnt++;

            this->ExpectSymbol('(');
            argsCnt = this->CompileExpressionList();
            this->ExpectSymbol(')');

            this->vmWriter.WriteCall(className, argsCnt);
        }
        // varName
        else
        {
            this->tokenizer.PutbackToken();

            // variable does not exist
            if (varKind == eVariableKindNone)
            {
                this->tokenizer.ThrowException(UNDECLARED_ID_ERR);
            }

            // push SEGMENT INDEX
            this->vmWriter.WritePush(GetSegmentOf(varKind), varIndex);
        }
    }
    // '(' expression ')'
    else if (tokenType == eTokenTypeSymbol &&
             this->tokenizer.GetSymbol() == '(')
    {
        this->tokenizer.PutbackToken();

        this->ExpectSymbol('(');
        this->CompileExpression();
        this->ExpectSymbol(')');
    }
    // unaryOp term
    else if (tokenType == eTokenTypeSymbol &&
             (this->tokenizer.GetSymbol() == '-' ||
              this->tokenizer.GetSymbol() == '~'))
    {
        char symbol = this->tokenizer.GetSymbol();
        this->CompileTerm();
        if (symbol == '-')
        {
            this->vmWriter.WriteArithmetic(eVMOperationNeg);
        }
        else if (symbol == '~')
        {
            this->vmWriter.WriteArithmetic(eVMOperationNot);
        }
        // invalid unary op
        else
        {
            assert(false);
        }
    }
    else
    {
        this->tokenizer.ThrowException(EXPECTED_TERM_ERR);
    }
}

void CompilationEngine::CompileFile()
{
    this->CompileClass();
}
