//
package main

func main () {
    for a := 1; a < 10; a = a + 1 {
        if a >= 5 {
            print(a, "\n")
        }
        else {
        }
    }

    print("\n")

    a := 1  // Necessary to redefine a in this scope
    for ; a < 10; a = a + 1 {
        print(a, "\n")
    }

    for b := 0; b != 5; {
        print(b, "\n")
        b = b + 1
    }
}
