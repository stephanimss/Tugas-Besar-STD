#include "text_editor.hpp"
#include <iostream>
using namespace std;

int main() {
    TextEditor editor;
    createTextEditor(editor);

    int choice;
    infotype text;
    int posisi;
    int charCount, wordCount;

    while (true) {
        menu();
        cout << "Pilih menu (1-9): ";
        cin >> choice;

        if (choice == 1) { // Menambah Teks/Kata
            cout << "Masukkan teks: ";
            cin >> text.text; // Membaca satu kata
            // Menghitung panjang teks secara manual
            text.length = 0;
            while (text.text[text.length] != '\0') {
                text.length++;
            }
            cout << "Masukkan posisi untuk menyisipkan teks (1 - " << editor.lineCount + 1 << "): ";
            cin >> posisi;
            insertText(editor, posisi, text); // Menyisipkan teks pada posisi yang ditentukan
        }
        else if (choice == 2) { // Menghapus Teks/Kata
            cout << "Masukkan posisi untuk menghapus teks (1 - " << editor.lineCount << "): ";
            cin >> posisi;
            deleteText(editor, posisi, true); // Menghapus satu baris
        }
        else if (choice == 3) { // Menampilkan Teks/Kata
            cout << "Teks dalam editor: " << endl;
            displayText(editor); // Menampilkan seluruh teks dalam editor
        }
        else if (choice == 4) { // Undo
            undo(editor); // Melakukan undo
            cout << "Undo berhasil!" << endl;
        }
        else if (choice == 5) { // Redo
            redo(editor); // Melakukan redo
            cout << "Redo berhasil!" << endl;
        }
        else if (choice == 6) { // Mencari Kata
            cout << "Masukkan kata yang ingin dicari: ";
            cin >> text.text; // Membaca kata yang ingin dicari
            // Menghitung panjang kata
            text.length = 0;
            while (text.text[text.length] != '\0') {
                text.length++;
            }
            if (searchWord(editor, text)) {
                cout << "Kata ditemukan!" << endl;
            } else {
                cout << "Kata tidak ditemukan." << endl;
            }
        }
        else if (choice == 7) { // Mengganti Kata
            cout << "Masukkan kata yang akan diganti: ";
            cin >> text.text; // Membaca kata yang akan diganti
            infotype kataLama = text;

            cout << "Masukkan kata pengganti: ";
            cin >> text.text; // Membaca kata pengganti
            infotype kataBaru = text;

            replaceWord(editor, kataLama, kataBaru); // Mengganti kata yang sesuai
            cout << "Kata berhasil diganti!" << endl;
        }
        else if (choice == 8) { // Menghitung Kata
            calculateStats(editor, charCount, wordCount); // Menghitung jumlah karakter dan kata
            cout << "Jumlah karakter: " << charCount << endl;
            cout << "Jumlah kata: " << wordCount << endl;
        }
        else if (choice == 9) { // Keluar
            cout << "Keluar dari editor." << endl;
            return 0;
        }
        else { // Pilihan tidak valid
            cout << "Pilihan tidak valid!" << endl;
        }
    }
    return 0;
}
