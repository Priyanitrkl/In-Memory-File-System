#include <iostream>
#include <unordered_map>
#include <sstream>
#include <stack>

using namespace std;

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

    void mkdir(string name) {
        if (currentDir->dirs.find(name) == currentDir->dirs.end()) {
            currentDir->dirs[name] = new Directory(name);
            cout << "Directory created: " << name << endl;
        } else {
            cout << "Directory already exists: " << name << endl;
        }
    }

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

        while (getline(ss, dir, '/')) {
            if (dir == "..") {
                if (!prevDirs.empty()) {
                    currentDir = prevDirs.top();
                    prevDirs.pop();
                    cout << "Successfully changed to the parent directory." << endl;
                }
            } else if (dir == "~") {
                currentDir = &root;
                while (!prevDirs.empty()) {
                    prevDirs.pop();
                }
                cout << "Successfully changed directory to root." << endl;
            } else {
                if (dir != "") {
                    if (dir == "/") {
                        currentDir = &root;
                        while (!prevDirs.empty()) {
                            prevDirs.pop();
                        }
                        cout << "Successfully changed directory to root." << endl;
                    } else if (currentDir->dirs.find(dir) != currentDir->dirs.end()) {
                        prevDirs.push(currentDir);
                        currentDir = currentDir->dirs[dir];
                        cout << "Successfully changed directory to: " << dir << endl;
                    } else {
                        cout << "Directory doesn't exist: " << dir << endl;
                        return;
                    }
                }
            }
        }
    }

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
void touch(string filename) {
    if (currentDir->files.find(filename) == currentDir->files.end()) {
        currentDir->files[filename] = new File(filename);
        cout << "File created: " << filename << endl;
    } else {
        cout << "Error: File already exists: " << filename << endl;
    }
}
};

int main() {
    FileSystem fs;
    fs.mkdir("doc");
   
    fs.mkdir("images");
    fs.cd(".."); // Moves back to the parent directory
   fs.ls();
   cout<<endl;
    fs.cd("/"); // Moves to the root directory
   fs.ls();
   cout<<endl;
    fs.cd("images"); // Changes directory to "dir1"
   fs.ls();
   cout<<endl;
    fs.cd(".."); // Moves back to the parent directory
   fs.ls();
   cout<<endl;
    fs.cd("~"); // Changes directory to root from any level
   fs.ls();

    return 0;
}
