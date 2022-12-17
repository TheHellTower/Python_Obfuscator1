#include <ctime>
#include <format>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include "Base64.h"
#include "Utils.h"

using namespace std;

Base64 b64 = Base64();
Utils utils = Utils();

string templateOutput = R"(
import base64, codecs
magic = 'part1'
love = 'part2'
god = 'part3'
destiny = 'part4'
joy = '\x72\x6f\x74\x31\x33'
trust = eval('\x6d\x61\x67\x69\x63') + eval('\x63\x6f\x64\x65\x63\x73\x2e\x64\x65\x63\x6f\x64\x65\x28\x6c\x6f\x76\x65\x2c\x20\x6a\x6f\x79\x29') + eval('\x67\x6f\x64') + eval('\x63\x6f\x64\x65\x63\x73\x2e\x64\x65\x63\x6f\x64\x65\x28\x64\x65\x73\x74\x69\x6e\x79\x2c\x20\x6a\x6f\x79\x29')
)";
std::string normalTemplateOutput = R"(eval(compile(base64.b64decode(eval('\x74\x72\x75\x73\x74')),'<string>','exec')))";
std::string hexTemplateOutput = R"(eval(compile(base64.b64decode(eval('\x74\x72\x75\x73\x74')),'\x3c\x73\x74\x72\x69\x6e\x67\x3e','\x65\x78\x65\x63')))";

string filePath;
string mode;
bool correctMode = false;
ifstream file;
ofstream outputFile;

int main()
{
    //Need a valid file
    while (!utils.doesFileExist(filePath)) {
        std::cout << "Python File Path: ";
        std::getline(std::cin, filePath);
        system("cls");
    }
    while (!correctMode) {
        std::cout << "Mode(a = Normal / b = Hex): ";
        std::cin >> mode;
        
        if (mode == "a") {
            templateOutput.append(normalTemplateOutput);
            correctMode = true;
        }
        else if (mode == "b") {
            templateOutput.append(hexTemplateOutput);
            correctMode = true;
        }

        system("cls");
    }

    //Open the file
    file.open(filePath);

    //Iterate through the content
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    //Base64 encode the contnet
    fileContent = b64.Encode(fileContent);
    
    //Lenght divided by 4 to split
    int newLength = fileContent.length() / 4;

    //Part 1
    utils.replace_all(templateOutput, "part1", fileContent.substr(0, newLength));

    //Part 2 (Rot13)
    utils.replace_all(templateOutput, "part2", utils.rot13(fileContent.substr(newLength, newLength)));

    //Part 4
    utils.replace_all(templateOutput, "part3", fileContent.substr(newLength*2, newLength));

    //Part 4 (Rot13)
    utils.replace_all(templateOutput, "part4", utils.rot13(fileContent.substr(newLength * 3, newLength)));

    //Watermark
    templateOutput = "#Obfuscated By https://github.com/TheObfuscators/Python_Obfuscator1 (https://development-tools.net/python-obfuscator parody)\n" + templateOutput;

    //Output Path
    utils.replace_all(filePath, ".py", "-j0k3.py");

    //Finished
    outputFile.open(filePath, ofstream::binary);
    outputFile.write(templateOutput.c_str(), templateOutput.length());
    outputFile.close();
}