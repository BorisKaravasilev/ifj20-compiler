//
package main

func main () {
    // TODO opravit reinicializace na zacatku vnorenych cyklu
    for i := 1; i <= 10; i = i + 1 {
        for j := 1; j <= 10; j = j + 1 {
            x := i * j
            if x < 10 {
                print(x, "   ")
            }
            else {
                if x < 100 {
                    print(x, "  ")
                }
                else {
                    print(x, " ")
                }
            }
        }
        print("\n")
    }
    print("\n")

    /*
    // druha hardcore varianta, pomichaji se labely
    j := 1
    k := 1
    x := 0
    for i := 1; i <= 10; i = i + 1 {
        for ; j <= 10; j = j + 1 {
            for ; k <= 10; k = k + 1 {
                x = i * j * k

                if x < 100 {
                    print(x, "   ")
                }
                else {
                    if x < 1000 {
                        print(x, "  ")
                    }
                    else {
                        print(x, " ")
                    }
                }
            }
        }
        print("\n")
    }*/

    for k := 1; k <= 10; k = k + 1 {
        y := k
        print(y, " ")
    }
    print("\n\n")

    for a := 1 * 2; a < 10 - 1; a = a + 1 {
        if a <= 5 {
            print(a, " je mensi nebo rovno 5\n")
        }
        else {
            print(a, " je vetsi nez 5\n")
        }
    }

    prom := 0
    print("\n")

    for b := 0; b != 5; {
        print(b, "\n")
        b = b + 1
    }

    c := 1
    for ; c < 10; c = c + 1 {
        print(c, "\n")
    }
}
