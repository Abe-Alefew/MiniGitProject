#include "core/merge.hpp"//to define the functions used in implementing merge
#include <iostream> 
#include <fstream> // for reading an writng to files like ".minigit/commits/"...
#include <set> //for getting the unique file names from each commits 
#include <unordered_map> 
#include <chrono> // tracking the current time, used when creating a commit of the merge
#include <ctime> // changing the created time to human recongnizable form 
#include <string> 
#include <filesystem> 
#include <sstream> // chaing number to string, used for chaning commitid's to strings
#include <map> // for creating file->hash key and value pairs 
#include <unordered_set> 
#include<climits>// for INT_MAX
#include<algorithm>
using namespace std; 
namespace f = std::filesystem; // just an alias to shorten filesystem usage

//we first have to check if the files are present in the first place
// the runtime files like ".minigit/branches", ".minigit/commits", ".minigit/blobs"...\
// we use the filesystem to check and create them 
void folderExists(){
    //check .minigit subfolders if existent then use them, if not create them
    if(!f::exists(".minigit/commits")) f::create_directory(".minigit/commits"); 
    if(!f::exists(".minigit/blobs")) f::create_directory(".minigit/blobs");
    if(!f::exists(".minigit/branches")) f::create_directory(".minigit/branches");
}
// a function to get the contents of the blob files(actually the files in a commit)
string blobContent(const string& blobHash){
    ifstream blobFile(".minigit/blobs/" + blobHash);// read from the specific bob
    if (!blobFile.is_open()) {
    cerr << "Error: file not found." << endl; 
    return "";
}
    stringstream wholeFile;// just to store the content in blob
    
    wholeFile << blobFile.rdbuf();//.rdbuf() stores the whole file content to wholeFile
    blobFile.close(); 
    string output = wholeFile.str();//wholeFile string stream to string 

    return output; 
}
//function to save the blobs in the commits file 
string saveBlob(const string& content){
    stringstream hash; 
    hash << chrono::system_clock::now().time_since_epoch().count();//taking time from a reference as a hashID for the blob file
    string blobhash = hash.str(); 
    ofstream write(".minigit/blobs/" + blobhash);// creating a file with blobhash as a name for the blob content
    write << content; 
    write.close(); 

    return blobhash;//returning the file name or the hash
}
//a function to make maps of file-name and blob hashes 
//first we use the commit id to get the metadata, specificly the files: part
//this function is used to compare the files in the two commits
map<string, string> commitToMap(const string& commitId){
    map<string, string> file_hash; 
    ifstream commitContent(".minigit/commits/" + commitId);//enter the file with the specific commitId
    if (!commitContent.is_open()) {
    cerr << "Error: file not found." << endl; 
    return file_hash;
}
    string line;//sotrage for each line
    bool isfile = false;//to track file:
    while(std::getline(commitContent, line)){//read each line 
        if(line == "files:"){
            isfile = true; 
            continue; 
        }
        if(isfile && !line.empty()){//read the lines after files:
            int position = 0; 
            for(int i = 0; i < line.size(); i++){
                if(line[i] == ':'){
                    position = i; 
                }
            }
            string filename = line.substr(0, position); 
            string hash = line.substr(position + 1); 

            file_hash[filename] = hash; 
        }
    }
    return file_hash; 
}

//a function to find the lowest common ancestor(LCA) of two commit branches 
//here we have to functions one is a helper function 
void collectAncestors(const string& commitId, map<string, int>& ancestors, int depth = 0){
    if(commitId.empty() || ancestors.count(commitId)) return;// if root is reached or if already visited

    ancestors[commitId] = depth;


    ifstream file(".minigit/commits/" + commitId);//open the commit to find the parent and traverse deep
    if (!file.is_open()) {
    cerr << "Error: file not found." << endl; 
    return;
    }
    string commitContent; 
    while(std::getline(file, commitContent)){//read each line
        if(commitContent.rfind("parents:", 0) == 0){//if the line starts with parents: 
            string Parents = commitContent.substr(9);//skip "parents: " and get the content
            //now we use stringstream to split by comma
            stringstream ss(Parents);
            string parent; 
            while(std::getline(ss, parent, ',')){//get each parents
                parent.erase(remove_if(parent.begin(), parent.end(), ::isspace), parent.end());//removing spaces from each parent, specifically for second and so on parents
                collectAncestors(parent, ancestors, depth + 1);//now 
            } 
        }
    }

}

