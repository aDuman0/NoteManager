#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <limits>
#include <string>
#include <corecrt_startup.h>
#include <locale>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::time(nullptr);
    std::tm tmStruct;
    errno_t err = localtime_s(&tmStruct, &t);
    
    if (err != 0)
    {
        std::cerr << "localtime_s failed!\n";
        return "00-00-0000_00-00-00";
    }

    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%d.%m.%Y %H:%M");
    return oss.str();
}

void updateNote(char*& note, const char* newContent, size_t newSize)
{
    delete[] note;
    note = new char[newSize];

    // Safe Copy
    errno_t result = strcpy_s(note, newSize, newContent);
    if (result != 0)
    {
        std::cerr << "Note update failed!\n";
    }
}

void listNotes(const std::vector<char*>& notes)
{
    std::cout << "\n--- Notes ---\n";
    for (size_t i = 0; i < notes.size(); i++)
    {
        std::cout << i + 1 << ". " << notes[i] << "\n";
    }
    std::cout << "-------------\n";
}

int getChoice()
{
    int choice;
    while (true)
    {
        std::cout << "Choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice! Please enter a number.\n";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

class FileConflictResolver
{
public:
    static bool resolve(std::string& fullPath)
    {
        if (!std::filesystem::exists(fullPath))
            return true;

        std::cout << "File '" << fullPath << "' already exists!\n";
        std::cout << "1. Overwrite\n2. Rename\n3. Cancel\n";

        int decision;
        while (true)
        {
            decision = getChoice();
            if (decision == 1)
            {
                return true;
            }
            else if (decision == 2)
            {
                std::string newName;
                std::cout << "Enter new filename: ";
                std::getline(std::cin, newName);
                if (newName.size() < 4 || newName.substr(newName.size() - 4) != ".txt")
                    newName += ".txt";
                fullPath = "Notes/" + newName;
                return true;
            }
            else if (decision == 3)
            {
                std::cout << "Save cancelled.\n";
            }
            else
            {
                std::cout << "Invalid choice! Please enter a number.\n";
            }
        }
    }
};

void deleteNote(std::vector<char*>& notes)
{
    if (notes.empty())
    {
        std::cout << "No notes to delete.\n";
        return;
    }

    listNotes(notes);
    std::cout << "Enter the number of the note to delete: ";
    size_t index;
    std::cin >> index;
    std::cin.ignore();

    if (index == 0 || index > notes.size())
    {
        std::cout << "invalid index!\n";
        return;
    }

    delete[] notes[index - 1];
    notes.erase(notes.begin() + index - 1);
    std::cout << "Note detected.\n";
}

void ensureNotesFolder()
{
    std::filesystem::create_directory("Notes");
}

void saveNoteToFile(const char* note, const std::string& rawFilename)
{
    ensureNotesFolder();

    std::string filename = rawFilename;
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt")
    {
        filename += ".txt";
    }
    
    std::string fullPath = "Notes/" + filename;

    if (!FileConflictResolver::resolve(fullPath))
    {
        return;
    }
    
    std::ofstream file(fullPath);
    if (!file)
    {
        std::cerr << "The file could not be opened!\n";
        return;
    }

    file << "[" << getTimestamp() << "]\n";
    file << note;
    file.close();
    
    std::cout << "Note " << filename << "saved to " << fullPath << "\n";
}

void saveAllNotes(const std::vector<char*>& notes, const std::string& baseName)
{
    ensureNotesFolder();

    for (size_t i = 0; i < notes.size(); i++)
    {
        std::string filename = baseName + std::to_string(i + 1) + ".txt";
        std::string fullPath = "Notes/" + filename;

        if (!FileConflictResolver::resolve(fullPath))
            continue;

        std::ofstream file(fullPath);
        if (!file)
        {
            std::cerr << "The file could not be opened!\n";
            continue;
        }

        file << "[" << getTimestamp() << "]\n";
        file << notes[i];
        file.close();

        std::cout << "Note " << i + 1 << " saved to " << fullPath << "\n";
    }
}

void loadNotesFromFile(std::vector<char*>& notes, std::string rawFilename)
{
    std::string filename = rawFilename;

    std::ifstream file("Notes/" + filename);
    if (!file)
    {
        std::cerr << "The file could not be opened!\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        char* loadedNote = new char[line.size() + 1];
        strcpy_s(loadedNote, line.size() + 1, line.c_str());
        notes.push_back(loadedNote);
    }

    file.close();
    std::cout << "Notes loaded from file.\n";
}

int main(int argc, char* argv[])
{
    std::vector<char*> notes;

    while (true)
    {
        std::cout << "\nNotepad\n";
        std::cout << "1. Add new note\n";
        std::cout << "2. List notes\n";
        std::cout << "3. Update notes\n";
        std::cout << "4. Save note to disk\n";
        std::cout << "5. Delete note\n";
        std::cout << "6. Save all notes\n";
        std::cout << "7. Load notes from file\n";
        std::cout << "8. Exit\n";
        int choice = getChoice();
        
        if (choice == 1)
        {
            const size_t size = 256;
            char* newNote = new char[size];
            std::cout << "Enter your note: ";
            std::cin.getline(newNote, size);
            notes.push_back(newNote);
        }
        else if (choice == 2)
        {
            listNotes(notes);
        }
        else if (choice == 3)
        {
            if (notes.empty())
            {
                std::cout << "There is no notes yet!\n";
                continue;
            }

            listNotes(notes);
            std::cout << "Enter the number of the note to be updated: ";
            size_t index;
            std::cin >> index;
            std::cin.ignore();

            if (index == 0 || index > notes.size())
            {
                std::cout << "Invalid index!\n";
                continue;
            }

            const size_t size = 256;
            char newContent[size];
            std::cout << "Enter new content: ";
            std::cin.getline(newContent, size);

            updateNote(notes[index - 1], newContent, strlen(newContent) + 1);
        }
        else if (choice == 4)
        {
           if (notes.empty())
           {
               std::cout << "There is no notes yet!\n";
               continue;
           }

            listNotes(notes);
            std::cout << "Enter the number of the note to be saved: ";
            size_t index;
            std::cin >> index;
            std::cin.ignore();

            if (index == 0 || index > notes.size())
            {
                std::cout << "Invalid index!\n";
                continue;
            }

            std::string filename;
            std::cout << "Enter the filename to save the notes: ";
            std::getline(std::cin, filename);

            if (filename.size() < 4 || filename.substr(filename.size() -4) != ".txt")
            {
                filename += ".txt";
            }

            saveNoteToFile(notes[index - 1], filename);
        }
        else if (choice == 5)
        {
            deleteNote(notes);
        }
        else if (choice == 6)
        {
            std::string filename;
            std::cout << "Enter filename to save all notes: ";
            std::getline(std::cin, filename);
            saveAllNotes(notes, filename);
        }
        else if (choice == 7)
        {
            std::string filename;
            std::cout << "Enter filename to load notes from: ";
            std::getline(std::cin, filename);
            loadNotesFromFile(notes, filename);
        }
        else if (choice == 8)
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice!\n";
        }
    }
    
    // Memory clearing
    for (char* n : notes)
    {
        delete[] n;
    }
    
    return 0;
}
