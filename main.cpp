#include <iostream>
#include <string>
#include <algorithm> // Untuk swap
#include <chrono>    // Untuk menghitung waktu (running time)

using namespace std;

// 1. Struktur Data Lagu
struct Lagu {
    string judul;
    string artis;
    int durasi; // dalam detik
};

// Fungsi bantuan untuk menukar data
void tukar(Lagu& a, Lagu& b) {
    Lagu temp = a;
    a = b;
    b = temp;
}

// ---------------------------------------------------------
// FUNGSI PARTISI (Dipakai oleh Rekursif dan Iteratif)
// ---------------------------------------------------------
int partition(Lagu* arr, int low, int high) {
    int pivot = arr[high].durasi; // Pivot: elemen terakhir
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // Bandingkan durasi
        if (arr[j].durasi < pivot) {
            i++;
            tukar(arr[i], arr[j]);
        }
    }
    tukar(arr[i + 1], arr[high]);
    return (i + 1);
}

// ---------------------------------------------------------
// ALGORITMA 1: QUICK SORT REKURSIF
// ---------------------------------------------------------
void quickSortRecursive(Lagu* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSortRecursive(arr, low, pi - 1);
        quickSortRecursive(arr, pi + 1, high);
    }
}

// ---------------------------------------------------------
// ALGORITMA 2: QUICK SORT ITERATIF (Manual Stack)
// ---------------------------------------------------------
void quickSortIterative(Lagu* arr, int n) {
    // Buat array stack manual. Ukuran n cukup aman.
    // Stack ini akan menyimpan index 'low' dan 'high'
    int* stack = new int[n];
    int top = -1;

    // Push nilai awal (0, n-1) ke stack
    stack[++top] = 0;
    stack[++top] = n - 1;

    // Loop selama stack tidak kosong
    while (top >= 0) {
        // Pop high dan low
        int high = stack[top--];
        int low = stack[top--];

        // Set pivot
        int pi = partition(arr, low, high);

        // Jika ada elemen di sisi kiri pivot, push ke stack
        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }

        // Jika ada elemen di sisi kanan pivot, push ke stack
        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }

    // Bersihkan memori stack bantu
    delete[] stack;
}

// ---------------------------------------------------------
// UTILITIES
// ---------------------------------------------------------

// Fungsi untuk generate data dummy (agar tidak capek input manual saat N=10000)
void generateData(Lagu* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i].judul = "Lagu " + to_string(i + 1);
        arr[i].artis = "Artis Random";
        arr[i].durasi = rand() % 600; // Durasi acak 0-600 detik
    }
}

void printArray(Lagu* arr, int n) {
    for (int i = 0; i < n; i++) {
        cout << i+1 << ". " << arr[i].judul << " (" 
             << arr[i].durasi << " det)" << endl;
    }
}

// ---------------------------------------------------------
// MAIN PROGRAM
// ---------------------------------------------------------
int main() {
    int n;
    cout << "Masukkan jumlah lagu (N): ";
    cin >> n;

    // ALOKASI MEMORI DINAMIS (Pengganti Vector)
    Lagu* playlist = new Lagu[n];

    char pilihanInput;
    cout << "Ingin input manual satu per satu? (y/n): ";
    cin >> pilihanInput;

    if (pilihanInput == 'y' || pilihanInput == 'Y') {
        for (int i = 0; i < n; i++) {
            cout << "Data ke-" << i + 1 << endl;
            cout << "Judul: "; cin >> playlist[i].judul;
            cout << "Artis: "; cin >> playlist[i].artis;
            cout << "Durasi (detik): "; cin >> playlist[i].durasi;
        }
    } else {
        cout << "Generating " << n << " random data..." << endl;
        generateData(playlist, n);
    }

    // Salin array untuk perbandingan (Agar data asli tidak hilang saat disort pertama kali)
    Lagu* dataRekursif = new Lagu[n];
    Lagu* dataIteratif = new Lagu[n];
    
    // Copy data manual
    for(int i=0; i<n; i++) {
        dataRekursif[i] = playlist[i];
        dataIteratif[i] = playlist[i];
    }

    cout << "\n--- MULAI PENGUJIAN ---\n";

    // 1. Uji Rekursif
    auto start = chrono::high_resolution_clock::now();
    quickSortRecursive(dataRekursif, 0, n - 1);
    auto stop = chrono::high_resolution_clock::now();
    auto durasiRec = chrono::duration_cast<chrono::microseconds>(stop - start);
    
    cout << ">> Quick Sort Rekursif selesai dalam: " << durasiRec.count() << " mikrodeitk." << endl;

    // 2. Uji Iteratif
    start = chrono::high_resolution_clock::now();
    quickSortIterative(dataIteratif, n);
    stop = chrono::high_resolution_clock::now();
    auto durasiIter = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << ">> Quick Sort Iteratif selesai dalam: " << durasiIter.count() << " mikrodeitk." << endl;

    // Opsional: Tampilkan hasil jika N kecil
    if (n <= 20) {
        cout << "\nHasil Sorting (Rekursif):" << endl;
        printArray(dataRekursif, n);
    }

    // PENTING: Hapus memori manual
    delete[] playlist;
    delete[] dataRekursif;
    delete[] dataIteratif;

    return 0;
}