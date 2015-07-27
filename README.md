# CaesarCypher
Caesar Cypher in C

The	Modified	Cypher

The modified Caeser you will be implementing is a combination of Caeser cypher I and II.
Caeser cypher I used an offset alphabet, where the mapping between plain and enciphered characters
was determined by a numerical offset, in the following example 13, so A maps to M:

Plain A B C D E F G H …
Enciphered M N O P Q R S T …

The Caeser cypher II uses a ‘key word’ where it is placed at the start of the encipher mapping – sans
duplicates, the key in this example is “I came, I saw” (without punctuation or whitespace etc.):
Plain A B C D E F G H …
Enciphered I C A M E S W X …

The next letter in the encipher mapping is the next non duplicate letter of the alphabet – in this case X
follows W, and it is not a duplicate. This is followed by Y, Z, A, B, … Note A is not used as it is a
duplicate.

In our modified cypher, what we will do is use an offset (like cypher I) which will be the length of the
key (including duplicates and whitespace and punctuation, i.e., the key “I came, I saw” has a total length
of 13. From this offset of 13, we will then apply a normal Caeser cypher II, as in the following example:

1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
H J K L N O P Q R T U V I C A M E S W X Y Z B D F G

Note: numbers included only for your reference
So, ‘Hello’ – would encode to: ‘QNVVA’
