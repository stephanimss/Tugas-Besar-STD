#include "text_editor.hpp"
#include <iostream>
#include <string>
using namespace std;

// Menampilkan menu editor
void menu() {
    cout << "----------------------------" << endl;
    cout << "        TEXT EDITOR         " << endl;
    cout << "----------------------------" << endl;
    cout << "1. Menambah Teks" << endl;
    cout << "2. Menghapus Teks" << endl;
    cout << "3. Menampilkan Teks" << endl;
    cout << "4. Undo Teks" << endl;
    cout << "5. Redo Teks" << endl;
    cout << "6. Mencari Kata" << endl;
    cout << "7. Mengganti Kata" << endl;
    cout << "8. Menghitung Teks" << endl;
    cout << "9. Keluar" << endl;
}

// Fungsi untuk membuat text editor baru
void createTextEditor(TextEditor &editor) {
    editor.head = nullptr;
    editor.tail = nullptr;
    editor.lineCount = 0;
    editor.charCount = 0;
    editor.undoStack.top = -1;
    editor.redoStack.top = -1;
}

// Fungsi untuk membuat elemen baru dalam text editor
adr_List createElementEditor(infotype lineData) {
    adr_List newElement = new Elm_List;
    newElement->line = lineData;
    newElement->prev = nullptr;
    newElement->next = nullptr;
    return newElement;
}

// Fungsi untuk menyisipkan teks di posisi tertentu
void insertText(TextEditor &editor, int posisi, infotype text) {
    adr_List newElement = createElementEditor(text);
    if (posisi < 1 || posisi > editor.lineCount + 1) {
        cout << "Posisi tidak valid!" << endl;
        return;
    }

    // Menyisipkan di posisi pertama
    if (posisi == 1) {
        if (editor.head == nullptr) {
            editor.head = newElement;
            editor.tail = newElement;
        } else {
            newElement->next = editor.head;
            editor.head->prev = newElement;
            editor.head = newElement;
        }
    }
    // Menyisipkan di posisi terakhir
    else if (posisi == editor.lineCount + 1) {
        if (editor.tail == nullptr) {
            editor.head = newElement;
            editor.tail = newElement;
        } else {
            editor.tail->next = newElement;
            newElement->prev = editor.tail;
            editor.tail = newElement;
        }
    }
    // Menyisipkan di posisi tengah
    else {
        adr_List current = editor.head;
        for (int i = 1; i < posisi - 1; ++i) {
            current = current->next;
        }
        newElement->next = current->next;
        if (current->next != nullptr) {
            current->next->prev = newElement;
        }
        current->next = newElement;
        newElement->prev = current;
    }

    editor.lineCount++;
    editor.charCount += text.length;

    // Menambahkan operasi ke undo stack
    editor.undoStack.top++;
    editor.undoStack.operation[editor.undoStack.top] = "insert";
    editor.undoStack.data[editor.undoStack.top] = text;
    editor.undoStack.position[editor.undoStack.top] = posisi;
}

// Fungsi untuk menghapus karakter atau baris tertentu
void deleteText(TextEditor &editor, int posisi, bool isLine) {
    if (posisi < 1 || posisi > editor.lineCount) {
        cout << "Posisi tidak valid!" << endl;
        return;
    }

    adr_List current = editor.head;
    for (int i = 1; i < posisi; ++i) {
        current = current->next;
    }

    // Menghapus baris atau teks
    if (isLine) {
        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else {
            editor.head = current->next;
        }

        if (current->next != nullptr) {
            current->next->prev = current->prev;
        } else {
            editor.tail = current->prev;
        }

        // Menyimpan data untuk undo
        editor.undoStack.top++;
        editor.undoStack.operation[editor.undoStack.top] = "delete";
        editor.undoStack.data[editor.undoStack.top] = current->line;
        editor.undoStack.position[editor.undoStack.top] = posisi;

        editor.lineCount--;
        editor.charCount -= current->line.length;
        delete current;
    }
}

// Fungsi untuk menampilkan seluruh teks
void displayText(TextEditor &editor) {
    adr_List current = editor.head;
    while (current != nullptr) {
        cout << current->line.text << " ";  // Menampilkan teks dalam satu baris dengan spasi sebagai pemisah
        current = current->next;
    }
    cout << endl;  // Untuk memastikan hasil ditampilkan pada satu baris
}

