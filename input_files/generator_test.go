// Pro testovani generovani
package main

func main() {

	a := ""
	b := 0
	a, _ = inputs()
	print(a)
	print("ahoj svete	tab#\n\\
	")
	print(a, b, "ahoj")

	b = 5

    c := 1.2
	c = int2float(7)
    print(c)
    int2float(7)

    b = float2int(c)
    b = float2int(7.7)

    a = "cau"
    b = len(a)
    print(b)

    err := 0   // for error flag
    s := "Hello World!"
    pos := 6
    s, err = substr(s, pos, pos)
    print(s, " ", err, "
") // should be "World!"

    ord_val := 0
    pos = 1
    ord_val, err = ord(s, pos)
    print(ord_val, " ", err, "
")  // should be 111 - ASCII o

    s, err = chr(69)
    print(s, " ", err, "
")    // should be 'E'
}
