#include <bits/stdc++.h>
#include <unordered_map>
#include <sstream>
#include <stack>
#include <fstream>

using namespace std;

bool once = 0;
bool twice = 0;


class File {
public:
    string name;
    string content;

    File(string n) : name(n) {}
};


class Directory {
public:
    string name;
    unordered_map<string, File*> files;
    unordered_map<string, Directory*> dirs;

    Directory(string n) : name(n) {}
};


class FileSystem {

private:
    Directory root;
    Directory* currentDir;
    stack<Directory*> prevDirs;

public:

    FileSystem() : root("/") {
        currentDir = &root;
    }

/***** mkdir to create new directories *****/

void mkdir(string path) {
    stringstream ss(path);
    string dir;
    Directory* tempDir = currentDir;

    bool pathExists = true;

    while (getline(ss, dir, '/')) {
        if (dir != "") {
            if (tempDir->dirs.find(dir) == tempDir->dirs.end()) {
                pathExists = false;
                tempDir->dirs[dir] = new Directory(dir);
                
            }
            tempDir = tempDir->dirs[dir];
        }
    }

    if (pathExists) {
        cout << "Path already exists: " << path << endl;
    }
  else {
          cout << "Directory created: " << path << endl;
      }

    currentDir = &root;
}

/***** implement cd to change the current directory *****/

void cd(string path) {
  
    if (path == "/") {
        currentDir = &root;
        while (!prevDirs.empty()) {
            prevDirs.pop();
        }
        cout << "Successfully changed directory to root." << endl;
        return;
    }

    stringstream ss(path);
    string dir;
    Directory* tempDir = currentDir;
    bool pathExists = true;

    while (getline(ss, dir, '/')) {
      
        if (dir == "..") {
            if (!prevDirs.empty()) {
                tempDir = prevDirs.top();
                prevDirs.pop();
                cout << "Successfully changed to the parent directory." << endl;
            } else {
                cout << "Already at the root directory." << endl;
            }
        } else if (dir == "~") {
            tempDir = &root;
            while (!prevDirs.empty()) {
                prevDirs.pop();
            }
            cout << "Successfully changed directory to root." << endl;
        } else {
            if (dir != "") {
                if (dir == "/") {
                    tempDir = &root;
                    while (!prevDirs.empty()) {
                        prevDirs.pop();
                    }
                    cout << "Successfully changed directory to root." << endl;
                } else if (tempDir->dirs.find(dir) != tempDir->dirs.end()) {
                    prevDirs.push(tempDir);
                    tempDir = tempDir->dirs[dir];
                } else {
                   Directory* temp = currentDir;
                    if(once==false){
                    once = true;  
                    currentDir = &root;
                    cd(path);
                    once = false;
                    if(twice == false) currentDir = temp; 
                      cout<<currentDir->name<<endl;
                    return;
                    }
                    pathExists = false;
                    cout << "Directory doesn't exist: " << dir << endl;
                    break;
                }
            }
        }
    }

    if (pathExists) {
        twice = true;
        currentDir = tempDir;
        if (path == "~") {
            cout << "Successfully changed directory to root." << endl;
        } else if (path != "..") {
            cout << "Successfully changed directory to: " << path << endl;
        }
    }
}

/***** implement ls to list the contents of the current directory *****/

void ls() {
    cout << "Contents of directory '" << currentDir->name << "':" << endl;
    cout << "Directories: ";
    for (auto& dir : currentDir->dirs) {
        cout << dir.first << " ";
    }
    cout << endl;

    cout << "Files: ";
    for (auto& file : currentDir->files) {
        cout << file.first << " ";
    }
    cout << endl;
}

/***** implement touch to create a new file *****/

void touch(string path) {
    stringstream ss(path);
    string dir;
    Directory* tempDir = &root;

    while (getline(ss, dir, '/')) {
        if (!dir.empty()) {
            if (tempDir->dirs.find(dir) != tempDir->dirs.end()) {
                tempDir = tempDir->dirs[dir];
            } else if (tempDir->files.find(dir) != tempDir->files.end()) {
                cout << "File already exists: " << dir << endl;
                return;
            } else {
                tempDir->files[dir] = new File(dir);
                cout << "File created: " << dir << endl;
                break;
            }
        }
    }
}

/***** implement echo to add content in a file  *****/

void echo(string text, string filename) {
    stringstream ss(filename);
    string dirPath, fName;
    Directory* tempDir = &root;

    while (getline(ss, dirPath, '/')) {
        if (!dirPath.empty()) {
            if (tempDir->dirs.find(dirPath) != tempDir->dirs.end()) {
               tempDir = tempDir->dirs[dirPath];
              
             } else {
              if(tempDir->files.find(dirPath) != tempDir->files.end()) break;

              
                cout << "Directory path not found: " << filename << endl;
                return;
            }
        }
    }

    fName = dirPath; // Store the file name

    // Check if the file exists within the current directory
    auto it = tempDir->files.find(fName);
    if (it != tempDir->files.end()) {
        it->second->content += text;
        cout << "Text added to file '" << fName << "' successfully." << endl;
    } else {
        File* newFile = new File(fName);
        newFile->content = text;
        tempDir->files[fName] = newFile;
        cout << "File created: " << fName << " with content." << endl;
    }
}

/***** implement cat to display the content of a file *****/

void cat(string filename) {
    stringstream ss(filename);
    string dirPath, fName;
    Directory* tempDir = &root;

    // Extract the file name and navigate the directory path
    while (getline(ss, dirPath, '/')) {
        if (!dirPath.empty()) {
            if (tempDir->dirs.find(dirPath) != tempDir->dirs.end()) {
                tempDir = tempDir->dirs[dirPath];
            } else {
                 if(tempDir->files.find(dirPath) != tempDir->files.end()) break;
                cout << "Error: Directory path not found: " << filename << endl;
                return;
            }
        }
    }

    // The last token obtained will be the file name
    fName = dirPath;

    // Check if the file exists within the current directory
    auto it = tempDir->files.find(fName);
    if (it != tempDir->files.end()) {
        cout << "Contents of file '" << fName << "':" << endl;
        cout << it->second->content << endl;
    } else {
        cout << "Error: File does not exist: " << fName << endl;
    }
}

/***** implement mv to move a file or directory *****/

void mv(string sourcePath, string destinationPath) {
    // Check if the source and destination are the same
  
   if (sourcePath == destinationPath) {
        cout << "Source and destination paths are the same." << endl;
        return;
    }

    stringstream ssSource(sourcePath);
    stringstream ssDest(destinationPath);
    string sourceDir, destDir;

    Directory* sourceParent = &root;
    Directory* destParent = &root;

    // Extract the source file/directory name and navigate to the source's parent directory
    while (getline(ssSource, sourceDir, '/')) {
        if (!sourceDir.empty()) {
            if (sourceParent->dirs.find(sourceDir) != sourceParent->dirs.end()) {
                sourceParent = sourceParent->dirs[sourceDir];
            } else if (sourceParent->files.find(sourceDir) != sourceParent->files.end()) {
                break; // Found the source file
            } else {
                cout << "Source path does not exist: " << sourcePath << endl;
                return;
            }
        }
    }

    // Navigate to the destination's parent directory
    while (getline(ssDest, destDir, '/')) {
        if (!destDir.empty()) {
            if (destParent->dirs.find(destDir) != destParent->dirs.end()) {
                destParent = destParent->dirs[destDir];
            } else {
                cout << "Destination path does not exist: " << destinationPath << endl;
                return;
            }
        }
    }

    // Check if the source exists in the source parent directory
    if (sourceParent->files.find(sourceDir) != sourceParent->files.end()) {
        // Check if the destination already contains the source file
        if (destParent->files.find(sourceDir) != destParent->files.end()) {
            cout << "Destination already contains a file with the same name." << endl;
            return;
        }

        // Move the source file to the destination
        destParent->files[sourceDir] = sourceParent->files[sourceDir]; 
        sourceParent->files.erase(sourceDir);
        cout << "Moved successfully." << endl;
      
    } else {
        cout << "Source does not exist: " << sourcePath << endl;
    }

}
/***** implement cp to copy source to destination *****/

void cp(string sourcePath, string destinationPath) {
    stringstream ssSource(sourcePath);
    stringstream ssDest(destinationPath);
    string sourceDir, destDir;

    Directory* sourceParent = &root;
    Directory* destParent = &root;

    // Extract the source file/directory name and navigate to the source's parent directory
    while (getline(ssSource, sourceDir, '/')) {
        if (!sourceDir.empty()) {
            if (sourceParent->dirs.find(sourceDir) != sourceParent->dirs.end()) {
                sourceParent = sourceParent->dirs[sourceDir];
            } else if (sourceParent->files.find(sourceDir) != sourceParent->files.end()) {
                break; // Found the source file
            } else {
                cout << "Source path does not exist: " << sourcePath << endl;
                return;
            }
        }
    }

    // Navigate to the destination's parent directory
    while (getline(ssDest, destDir, '/')) {
        if (!destDir.empty()) {
            if (destParent->dirs.find(destDir) != destParent->dirs.end()) {
                destParent = destParent->dirs[destDir];
            } else {
                cout << "Destination path does not exist: " << destinationPath << endl;
                return;
            }
        }
    }

    // Check if the source exists in the source parent directory
    if (sourceParent->files.find(sourceDir) != sourceParent->files.end()) {
        // Check if the destination already contains the source file
        if (destParent->files.find(sourceDir) != destParent->files.end()) {
            cout << "Destination already contains a file with the same name." << endl;
            return;
        }

        // Copy the source file to the destination
        File* newFile = new File(sourceDir);
        newFile->content = sourceParent->files[sourceDir]->content;
        destParent->files[sourceDir] = newFile;
        cout << "Copied successfully." << endl;
    } else {
        cout << "Source does not exist: " << sourcePath << endl;
    }
}

/***** implemetn rm to remove a file or directory *****/

void rm(string path) {
    stringstream ss(path);
    string dir;
    Directory* tempDir = currentDir;
    bool pathExists = true;

    while (getline(ss, dir, '/')) {
        if (!dir.empty()) {
            if (dir == "/") {
                tempDir = &root;
            } else if (dir == "~") {
                tempDir = &root;
                while (!prevDirs.empty()) {
                    prevDirs.pop();
                }
            } else if (dir == "..") {
                if (!prevDirs.empty()) {
                    tempDir = prevDirs.top();
                    prevDirs.pop();
                } else {
                    cout << "Already at the root directory." << endl;
                    return;
                }
            } else {
               if (tempDir->dirs.find(dir) != tempDir->dirs.end()) {
                    prevDirs.push(tempDir);
                    tempDir = tempDir->dirs[dir];
                } else if (tempDir->files.find(dir) != tempDir->files.end()) {
                    delete tempDir->files[dir];
                    tempDir->files.erase(dir);
                    cout << "File removed: " << dir << endl;
                    return;
                } else {
                    pathExists = false;
                    cout << "Path doesn't exist: " << path << endl;
                    return;
                }
            }
        }
    }

    if (pathExists) {
        cout << "Directory removed: " << path << endl;
    }
}

/***** implement grep to find a pattern in a file *****/

void grep(const string& pattern, const string& filename) {
    Directory* tempDir = &root;
    string dirPath, fName;

    stringstream ss(filename);
    while (getline(ss, dirPath, '/')) {
        if (!dirPath.empty()) {
            if (tempDir->dirs.find(dirPath) != tempDir->dirs.end()) {
                tempDir = tempDir->dirs[dirPath];
            } else if (tempDir->files.find(dirPath) != tempDir->files.end()) {
                break; // Found the file
            } else {
                cout << "Error: File does not exist: " << filename << endl;
                return;
            }
        }
    }

    fName = dirPath;

    auto it = tempDir->files.find(fName);
    if (it != tempDir->files.end()) {
        string content = it->second->content;
        stringstream contentStream(content);
        string line;
        int lineNum = 1;
        bool patternFound = false;

        while (getline(contentStream, line, '\n')) {
            size_t found = line.find(pattern);
            if (found != string::npos) {
                size_t pos = 0;
                while (found != string::npos) {
                    cout << "Pattern found in " << filename << " at line " << lineNum << ", position " << found << ": " << line << endl;
                    pos = found + 1;
                    found = line.find(pattern, pos);
                    patternFound = true;
                }
            }
            lineNum++;
        }

        if (!patternFound) {
            cout << "Pattern '" << pattern << "' not found in " << filename << endl;
        }
    } else {
        cout << "Error: File does not exist: " << filename << endl;
    }
}



};

