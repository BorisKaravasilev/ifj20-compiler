//
package main

func main () {
    for a := 2 + 4; a < 10; a = a + 1 {
        if a >= 5 {
            print(a, "\n")
        }
        else {
        }
    }

    print("\n")

    /*for b := 0; b != 5; {
        print(b, "\n")
        b = b + 1
    }*/

    c := 1  // Necessary to redefine a in this scope
    for ; c < 10; c = c + 1 {
        print(c, "\n")
    }
}
