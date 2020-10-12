# Chorded Keyboards

A chorded keyboard is a a keyboard that uses the simultaneous pressing ("chording") of several keys to emulate a single keystroke. This allows the chorded keyboard to achieve the same coverage as a full-sized keyboard with a minimal number of keys- the entire ASCII range, for example, may be accessed with as few as 7 keys.

![The keyboard](https://github.com/yvnat/chorded-keyboard/blob/main/chordedkeyboard.png)

My keyboard uses four columns of two keys each. Each column is composed of a high key and a low key, and only one of the two is ever used for a given chord. This causes each column to behave as a virtual ternary key, giving a total of  3<sup>4</sup> = 81 possible keystrokes. An additional shift key at the bottom of the keyboard, operated independently of the chords, provides access to the capital letters and symbols that would be accessed with shift on a QWERTY keyboard. This keyboard has almost the entire range of a full-sized keyboard, significantly missing the numpad and the function keys due to its limited prototype chord map.

The keyboard permits keystrokes with no minimum press length, as well as reliable sustained holding and releasing of keys. It supports this for combinations of keystrokes (such as Ctrl + C) as well, through the usage of a "combo" chord that demarkates combinations of keystrokes.

# Chord Map
These are the chords that correspond to each keystroke
```
O = pressed
. = unpressed

....
...O : Right Arrow

....
..O. : Up Arrow

....
.O.. : Down Arrow

....
O... : Left Arrow

...O 
.... : 1

..O. 
.... : 2

..OO 
.... : 3

.O.. 
.... : 4

.O.O 
.... : 5

.OO. 
.... : 6

.OOO 
.... : 7

O... 
.... : 8

O..O 
.... : 9

O.O. 
.... : 0

.... 
..OO : Space

.... 
.O.O : Delete

.... 
O..O : Return

.... 
O.O. : Backspace

.... 
OO.. : Tab

.... 
OOOO : Combo

..O. 
.O.. : a

.O.. 
..OO : b

.O.. 
O..O : c

..O. 
OO.. : d

..O. 
...O : e

.OO. 
O... : f

.O.. 
...O : g

..O. 
O... : h

..OO 
.O.. : i

..OO 
OO.. : j

..O. 
O..O : k

.O.. 
O.OO : l

.OOO 
O... : m

.OO. 
O..O : n

.OO. 
...O : o

.O.. 
..O. : p

...O 
OOO. : q

.O.. 
O... : r

..OO 
O... : s

..O. 
.O.O : t

..O. 
OO.O : u

.O.. 
O.O. : v

.O.O 
O... : w

.O.O 
O.O. : x

.O.O 
..O. : y

...O 
OO.. : z 

O... 
.OO. : [

O... 
..OO : ]

O... 
...O : .

O... 
..O. : ,

O... 
.O.. : ;

O... 
.O.O : '

O..O 
.OO. : `

O... 
.OOO : /

...O 
O.O. : \

O..O 
..O. : Left GUI

O..O 
.O.. : Right GUI

O.OO 
.... : Left Alt

OO.O 
.... : Left Control

OOO. 
.... : Escape

OOOO 
.... : Capslock
```
# Circuit Diagram

![circuit diagram](https://github.com/yvnat/chorded-keyboard/blob/main/diagram.png)
