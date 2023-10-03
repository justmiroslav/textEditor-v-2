#include <iostream>
#include <fstream>
#include <string>
#include <stack>
using namespace std;

class TextEditor {
public:
    string getText() {
        return text;
    }

    void setText(const string& newText) {
        text = newText;
    }

    stack<string>& getUndoStack() {
        return undoStack;
    }

    stack<string>& getRedoStack() {
        return redoStack;
    }

    void clearFileText() {
        fileText.clear();
    }

    void appendFileText(const string &newText) {
        fileText += newText;
    }

    void appendText(const string &newText) {
        undoStack.push(text);
        text += newText;
    }

    void newLine() {
        undoStack.push(text);
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

    bool performTextAction(int line, int index, const string &newText, bool replaceMode, int numOfSymbols = 0) {
        if (line < 0 || line >= text.length()) {
            cout << "Error: Invalid line" << endl;
            return false;
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
            return false;
        }

        if (numOfSymbols > 0 && numOfSymbols <= lineEnd - (i + index)) {
            text.erase(i + index, numOfSymbols);
        }

        if (!newText.empty()) {
            if (replaceMode) {
                text.erase(i + index, newText.length());
            }
            text.insert(i + index, newText);
        }

        return true;
    }

    void insertText(int line, int index, const string &newText, bool replaceMode) {
        undoStack.push(text);
        performTextAction(line, index, newText, replaceMode);
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

    void deleteText(int line, int index, int numOfSymbols) {
        undoStack.push(text);
        performTextAction(line, index, "", false, numOfSymbols);
    }

private:
    string text;
    string fileText;
    stack<string> undoStack;
    stack<string> redoStack;
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

class UndoRedoManager {
public:
    void undo(TextEditor &textEditor) {
        if (!textEditor.getUndoStack().empty()) {
            if (textEditor.getUndoStack().size() > 3) {
                for (int i = 0; i < 3; i++) {
                    textEditor.getRedoStack().push(textEditor.getText());
                    textEditor.setText(textEditor.getUndoStack().top());
                    textEditor.getUndoStack().pop();
                }
                cout << "Undo successful" << endl;
            } else {
                while (!textEditor.getUndoStack().empty()) {
                    textEditor.getRedoStack().push(textEditor.getText());
                    textEditor.setText(textEditor.getUndoStack().top());
                    textEditor.getUndoStack().pop();
                }
                cout << "Undo successful for all commands" << endl;
            }
        } else {
            cout << "Nothing to undo" << endl;
        }
    }

    void redo(TextEditor &textEditor) {
        if (!textEditor.getRedoStack().empty()) {
            while (!textEditor.getRedoStack().empty()) {
                textEditor.setText(textEditor.getRedoStack().top());
                textEditor.getRedoStack().pop();
            }
            cout << "Redo successful" << endl;
        } else {
            cout << "Nothing to redo" << endl;
        }
    }
};

int main() {
    TextEditor textEditor;
    UndoRedoManager undoRedoManager;
    FileManager fileManager;

    int command, line, index, printChoice, numOfSymbols;
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
        } else if (command == 8) {
            cout << "Enter the line, index and number of symbols to delete:" << endl;
            cin >> line >> index >> numOfSymbols;
            textEditor.deleteText(line, index, numOfSymbols);
        } else if (command == 9) {
            undoRedoManager.undo(textEditor);
        } else if (command == 10) {
            undoRedoManager.redo(textEditor);
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