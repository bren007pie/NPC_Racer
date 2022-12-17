# NPC Racer Developer Documentation

```C++
McMaster University 
CSE 701: Foundations of Modern Scientific Programming
Final Project
NPC Racer
A Performance Analysis of Game Agent Pathfinding Algorithms
```


### Purpose

This document is for developers. It acts as not only a documentation of my design and syntax decisions but as a history and work cited. I'll take you through what I initially thought, what I may have found online, and my naive reasoning for choosing one way over another.

### Code Structure

The implementation algorithm was heavily inspired hardware implementations of digital adders, subtractors, and multipliers. If you want to get meta about it in this way I've embedded part of an ALU (arithmetic logic unit) in C++ (instead of into electrical circuits) with strings of 0-9 characters instead of 0s and 1s. And the control of the ALU is done by C++ and the software wiring of the class and the user requests.

- based a lot off of the matrix.hpp from the course notes
- For implementations that are one line (e.g. just a return statement) they are included in the interface because of their simplicity
- Trying to keep the character per line down to 80 so fits on most screens but maxing out at 120 character (<https://softwareengineering.stackexchange.com/questions/604/is-the-80-character-limit-still-relevant-in-times-of-widescreen-monitors>)

Design philosophy for this assignment is going to be code simplicity over performance. The goal is to learn C++ concepts and syntax.

### Testing

We will be writing incremental and testing as we go. Ideally we do formal unit tests of each function with a formal system but for now it will all be done by hand.

#### String Input

- non 0-9 and - character
- empty string/all whitespace

### Naming

Named intinf_t for a play on the fixed-width integer types of uint64_t, int8_t, etc.
Q) Why are all the fixed-width precision integers have "_t" appended to them?

### Constructors

#### String Infinite Integer Input

To get an into of any size we'll need to take a infinitely long data type, in this case we will be using a std::string.

