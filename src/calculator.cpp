#include "calculator.hpp"

//------------------------------------------------------------------------------
std::stringstream ss;
Token_stream ts;   
// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get() // read characters from cin and compose a Token
{
    
    if (full) {         // check if we already have a Token ready
        full=false;
        return buffer;
    }  

    char ch;
    // cin >> ch;          // note that >> skips whitespace (space, newline, tab, etc.)
    ss >> ch;
    
    switch (ch) {
    case quit:
    case print:
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/': 
    case '%':
    case '^':
    case '!':
        return Token(ch); // let each character represent itself
    case '.':             // a floating-point literal can start with a dot
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':    // numeric literal
    {
        // cin.putback(ch);// put digit back into the input stream
        ss.putback(ch);
        double val;
        // cin >> val;     // read a floating-point number
        ss >> val;
        return Token(number,val);
    }
    case 0:
        return NULL;
    default:
        error("Bad token");
    }
}

//------------------------------------------------------------------------------

void Token_stream::ignore(char c)
    // c represents the kind of a Token
{
    // first look in buffer:
    if (full && c==buffer.kind) {
        full = false;
        return;
    }
    full = false;

    // now seach input:
    char ch = 0;
    while (ss>>ch)
        if (ch==c) return;
}

void Token_stream::init(){
    full = false;
    buffer = Token(0);
}

//------------------------------------------------------------------------------
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':           // handle '(' expression ')'
        {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
    case number:
        {
            Token t2 = ts.get();
            if(t2.kind == '^') {
                double left = t.value;
                left = pow(left, primary());
                return left;
            }
            else if(t2.kind == '!') {
                double left = t.value;
                left = factorial(static_cast<int>(left));
                return left;
            }
            else {
                ts.putback(t2);
                return t.value;
            }
        }
    case '-':
        return - primary();
    case '+':
        return primary();
    default:
        error("primary expected");
    }
}

double term()
{
    double left = primary();
    // cout << "primary: " << left << endl;
    Token t = ts.get(); // get the next token from token stream

    // cout << "token: " << t.kind << endl;
    while(true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/':
            {    
                double d = primary();
                if (d == 0) error("divide by zero");
                left /= d; 
                t = ts.get();
                break;
            }
        case '%':
            {    
                int i1 = static_cast<int>(left); //narrow_cast
                int i2 = static_cast<int>(term());
                if (i2 == 0) error("%: divide by zero");
                left = i1%i2; 
                t = ts.get();
                break;
            }
        default: 
            ts.putback(t);        // put t back into the token stream
            return left;
        }
    }
}
double expression()
{   
    double left = term();      // read and evaluate a Term

    Token t = ts.get();        // get the next token from token stream

    while(true) {    
        switch(t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default: 
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

double calculate(string input)
{
    ts.init();
    ss.clear();
    ss << input;
    while(ss)
      try {
        return expression();
    }
    catch (exception& e) {
        cerr << e.what() << endl;        // write error message
        // clean_up_mess();
    }
    return 0;
}