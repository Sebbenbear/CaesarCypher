#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer (c) 2014-15.
   Uses a modified caeser I & II (in 2015)

   Compile:  gcc -o decode decode.c

   See encode for  examples on using it
 */

static const int kAlphabetLength = 26;

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}
void fixkey(char* s){
  size_t length = strlen(s);
  int j = 0; // assume key < length of alphabet, local array on stack, will go away!

  for(int i = 0; i < length; ++i){
    if(isalpha(s[i])){
      s[j++] = upcase(s[i]);
    }
  }

  s[j] = '\0'; 
}

/* Remove the duplicates from the key */
void removeDuplicates(char* s) {
  int j = 0;
  size_t length = strlen(s);
  
  char present[kAlphabetLength];
  for(int i = 0; i < kAlphabetLength; ++i) {
    present[i] = 0;
  }
  
  for(int i = 0; i < length; ++i) {
    //check if the duplicate has been seen before
    
    char c = upcase(s[i]);
    
    if(present[c-'A'] == 0) {                //if it hasn't been seen, add it
      s[j++] = c;
      present[c-'A'] = 1;
    }
  }
  
  s[j] = '\0';
}


/*
  Takes in a char, a string, and a position
*/
int in(char c, char* s, int pos){
  // Assume everything is already in the same case
  int i;

  for(i = 0; i < pos; i++)
    if(c == s[i]) return 1;
    
  return 0;
} 


void buildtable (char* key, char* encode, char* decode){ // this changed from encode
  
  // This function needs to build an array of mappings in 'encode' from plaintext characters
  // to encihered characters.

  // You are implementing a Caesar 1 & 2 combo Cypher as given in the lab handout.
  // Your code here:

  // probably need to declare some stuff here!
  size_t offset = strlen(key);  //Get the offset in the alphabet

  fixkey(key); // fix the key, i.e., uppercase and remove whitespace and punctuation

  removeDuplicates(key);  //remove the duplicates

  size_t keyLen = strlen(key);  //take the length of the key, dups/punctuation removed

  size_t outPutOffSet = offset - 1;  //-1 to account for array indexing at 0
  //initialise array to check for duplicates in alphabet - this is the table to look up
  char present[kAlphabetLength];
  for(int i = 0; i < kAlphabetLength; ++i) {
    present[i] = 0;
  }

  char lastChar = '\0'; 

  //LOOPS AROUND AND CONSTRUCTS THE ENCODING
    for (int i = 0; i < kAlphabetLength; ++i) {
    
      if (i < keyLen) {
      // Write key
        lastChar = key[i];
      }
      else {
      // Write remaining letters
      
        do {
          // Find next letter
          lastChar = (lastChar + 1 - 'A') % kAlphabetLength + 'A';
        
        //check for duplicates
        } while (present[lastChar -'A']);
      }
    
    present[lastChar - 'A'] = 1;  //Update the map
    
    encode[outPutOffSet] = lastChar;
    outPutOffSet = (outPutOffSet + 1) % kAlphabetLength;  //make sure it doesn't run off the end
  }
  //NOW JUST PUT THE ENCODING IN ALPHABETICAL ORDER - E.G. O WOULD BE INDEXED AT 0, WHERE A SHOULD BE

  char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for(int i = 0; i < kAlphabetLength; ++i){  //for every letter of the alphabet
    for(int j = 0; j < kAlphabetLength; ++j){
      if(alphabet[i] == encode[j]){
        decode[i] = alphabet[j];
      }
    }
  }



  // the simplest way to do this is to do exactly the same as you did when creating the 
  // encode table, and then look up the encode table to get the translations, and build the
  // decode table from this.  This isn't the most efficient approach, but it will get the 
  // job done unless you want to be fancy.


}

int main(int argc, char **argv){
  // format will be: 'program' key {encode|decode}
  // We'll be using stdin and stdout for files to encode and decode.

  // first allocate some space for our translation table.

  char* encode = (char*)malloc(sizeof(char)*26); // this changed from encode
  char* decode = (char*)malloc(sizeof(char)*26); // this changed from encode

  char ch;

  if(argc != 2){
    fprintf(stderr,"format is: '%s' key", argv[0]);
    exit(1);
  }

  // Build translation tables, and ensure key is upcased and alpha chars only.
  printf("DECODE - Argv[1]: %s decode: %s \n", argv[1], decode);
  buildtable(argv[1], encode, decode); // this changed from encode

  // write the key to stderr (so it doesn't break our pipes)

  fprintf(stderr,"key: %s - %d\n", decode, strlen(decode));


  // the following code does the translations.  Characters are read 
  // one-by-one from stdin, translated and written to stdout.

    ch = fgetc(stdin);
    while (!feof(stdin)) {
      if(isalpha(ch))          // only decrypt alpha chars
  fputc(decode[ch-'A'], stdout);
     else 
  fputc(ch, stdout);
      ch = fgetc(stdin);      // get next char from stdin
    }
}
