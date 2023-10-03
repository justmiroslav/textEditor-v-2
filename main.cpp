#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TextEditor {
public:
    string getText() {
        return text;
    }

    void clearFileText() {
        fileText.clear();
    }

    void appendFileText(const string &newText) {
        fileText += newText;
    }

    void appendText(const string &newText) {
        text += newText;
    }

    void newLine() {
        text.push_back('\n');
    }

    void printText(int choice) {
        if (choice == 1) {
            if (!text.empty()) {
                cout << "Current text:" << endl;
                cout << text << endl;
            } else {
                cout << "Current text is empty" << endl;
            }
        } else if (choice == 2) {
            if (!fileText.empty()) {
                cout << "Text loaded from the file:" << endl;
                cout << fileText << endl;
            } else {
                cout << "Text loaded from the file is empty" << endl;
            }
        } else {
            cout << "Invalid choice for printing text" << endl;
        }
    }

    void insertText(int line, int index, const string &newText, bool replaceMode) {
        if (line < 0 || line >= text.length()) {
            cout << "Error: Invalid line" << endl;
        }
        int currentLine = 0;
        int i = 0;
        while (currentLine < line) {
            if (text[i] == '\n') {
                currentLine++;
            }
            i++;
        }
        int lineEnd = i;
        while (text[lineEnd] != '\n' && lineEnd < text.length()) {
            lineEnd++;
        }
        if (index < 0 || index > lineEnd) {
            cout << "Error: Invalid index" << endl;
        }
        if (replaceMode) {
            text.erase(i + index, newText.length());
        }
        text.insert(i + index, newText);
    }

    void searchText(const string &searchText) {
        int currentLine = 0;
        int idx = 0;
        bool correctWord = false;
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == '\n') {
                currentLine++;
                idx = 0;
            } else {
                bool found = true;
                for (int j = 0; j < searchText.length(); j++) {
                    if (text[i + j] != searchText[j]) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    cout << "Text is present in this position: " << currentLine << " " << idx << endl;
                    correctWord = true;
                }
            }
            if (text[i] != '\n') {
                idx++;
            }
        }
        if (!correctWord) {
            cout << "Text not found" << endl;
        }
    }

private:
    string text;
    string fileText;
};

class FileManager {
public:
    void saveText(const string &filename, const string &text) {
        ofstream file(filename);
        if (file.is_open()) {
            file << text;
            file.close();
            cout << "Text has been saved successfully" << endl;
        } else {
            cout << "Error opening file for saving" << endl;
        }
    }

    void loadText(const string &filename, TextEditor &textEditor) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            textEditor.clearFileText();
            bool firstLine = true;
            while (getline(file, line)) {
                if (!firstLine) {
                    textEditor.appendFileText("\n");
                }
                textEditor.appendFileText(line);
                firstLine = false;
            }
            file.close();
            cout << "Text has been loaded successfully" << endl;
        } else {
            cout << "Error opening file for loading" << endl;
        }
    }
};



int main() {
    TextEditor textEditor;
    FileManager fileManager;

    int command, line, index, printChoice;
    string input, filename;
    while (true) {
        cout << "Enter the command:" << endl;
        cin >> command;
        if (command == 1) {
            cout << "Enter text to append:" << endl;
            cin.ignore();
            getline(cin, input);
            textEditor.appendText(input);
        } else if (command == 2) {
            textEditor.newLine();
            cout << "New line is started" << endl;
        } else if (command == 3) {
            cout << "Enter the file name for saving:" << endl;
            cin.ignore();
            getline(cin, filename);
            fileManager.saveText(filename, textEditor.getText());
        } else if (command == 4) {
            cout << "Enter the file name for loading:" << endl;
            cin.ignore();
            getline(cin, filename);
            fileManager.loadText(filename, textEditor);
        } else if (command == 5) {
            cout << "Enter what to print:" << endl;
            cin >> printChoice;
            textEditor.printText(printChoice);
        } else if (command == 6) {
            cout << "Enter the line and index:" << endl;
            cin >> line >> index;
            cin.ignore();
            cout << "Enter text to insert:" << endl;
            getline(cin, input);
            textEditor.insertText(line, index, input, false);
        } else if (command == 7) {
            cout << "Enter text to search:" << endl;
            cin.ignore();
            getline(cin, input);
            textEditor.searchText(input);
        } else if (command == 14) {
            cout << "Enter the line and index:" << endl;
            cin >> line >> index;
            cin.ignore();
            cout << "Enter text to insert:" << endl;
            getline(cin, input);
            textEditor.insertText(line, index, input, true);
        } else if (command == 15) {
            cout << "The end" << endl;
            break;
        } else {
            cout << "Incorrect command" << endl;
        }
    }
    return 0;
}