#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>
#include "vector"
#include <unordered_map>
#include <stack> 
#include <memory>
using namespace std;
#include "ast.hpp"
#include "semAnalyzer.cpp"

//Data Structures
string dataSec, mainSec, funcSec;
int labelNum = 0, whileLabelNum = -1, currentRegister = 0;
unordered_map<string, entry> currSymTable;
vector<string> funcNames, variableStack;
bool inMain = false;

//Function
string createAssemblyCode(AST * root);
string writeTest(AST * node, int localLabelNum);
string getIntOrBool(AST* node);
string getOffset(string input);
string loadRegister(AST* node, int resultRegister);

void generateCode(AST * root) {
    ofstream file("output.asm");
    dataSec = "\t.globl main\n\t.data\n";
    root->Print();

    for (AST* child : root->getChildren()) {
        createAssemblyCode(child);
    }

    string stackAlloc = "\n\t.text\nmain:\nsub $sp, $sp, " + to_string(4*variableStack.size()).append("\n");
    mainSec = stackAlloc + mainSec;

    mainSec.append("end:\n");
    mainSec.append("li $v0, 10\n");
    mainSec.append("syscall\n"); 

    file << dataSec;
    file << mainSec;
    file << funcSec;
    file.close();

}

string createAssemblyCode(AST * node) {
    string temp = node->getNodeType();
    string output;
    if (temp == "maindecl") {
        inMain = true; 
        for (AST* child : node->getChildren()) {
            output.append(createAssemblyCode(child));

        }   
        mainSec.append(output);        
        inMain = false;
    }
    else if (temp == "funcdecl") {
        funcSec.append(node->getName()).append(":\n");
        for (AST* child : node->getChildren()) {
            output.append(createAssemblyCode(child));

        }       
        output.append("jr $ra\n");
        funcSec.append(output);  
    }

    else if (temp == "vardecl") {
        variableStack.push_back(node->getName());
    }

    else if (temp == "assnstmt") {
        string type = node->getChildren().at(1)->getNodeType();
        if (type == "funccall") {
            output.append(createAssemblyCode(node->getChildren().at(1)));
            output.append("move $t").append(to_string(currentRegister)).append(", $v0\n");            
        }
        else if (type == "id") {
            output.append(loadRegister(node->getChildren().at(1), currentRegister));
        }
        else if ((type == "literal") || (type == "num")) {
            output.append("li $t").append(to_string(currentRegister)).append(", ").append(getIntOrBool(node->getChildren().at(1))).append("\n");
        }
        else {
            output.append(createAssemblyCode(node->getChildren().at(1)));
        }
        output.append("sw $t").append(to_string(currentRegister)).append(", ").append(getOffset(node->getName())).append("($sp)\n");
    }

    else if (temp == "id") {
        // Return the offset of the id in the stack
        output.append(getOffset(node->getName()));
    }

    // Place params into the subroutine registers, then jump and link to the function given
    else if (temp == "funccall") {
        bool funcFound = false;
        auto it = symTables[5].begin();
        for (int i = 0; i < symTables[5].size(); i++) {
            if (it->first == node->getName()) {
                funcFound = true;
                for (auto& it2 : *it->second.symTable) {
                    if ((1 <= it2.second.paramNum) && (4 >= it2.second.paramNum)) {
                        string value;
                        string type = node->getChildren().at(it2.second.paramNum - 1)->getNodeType();
                        if (type == "num") {
                            value = node->getChildren().at(it2.second.paramNum - 1)->getValue();
                        } 
                        else if (type == "literal") {
                            if (node->getChildren().at(it2.second.paramNum - 1)->getValue() == "true") {
                                value = "1";
                            } else {
                                value = "0";
                            }
                        }
                        
                        output.append("li $a").append(to_string(it2.second.paramNum - 1)).append(", ").append(value);
                    } 
                    else if (it2.second.paramNum != 0) {
                        string value;
                        string type = node->getChildren().at(it2.second.paramNum - 1)->getNodeType();
                        if (type == "num") {
                            value = node->getChildren().at(it2.second.paramNum - 1)->getValue();
                        } 
                        else if (type == "literal") {
                            if (node->getChildren().at(it2.second.paramNum - 1)->getValue() == "true") {
                                value = "1";
                            } else {
                                value = "0";
                            }
                        }
                        
                        output.append("li $t").append(to_string(it2.second.paramNum - 5)).append(", ").append(value);                        
                    }
                }
                output.append("jal ").append(node->getName()).append("\n");
                break;
            }
            it++;
        }
        if (!funcFound) {
            string name = node->getName();
            if (name == "halt") {
                output.append("j end\n");
            }
            else if (name == "getchar") {
                dataSec.append("label").append(to_string(labelNum)).append(": .asciiz \"Enter an int now:\"\n");
                output.append("li $v0, 4\nla $a0, label").append(to_string(labelNum)).append("\nsyscall\n");
                output.append("li $v0, 5\nsyscall\n");

                labelNum++;
            }
            else {
                string strOutput;
                if (name == "printb") {
                    dataSec.append("label").append(to_string(labelNum)).append(": .asciiz \"true\"\n");                    
                    dataSec.append("label").append(to_string(labelNum+1)).append(": .asciiz \"false\"\n");
                    output.append("li $v0, 4\n");
                    if (node->getChildren().at(0)->getNodeType() == "literal") {
                        strOutput = getIntOrBool(node->getChildren().at(0));  
                        output.append("li $a0, ").append(strOutput).append("\n");       
                    }
                    else if (node->getChildren().at(0)->getNodeType() == "id") {
                        output.append("lw $a0, ").append(getOffset(node->getChildren().at(0)->getName())).append("($sp)\n");
                    }       
                    //Print out the branching if else statement for the printb function
                    output.append("beq $0, $a0, label").append(to_string(labelNum+2)).append("\n");
                    output.append("la $a0, label").append(to_string(labelNum)).append("\nsyscall\nb label").append(to_string(labelNum+3)).append("\n");
                    output.append("label").append(to_string(labelNum+2)).append(":\n");
                    output.append("la $a0, label").append(to_string(labelNum+1)).append("\nsyscall\n");
                    output.append("label").append(to_string(labelNum+3)).append(":\n");    
                    labelNum+=4;                    
                }
                else if (name == "printc") {
                    char arr[strOutput.size()];
                    for (int i = 0; i < strOutput.size(); i++) {
                        arr[i] = (int)strOutput.at(i);
                    }
                    dataSec.append("label").append(to_string(labelNum)).append(": .asciiz \"").append(arr).append("\"\n");
                    output.append("li $v0, 4\nla $a0, label").append(to_string(labelNum)).append("\nsyscall\n");
                    labelNum++;    
                }
                else if (name == "printi") { 
                    strOutput = getIntOrBool(node->getChildren().at(0));
                    if (strOutput == "id") {
                        output.append("li $v0, 1\nlw $a0, ").append(getOffset(node->getChildren().at(0)->getName())).append("($sp)\n").append("syscall\n");
                    } 
                    else {
                        output.append("li $v0, 1\nli $a0, ").append(strOutput).append("\nsyscall\n");
                    }                                
                }
                else if (name == "prints") {
                    strOutput = node->getChildren().at(0)->getValue();
                    dataSec.append("label").append(to_string(labelNum)).append(": .asciiz ").append(strOutput).append("\n");
                    output.append("li $v0, 4\nla $a0, label").append(to_string(labelNum)).append("\nsyscall\n");
                    labelNum++;  
                }
            }
        }
    }

    else if (temp == "if") {
        bool elseStmt = false;
        int localLabelNum = labelNum;
        labelNum++;
        for (AST* child : node->getChildren()) {
            if (child->getNodeType() == "else") {
                elseStmt = true;
                break;
            }
        }
        if (!elseStmt) {
            temp = writeTest(node->getChildren().at(0), localLabelNum);
            output.append(temp);
            for (int i = 1; i < node->getChildren().size(); i++) {
                createAssemblyCode(node->getChildren().at(i));
            }      
            output.append("label").append(to_string(localLabelNum)).append(":\n");

        }
        else {
            temp = writeTest(node->getChildren().at(0), localLabelNum);
            output.append(temp);
            // Skip over the first child to not write the test again for 
            // the if statement
            for (int i = 1; i < node->getChildren().size(); i++) {
                AST * child = node->getChildren().at(i);
                if (child->getNodeType() != "else") {
                    createAssemblyCode(child);
                }
            }
            int labelAfter = labelNum;
            labelNum++;
            output.append("b label").append(to_string(labelAfter)).append("\n");            
            output.append("label").append(to_string(localLabelNum)).append(":\n");
            for (AST* child : node->getChildren()) {
                if (child->getNodeType() == "else") {
                    createAssemblyCode(child);
                }
            }
            output.append("label").append(to_string(labelAfter)).append(":\n");            
            labelNum++;
        }
    }
    else if (temp == "else") {
        for (AST* child : node->getChildren()) {
            createAssemblyCode(child);
        }        
    }

    else if (temp == "break") {
        output.append("b label").append(to_string(whileLabelNum)).append("\n");
    }
    else if (temp == "return") {
        if (inMain) {
            output.append("j end\n");
        }
        else {
            if (node->getChildren().size() > 0) {
                AST *child = node->getChildren().at(0);
                output.append("li $v0, ").append(getIntOrBool(child)).append("\n");
            }
        }
    }
    else if (temp == "while") {
        int firstLabel = labelNum;
        labelNum++;
        int secondLabel = labelNum;
        if (whileLabelNum == -1) {
            whileLabelNum = secondLabel;
        }
        labelNum++; 
        output.append("label").append(to_string(firstLabel)).append(":\n");
        temp = writeTest(node->getChildren().at(0), secondLabel);
        output.append(temp);
        for (int i = 1; i < node->getChildren().size(); i++) {
            createAssemblyCode(node->getChildren().at(i));
        }        
        output.append("b label").append(to_string(firstLabel)).append("\n");
        output.append("label").append(to_string(secondLabel)).append(":\n");
        if (whileLabelNum == secondLabel) {
            whileLabelNum = -1;
        }
    }
    else if (temp == "arithmetic") {
        int resultRegister = currentRegister;
        bool singleNegative = false;
        AST* leftChild = node->getChildren().at(0);
        // If there's only one child, then that child must be the right side of a '-' operation
        AST* rightChild;
        if (node->getChildren().size() > 1) {
            rightChild = node->getChildren().at(1);
        }
        output.append(loadRegister(leftChild, resultRegister+1));
        if (rightChild == NULL) {
            singleNegative = true;
        } 
        else {
            output.append(loadRegister(rightChild, resultRegister+2));            
        }
        //Write to the text section for the correct arithmetic operation
        if (node->getType() == "+") {
            output.append("add $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        else if (node->getType() == "-") {
            if (singleNegative) {
            output.append("neg $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append("\n");
            }
            else {
            output.append("sub $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");                
            }
        }
        else if (node->getType() == "*") {
            output.append("mul $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        else if (node->getType() == "/") {
            output.append("div $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        else if (node->getType() == "%") {
            output.append("rem $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        currentRegister = resultRegister;
    }
    else if (temp == "compare") {
        int resultRegister = currentRegister;
        AST* leftChild = node->getChildren().at(0);
        AST* rightChild = node->getChildren().at(1);

        output.append(loadRegister(leftChild, resultRegister+1));
        output.append(loadRegister(rightChild, resultRegister+2));

        if (node->getType() == "==") {
            output.append("seq $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        if (node->getType() == "!=") {
            output.append("sne $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }        
        else if (node->getType() == ">=") {
            output.append("sge $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        else if (node->getType() == "<=") {
            output.append("sle $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");            
        }
        else if (node->getType() == ">") {
            output.append("sgt $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");            
        }
        else if (node->getType() == "<") {
            output.append("slt $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");            
        }
        currentRegister = resultRegister;
    }
    else if (temp == "logical") {
        bool notOper = false;
        int resultRegister = currentRegister;
        AST* leftChild = node->getChildren().at(0);
        AST* rightChild;
        if ((node->getChildren().size()) > 1) {
            rightChild = node->getChildren().at(1);
        } else {
            notOper = true;
        }

        output.append(loadRegister(leftChild, resultRegister+1));
        if (!notOper) {
            output.append(loadRegister(rightChild, resultRegister+2));
        }

        if (node->getType() == "&&") {
            output.append("and $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        else if (node->getType() == "||") {
            output.append("or $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append(", $t").append(to_string(resultRegister+2)).append("\n");
        }
        else if (node->getType() == "!") {
            output.append("not $t").append(to_string(resultRegister)).append(", $t").append(to_string(resultRegister+1)).append("\n");     
        }       
        currentRegister = resultRegister;
    }

    else if (temp == "block") {
        for (AST* child : node->getChildren()) {
            output.append(createAssemblyCode(child));
        }
    }
    return output;
}

string writeTest(AST* node, int localLabelNum) {
    string output;
    if (node->getNodeType() == "literal") {
        if (node->getValue() == "true") {
            output.append("li $t0, 1\nbeq $0, $t0, label").append(to_string(localLabelNum)).append("\n");
        }
        else {
            output.append("li $t0, 0\nbeq $0, $t0, label").append(to_string(localLabelNum)).append("\n");
        }
    }
    else if (node->getNodeType() == "compare") {
        int outputRegister = currentRegister;
        createAssemblyCode(node);
        output.append("beq $0, $t").append(to_string(outputRegister)).append(", label").append(to_string(localLabelNum)).append("\n");        
    }
    else if (node->getNodeType() == "logical") {
        int outputRegister = currentRegister;
        createAssemblyCode(node);
        output.append("beq $0, $t").append(to_string(outputRegister)).append(", label").append(to_string(localLabelNum)).append("\n");               
    }
    return output;
}

string getIntOrBool(AST* node) {
    string output;
    if (node->getValue() == "true") {
        return "1";
    }
    else if (node->getValue() == "false") {
        return "0";
    }
    else if (node->getType() == "int") {
        return node->getValue();
    }   
    else if (node->getNodeType() == "id") {
        return "id";
    }
    return "error";
}

string getOffset(string input) {
    for (int i = 0; i < variableStack.size(); i++) {
        if (variableStack.at(i) == input) {
            return to_string(4 * (variableStack.size() - (i+1)));
        }
    }
    return "";
}

string loadRegister(AST* node, int resultRegister) {
    string output;
    if (node->getType() == "int") {
            output.append("li $t").append(to_string(resultRegister)).append(", ").append(node->getValue()).append("\n");
        }
    else if (node->getValue() == "true") {
            output.append("li $t").append(to_string(resultRegister)).append(", 1\n");
    }
    else if (node->getValue() == "false") {
            output.append("li $t").append(to_string(resultRegister)).append(", 0\n");
    }
    else if (node->getNodeType() == "id") {
        output.append("lw $t").append(to_string(resultRegister)).append(", ").append(getOffset(node->getName())).append("($sp)\n");
    }
    else {
        currentRegister++;
        output.append(createAssemblyCode(node));
    }
    return output;
}