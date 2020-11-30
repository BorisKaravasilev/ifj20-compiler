// Pro testovani generovani
package main

func main() {
	a := ""
	b := 0
	a, b = inputs()
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
}
