//
package main

func reverse(a int, b int, c int, d int, e int) (int, int, int, int, int) {
    print("\nInside reverse function\n")
    return e, d, c, b, a
}

func print_floats(x float64, y float64) () {
    print(x, " ", y, "\n")
}

func get(num int) (int) {
    return num
}

func empty(){
}

func empty_return(){
    return
}

func main () {
    a := 1
    b := 2
    c := 3
    d := 4
    e := 5

    print("Before\n")
    print(a, " ", b, " ", c, " ", d, " ", e, " ")

    a, b, c, d, e = reverse(a, b, c, d, e)

    print("After\n")
    print(a, " ", b, " ", c, " ", d, " ", e, " ")
    print("\n")

    empty_return()
    empty()

	a = get(b)
	print("\n", a, "\n")

	fl1 := 0.69
    fl2 := 0.96
    print_floats(fl1, fl2)
}
