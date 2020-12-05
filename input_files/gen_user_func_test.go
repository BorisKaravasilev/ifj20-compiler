//
package main

/*func swap(x int, y int) (int, int) {
    return y, x
}*/

func get(num int) (int) {
    return num
}

func empty(){
}

func empty_return(){
    return
}

func main () {
    a := 2
    b := 3
    empty_return()
    empty()
	//a, b = swap(a, b)
	a = get(b)
}
