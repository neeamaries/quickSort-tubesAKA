package main

import (
	"math/rand"
	"net/http"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
)

// Struktur Data
type Lagu struct {
	Judul  string `json:"judul"`
	Artis  string `json:"artis"`
	Durasi int    `json:"durasi"`
}

// --- GENERATOR DATA ---
func generateData(n int) []Lagu {
	dataset := make([]Lagu, n)
	for i := 0; i < n; i++ {
		dataset[i] = Lagu{
			Judul:  "Lagu " + strconv.Itoa(i+1),
			Artis:  "Artis ke- " + strconv.Itoa(i+1),
			Durasi: rand.Intn(500) + 60,
		}
	}
	return dataset
}

// --- FUNGSI PARTISI (Dipakai kedua metode) ---
func partition(arr []Lagu, low, high int) int {
	pivot := arr[high].Durasi
	i := low - 1
	for j := low; j < high; j++ {
		if arr[j].Durasi < pivot {
			i++
			arr[i], arr[j] = arr[j], arr[i]
		}
	}
	arr[i+1], arr[high] = arr[high], arr[i+1]
	return i + 1
}

// --- METODE 1: REKURSIF ---
func quickSortRecursive(arr []Lagu, low, high int) {
	if low < high {
		pi := partition(arr, low, high)
		quickSortRecursive(arr, low, pi-1)
		quickSortRecursive(arr, pi+1, high)
	}
}

// --- METODE 2: ITERATIF (Manual Stack) ---
func quickSortIterative(arr []Lagu) {
	low := 0
	high := len(arr) - 1

	// Buat stack menggunakan slice
	// Stack akan menyimpan pasangan (low, high)
	stack := make([]int, 0)

	// Push low dan high awal
	stack = append(stack, low)
	stack = append(stack, high)

	// Selama stack tidak kosong
	for len(stack) > 0 {
		// Pop high (elemen terakhir)
		high = stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		// Pop low (elemen sebelum terakhir)
		low = stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		// Lakukan partisi
		pi := partition(arr, low, high)

		// Jika ada elemen di sisi kiri pivot, push ke stack
		if pi-1 > low {
			stack = append(stack, low)
			stack = append(stack, pi-1)
		}

		// Jika ada elemen di sisi kanan pivot, push ke stack
		if pi+1 < high {
			stack = append(stack, pi+1)
			stack = append(stack, high)
		}
	}
}

// --- MAIN PROGRAM ---
func main() {
	r := gin.Default()

	// Sajikan file HTML dari folder "view" (pastikan nama folder sesuai di laptop Anda)
	r.Static("/", "./view")

	r.POST("/api/process", func(c *gin.Context) {
		// 1. Tangkap Input (Jumlah Data & Metode)
		type RequestInput struct {
			JumlahData int    `json:"jumlah_data"`
			Metode     string `json:"metode"` // Tambahan field untuk memilih metode
		}

		var req RequestInput
		if err := c.BindJSON(&req); err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Input salah"})
			return
		}

		// 2. Generate Data
		data := generateData(req.JumlahData)

		// 3. Proses Sorting Sesuai Pilihan
		start := time.Now()

		if req.Metode == "iteratif" {
			quickSortIterative(data)
		} else {
			// Default ke rekursif
			quickSortRecursive(data, 0, len(data)-1)
		}

		duration := time.Since(start)

		// 4. Kirim Hasil
		c.JSON(http.StatusOK, gin.H{
			"waktu_eksekusi": duration.Microseconds(),
			"metode_dipakai": req.Metode, // Info tambahan (opsional)
			"hasil":          data,
		})
	})

	r.Run(":8080")
}
