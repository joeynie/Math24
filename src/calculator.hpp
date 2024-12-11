#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP
//
// This is example code from Chapter 7.7 "Recovering from errors" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "std_lib_facilities.h"
using namespace std;
inline int factorial(int n)
{
    int sum = 1;
    return n == 1? sum = 1: n * factorial(n - 1);
}
/*
    Simple calculator

    Revision history:

        Revised by Bjarne Stroustrup May 2007
        Revised by Bjarne Stroustrup August 2006
        Revised by Bjarne Stroustrup August 2004
        Originally written by Bjarne Stroustrup
            (bs@cs.tamu.edu) Spring 2004.

    This program implements a basic expression calculator.
    Input from cin; output to cout.

    The grammar for input is:

    Statement:
        Expression
        Print
        Quit

    Print:
        ;

    Quit:
        q 

    Expression:
        Term
        Expression + Term
        Expression - Term
    Term:
        Primary
        Term * Primary
        Term / Primary
        Term % Primary
    Primary:
        Number
        ( Expression )
        - Primary
        + Primary
    Number:
        floating-point-literal


        Input comes from cin through the Token_stream called ts.
*/

//------------------------------------------------------------------------------

const char number = '8';    // t.kind==number means that t is a number Token
const char quit   = 'q';    // t.kind==quit means that t is a quit Token
const char print  = ';';    // t.kind==print means that t is a print Token
const string prompt = "> ";
const string result = "= "; // used to indicate that what follows is a result

//------------------------------------------------------------------------------

class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }    
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------
//* help interact with cin , provide get() and putback() 
//* get() return a Token from cin,  putback() put a Token back into the Token_stream
class Token_stream {
public: 
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
    void ignore(char c);      // discard tokens up to an including a c
    void init();           
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

// //------------------------------------------------------------------------------

// // The constructor just sets full to indicate that the buffer is empty:
// Token_stream::Token_stream()
// :full(false), buffer(0)    // no Token in buffer
// {
// }

// //------------------------------------------------------------------------------

// // The putback() member function puts its argument back into the Token_stream's buffer:
// void Token_stream::putback(Token t)
// {
//     if (full) error("putback() into a full buffer");
//     buffer = t;       // copy t to buffer
//     full = true;      // buffer is now full
// }

// //------------------------------------------------------------------------------

// Token Token_stream::get() // read characters from cin and compose a Token
// {
    
//     if (full) {         // check if we already have a Token ready
//         full=false;
//         return buffer;
//     }  

//     char ch;
//     // cin >> ch;          // note that >> skips whitespace (space, newline, tab, etc.)
//     ss >> ch;
    
//     switch (ch) {
//     case quit:
//     case print:
//     case '(':
//     case ')':
//     case '+':
//     case '-':
//     case '*':
//     case '/': 
//     case '%':
//     case '^':
//     case '!':
//         return Token(ch); // let each character represent itself
//     case '.':             // a floating-point literal can start with a dot
//     case '0': case '1': case '2': case '3': case '4':
//     case '5': case '6': case '7': case '8': case '9':    // numeric literal
//     {
//         // cin.putback(ch);// put digit back into the input stream
//         ss.putback(ch);
//         double val;
//         // cin >> val;     // read a floating-point number
//         ss >> val;
//         return Token(number,val);
//     }
//     case 0:
//         return NULL;
//     default:
//         error("Bad token");
//     }
// }

// //------------------------------------------------------------------------------

// void Token_stream::ignore(char c)
//     // c represents the kind of a Token
// {
//     // first look in buffer:
//     if (full && c==buffer.kind) {
//         full = false;
//         return;
//     }
//     full = false;

//     // now seach input:
//     char ch = 0;
//     while (ss>>ch)
//         if (ch==c) return;
// }

// void Token_stream::init(){
//     full = false;
//     buffer = Token(0);
// }

// //------------------------------------------------------------------------------

// provides get() and putback() 
// Token_stream ts;        

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

// deal with numbers and parentheses ADD: ^, !
double primary();
// {
//     Token t = ts.get();
//     switch (t.kind) {
//     case '(':           // handle '(' expression ')'
//         {
//             double d = expression();
//             t = ts.get();
//             if (t.kind != ')') error("')' expected");
//             return d;
//         }
//     case number:
//         {
//             Token t2 = ts.get();
//             if(t2.kind == '^') {
//                 double left = t.value;
//                 left = pow(left, primary());
//                 return left;
//             }
//             else if(t2.kind == '!') {
//                 double left = t.value;
//                 left = factorial(static_cast<int>(left));
//                 return left;
//             }
//             else {
//                 ts.putback(t2);
//                 return t.value;
//             }
//         }
//     case '-':
//         return - primary();
//     case '+':
//         return primary();
//     default:
//         error("primary expected");
//     }
// }

//------------------------------------------------------------------------------

// deal with *, /, and % 
//* 
double term();
// {
//     double left = primary();
//     // cout << "primary: " << left << endl;
//     Token t = ts.get(); // get the next token from token stream

//     // cout << "token: " << t.kind << endl;
//     while(true) {
//         switch (t.kind) {
//         case '*':
//             left *= primary();
//             t = ts.get();
//             break;
//         case '/':
//             {    
//                 double d = primary();
//                 if (d == 0) error("divide by zero");
//                 left /= d; 
//                 t = ts.get();
//                 break;
//             }
//         case '%':
//             {    
//                 int i1 = static_cast<int>(left); //narrow_cast
//                 int i2 = static_cast<int>(term());
//                 if (i2 == 0) error("%: divide by zero");
//                 left = i1%i2; 
//                 t = ts.get();
//                 break;
//             }
//         default: 
//             ts.putback(t);        // put t back into the token stream
//             return left;
//         }
//     }
// }

//------------------------------------------------------------------------------

// deal with + and -
double expression();
// {   
//     double left = term();      // read and evaluate a Term

//     Token t = ts.get();        // get the next token from token stream

//     while(true) {    
//         switch(t.kind) {
//         case '+':
//             left += term();    // evaluate Term and add
//             t = ts.get();
//             break;
//         case '-':
//             left -= term();    // evaluate Term and subtract
//             t = ts.get();
//             break;
//         default: 
//             ts.putback(t);     // put t back into the token stream
//             return left;       // finally: no more + or -: return the answer
//         }
//     }
// }

//------------------------------------------------------------------------------

// void clean_up_mess()
// { 
//     ts.ignore(print);
// }

//------------------------------------------------------------------------------
double calculate(string input);

//------------------------------------------------------------------------------

// int main()
// try {
//     calculate();
//     keep_window_open();    // cope with Windows console mode
//     return 0;
// }
// catch (runtime_error& e) {
//     cerr << e.what() << endl;
//     keep_window_open("~~");
//     return 1;
// }
// catch (...) {
//     cerr << "exception \n";
//     keep_window_open("~~");
//     return 2;
// }

//------------------------------------------------------------------------------
#endif // CALCULATOR_HPP