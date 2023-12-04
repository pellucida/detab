# Expand tabs into spaces or spaces into tabs

Two simple programs to expand tabs set at specified positions into spaces and
the reverse.

I had a recent need to do this but couldn't find an obvious program but I vaguely remembered converting a small program from *Software Tools in Pascal*  (Kernighan and  Plauger, 1981.) 

#### Synopsis

**detab**  *[-T repeat]*  *[-t stop1,stop2,stop3,...]*  *[filename]*

Set tab stops at positions **stop1 stop2 stop3** columns and at every **repeat** column thereafter.

**entab**  *[-Z|-F]*  *[-E  (d|h|i|s|p)]*  *[-T repeat]*  *[-t stop1,stop2,stop3,...]*  *[filename]*

Insert tabs either to compress spaces (*-Z)* or to insert tabs as field separators (*-F*) at the specified tab stops while absorbing any trailing spaces.
The handling of embedded tab (*-E*) characters in the input is where the complexity arises. The options considered are delete (**d**), honour (**h**), insert (**i**) sufficient spaces to move to next standard (*-T* **8**) tab stop, replace with a single space (**s)** or preserve (**p**) passing the tab character as an ordinary character.

#### Examples

`detab -T 7 -t 3,6,10`

Set tab stops at columns **3, 6** and **10** then **14, 21, 28** et seq.

Input

```
12345678901234567890123456789
a\tb\tc\td\te\tf\tg
```

Output

```
12345678901234567890123456789
a  b  c   d   e      f      g
```

#### Bugs
Doesn't make any allowance for back space characters.
The handling of embedded tabs by **entab** is a hack at best.

#### License
[Creative Commons CC0](http://creativecommons.org/publicdomain/zero/1.0/legalcode)
