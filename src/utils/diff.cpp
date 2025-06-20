#include"utils/diff.hpp"
#include <iostream> 
#include <fstream>
#include <sstream> 
#include <string> 
#include <filesystem> 
#include <vector> 
#include <algorithm> 
#include <map> 
#include<nlohmann/json.hpp>
using json = nlohmann::json;

namespace f = std::filesystem; 
using namespace std; 

//here we also need to insert the different colors to show insertion deletion and changes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
//here we use reset to reset the color changed by the above colors 
#define RESET "\033[0m"
 

string getLastBlobHash(const string& filename){//get the blob hash of the file... before we have done some editing
    ifstream headFile(".minigit/HEAD");//use HEAD to get the last commit
    if (!headFile.is_open()) return "";

    string headLine; 
    std::getline(headFile, headLine); 
    headFile.close(); 

    if (headLine.rfind("ref: ", 0) != 0) return "";

    string branchPath = headLine.substr(5);  // "branches/main"
    ifstream branchFile(".minigit/" + branchPath);
    if (!branchFile.is_open()) return "";

    string commitId;
    getline(branchFile, commitId);
    branchFile.close();

    if (commitId.empty()) return "";
    //use the commit id found from step one to find the corresponding blob(file) based on its name
    ifstream commitJsonFile(".minigit/commits/" + commitId + ".json");
    if (!commitJsonFile.is_open()) return "";

    json commitJson;
    commitJsonFile >> commitJson;

    if (!commitJson.contains("files")) return "";

    json files = commitJson["files"];
    if (files.contains(filename)) {
        return files[filename];
    }

    return "";
}

string blobContent_diff(const string& blobHash){//now that we have got the blobhash from the previous function we can read the content of the file
    ifstream blob(".minigit/blobs/" + blobHash);//get into the specific file  
    stringstream content; 
    content << blob.rdbuf();//read the whole file at once and put it in the stream content 
    blob.close(); 
    string result = content.str();//change the whole content to string 
  
    return result; 
}

vector<string> splitLines(string content){//I am going to split the content(the string that we already got from blobContent) into lines
    vector<string> lines; 
    stringstream ss(content);//change the content string to string stream for better manipulation of content 
    string line; 
    while(getline(ss, line, '\n')){//go line by line, use the '\n' as a delimiter to create each lines 
        lines.push_back(line); 
    }

    return lines; 
}

void showDiff(const vector<string>& oldLines, const vector<string>& newLines){
    int oldPtr = 0, newPtr = 0; //we can use two pointers to detect differences in each line
    int o = oldLines.size(), n = newLines.size(); 
    while(oldPtr < o || newPtr < n){//while we are haven't reached the last eleement in either the vectors
        if(oldPtr < o && newPtr < n){
            if(oldLines[oldPtr] == newLines[newPtr]){
                cout << oldLines[oldPtr] << endl; 
                oldPtr++;
                newPtr++; 
            }else{
                //hunk header print 
                cout << YELLOW << "@@ -" << oldPtr + 1 << "+ " << newPtr + 1 << " @@\n" << RESET;
                //insertion  
                if(newPtr + 1 < n && oldLines[oldPtr] == newLines[newPtr + 1]){
                    //cout << "Line: " << newPtr + 1 << " inserted." << endl; 
                    cout << GREEN << "+ " << newLines[newPtr] << RESET << endl; 
                    newPtr++; 
                }
                //deletion
                else if (oldPtr + 1 < o && newLines[newPtr] == oldLines[oldPtr + 1]){
                    //cout << "Line: " << oldPtr + 1 << " deleted." << endl; we could actually do this instead of hunks
                    cout << RED << "- " << oldLines[oldPtr] << RESET << endl; 
                    oldPtr++; 
                }
                //change (both insertion and deletion)
                else{
                    //cout << "Line: " << oldPtr + 1 << "changed" << endl;
                    cout << RED <<  "-" << oldLines[oldPtr] << RESET << endl; 
                    cout << GREEN << "+" << newLines[newPtr] << RESET << endl; 
                    oldPtr++; 
                    newPtr++;
                }
                 
            }

        }
        //deletion
        else if(oldPtr < o){
            cout << RED << "@@ -" << oldPtr + 1  << " +0 @@\n" << RESET; 
            while(oldPtr< o){
                cout << RED << "-" << oldLines[oldPtr] << RESET << endl; 
                oldPtr++; 
            }
            
        }
        //insertion
        else if(newPtr < n){
            cout << YELLOW << "@@ -0 +" << newPtr + 1 << " @@\n" << RESET; 
            while(newPtr< n){
                cout << GREEN << "+" << newLines[newPtr] << RESET << endl; 
                newPtr++;
            }
            
        }
    }
}

void diffCommand(const string& filename){
    string blobHash = getLastBlobHash(filename);//get the blob_hash of the file from the last commit
    if(blobHash.empty()){
        cout << "no previous commit found for file: " << filename << endl;//blob hash is empty it implies no record of the file begin commited 
        return; 
    }
    //now read the content of the old version, we could just use the blobcontent function that we already did
    string oldContent = blobContent_diff(blobHash); 
    //read the contents from the current working file
    ifstream currentFile(filename); 
    if(!currentFile.is_open()){
        cout << "unable to open " << filename << endl;//an exception if we can't open the file for some reasons
        return; 
    }

    stringstream currentContent; 
    currentContent << currentFile.rdbuf();//read the whole thing from the opened file
    string newContent = currentContent.str(); 
    currentFile.close(); 

    //now we convert both string contents from the previous commit and the file content to vectors using splitLines 
    vector<string> oldLines = splitLines(oldContent); 
    vector<string> newLines = splitLines(newContent); 

    cout << "differnce in file: " << filename << endl; 
    showDiff(oldLines, newLines); 
}

// a  function for showing the difference between two files 
void diffBetweenFiles(const string& file1, const string& file2){
    ifstream file1content(file1); 
    ifstream file2content(file2); 
    
    //check if we have opened the files, if now throw some exception lines 
    if(!file1content.is_open()){
        cout << RED << "failed to open " << file1 << endl; 
        return; 
    }
    if(!file2content.is_open()){
        cout << RED << "failed to open " << file2 << endl; 
        return; 
    }

    //now read all he contents of file1 and file2 and compare on a line_by_line based comparison
    stringstream buffer1, buffer2;//for reading all the contetns in both files
    string file1stringcontent, file2stringcontent;//for storing the read contents from the two files 

    //add all the content of the files to stringstreams for a better read of all contents at once
    buffer1 << file1content.rdbuf(); 
    buffer2 << file2content.rdbuf(); 

    //change the string streams to strings 
    file1stringcontent = buffer1.str(); 
    file2stringcontent = buffer2.str(); 

    file1content.close(); 
    file2content.close(); 
    //now we just split using our already made function splitLines 
    vector<string> file1vector = splitLines(file1stringcontent); 
    vector<string> file2vector = splitLines(file2stringcontent); 

    //we are now using the show diff function to show the difference and print it
    showDiff(file1vector, file2vector);
}