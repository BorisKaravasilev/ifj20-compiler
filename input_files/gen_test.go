// Pro testovani generovani
package main

func main() {
    x := 5
    _ = 10
    y := 1 + 2
    z := 0
    y = 2 * 3
    z = x

    x, y, z = 420, 69, 9000
    // TODO tohle vyhazuje semantickou chybu
    //z, y, x = 1 + 1, 2 * 2, 3 - 3

	a := ""
	b := 0
	a, _ = inputs()
	print(a, " ", b)
	print("ahoj svete	tab#\n\\")

	b = 5

    c := 1.2
	c = int2float(7)
    print(c, "\n")
    int2float(7)

    b = float2int(c)
    b = float2int(7.7)

    a = "cau"
    b = len(a)
    print(b, "\n")

    err := 0   // for error flag
    s := "Hello World!"
    pos := 6
    s, err = substr(s, pos, pos)
    print(s, " ", err, "\n") // should be "World!"

    ord_val := 0
    pos = 1
    ord_val, err = ord(s, pos)
    print(ord_val, " ", err, "\n")  // should be 111 - ASCII o

    s, err = chr(69)
    print(s, " ", err, "\n")    // should be 'E'*/
}