int main() {
  FileSystem fs;
 
  
  cout << "Enter a command (mkdir, cd, ls, touch, echo, cat, mv, cp, rm, grep, exit): "<<endl;
  cout<<endl;
  while (true) {
      string command;
      cin >> command;
      
      if (command != "mkdir" && command != "cd" && 
          command != "ls" && command != "touch" &&
          command != "echo" && command != "cat" &&
          command != "mv" && command != "cp" &&
          command != "rm" && command != "grep" &&
          command != "exit") {
          cout << "Please enter a valid command." << endl;
          cout<<endl;
          continue;
      }

      if (command == "mkdir") {
             cout<<"Enter a new directory :"<<endl;
             string path;
             cin>>path;
             fs.mkdir(path);
             cout<<endl;
             continue;
     
      } else if (command == "cd") {
               cout<<"Enter directory :"<<endl;
               string path;
               cin>>path;
               fs.cd(path);
               cout<<endl;
              continue;
      } else if (command == "ls") {
          fs.ls();
          cout<<endl;
          continue;
      } else if (command == "touch") {
           cout<<"Enter file name or directory containing file name:"<<endl;
           string path;
           cin>>path;
           fs.touch(path);
           cout<<endl;
           continue;
      } else if (command == "echo") {

        string content;
        cout << "Enter content:" << endl;
        getline(cin >> ws, content); 
        string path;

        cin.ignore(); 
        cout << "Enter file name or directory containing file name:" << endl;
        getline(cin, path);
        fs.echo(content, path);
        cout << endl;
        continue;
    } else if (command == "cat") {
            cout<<"Enter file name :"<<endl;
            string path;
            cin>>path;
            fs.cat(path);
            cout<<endl;
            continue;
    } else if (command == "mv") {
            cout<<"Enter source path and destination path :"<<endl;
            string sourcePath, destinationPath;
            cin>>sourcePath>>destinationPath;
            fs.mv(sourcePath, destinationPath);
            cout<<endl;
            continue;
    } else if (command == "cp") {
            cout<<"Enter source path and destination path :"<<endl;
            string sourcePath, destinationPath;
            cin>>sourcePath>>destinationPath;
            fs.cp(sourcePath, destinationPath);
            cout<<endl;
            continue;
    } else if (command == "rm") {
            cout<<"Enter path :"<<endl;
            string path;
            cin>>path;
            fs.rm(path);
            cout<<endl;
            continue;
    } else if (command == "grep") {
            cout<<"Enter pattern"<<endl;
            string pattern;
            getline(cin>>ws, pattern);
            cin.ignore();
            cout<<"Enter file name :"<<endl;
            string filename;
            cin>>filename;
            fs.grep(pattern, filename);
            cout<<endl;
            continue;
    } else if (command == "exit") {
            cout<<endl;
            break;
    } else {
            cout << "Invalid command. Please try again." << endl;
    }
  }

  return 0;
  
}
