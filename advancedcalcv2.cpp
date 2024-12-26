/*****************************************************************************
* Author      : Ashton Kunkle
* Filename    : advancedcalcv2.cpp
* Date        : 12/02/24
* Description : A semi-functional multi-digit calculator.
* Sources     : https://www.geeksforgeeks.org/convert-infix-expression-to-postfix-expression/
*                   For help with infix-to-postfix convertion algorithm
*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <stack>
#include <stdexcept>
#include <cmath>
#include <typeinfo>
#include <iomanip>


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stack;
using std::variant;
using std::holds_alternative;
using std::get;
using std::setprecision;
using std::fixed;








/********************************* Determines the precedence of a character */
int precedence (char c)
{   
    switch (c)
    {
        case '(':
            return 0;
        
        case ')':
            return 1;

        case '~':
            return 2;
        
        case '+': case '-':
            return 3;

        case '*': case '/':
            return 4;
        
        case '^':
            return 5;
        
        default:
            return -1;
    }
}








/*********************** Converts a string of number characters to a number */
double asciiToNum (string s)
{
    double sum = 0;
    int countDigits = 0;


    if (!s.find('.'))
    {
        countDigits = s.length() - 1;
    }

    else
    {
        countDigits = s.substr(0, s.find('.')).length() - 1;
    }


    for (char c : s)
    {
        if (c != '.')
        {
            sum += (double(c) - 48) * pow (10, countDigits);
            countDigits--;
        }
    }
    return sum;
}








/**************** Separates a string into individual operands and operators */
vector<variant<double, char>> split (string s)
{
    string str;
    double num = 0.0;
    vector<variant<double, char>> splitExp;


    for (char c : s)
    {
        if (precedence(c) < 0)
        {
            str += c;
        }

        else
        {
            if (str != "")
            {
                num = asciiToNum(str);
                splitExp.push_back(num);
            }
            splitExp.push_back(c);

            str = "";
        }
    }


    if (str != "")
    {
        num = asciiToNum(str);
        splitExp.push_back(num);
    }
    return splitExp;
}








/********************************** Computes partial expressions like 1 + 2 */
double compute (double first, double second, char c)
{
    switch (c)
    {
        case '+':
            return first + second;

        case '-':
            return first - second;

        case '*':
            return first * second;

        case '/':
            if (second != 0)
            {
                return first / second;
            }
            cout << "Cannot divide by 0" << endl << endl;

        case '^':
            return pow(first, second);

        default:
            throw std::invalid_argument ("Not a valid expression");
    }
}








/************************* Converts an infix expression to postfix notation */
vector<variant<double, char>> convert (vector<variant<double, char>> infix)
{
    stack<variant<double, char>> operators;
    vector<variant<double, char>> postfix;


    /* Prints the infix expression to the console */
    cout << endl << "******************************************************************************";
    cout << endl << "Infix Expression   : ";
    
    for (auto a : infix)
    {
        if (holds_alternative<double>(a))
        {
            cout << get<double>(a) << " ";
        }
        
        else
        {
            cout << get<char>(a) << " ";
        }
    }
    cout << endl;


    for (auto a : infix)
    {
        if (holds_alternative<double>(a))
        {
            postfix.push_back(a);
        }

        else
        {
            if (get<char>(a) == '(')
            {
                operators.push(a);
            }

            else if (get<char>(a) == ')')
            {
                while (precedence(get<char>(operators.top())) != 0)
                {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.pop();
            }

            else
            {

                while (!operators.empty() && precedence(get<char>(a)) <= precedence(get<char>(operators.top())))
                {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(a);
            }
        }
    }
    
    while (!operators.empty())
    {
        postfix.push_back(operators.top());
        operators.pop();
    }


    /* Prints the postfix expression to the console */
    cout << endl << "Postfix Expression : ";
    
    for (auto a : postfix)
    {
        if (holds_alternative<double>(a))
        {
            cout << get<double>(a) << " ";
        }
        else
        {
            cout << get<char>(a) << " ";
        }
    }
    cout << endl << endl;
    
    return postfix;
}








/******************************************* Evaluates a postfix expression */
void evaluate (vector<variant<double, char>> postfix, int precision)
{
    stack<variant<double, char>> solution;

    for (auto a : postfix)
    {
        if (holds_alternative<double>(a))
        {
            solution.push(a);
        }

        else
        {
            double first = get<double>(solution.top());
            solution.pop();
            double second = get<double>(solution.top());
            solution.pop();
            double result = compute(second, first, get<char>(a));
            solution.push(result);
        } 
    }

    /* Prints the result to the console with the user-set precision */
    cout << fixed << setprecision(precision) << "Result             : " << get<double>(solution.top()) << endl;
    solution.pop();
    cout << "******************************************************************************" << endl;
}








int main()
{
    /* Reads in a string from an external file */
    std::ifstream myFile;
    myFile.open("infixExpression.txt");
    std::string s;
    myFile >> s;


    /* Prompts the user to set the floating point precision */
    cout << "Set precision: ";
    int precision;
    cin >> precision;

    if (precision > 10)
    {
        precision = 10;
    }


    /* Splits the input string into meaningful operands and operators */
    vector<variant<double, char>> infix;
    infix = split(s);

    
    /* Converts the infix expression to postfix notation */
    vector<variant<double, char>> postfix;
    postfix = convert(infix);


    /* Solves the postfix expression */
    evaluate(postfix, precision);

    return 0;
}