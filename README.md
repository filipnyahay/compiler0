# compiler0

# this is an attempt to make a simple compiler that targets x86 asm

keywords:
i64, f64, u8, if, else, null, return, var, fun, while

language example:
```
var i64 global = 10;

fun foo(i64 a, i64 b) {
    return a + b;
}

fun i64 main() {
    var i64 x = 10;

    if (x == 10) {
        // print to be created
    }
    else {
        x = 5;
    }

    var f64 y = 3.14;
    var f64 z = 42e-10;
    var u8[] = "this is a string literal"
}
```
