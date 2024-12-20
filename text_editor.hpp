#ifndef TEXT_EDITOR_HPP
#define TEXT_EDITOR_HPP

#include <iostream>
#include <string> 
using namespace std;

// Struktur untuk menyimpan teks dan panjangnya
struct infotype {
    string text;  // Menyimpan teks sebagai string
    int length;   // Panjang teks
};

// Definisi address untuk node dalam struktur data
typedef struct Elm_List *adr_List;

// List untuk Doubly Linked List (DLL)
struct Elm_List {
    infotype line;        // Menyimpan satu baris teks
    adr_List prev;        // Pointer ke node sebelumnya
    adr_List next;        // Pointer ke node berikutnya
};

// Struktur Stack untuk undo dan redo
struct Elm_Stack {
    string operation[100];  // Operasi yang dilakukan (insert, delete, dll.)
    infotype data[100];     // Data terkait operasi
    int position[100];      // Posisi terkait operasi
    int top;                // Menunjukkan indeks elemen teratas
};

// Struktur utama text editor
struct TextEditor {
    adr_List head;        // Pointer ke awal DLL
    adr_List tail;        // Pointer ke akhir DLL
    Elm_Stack undoStack;  // Stack untuk undo
    Elm_Stack redoStack;  // Stack untuk redo
    int lineCount;        // Jumlah total baris
    int charCount;        // Jumlah total karakter
};

// Fungsi untuk menampilkan menu
void menu();

// Fungsi untuk membuat text editor baru (inisialisasi)
void createTextEditor(TextEditor &editor);

// Fungsi untuk membuat elemen baru dalam text editor (seperti node DLL)
adr_List createElementEditor(infotype lineData);

// Fungsi untuk menyisipkan teks di posisi tertentu
void insertText(TextEditor &editor, int position, infotype text);

// Fungsi untuk menghapus karakter atau baris tertentu
void deleteText(TextEditor &editor, int position, bool isLine);

// Fungsi untuk menampilkan seluruh teks
void displayText(TextEditor &editor);

// Fungsi untuk melakukan undo
void undo(TextEditor &editor);

// Fungsi untuk melakukan redo
void redo(TextEditor &editor);

// Fungsi untuk mencari kata
bool searchWord(TextEditor &editor, infotype word);

// Fungsi untuk mengganti kata
void replaceWord(TextEditor &editor, infotype oldWord, infotype newWord);

// Fungsi untuk menghitung jumlah karakter dan kata
void calculateStats(TextEditor &editor, int &charCount, int &wordCount);

#endif
