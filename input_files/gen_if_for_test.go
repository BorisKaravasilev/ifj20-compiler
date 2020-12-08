//
package main

func main () {
    for a := 1 * 2; a < 10 - 1; a = a + 1 {
        if a <= 5 {
            print(a, " je mensi nebo rovno 5\n")
        }
        else {
            print(a, " je vetsi nez 5\n")
        }
    }

    print("\n")

    // TODO vnorene jeste blbnou
    /*x := 0
    for i := 0; i < 10; i = i + 1 {
        for j := 0; j < 10; j = j + 1 {
            x = i * j
            print(x, " ")
        }
    }*/

    for b := 0; b != 5; {
        print(b, "\n")
        b = b + 1
    }

    c := 1
    for ; c < 10; c = c + 1 {
        print(c, "\n")
    }
}