[C String container reference](https://en.cppreference.com/w/cpp/string)

1. Sanitize input by [removing whitespace]( https://www.scaler.com/topics/removing-whitespace-from-a-string-in-cpp/ )

^ there is lots of problems with this and is contentious depending on C localization defines white space. "isspace() method checks for those whitespace characters that are classified by the present C locale (environment)" ( <https://www.scaler.com/topics/removing-whitespace-from-a-string-in-cpp/> )

```C
s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
```

2. Look for leading '-' sign

Using .starts_with() method for this just incase empty string

3. Checking rest of string is only digits using std::string .find_first_not_of ( inspiration: <https://stackoverflow.com/questions/8888748/how-to-check-if-given-c-string-or-char-contains-only-digits>)

Syntax Note) Using `namespace std;` is [bad practice according to some](https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice). Due to allowing for change, readability, and limited possibility of failing silently. Will get used to it anyways and is more explicit. Will use it for a proper class but exclude it from the demo for readability.

### Member Functions

- most member functions just return a version of the value or print it.
- The most complex functions are the get digit and abs methods
- get digit converts the digit at a certain position to an integer and returns it. It needs to be in the class to use private variables.

```C++
int intinf_t::get_dgt(const size_t position) const // returns the integer value of a digit at index position
{
    if ((position < this->value.max_size()) && (position < this->get_num_digits())) // as long as within size and max size
    {
        int digit = std::stoi(this->value.substr(position, 1)); // need to use substr to get string to convert
        return digit;
    }
    else
    {
        // TODO: Needs to preserve invariant to make sure not out of string bounds, throw exception if so
        std::cout << "ERROR: position bigger than max size of string.\n";
        return 0;
    }
}
```

- abs() simply converts the infinite precision integer to an unsigned version. It was based on the operator= copy assignment operator.

```C++
intinf_t &intinf_t::abs() // converts to absolute value unsigned version
{
    pos = true;
    return *this;
}
```

### Overloaded Operators

For the basic operators we will be doing element-wise operations. This will be very inefficient but is simplest to implement.

1. Find smaller string
2. Loop size of the smaller string
3. Go over each element right to left (least significant to most significant) convert to [integer](https://en.cppreference.com/w/cpp/string/basic_string/stol)

- perform element-wise addition, and carry over any overflow to the next
- e.g. addition = add each together, keep carry based on position
- Maybe it's worth to store string in least significant digit first (reverse) so that the index is the digit
- Multiply each element by the other in order
19*19 = 361
9*9+9*10+10*9+10*10 = 361

#### Addition (+ and +=)

- separating code implementation for addition of same sign vs different sign (subtraction) because the code is very long
- As shown with this diagram if we add two positive values or two negative values the unsigned value always grows, via addition. But if the signs are different the unsigned value will shrink, via subtraction. So it only makes sense to instead only deal with the cases where the sign is the same. For the other cases we can do a transformation and negate the right hand side of the operator to turn it into a subtraction problem, so we can call the subtraction operator

```
   + + + -> +   (add unsigned value)
   + + - -> +/- (subtract unsigned value, sign depends on values) -> + - + (unary minus RHS)
   - + + -> +/- (subtract unsigned value , sign depends on values) -> - - - (unary minus RHS)
   - + - -> -   (add unsigned value)
```

- If adding two positive or two negative numbers the unsigned value still grows and can apply the sign at the end.
- The implementation was modelled after a [full adder circuit](https://en.wikipedia.org/wiki/Adder_(electronics)) that adds each digit one at a time and keeps a carry bit that adds one to the next digit.
- There were a number of workarounds needed to get strings to convert properly to digits, including needing to use integers.
- The implementation ended up being much longer than expected.
- Where-ever possible tried to avoid copying values and doing unnecessary function calls. Could have made this operator a friend function to directly access the private members but decided against it for consistency.
- There is also a scenario where if there is a borrow at the end we need to be able to propagate the additional 1 throughout the remaining string. To do this we make the remaining string into an infinite integer and add 1 to it. This means we recursively call addition any time there is a carry at the end of addition. This could result in a series of recursion calls for each 9 in a row in the remainder. Say for example we add 999999 + 1, we would need to recursively call this function on each substring of 999999, so 5 times. First it would be 99999+1 then 9999+1 then 999+1 then 99+1 and finally 9+1. This is not good for performance but is the simplest way in code to make sure the borrow propagates throughout the string. It also will be fairly rare that we have a long string of 9s unless we decrement from an even 10 and then increment it.

```C++
// Overloaded binary operator + to add two infinite precision integers.
intinf_t operator+(const intinf_t &lhs, const intinf_t &rhs)
{
    if (lhs.is_pos() == rhs.is_pos()) // Same sign, addition of unsigned value
    {
        // Variable Definitions
        size_t lhs_size = lhs.get_num_digits(); // Storing sizes to avoid function calls later
        size_t rhs_size = rhs.get_num_digits();
        int big_i;            // digit value of the larger int at element i, must be int for stoi
        int sml_i;            // digit value of the smaller int at element i, must be int for stoi
        int sum_i;            // sum digit(s) value at element i
        bool carry = false;   // flag for overflow addition
        std::string sum = ""; // accumulator string for the summed value

        // guard clauses for zero, avoids unnecessary computation
        if ((lhs.get_uval() == "0")) // if lhs is side is zero return rhs
            return rhs;
        else if (rhs.get_uval() == "0") // if rhs is side is zero return lhs
            return lhs;

        // getting size variables
        const intinf_t &larger = (lhs_size >= rhs_size) ? lhs : rhs;  // getting whichever has more digits and storing a reference
        const intinf_t &smaller = (lhs_size >= rhs_size) ? rhs : lhs; // getting whichever has less digits and storing a reference
        size_t large_size = larger.get_num_digits();                  // storing sizes of the integers to avoid function calls later
        size_t small_size = smaller.get_num_digits();
        bool same_size = false;  // flag for if integers are same size
        sum.reserve(large_size); // reserves size of the string so doesn't have to resize during addition

        if (large_size == small_size)
            same_size = true;

        // looping over values doing addition
        for (size_t i = 0; i < small_size; i++)
        {
            big_i = larger.get_dgt(large_size - (i + 1));
            sml_i = smaller.get_dgt(small_size - (i + 1));
            sum_i = big_i + sml_i;
            if (carry)
            { // if carry from last digit addition, increment by 1 and toggle carry
                sum_i += 1;
                carry = false;
            }
            if (sum_i >= 10) // if over the digit limit, subtract 10 and toggle carry for next addition
            {
                sum_i -= 10;
                carry = true;
            }
            sum = std::to_string(sum_i) + sum; // appends to the sum variable
        }

        // copying over remaining digits
        if (same_size && carry) // if same side and had a carry still need to add a 1 at the end, e.g 999 + 111 = 1110
        {
            sum = "1" + sum;
            carry = false;
        }
        else if (carry)
        {
            // if still had carry need to add 1 to the remainder, so call recursively
            // recurses as many time as there are 9s in a row in the remainder string
            std::string remainder = larger.get_uval().substr(0, (large_size - (small_size))); // get remaining substring
            intinf_t remainder_int(remainder);                                                // making a positive infinite precision integer
            intinf_t pos_1("1");
            remainder_int = remainder_int + pos_1; // increment recursion! But only for as many 9s in a row there is
            carry = false;
            sum = remainder_int.get_uval() + sum; // append to front
        }
        else // if no carry just copy rest of substring
        {
            sum = larger.get_uval().substr(0, (large_size - (small_size))) + sum; // appends rest of substring from left off position
        }
        if (!lhs.is_pos()) // if negative value add the '-'
        {
            sum = "-" + sum;
        }

        intinf_t int_sum(sum);
        return int_sum;
    }
    else // opposite signs, subtraction of negated rhs
    {
        return (lhs - (-rhs));
    }
}
```

#### Subtraction (- and -=)

- Similar to addition we will be splitting up the cases based on the sign. We will only be doing subtraction on equal signed values and then using the addition operator with a negated RHS for the other cases.
- In a similar argument to addition diagram, if we subtract two positive or two negative values the unsigned value always shrinks, via subtraction. But if the signs are different the unsigned value will grow, via addition. So it only makes sense to instead only deal with the cases where the sign is the same. For the other cases we can do a transformation and negate the right hand side of the operator to turn it into a addition problem, so we can call the addition operator

```
   + - + -> +/- (subtract unsigned value, sign depends on values)
   + - - -> +   (add unsigned value) -> + + + (unary minus RHS)
   - - + -> -   (add unsigned value) -> - + - (unary minus RHS)
   - - - -> +/- (subtract unsigned value, sign depends on values)
```

- For subtraction we will be copying and modifying our addition operator code. There will be two key changes:

1. Similar to the [electronic Subtractor](https://en.wikipedia.org/wiki/Subtractor), instead of a carry bit there will be a borrow bit to designate when we need to subtract one from the next digit.
2. Unlike addition, subtraction of the same sign can change the sign of the difference based on the size of the left hand side compared to the right hand side. If LHS = RHS then the result is zero no matter what the sign. For subtracting values of the same sign you get the same results. If the signed LHS < RHS the final result will be a negative and if LHS > RHS the result will be a positive. You can confirm these by with simple examples:

```
Positives
8 < 9
+9 - +8 = +1 (LHS > RHS)
+8 - +9 = -1 (LHS < RHS)
+8 - +8 =  0 (LHS == RHS)
Negatives
-9 < -8
-8 - -9 = +1 (LHS > RHS)
-9 - -8 = -1 (LHS < RHS)
-8 - -9 =  0 (LHS == RHS)
```

^This took me a while to wrap my head around. We'll be using the overloaded < operator to do these checks because it's the least amount of redundant equality checks.

3. Also unlike addition we cannot rearrange the operation to have the larger number - the smaller number. Subtraction is not commutative so we must subtract the lhs (the minuend) by the rhs (the subtrahend) to get the difference (<https://en.wikipedia.org/wiki/Subtraction#Notation_and_terminology>).
4. However, whenever the value goes over zero when can use a trick to flip it so the rhs - lhs to get the subtraction of the digits is correct. To do this we note down when the sign flips and assign
5. There are cases where zeros could be prepended to the front. E.g. 9999 - 9990 = 0009. We want to get rid of these zeros to avoid having to consider them in future calculations. To do this we check if the accumulator string starts with zero, remove that zero, and continue to loop until it no longer does.

```C++
// Overloaded binary operator - to subtract two infinite precision integers.
intinf_t operator-(const intinf_t &lhs, const intinf_t &rhs)
{
    if (lhs.is_pos() == rhs.is_pos()) // Same sign, subtraction of unsigned value
    {
        // Variable Definitions
        size_t lhs_size = lhs.get_num_digits(); // Storing sizes to avoid function calls later
        size_t rhs_size = rhs.get_num_digits();
        int lhs_i;                // digit value of the minuend int at element i, must be int for stoi
        int rhs_i;                // digit value of the subtrahend int at element i, must be int for stoi
        int diff_i;               // difference digit(s) value at element i
        bool borrow = false;      // flag for overflow subtraction
        std::string diff = "";    // accumulator string for the difference value
        bool diff_sign;           // final sign of the difference. positive = true and negative = false.
        bool sign_change = false; // flag that the sign changed so the subtraction went over zero

        // guard clauses for zero, avoids unnecessary computation
        if ((lhs.get_uval() == "0")) // if lhs is side is zero return -rhs. 0 - rhs = -rhs
            return -rhs;
        else if (rhs.get_uval() == "0") // if rhs is side is zero return lhs. lhs - 0 = 0
            return lhs;

        // Getting final sign //
        // remember LHS and RHS signs are the same here, only + - + and - - - cases.

        if (lhs == rhs) // LHS == RHS guard, that means difference is 0 so can return early
        {
            intinf_t pos_0; // default constructor is 0
            return pos_0;
        }

        if (lhs < rhs)         // using < here because less linear equality checks
            diff_sign = false; // for positives or negatives, if LHS < RHS -> the final result is negative
        else                   // already checked for equal case so can else
            diff_sign = true;  // for positives or negatives, if LHS > RHS -> the final result is positive

        // flipping minuend and subtrahend based on if went over zero or not
        // can't be in an if statement or else will be in the wrong scope
        // Using ternary operator instead
        // if sign_change = false, minuend = lhs, subtrahend = rhs
        // if sign_change = true, minuend = rhs, subtrahend = lhs
        if (diff_sign != lhs.is_pos())                        // only need to check lhs b.c. both same sign
            sign_change = true;                               // if changed sign it went over zero
        const intinf_t &minuend = sign_change ? rhs : lhs;    // getting minuend (lhs of subtraction) based on sign_change
        const intinf_t &subtrahend = sign_change ? lhs : rhs; // getting subtrahend (lhs of subtraction) based on sign_change
        size_t minuend_size = minuend.get_num_digits();       // storing sizes of the integers to avoid function calls later
        size_t subtrahend_size = subtrahend.get_num_digits();

        // getting size variables
        const intinf_t &larger = (lhs_size >= rhs_size) ? lhs : rhs; // getting whichever is bigger or smaller and storing a reference
        const intinf_t &smaller = (lhs_size >= rhs_size) ? rhs : lhs;
        size_t large_size = larger.get_num_digits(); // storing sizes of the integers to avoid function calls later
        size_t small_size = smaller.get_num_digits();
        diff.reserve(large_size); // reserves the large of the string so doesn't have to resize during subtraction

        // looping over values doing subtraction
        for (size_t i = 0; i < small_size; i++)
        {
            lhs_i = minuend.get_dgt(minuend_size - (i + 1));
            rhs_i = subtrahend.get_dgt(subtrahend_size - (i + 1));
            diff_i = lhs_i - rhs_i; // subtract indicies
            if (borrow)
            { // if borrow from last digit subtraction, decrement by 1 and toggle borrow
                diff_i -= 1;
                borrow = false;
            }
            if (diff_i < 0) // if under the digit limit, add 10 and toggle borrow for next addition
            {
                diff_i += 10;
                borrow = true;
            }
            diff = std::to_string(diff_i) + diff; // appends to the front of the diff variable
        }

        if (borrow) // if still had borrow need to subtract a digit?
        {
            std::string remainder = larger.get_uval().substr(0, (large_size - (small_size))); // get remaining substring
            intinf_t remainder_int(remainder);                                                // making a positive infinite precision integer
            intinf_t pos_1("1");                                                              // a positive 1
            remainder_int = remainder_int - pos_1;                                            // decrement -1 b.c. no ++ operator yet, recursion!
            borrow = false;
            diff = remainder_int.get_uval() + diff; // append to front
        }
        else // if no borrow just copy rest of substring
        {
            diff = larger.get_uval().substr(0, (large_size - (small_size))) + diff; // appends rest of substring from left off position
        }

        // removing any prepended zeros for formatting
        while (diff.starts_with('0'))
        {
            diff.erase(diff.begin()); // ERASE the 0 in the value at the front
        }

        if (!diff_sign) // if negative value add the '-'
        {
            diff = "-" + diff;
        }

        intinf_t int_diff(diff);
        return int_diff;
    }
    else // opposite signs, subtraction of negated rhs
    {
        return (lhs + (-rhs));
    }
}

```

#### Multiplication (*and*=)

- We can do multiplication as a loop of +.
- By comparison this arithmetic operator was the easiest to implement, although it relies on the dependency of having the + operator.
- The only thing was that to add together the integers I needed to keep setting an intinf_t variable. To avoid allocating new variables in a loop I redefined the constructors in terms of public setter functions and used that setter function in the loop.
- In a similar way to +/- we were heavily inspired by the hardware [digital multiplier circuit](https://en.wikipedia.org/wiki/Binary_multiplier). This circuit multiplies each element by every other element to make the partial product. Then each partial product is shifted by the appropriate amount of digits so it's in the right spot. Then each of these partial products are summed together to make the final product. Much like you may have learned to do pencil and paper multiplication.
- Here we need to account for the sign changes but thankfully for multiplication they are easy.
- There is a nice guard check we can perform that stops computation. If any number is multiplied by zero we just need to return zero.

```
   + * + -> +
   + * - -> -
   - * + -> -
   - * - -> +
```

So if the signs are different the final product is negative and if they're the same it's positive.

- Here is the code for multiplication:

```C++
// Overloaded binary operator * to multiply two infinite precision integers.
intinf_t operator*(const intinf_t &lhs, const intinf_t &rhs)
{
    // Variable Definitions
    size_t lhs_size = lhs.get_num_digits(); // Storing sizes to avoid function calls later
    size_t rhs_size = rhs.get_num_digits();
    int big_i;                  // digit value of the larger int at element i, must be int for stoi
    int sml_i;                  // digit value of the smaller int at element i, must be int for stoi
    int prod_i;                 // partial product digit(s) value at element i
    std::string part_prod = ""; // accumulator string for the partial product value
    bool prod_sign;             // final sign for product. True = positive, false = negative
    intinf_t part_prod_int;     // partial product int type converter so can be added to product
    intinf_t product;           // the intinf_t accumulator for the sum of partial products. Initially 0.

    // guard clauses for zero, avoids unnecessary computation
    if ((lhs.get_uval() == "0") || (rhs.get_uval() == "0")) // if either side is zero
    {
        return product; // returns 0
    }

    // getting size variables
    // bigger one goes on top
    const intinf_t &larger = (lhs_size >= rhs_size) ? lhs : rhs;  // getting whichever has more digits and storing a reference
    const intinf_t &smaller = (lhs_size >= rhs_size) ? rhs : lhs; // getting whichever has less digits and storing a reference
    size_t large_size = larger.get_num_digits();                  // storing sizes of the integers to avoid function calls later
    size_t small_size = smaller.get_num_digits();
    // reserves the max possible size of the string so doesn't have to resize during addition.
    // For mult. (lhs x rhs) max # of digits = lhs_size + rhs_size
    part_prod.reserve(large_size + small_size);

    // final sign determination
    if (lhs.is_pos() == rhs.is_pos()) // if sign is the same it's positive
        prod_sign = true;
    else // if signs are different it's negative
        prod_sign = false;

    // looping over values doing addition
    for (size_t i = 0; i < large_size; i++)
        for (size_t j = 0; j < small_size; j++)
        {
            big_i = larger.get_dgt(large_size - (i + 1));
            sml_i = smaller.get_dgt(small_size - (j + 1));
            prod_i = big_i * sml_i;

            part_prod = std::to_string(prod_i); // storing un-shifted values in string
            // add the zeros at the end to get to the right shifted position
            part_prod.resize((part_prod.length() + i + j), '0');
            part_prod_int.set_val(part_prod); // store in infinite integer

            product = product + part_prod_int; // accumulating the partial products
        }

    if (!prod_sign) // if negative value add the '-'
    {
        product = -product; // set to negative, works b.c. product is positive value by default
    }

    return product;
}
```

#### Negation (unary -)

- We simply need to remove or add a '-' depending on if there was one there or not.

#### Increment (++) and decrement (--) (prefix and postfix)

- both prefix and postfix versions
- based on the [lecture notes](https://baraksh.com/CSE701/notes.php#introduction-to-operator-overloading) and [this](https://www.scaler.com/topics/pre-increment-and-post-increment-in-c/):
  - prefix/pre-increment = (e.g. ++x) increments the var (x) and returns the **incremented** var (x).
 e.g.

 ```C
  int a = 5;
 int b = ++a;
 printf("a = %d\nb = %d\n", a, b); 
 ```

 ```
 Output:
 a = 6
 b = 6

 ```

- prefix/pre-decrement = (e.g. --x) analogous to pre-increment but for subtraction.
- postfix/post-increment = (e.g. x++) increments the var(x) but returns the **original** value.

 ```C
 int a = 5;
 int b = a++;
 printf("a = %d\nb = %d\n", a, b); 
 ```

 ```
 Output:
 a = 6
 b = 5

 ```

- postfix/post-decrement = (e.g. x--) analogous to post-increment but for subtraction.

Examples borrowed from [here](https://www.scaler.com/topics/pre-increment-and-post-increment-in-c/).

- Our increments/decrements will use the overloaded operators for + and -.
- They are defined in the class itself to use private data and *this operator.
- Based on inspiration from [here](https://learn.microsoft.com/en-us/cpp/cpp/increment-and-decrement-operator-overloading-cpp?view=msvc-170) and [here](https://en.cppreference.com/w/cpp/language/operators).

- implementation is fairly straightforward but just need to pass the correct object back.

```C++
// Overloaded unary operator ++ to take the prefix/pre-increment of an infinite precision integer (e.g. ++x).
// Increments the value and returns that incremented value.
intinf_t &intinf_t::operator++()
{
    intinf_t pos_1("1");   // one to add to the negative value
    *this = *this + pos_1; // adds the value to increment
    return *this;          // returns the incremented value
}

// Overloaded unary operator ++ to take the postfix/post-increment of an infinite precision integer (e.g. x++).
// Increments the value and returns the original value
intinf_t intinf_t::operator++(int) // for some reason has to have int as its argument
{
    intinf_t temp = *this; // store this in a temporary variable
    ++*this;               // actually increment
    return temp;           // return the original value
}

// Overloaded unary operator -- to take the prefix/pre-decrement of an infinite precision integer (e.g. --x).
// Decrements the value and returns that decremented value.
intinf_t &intinf_t::operator--()
{
    intinf_t pos_1("1");   // one to add to the negative value
    *this = *this - pos_1; // adds the value to increment
    return *this;          // returns the incremented value
}

// Overloaded unary operator -- to take the postfix/post-decrement of an infinite precision integer (e.g. x--).
// Decrements the value and returns the original value
intinf_t intinf_t::operator--(int) // for some reason has to have int as its argument
{
    intinf_t temp = *this; // store this in a temporary variable
    --*this;               // actually decrement
    return temp;           // return the original value
}
```

#### Comparison (==, !=, <, >, <=, and >=)

- Because we are storing our values as strings we can simply use the overloaded string comparison operators and return the Boolean value. OR SO I THOUGHT.
- This works because lexicographically number comparisons work only for cases where numbers are of the same size. I found this out later when comparing "123456789" and "7899".
- They even work with negative sign `-` between negative and positive numbers but not between negative and negative numbers. That means this condition must be checked before returning.
- But these conditions are not fun so I decided to do a series of checks
- == simply checks if the value strings are equal, != does the opposite of == so we simply negate it for code reuse.

```C++
// Using string lexicographic compare overloaded operators instead of redefining my own.
constexpr bool operator==(const intinf_t &lhs, const intinf_t &rhs) { return (lhs.get_val() == rhs.get_val()); };

// apparently this can be inferred from the compiler but is listed here for readability
constexpr bool operator!=(const intinf_t &lhs, const intinf_t &rhs) { return (!(lhs == rhs)); }; // == negation
```

- We will define < then define the rest based off of it.
- We do a series of 4 guard/nested checks for <:

0. Equality - if equal it's always false
1. Sign - positive always bigger than negative.
2. Size - If same sign: bigger sized positive if bigger, bigger sized negative is smaller.
3. lexicographic order - If same sign and size: then we can use the string overloaded operator to check the lexicographic (dictionary) order of the unsigned string.

```C++
// Overloaded binary operator < to check if left hand side is less than right hand side.
constexpr bool operator<(const intinf_t &lhs, const intinf_t &rhs)
{
    // guard check for equality
    if (lhs == rhs) // x < x -> false
        return false;

    // guards for sign check
    if ((lhs.is_pos() == false) && (rhs.is_pos() == true)) // - < + -> true
        return true;
    else if ((lhs.is_pos() == true) && (rhs.is_pos() == false)) // + < - -> false
        return false;

    // means LHS sign == RHS sign (checked both cases where they were different signs)

    // guard checks for positive size
    if (lhs.is_pos() == true) // if both are positive
    {
        if (lhs.get_num_digits() < rhs.get_num_digits()) // +small < +big -> true
            return true;
        else if (lhs.get_num_digits() > rhs.get_num_digits()) // +big < +small -> false
            return false;
    }
    else // guard checks for negative size
    {
        if (lhs.get_num_digits() < rhs.get_num_digits()) // -small < -big -> false
            return false;
        else if (lhs.get_num_digits() > rhs.get_num_digits()) // -big < -small -> true
            return true;
    }

    // means LHS size == RHS size

    // lexicographic order checks
    bool lex_order = (lhs.get_uval() < rhs.get_uval()); // unsigned lexicographic order
    if (lhs.is_pos() == false)                          // if negative need to return the opposite of the unsigned ordering:
        return !lex_order;
    else
        return lex_order; // if positive can use normal lexicographic ordering
}
```

- We can then defined > as the negation of < EXCEPT when lhs==rhs, then it's false

- Then <= is basically < but with a check first for lhs==rhs first, if so then it's true

- And finally >= is the negation of < but with a check first for lhs==rhs first, if so then it's true

- Note: this does result in a wasted operation since equality needs to be checked twice. Once in the >,<=, and >= function and then again in the < function. Thankfully equality checking of strings is [constant](https://en.cppreference.com/w/cpp/string/basic_string/operator_cmp) but this certainly could be optimized in [future work](#future-work).

#### Assignment (=)

- Here we are implementing the copy assignment operator.
- This one is super weird and gave me a lot of errors I did not understand.
- Firstly the prototype definition needs to be defined in the class itself so it can access private data members.
- I had lots of errors, I at one point thought you could only define a copy assignment operator with smart pointers.
- Eventually I found a resource that showed me how to do this [without smart pointers]( https://www.learncpp.com/cpp-tutorial/overloading-the-assignment-operator/) but still based on pointers.

```C++
intinf_t &intinf_t::operator=(const intinf_t &rhs) // based on https://www.learncpp.com/cpp-tutorial/overloading-the-assignment-operator/
{
    value = rhs.value;
    pos = rhs.pos;
    return *this;
}

```

#### Insertion (<<)

- to print the integer string to a stream such as cout
- We will just return the string value of the string

```C++
// Overloaded binary operator << to print out a infinite precision integer to a stream.
std::ostream &operator<<(std::ostream &out, const intinf_t &inf_int)
{
    out << inf_int.get_val(); // just returning the string value
    return out;
}

```

#### Subscript ([])

- for accessing a specific digit of the integer
- We can reuse our

### Maximum Sizes of Containers

- if we're using containers we will always be limited by the max size it can hold
- it turns out this is well defined for most containers including std::string with the [.max_size()](https://en.cppreference.com/w/cpp/string/basic_string/max_size) method.
- However, we also need to make sure our algorithms can deal with giant containers.
- For example, [.size()](https://en.cppreference.com/w/cpp/string/basic_string/size) for strings returns a [size_type](https://en.cppreference.com/w/cpp/types/size_t) (synonym for size_t)
- If this size_type is not big enough to hold all the elements we need we'll have problems
- According to ( <https://pvs-studio.com/en/blog/posts/cpp/a0050/> ) the max_size for a size_t type is defined in the SIZE_MAX constant. This is backed up by the [C reference](https://en.cppreference.com/w/c/types/limits) and cpp reference [numerical_limits](https://en.cppreference.com/w/cpp/types/numeric_limits/max)
- Based on testing in my max_sizes.cpp I found:
On my 64 bit Windows machine:

  - Maximum size of a std::string is 9,223,372,036,854,775,807
  - Maximum size of a std::vector is 1,152,921,504,606,846,975
  - Maximum size of a size_t/size_type is 18,446,744,073,709,551,615 (2^64)

- This means in theory:
  - string could hold a number up to 10^9.2e18
  - A vector of 64 bit unsigned ints (2^64 each) in theory could hold (18e18)^1.2e18

- A vector certainly could hold a much bigger number than a string encoding.
- But the string can still hold 9 Quintillion 0-9 digits so it's MUCH bigger range than a 64 bit number.

- but practically both will be limited by the max_size of size_type (used in algorithms and probably for internals)
- which means each can only hold 18e18 elements, which actually in both cases is larger than the size limits of each type so we're good.

- Based on our design criteria we will not be using vectors for simplicity. But it is good to know that this implementation would not only be faster but able to store much larger numbers. This is because each element of the vector container can store 2^64/10 more information than a base 10 string element.

- Therefore an invariant of the intinf_t is that it must not exceed std::string::max_size().

### Intinf_t Exceptions
- These are defined within the class to avoid any namespace clashes

1. Empty string : invalid_argument,
 has just an empty in there
2. String w. wrong formating: invalid_argument,
 Not only "0123456789"
3. digit out of bounds : out-of-range error,
 bigger than the size
4. digit too big : length_error,
 bigger than max_length

```C++
    // Exception to be thrown if the input string given has no input.
    class empty_input : public std::invalid_argument
    {
    public:
        empty_input() : std::invalid_argument("Intinf_t cannot have no values!"){};
    };

    // Exception to be thrown if the input string has a character not in "0123456789".
    class invalid_digit : public std::invalid_argument
    {
    public:
        invalid_digit() : std::invalid_argument("Digits must only be '0-9'!"){};
    };

    // Exception to be thrown if asking for element outside the size of the Intinf_t.
    class digit_out_of_bounds : public std::out_of_range
    {
    public:
        digit_out_of_bounds() : std::out_of_range("Asking for digit outside the size of the intinf_t!"){};
    };

    // Exception to be thrown if string given is bigger than max_size.
    class number_too_big : public std::length_error
    {
    public:
        number_too_big() : std::length_error("Number given is bigger than max possible size!"){};
    };
```

### Intinf_t Invariants

1. Cannot be an empty string - must always have a value for operations.
2. Cannot exceed the max size of the string container - doing so would lead to errors.

### VSCode Notes

- this code was written in the Visual Studio Code (VSCode) IDE. While you to not need this IDE to follow the conventions/style it certainly helps to follow my settings. If you are familiar with VSCode these will make sense to you, if not then they will not
- Have C++ save on format on in the settings.
- Make sure to have code spell-checker on.
- ctrl+shift+0 brings up full tooltip/tooltip at cursor position. This is very useful to see the overloads and full type signatures of an operator or function.
- You can make rules to make sure you adhear to the 80-120 character limit. In the VSCode UI settings search for "rulers" and add this entry to the settings.json file:

```json
"editor.rulers": [
        80,
        120
    ],
```

- I would also suggest syncing your VSCode settings if you use multiple computers to make it easier.

### Future Work

- Optimizing to see how fast we can make it go

### Hand-in Checklist

- [ ] Remove all TODOs
- [ ] Remove print statements, debug prints, and unused commented code
- [ ] Clean up documentation, spell/grammar check, and Pandoc to Tex/HTML
- [ ] Run through Grammarly to check
- [ ] Make sure all lines under 120 character long

### Work Checklist

- [X] Define base class
- [X] Sanitize and parse basic string input
- [X] Overload Addition of two intinf_ts
- [X] Assignment operator
- [X] Fix comparisons
- [X] Fix addition
- [X] Finish subtraction
- [X] Write down multiplication
- [X] Implement multiplication
- [X] +=, -=, and *=
- [X] Increment/decrement
- [ ] Define exceptions
- [ ] Make user/interface documentation
- [ ] Hand-in checklist
- [ ] [], division, and modulo

Program guidelines I need to double check:

- Make sure code snippets show when they begin and end in terms of inspiration
- Avoid common errors in the big red boxes
- detect and handle errors, including: user errors (e.g. the user entered a string where they should have entered a number),
 file errors (e.g. invalid file format), and system errors (e.g. out of memory),
 and terminate the program with an informative error message, e.g. "Invalid input on line 7 of input.txt: Expected a number".

- comments explaining each function (via doxygen comments)
- Must include clear external documentation, explaining relevant equations,algorithms, and concepts.
 must include several examples of inputs, interests, and outputs (Markdown/pandoc)
- Your code and documentation should be free of typos and grammar issues.

For Project

- Comments in Doxygen format must be written for all classes, functions, and variables.
- All code must be properly formatted for maximum readability. I highly recommend using VS Code's automatic formatting feature (Shift+Alt+F) and even set it up to automatically format code on save, as explained in the lecture note
- object-oriented, which means objects and their member variables and functions should be used whenever possible, and correctly implement the object-oriented techniques we learned in class, such as encapsulation and class invariants
- may use another programming language, such as Python or Mathematica, to visualize the output of your C/C++ program if desired
