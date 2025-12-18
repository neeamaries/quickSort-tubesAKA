package main

import "fmt"

func quickSort(data []int) []int {
	if len(data) < 2 {
		return data
	}

	pivot := data[len(data)/2]
	left := []int{}
	right := []int{}
	equal := []int{}

	for _, v := range data{
		if v < pivot{
			left = append(left, v)
		} else if v > pivot{
			right = append(right, v)
		} else {
			equal = append(equal, v)
		}
	}

	left = quickSort(left)
	right = quickSort(right)

	return append(append(left, equal...), right ...)
}

func main(){
	data := []int{8, 4, 2, 6, 4, 6, 7, 9, 1}
	fmt.Print("Sebelum: ", data)
	fmt.Print("\n Sesudah: ", quickSort(data))
}
