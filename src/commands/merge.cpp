#include "core/merge.hpp"//to define the functions used in implementing merge
#include <iostream> 
#include <fstream> // for reading an writng to files like ".minigit/commits/"...
#include <set> //for getting the unique file names from each commits 
#include <unordered_map> 
#include <chrono> // tracking the current time, used when creating a commit of the merge
#include <ctime> // changing the created time to human recongnizable form 
#include <string> 
#include <filesystem> 
#include <sstream> // changing number to string, used for chaning commitid's to strings
#include <map> // for creating file->hash key and value pairs 
#include <unordered_set> 
#include<climits>// for INT_MAX
#include<algorithm>
#include<nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std; 
namespace f = std::filesystem; // just an alias to shorten filesystem usage

//we first have to check if the files are present in the first place
// the runtime files like ".minigit/branches", ".minigit/commits", ".minigit/blobs"...
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
    string path = ".minigit/commits/" + commitId + ".json";
    cout << "Opening commit file: " << path << endl;
     ifstream commitContent(path);//enter the file with the specific commitId
    if (!commitContent.is_open()) {
    cerr << "Error: commit file not found." << commitId << endl; 
    return file_hash;
}
     cout << "Trying to open: .minigit/commits/" << commitId <<".json"<< endl;

    json commitJson;
    try {
        commitContent >> commitJson;
    } catch (json::parse_error& e) {
        cerr << "Error: Failed to parse commit JSON: " << e.what() << endl;
        return file_hash;
    }

    if (!commitJson.contains("files") || !commitJson["files"].is_object()) {
        cerr << "Error: commit JSON missing 'files' object." << endl;
        return file_hash;
    }

    for (auto& [filename, blobHash] : commitJson["files"].items()) {
        file_hash[filename] = blobHash.get<string>();
    }
    return file_hash; 
}

//a function to find the lowest common ancestor(LCA) of two commit branches 
//here we have to functions one is a helper function 
void collectAncestors(const string& commitId, map<string, int>& ancestors, int depth = 0){
    if(commitId.empty() || ancestors.count(commitId)) return;// if root is reached or if already visited

    ancestors[commitId] = depth;


    ifstream file(".minigit/commits/" + commitId + ".json");//open the commit to find the parent and traverse deep
    if (!file.is_open()) {
    cerr << "Error: file not found." << " .minigit/commits/" << commitId << ".json" <<endl; 
    return;
    }
    cout << "Trying to open: .minigit/commits/" << commitId << ".json"<< endl;

    json commitJson;
    try {
        file >> commitJson;
    } catch (json::parse_error& e) {
        cerr << "Error parsing commit JSON: " << e.what() << endl;
        return;
    }

    if (commitJson.contains("parents") && commitJson["parents"].is_array()) {
        for (const auto& parent : commitJson["parents"]) {
            if (parent.is_string()) {
                collectAncestors(parent.get<string>(), ancestors, depth + 1);
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

    // read current branch from head
    ifstream head(".minigit/HEAD"); 
    if (!head.is_open()) {
    cerr << "Error: file not found." << endl; 
    return;
}
    string refLine; 
    std::getline(head, refLine); 
    head.close(); 

    if (refLine.rfind("ref: ", 0) != 0) {
        cerr << "Invalid HEAD format.\n";
        return;
    }


    string currentBranchPath = refLine.substr(5);  // e.g., "branches/main"
    string currentBranch = currentBranchPath.substr(currentBranchPath.find_last_of('/') + 1);
    //get the commitId 
    ifstream branchFile(".minigit/" + currentBranchPath);
    if (!branchFile.is_open()) {
        cerr << "Error: Cannot read current branch commit ID.\n";
        return;
    }
     string ourCommitId;
    getline(branchFile, ourCommitId);
    branchFile.close();

    // Theirs (from target branch)
    ifstream targetFile(".minigit/branches/" + targetBranch);
    if (!targetFile.is_open()) {
        cerr << "Error: Target branch '" << targetBranch << "' not found.\n";
        return;
    }
    string theirCommitId;
    getline(targetFile, theirCommitId);
    targetFile.close();

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


            string conflict = "<<<<<<< ours\n" + oursContent + "\n=======\n" + theirsContent + "\n>>>>>>> theirs\n"; 

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

    //now create the json file
    // Prepare parents as a JSON array
    json parents = json::array({ourCommitId, theirCommitId});

    // Convert mergedFiles map<string, string> to json
    json filesJson = json::object();
    for (const auto& [file, blobHash] : mergedFiles) {
        filesJson[file] = blobHash;
    }

    // Build the JSON commit object
    json commitJson;
    commitJson["commit_id"] = newCommitId;
    commitJson["timestamp"] = timestamp;
    commitJson["message"] = "Merge " + targetBranch + " into current branch";
    commitJson["parents"] = parents;
    commitJson["files"] = filesJson;

    // Write JSON commit to file
    ofstream commitFile(".minigit/commits/" + newCommitId + ".json");
    if (!commitFile.is_open()) {
        cerr << "Error: Could not open commit file for writing\n";
        return;
    }
    commitFile << commitJson.dump(4); // pretty-print with 4 spaces indent
    commitFile.close();

    //Final code, making head point to newCommitId
    ofstream(".minigit/" + currentBranchPath) << newCommitId;
    ofstream(".minigit/HEAD") << "ref: " + currentBranchPath;

    cout << "Merged branch '" << targetBranch << "' into '" << currentBranch << "'. New commit: " << newCommitId << "\n";
}
