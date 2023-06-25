# Expand tabs into spaces

Simple program to expand tabs set at specified positions into spaces.

I had a recent need to do this but couldn't find an obvious program but I vaguely remembered converting a small program from *Software Tools in Pascal*  (Kernighan and  Plauger, 1981) to Small C on a CP/M 3 computer.  Couldn't be that hard.

#### Synopsis

**detab**  *[-T repeat]*  *[-t stop1,stop2,stop3,...]*  *[filename]*

Set tab stops at positions **stop1 stop2 stop3** columns and at every **repeat** column thereafter.

#### Example

`detab -T 7 -t 3,6,10`

Set tab stops at columns **3, 6** and **10 **then **14, 21, 28** et seq.

Input

```C
12345678901234567890123456789
a\tb\tc\td\te\tf\t\g
```

Output

```
12345678901234567890123456789
a  b  c   d   e      f      g
```