// Fungsi untuk melakukan undo
void undo(TextEditor &editor) {
    if (editor.undoStack.top == -1) {
        cout << "Tidak ada operasi yang dapat di-undo!" << endl;
        return;
    }

    // Ambil operasi terakhir dari undo stack
    int posisiTerakhir = editor.undoStack.position[editor.undoStack.top];
    string operasiTerakhir = editor.undoStack.operation[editor.undoStack.top];
    infotype dataTerakhir = editor.undoStack.data[editor.undoStack.top];

    if (operasiTerakhir == "insert") {
        // Melakukan undo untuk insert (menghapus teks yang disisipkan)
        deleteText(editor, posisiTerakhir, true);
    } else if (operasiTerakhir == "delete") {
        // Melakukan undo untuk delete (menambahkan kembali teks yang dihapus)
        insertText(editor, posisiTerakhir, dataTerakhir);
    } else if (operasiTerakhir == "replace") {
        // Melakukan undo untuk replace (mengembalikan kata lama)
        adr_List current = editor.head;
        for (int i = 1; i < posisiTerakhir; ++i) {
            current = current->next;
        }
        // Kembalikan kata lama
        if (current != nullptr) {
            editor.redoStack.top++;
            editor.redoStack.operation[editor.redoStack.top] = "replace";
            editor.redoStack.data[editor.redoStack.top] = current->line; // Simpan kata saat ini ke redo stack
            editor.redoStack.position[editor.redoStack.top] = posisiTerakhir;

            current->line = dataTerakhir; // Ganti kembali ke kata lama
        }
    }

    // Pindahkan ke redo stack
    editor.redoStack.top++;
    editor.redoStack.operation[editor.redoStack.top] = operasiTerakhir;
    editor.redoStack.data[editor.redoStack.top] = dataTerakhir;
    editor.redoStack.position[editor.redoStack.top] = posisiTerakhir;

    // Hapus operasi terakhir dari undo stack
    editor.undoStack.top--;
}

// Fungsi untuk melakukan redo
void redo(TextEditor &editor) {
    if (editor.redoStack.top == -1) {
        cout << "Tidak ada operasi yang dapat di-redo!" << endl;
        return;
    }

    // Ambil operasi terakhir dari redo stack
    int posisiTerakhir = editor.redoStack.position[editor.redoStack.top];
    string operasiTerakhir = editor.redoStack.operation[editor.redoStack.top];
    infotype dataTerakhir = editor.redoStack.data[editor.redoStack.top];

    if (operasiTerakhir == "insert") {
        // Melakukan redo untuk insert (menyisipkan kembali teks)
        insertText(editor, posisiTerakhir, dataTerakhir);
    } else if (operasiTerakhir == "delete") {
        // Melakukan redo untuk delete (menghapus kembali teks)
        deleteText(editor, posisiTerakhir, true);
    } else if (operasiTerakhir == "replace") {
        // Melakukan redo untuk replace (mengganti kata kembali)
        adr_List current = editor.head;
        for (int i = 1; i < posisiTerakhir; ++i) {
            current = current->next;
        }
        // Ganti kata
        if (current != nullptr) {
            editor.undoStack.top++;
            editor.undoStack.operation[editor.undoStack.top] = "replace";
            editor.undoStack.data[editor.undoStack.top] = current->line; // Simpan kata saat ini ke undo stack
            editor.undoStack.position[editor.undoStack.top] = posisiTerakhir;

            current->line = dataTerakhir; // Ganti ke kata dari redo stack
        }
    }

    // Pindahkan ke undo stack
    editor.undoStack.top++;
    editor.undoStack.operation[editor.undoStack.top] = operasiTerakhir;
    editor.undoStack.data[editor.undoStack.top] = dataTerakhir;
    editor.undoStack.position[editor.undoStack.top] = posisiTerakhir;

    // Hapus operasi terakhir dari redo stack
    editor.redoStack.top--;
}

// Fungsi untuk mencari kata
bool searchWord(TextEditor &editor, infotype word) {
    adr_List current = editor.head;
    while (current != nullptr) {
        if (current->line.text == word.text) {
            return true;  // Kata ditemukan
        }
        current = current->next;
    }
    return false;  // Kata tidak ditemukan
}

// Fungsi untuk mengganti kata
void replaceWord(TextEditor &editor, infotype kataLama, infotype kataBaru) {
    adr_List current = editor.head;
    int posisi = 1; // Posisi baris saat ini
    while (current != nullptr) {
        if (current->line.text == kataLama.text) {
            // Simpan operasi ke undo stack
            editor.undoStack.top++;
            editor.undoStack.operation[editor.undoStack.top] = "replace";
            editor.undoStack.data[editor.undoStack.top] = current->line; // Simpan kata lama
            editor.undoStack.position[editor.undoStack.top] = posisi;

            // Ganti kata
            current->line.text = kataBaru.text;
        }
        current = current->next;
        posisi++;
    }
}

// Fungsi untuk menghitung jumlah karakter dan kata
void calculateStats(TextEditor &editor, int &charCount, int &wordCount) {
    charCount = 0;
    wordCount = 0;
    adr_List current = editor.head;
    
    while (current != nullptr) {
        // Menghitung karakter tanpa spasi
        for (int i = 0; i < current->line.length; ++i) {
            if (current->line.text[i] != ' ') {  // Hanya hitung jika bukan spasi
                charCount++;
            }
        }
        // Hitung satu kata per baris
        wordCount++;
        current = current->next;
    }
}
