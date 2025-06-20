#include "diff.hpp"
#include <iostream> 
#include <fstream>
#include <sstream> 
#include <string> 
#include <filesystem> 
#include <vector> 
#include <algorithm> 
#include <map> 

namespace f = std::filesystem; 
using namespace std; 

string getLastBlobHash(const string& filename){//get the blob hash of the file... before we have done some editing
    ifstream commitFile(".minigit/HEAD");//use HEAD to get the last commit
    string commitId; 
    std::getline(commitFile, commitId); 
    commitFile.close(); 

    //use the commit id found from step one to find the corresponding blob(file) based on its name
    ifstream blobFile(".minigit/commits/" + commitId); 
    string line; 
    bool isfile = false; 
    //fine the file by comparing it with the file name that is found in the latest commit
    while(std::getline(blobFile, line)){
        if(line == "files:"){
            isfile = true; 
            continue; 
        }
        if(isfile && !line.empty()){
            size_t colon_pos = line.find(':'); 
            string file = line.substr(0, colon_pos); 
            string hash = line.substr(colon_pos + 1);
            if(file == filename) return hash; 
        }
    }
    return ""; 
}

string blobContent(const string& blobHash){//now that we have got the blobhash from the previous function we can read the content of the file
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
    while(oldPtr < o || newPtr < n){
        if(oldPtr < o && newPtr < n){
            if(oldLines[oldPtr] == newLines[newPtr]){
                cout << oldLines[oldPtr] << endl; 
                oldPtr++; 
                newPtr++; 
            }else{
                if(newPtr + 1 < n && oldLines[oldPtr] == newLines[newPtr + 1]){
                    cout << "Line: " << newPtr + 1 << " inserted." << endl; 
                    cout << "+ " << newLines[newPtr] << endl; 
                    newPtr++; 
                }else if (oldPtr + 1 < o && newLines[newPtr] == oldLines[oldPtr + 1]){
                    cout << "Line: " << oldPtr + 1 << " deleted." << endl; 
                    cout << "- " << oldLines[oldPtr] << endl; 
                    oldPtr++; 
                }

                else{
                    cout << "Line: " << oldPtr + 1 << "changed" << endl;
                    cout << "-" << oldLines[oldPtr] << endl; 
                    cout << "+" << newLines[newPtr] << endl; 
                    oldPtr++; 
                    newPtr++;
                }
                 
            }

        }else if(oldPtr < o){
            cout << "Line: " << oldPtr + 1 << "deleted" << endl;
            cout << "-" << oldLines[oldPtr] << endl; 
            oldPtr++; 
        }else if(newPtr < n){
            cout << "Line: " << newPtr + 1 << "inserted" << endl; 
            cout << "+" << newLines[newPtr] << endl; 
            newPtr++; 
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
    string oldContent = blobContent(blobHash); 
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