//function to find the lowest common ancestor
string findCommonAncestor(const string& commitIdOurs, const string& commitIdTheirs){
    map<string, int> ancestors_ours; 
    map<string, int> ancestors_theirs; 

    //collect all ancestors of both commits
    collectAncestors(commitIdOurs, ancestors_ours); 
    collectAncestors(commitIdTheirs, ancestors_theirs); 

    string lCommonAncestor = "";//the commitId of the base of LCA
    int minDepth = INT_MAX;//for comparing purpose

    for(const auto& [ancestor, depthA] : ancestors_ours){//loop through the ancestor map of ours
        if(ancestors_theirs.count(ancestor)){//if the ancestor is also found in theirs...
            int totalDepth = depthA + ancestors_theirs[ancestor];//add the two depths
            if(totalDepth < minDepth){
                minDepth = totalDepth;// find the minimum sum of depths
                lCommonAncestor = ancestor; 
            }
        }
    } 
    return lCommonAncestor;//base
}
//main merge function to merge the two functions 
void mergeBranch(const string& targetBranch){
    folderExists(); 

    //get the commitIds of ours and their
    //head commit Id
    ifstream head(".minigit/HEAD"); 
    if (!head.is_open()) {
    cerr << "Error: file not found." << endl; 
    return;
}
    string ourCommitId; 
    std::getline(head, ourCommitId); 
    head.close(); 

    //get the commitId of their 
    ifstream mergedBranch(".minigit/branches/" + targetBranch);//enter to the specific branch by using tragetBranch
    if (!mergedBranch.is_open()) {
    cerr << "Error: file not found." << endl; 
    return;
}
    string theirCommitId; 
    std::getline(mergedBranch, theirCommitId); 
    mergedBranch.close(); 

    //get the commitId for base 
    string baseCommitId = findCommonAncestor(ourCommitId, theirCommitId); 


    //now we need to access each files in the two commits, for making the map of filename and blob hash
    map<string, string> ours = commitToMap(ourCommitId); 
    map<string, string> theirs = commitToMap(theirCommitId);
    map<string, string> base = commitToMap(baseCommitId);
    map<string, string> mergedFiles;//this map is used to fill the new commit which is created by meging the two commits

    //now we check each files in both commits
    set<string> allFiles; // storing each files in each commit
    for(auto& [file, _] : ours) allFiles.insert(file); 
    for(auto& [file, _] : theirs) allFiles.insert(file); 
    for(auto& [file, _] : base) allFiles.insert(file); 

    for(const string& file : allFiles){
        string oursFileHash = ours.count(file) ? ours[file] : "";//check if the file is from the ours branch or not
        string theirsFileHash = theirs.count(file) ? theirs[file] : ""; 
        string baseFileHash = base.count(file) ? base[file] : "";

        if(oursFileHash == theirsFileHash){
            mergedFiles[file] = oursFileHash; 
        }else if(baseFileHash == theirsFileHash){
            mergedFiles[file] = oursFileHash; 
        }else if(baseFileHash == oursFileHash){
            mergedFiles[file] = theirsFileHash; 
        }else{// conflic happens when all the three 
            //retrieve both files contents
            string oursContent = blobContent(oursFileHash); 
            string theirsContent = blobContent(theirsFileHash);


            string conflict = "<<<<<<< ours\n" + oursContent + 
            "\n=======\n" + theirsContent + "\n>>>>>>> theirs\n"; 

            string mergedHash = saveBlob(conflict);//create a new blob file for conflict 
            mergedFiles[file] = mergedHash; 
        }
    }

    //creating of new commit for the merged 
    //we need commitId
    stringstream Id; 
    Id << chrono::system_clock::now().time_since_epoch().count();// create a specific_for_all id
    string newCommitId = Id.str();// convert the Id to string 

    //timestamp
    stringstream time;
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string timestamp = string(ctime(&now)); 
    timestamp.pop_back(); 

    //now create the file
    ofstream Commit(".minigit/commits/" + newCommitId); 
    Commit << "commit_id: " << newCommitId << endl;
    Commit << "timestamp: " << timestamp << endl;
    Commit << "message: Merge " << targetBranch << " into current branch" << endl;
    Commit << "parents: " << ourCommitId << ", " << theirCommitId << endl;
    Commit << "files:" << endl;
    for(auto& [file, blobHash] : mergedFiles){
        Commit << file << ":" << blobHash << endl;// add every blob names and their unique id from mergedFiles
    }
    Commit.close(); 

    //Final code, making head point to newCommitId
    ifstream currentBranchFile(".minigit/current_branch");
    if (!currentBranchFile.is_open()) {
    cerr << "Error: file not found." << endl; 
    return;
} 
    string currentBranch; 
    std::getline(currentBranchFile, currentBranch); 
    currentBranchFile.close(); 

    ofstream headWrite(".minigit/HEAD"); 
    headWrite << newCommitId; 
    headWrite.close(); 

    ofstream branchWrite(".minigit/branches/" + currentBranch); 
    branchWrite << newCommitId; 
    branchWrite.close(); 
}
