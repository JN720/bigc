# Big See Lemur Interpreter

Big See (bigc) is my custom programming language.
It is interpreted similar to Python and JavaScript and runs using the Lemur Interpreter, written in C++.
The type system is dynamic but strongly typed, again akin to Python.
I originally created the language trying to figure out how to do the Basic Calculator LeetCode problem.
The idea evolved from a 4-function calculator, to an advanced calculator with variables, to an array computation language (like APL), finally to a full-blown object-oriented and functional programming language.

# TODO: install instructions

# Basics

## Hello World

To greet the world, put the following into a file with the .lms extension.

```
println("Hello World!")
```

## Variables

Variables are declared and assigned in a manner similar to Python.

```
a = 3
b = 5
```

## TODO: Arithmetic

Big See supports several common arithmetic operations.

```
x = 10
y = 5

# Addition
result = x + y # result is 15

# Subtraction
result = x - y # result is 5

# Multiplication
result = x * y # result is 50

# Division
result = x / y # result is 2

# Modulo
result = x % y # result is 0
```

Certain operations depend on the data types provided, such as with integer versus float division.
Boolean addition is the logical OR while multiplication is the logical AND. (this idea was inspired by Abstract Algebra)

## TODO: Control

### Conditional Statements

You can use `if`, `else if`, and `else` to control the flow of execution based on boolean expressions.

```
x = 10

if x > 0 {
    println("x is positive")
} else if x < 0 {
    println("x is negative")
} else {
    println("x is zero")
}
```

### Loops

Big See supports `while` loops for basic iteration.

#### While Loop

The `while` loop continues to execute as long as the condition is true.
It also behaves like an expression, returning the last value in the loop sequence unless other control signals are used.

```
count = 0
finalCount = while count < 5 {
    println("Count is: " + count@str)
    count += 1
}
```

#### Iterate Loop

The `iterate` loop iterates over a range of values.
It also has similar expression-like behavior as the while loop.

```
iterate element [5, 6, 8] {
    println("Iteration: " + i@str)
}
```

#### Collect Loop

The `collect` loop functions like a while loop but collects results into an array.

```
count = 0
accumulatedItems = collect count < 8 {
    println("Iteration: " + i@str)
}
```

### TODO: Break and Continue

You can use `break` to exit a loop prematurely and `continue` to skip the current iteration and proceed to the next one.

```
count = 0
while count < 10 {
    count += 1;
    if 0 == count % 2 {
        continue;
    }
    println(count)
    if count > 6 {
        break;
    }
}

# 1 3 5 7
```

You can also break or continue with a value (similar to return), causing that value to be used for the expression behavior.

```
count = 2
nums = collect count < 20 {
    count += 1
    if (count % 2) == 0 {
        continue 0;
    }
    if (count % 9) == 0 {
        break 500;
    }
    count
}
# [3, 0, 5, 0, 7, 0, 500]
iterate num nums {
    println(num)
}
```

## TODO: Functions

Functions are values and are declared with the `funion` keyword. They use the following syntax and return the last statement.

```
sum = funion a, b {
    a + b
}

sum(5, 10) | println # 15
```

Functions can require types in their arguments which are asserted when the function is called.

```
square = funion x@int {
    println("squaring")
    x * x
}

square(5) | println #25
square("hello") #errors
```

The `return` keyword can be used to return a value early.

```
fibonacci = funion index@int {
    if index <= 1 {
        return 1
    }
    fibonacci(index - 1) + fibonacci(index - 2)
}

fibonacci(10) | println #55
```

## TODO: Classes

## TODO: Interfaces

# TODO: Libraries

```

```
