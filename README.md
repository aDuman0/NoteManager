# 📝NoteManager  
**NoteManager** is a lightweight, pointer-based C++ notepad application designed for hands-on memory management and modular file operations. It supports note creation, editing, deletion, timestamped saving, and keyword-based search—all while keeping the code low-level and educational.

# 🔧 Features  
- **Pointer-based note storage**
  Notes are stored using raw char* pointers, emphasizing manual memory management and C-style string handling.

- **Add, list, and update notes**
  Users can create new notes, view existing ones, and update content by index.

- **Save notes to disk with timestamp**
  Each note is saved to a .txt file with an automatic [dd-mm-yyyy HH-MM] timestamp prepended to the content.

- **File conflict resolution**
  When saving to a file that already exists, users are prompted to:
  -  Overwrite – replace the existing file
  -  Rename – choose a new filename
  -  Cancel – abort the save

- **Delete notes**
  Notes can be removed by selecting their index from the list.

- **Save all notes individually**
  Notes are saved as separate files using a base name with auto-incremented suffixes (e.g., note1.txt, note2.txt, ...).

- **Load notes from file**
  Notes can be loaded from .txt files. Timestamp lines are automatically skipped during import. If the user omits the .txt extension, it is appended automatically.

- **Keyword-based search**
  Users can search for specific words across all notes. Matching notes are displayed with their index and content.

# 🚀Getting Started  
Compile with a C++17-compatible compiler. No external dependencies required.
```
g++ -std=c++17 NoteManager.cpp -o NoteManager
./NoteManager
```

# 📁Folder Structure  
All saved notes are stored in a `Notes/` directory, which is automatically created if it doesn't exist.

# 🧠Why Use This Project?  
This project is ideal for:

- Practicing pointer manipulation and manual memory management

- Understanding file I/O in C++

- Building modular systems with reusable components

- Exploring low-level string handling and safety practices (`strcpy_s`, `localtime_s`, etc.)
