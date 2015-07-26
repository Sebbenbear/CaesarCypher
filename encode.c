//
//  encode-skel.c
//  
//
//  Created by Pauline Kelly on 26/07/15.
//
//

#include "encode.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
 Author: Kris Bubendorfer (c) 2015.
 Uses a modified caeser I & II
 
 Compile:  gcc -o encode encode.c
 
 // Make a file with a plain aplhabet in it:
 
 % cat > test
 ABCDEFGHIJKLMNOPQRSTUVWXYZ
 ^D
 
 // test it - this should be the correct result when encoding.
 
 % cat test | encode "i came, i saw"
 key: HJKLNOPQRTUVICAMESWXYZBDFG - 26
 HJKLNOPQRTUVICAMESWXYZBDFG
 
 */

static const int kAlphabetLength = 26;

char upcase(char ch){
	if(islower(ch))
		ch -= 'a' - 'A';
	return ch;
}

void fixkey(char* s) {
	size_t length = strlen(s);
	int j = 0;
	
	for(int i = 0; i < length; ++i) {
		if(isalpha(s[i])) {
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

void buildtable (char* key, char* encode){
	
	// This function needs to build an array of mappings in the 'encode' array from plaintext characters
	// to encypered characters.  The encode array will be indexed by the plaintext char.  To
	// make this a useful 0-26 index for the array, 'A' will be stubtracted from it (yes you
	// can do this in C).  You can see this in the main(){} below.  The values in the array
	// will be the cipher value, in the example at the top A -> H, B -> J, etc.
	
	// You are implementing a Caesar 1 & 2 combo Cypher as given in handout.
	// Your code here:
	
	// probably need to declare some stuff here!
	
	//"i came, i saw" has length 13, so the offset starts at 13/26.
	size_t offset = strlen(key);  //get the length of the code for cypher 1. Initial offset, start at this index
	
	fixkey(key); // fix the key, i.e., uppercase and remove whitespace and punctuation
	//now remove duplicates and get the total length
	removeDuplicates(key);
	
	size_t keyLen = strlen(key);
	
	size_t outPutOffSet = offset -1;
	
	//initialise array to check for duplicates in alphabet
	char present[kAlphabetLength];
	for(int i = 0; i < kAlphabetLength; ++i) {
		present[i] = 0;
	}
	
	char lastChar = '\0';  //last character we dealt with
	
	for (int i = 0; i < kAlphabetLength; ++i) {
		//keep a track of what's been seen
		
		
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
}

int main(int argc, char **argv){
	// format will be: 'program' key {encode|decode}
	// We'll be using stdin and stdout for files to encode and decode.
	
	// first allocate some space for our translation table.
	
	char* encode = (char*)malloc(sizeof(char)*27);
	encode[26] = '\0';
	char ch;
	
	if(argc != 2){
		printf("format is: '%s' key\n", argv[0]);
		exit(1);
	}
	
	// Build translation tables, and ensure key is upcased and alpha chars only.
	
	buildtable(argv[1], encode);
	
	// write the key to stderr (so it doesn't break our pipes)
	
	fprintf(stderr,"key: %s - %lu\n", encode, strlen(encode));
	
	// the following code does the translations.  Characters are read
	// one-by-one from stdin, translated and written to stdout.
	
	ch = fgetc(stdin);
	while (!feof(stdin)) {
		if(isalpha(ch)){        // only encrypt alpha chars
			ch = upcase(ch);      // make it uppercase
			fputc(encode[ch-'A'], stdout);
		}else
			fputc(ch, stdout);
		ch = fgetc(stdin);      // get next char from stdin
	}
}
