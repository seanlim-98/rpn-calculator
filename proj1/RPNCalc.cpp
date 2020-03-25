/*
 *
 *  COMP 15 Project 1 CalcYouLater
 *
 *  RPNCalc.cpp
 *  The implementation of the RPNCalc class - each RPNCalc instance is one
 *  round contains a Datum Stack which is what enables us to conduct our
 *  calculator operations. Here we manage the functionalities behind the
 *  running of the program.
 *  Modified By (UTLN): Sean Lim (slim07)
 *           On       : 2/25/2020
 *
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include "RPNCalc.h"

using namespace std;

// Function RPNCalc
// Parameters: none
// Returns: nothing
// Does: default RPNCalc constructor; initializes a DatumStack
RPNCalc::RPNCalc()
{
    calculatorStack = DatumStack();
};

// Function ~RPNCalc
// Parameters: none
// Returns: nothing
// Does: RPNCalc destructor - clears stack 
RPNCalc::~RPNCalc()
{
    clear();
}

// Function run
// Parameters: none
// Returns: nothing
// Does: takes query from user and then runs calculator
void RPNCalc::run()
{
    string query;
    while (query != "quit") {
        cin >> query;
        stringstream iss(query);
        char first_char = iss.get();
        runQueries(query, first_char, cin);
    }
}

// Function pushIntToStack
// Parameters: string query
// Returns: nothing
// Does: Takes an integer input from user and pushes integer Datum to stack
void RPNCalc::pushIntToStack(string query)
{
    int number;
    int count = 0;
    stringstream new_stream(query);
    new_stream >> number;
    if (query[0] == '-') {
        count += 1;
    }
    for (int i = 0; i < query.length(); i++) {
        if (isdigit(query[i])) {
            count++;
        }
    }
    if (count != query.length()) {
        cerr << query << ":  unimplemented" << endl;
    } else {
        calculatorStack.push(Datum(number));
    }
}

// Function pushBoolToStack
// Parameters: bool value
// Returns: nothing
// Does: Takes an boolean input from user and pushes bool Datum to stack
void RPNCalc::pushBoolToStack(bool value)
{
    calculatorStack.push(Datum(value));
}

// Function notQuery
// Parameters: nothing
// Returns: nothing
// Does: Handles the not query - if top of the Datumstack is a boolean, this
// function reverses the value of that Datum.
void RPNCalc::notQuery()
{
    if (!calculatorStack.top().isBool()) {
        popFromStack();
        cerr << "Error: datum_not_bool" << endl;
    } else {
        if (calculatorStack.top().getBool() == true) {
            popFromStack();
            pushBoolToStack(false);
        } else {
            popFromStack();
            pushBoolToStack(true);
        }
    }
}

// Function popFromStack
// Parameters: nothing
// Returns: nothing
// Does: Pops Datum from top of DatumStack
void RPNCalc::popFromStack()
{
    if (!calculatorStack.isEmpty()) {
        calculatorStack.pop();
    } else {
        cerr << "Error: empty_stack\n";
    }
}

// Function print
// Parameters: nothing
// Returns: nothing
// Does: Prints out Datum at the top of DatumStack
void RPNCalc::print()
{
    if (calculatorStack.isEmpty()) {
        cerr << "Error: empty_stack\n";
    } else {
        cout << calculatorStack.top().toString() << endl;
    }
}

// Function clear
// Parameters: nothing
// Returns: nothing
// Does: Clears DatumStack
void RPNCalc::clear()
{
    calculatorStack.clear();
}

// Function duplicate
// Parameters: nothing
// Returns: nothing
// Does: Duplicates the Datum at the top of the stack and pushes copy to the top
void RPNCalc::duplicate()
{
    if (calculatorStack.isEmpty()) {
        cerr << "Error: empty_stack\n";
    } else {
        calculatorStack.push(calculatorStack.top());
    }
}

// Function swap
// Parameters: nothing
// Returns: nothing
// Does: Swaps the positions of the top 2 Datums in the stack
void RPNCalc::swap()
{
    if (calculatorStack.isEmpty()) {
        cerr << "Error: empty_stack\n";
    } else if (calculatorStack.size() < 2) {
        clear();
        cerr << "Error: empty_stack\n";
    } else {
        Datum temp = calculatorStack.top();
        popFromStack();
        Datum temp_2 = calculatorStack.top();
        popFromStack();
        calculatorStack.push(temp);
        calculatorStack.push(temp_2);
    }
}

// Function integer_operator
// Parameters: string query - the mathematical operator
// Returns: nothing
// Does: Runs a mathematical equation based on the operator pushed to the top
// of the stack. Operator concerns the next 2 Datums in the stack after the
// operator. An integer is pushed atop the stack.
void RPNCalc::integer_operator(string query)
{
    int result = 0;
    int first_number, second_number;
    if (calculatorStack.isEmpty() || calculatorStack.size() < 2) {
        clear();
        cerr << "Error: empty_stack\n";
    } else {
        Datum first_num = calculatorStack.top();
        calculatorStack.pop();
        Datum second_num = calculatorStack.top();
        calculatorStack.pop();
        if (!first_num.isInt() || !second_num.isInt()) {
            cerr << "Error: datum_not_int\n";
        } else if (query == "/" && first_num.getInt() == 0) {
            calculatorStack.push(second_num);
            calculatorStack.push(first_num);
            cerr << "Error: division by 0.\n";
        } else {
            first_number = first_num.getInt();
            second_number = second_num.getInt();
            if (query == "+") {
                result = (second_number + first_number);
            } else if (query == "-") {
                result = (second_number - first_number);
            } else if (query == "*") {
                result = (second_number * first_number);
            } else if (query == "/") {
                result = (second_number / first_number);
            } else if (query == "mod") {
                result = (second_number % first_number);
            }
            calculatorStack.push(Datum(result));
        }
    }
}

// Function comparison_operator
// Parameters: string query - the comparison operator
// Returns: nothing
// Does: Runs a mathematical equation based on the operator pushed to the top
// of the stack. Operator concerns the next 2 Datums in the stack after the
// operator. Boolean value (result) pushed on top of stack.
void RPNCalc::comparison_operator(string query)
{
    bool result = true;
    int first_number, second_number;
    if (calculatorStack.isEmpty() || calculatorStack.size() < 2) {
        clear();
        cerr << "Error: empty_stack\n";
    } else {
        Datum first_datum = calculatorStack.top();
        calculatorStack.pop();
        Datum second_datum = calculatorStack.top();
        calculatorStack.pop();
        if (query == "==") {
            result = (first_datum == second_datum);
        } else {
            if (!first_datum.isInt() || !second_datum.isInt()) {
                cerr << "Error: datum_not_int\n";
            } else {
                first_number = first_datum.getInt();
                second_number = second_datum.getInt();
                if (query == "<") {
                    result = (second_number < first_number);
                } else if (query == ">") {
                    result = (second_number > first_number);
                } else if (query == "<=") {
                    result = (second_number <= first_number);
                } else if (query == ">=") {
                    result = (second_number >= first_number);
                }
            }
        }
        calculatorStack.push(Datum(result));
    }
}

// Function parseRString
// Parameters: stream &input
// Returns: string - the parsed RString
// Does: Parses RString into a string
string RPNCalc::parseRString(istream &input)
{
    int num_open_braces = 1;
    int num_closed_braces = 0;
    string output = "{ ";
    string toAdd = "";

    string query;
    while (num_open_braces != num_closed_braces) {
        input >> query; // takes the next character after a space
        if (query == "{") {
            num_open_braces++;
        }
        if (query == "}") {
            num_closed_braces++;
        }
        toAdd += query;
        output += toAdd;
        toAdd = " ";
    }
    return output;
}

// Function executeRString
// Parameters: nothing
// Returns: nothing
// Does: Extracts commands within the RString and then runs them in the calc
// program as individual commands.
void RPNCalc::executeRString()
{
    if (calculatorStack.isEmpty()) {
        cerr << "Error: empty_stack" << endl;
    } else {
        if (!calculatorStack.top().isRString()) {
            cerr << "Error: file operand not rstring" << endl;
        } else {
            int num_open_braces = 1, num_closed_braces = 0;
            string query;
            string rString = calculatorStack.top().toString();
            calculatorStack.pop();
            stringstream ss(rString);
            ss >> query;
            ss >> query;
            while (num_open_braces != num_closed_braces) {
                stringstream word(query);
                char first_char = word.get();
                if (query != "}" || query != "{") {
                    runQueries(query, first_char, ss);
                }
                ss >> query;
                if (query == "{") {
                    num_open_braces++;
                }
                if (query == "}") {
                    num_closed_braces++;
                }
            }
        }
    }
}

// Function ifQuery
// Parameters: nothing
// Returns: nothing
// Does: Extracts the if query. Executes an operation in an RString based on
// boolean value of the 3rd Datum from the top in the stack.
void RPNCalc::ifQuery()
{
    if (calculatorStack.isEmpty()) {
        cerr << "Error: empty_stack" << endl;
    } else if (calculatorStack.size() < 3) {
        clear();
        cerr << "Error: empty_stack" << endl;
    } else {
        Datum false_condition = calculatorStack.top();
        calculatorStack.pop();
        Datum true_condition = calculatorStack.top();
        calculatorStack.pop();
        Datum bool_Datum = calculatorStack.top();
        calculatorStack.pop();

        if (!false_condition.isRString()) {
            cerr << "Error: expected rstring in if branch\n";
            return;
        } else if (!true_condition.isRString()) {
            cerr << "Error: expected rstring in if branch\n";
            return;
        } else if (!bool_Datum.isBool()){
            cerr << "Error: expected boolean in if test\n";
            return;
        }

        if (bool_Datum.getBool() == true) {
            calculatorStack.push(true_condition);
        } else {
            calculatorStack.push(false_condition);
        }
        executeRString();
    }
}

// Function fileQuery
// Parameters: nothing
// Returns: nothing
// Does: Handles file query. Takes file name from RString and then reads the
// commands in the file of that name.
void RPNCalc::fileQuery() {
    if (calculatorStack.isEmpty()) {
        cerr << "Error: empty_stack" << endl;
    } else {
        Datum fileString = calculatorStack.top();
        calculatorStack.pop();
        if (!fileString.isRString()) {
            cerr << "Error: file operand not rstring\n";
        } else {
            string first, filename, end;
            string rString = fileString.toString();
            stringstream ss(rString);
            ss >> first >> filename >> end;
            ifstream infile;
            string query;
        	infile.open(filename);
        	if (infile.fail()) {
                cerr << "Unable to read " << filename << endl;
                return;
            }
            while (infile >> query) {
                if (query == "quit") {
                    cerr << "Thank you for using CalcYouLater.\n";
                    exit(0);
                }
                runQueries(query, query[0], infile);
            }
        	infile.close();
        }
    }
}

// Function subtraction_operator
// Parameters: string query
// Returns: nothing
// Does: If first character of command is '-', we check whether it is the
// negative sign for a number, or if it is the subtract integer operator.
void RPNCalc::subtraction_operator(string query) {
    if (query.length() == 1) {
        integer_operator(query);
    } else {
        pushIntToStack(query);
    }
}

// Function runQueries
// Parameters: string query, char first_char, istream& infile
// Returns: nothing
// Does: Runs different queries based on the command that user inputs
void RPNCalc::runQueries(string query, char first_char, istream& infile)
{
    if (int(first_char) <= 57 && int(first_char) >= 48) {
        pushIntToStack(query);
    } else if (query == "#t") {
        pushBoolToStack(true);
    } else if (query == "#f") {
        pushBoolToStack(false);
    } else if (query == "not") {
        notQuery();
    } else if (query == "print") {
        print();
    } else if (query == "clear") {
        clear();
    } else if (query == "drop") {
        popFromStack();
    } else if (query == "dup") {
        duplicate();
    } else if (query == "swap") {
        swap();
    } else if (query == "quit") {
        cerr << "Thank you for using CalcYouLater.\n";
        return;
    } else if (query == "+" || query == "-" || query == "*" || query == "/"
    || query == "mod") {
        integer_operator(query);
    } else if (int(first_char) == 45) {
        subtraction_operator(query);
    } else if (query == "<" || query == ">" || query == "<=" || query ==
    ">=" || query == "==") {
        comparison_operator(query);
    } else if (int(first_char) == 123) {
        string output = parseRString(infile);
        calculatorStack.push(Datum(output));
    } else if (query == "exec") {
        executeRString();
    } else if (query == "if") {
        ifQuery();
    } else if (query == "file") {
        fileQuery();
    } else if (query == "}") {
        // do nothing
    } else {
        cerr << query << ":  unimplemented" << endl;
    }
}
