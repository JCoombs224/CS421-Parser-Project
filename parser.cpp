#include<iostream>
#include<fstream>
#include<string>
using namespace std;

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/
//=====================================================
// File scanner.cpp written by: Group Number: 14
//=====================================================

// --------- Helper Functions ---------------------------------

// Check if character is a starting consonant
bool isConsonantStart(char c)
{
  return (c == 'b' || c == 'g' || c == 'h' || c == 'k' || c == 'm' || c == 'n' || c == 'p' || c == 'r');
}

// Check if character is ending consonant
bool isConsonantEnd(char c)
{
  return (c == 'd' || c == 'w' || c == 'z' || c == 'y' || c == 'j');
}

// Check if character is a vowel
bool isVowel(char c)
{
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'E' || c == 'I');
}

// --------- Two DFAs ---------------------------------

// WORD DFA
// Done by: Jamison Coombs
// RE: (V | dwzyj V | c h V | s V | s h V | t V | t h V | bghkmnpr V | bghkmnpr y V | V n | dwzyj V n 
//      | c h V n | s V n | s h V n | t V n | t h V n | bghkmnpr V n | bghkmnpr y V n)^+            
bool word(string s)
{
  int state = 0; // set state to 0
  int charpos = 0; // set character position to 0

  while (s[charpos] != '\0')
  {
    char c = s[charpos]; // set char variable to character position
    if (state == 0) // ~~~~~~~~~~~~ State 0 (q0) ~~~~~~~~~~~~
    {
      if (isConsonantStart(c)) // bghkmnpr
        state = 6;
      else if (isConsonantEnd(c)) // dwzj
        state = 5;
      else if (c == 's') // s
        state = 4;
      else if (c == 't') // t
        state = 2;
      else if (isVowel(c)) // aeiouIE
        state = 1;
      else // Error state
        state = -1;
    }
    else if (state == 1) // ~~~~~~~~~~~~ State 1 (q0q1) ~~~~~~~~~~~~
    {
      if (state == 1 && isVowel(c)) // aeiouIE
        state = 1;
      else if (c == 'n') // n
        state = 3;
      else if (isConsonantEnd(c)) // dwzj
        state = 5;
      else if (isConsonantStart(c)) // bghkmnpr
        state = 6;
      else if (c == 's') // s
        state = 4;
      else if (c == 't') // t
        state = 2;
      else // error state
        state = -1;
    }
    else if (state == 2) // ~~~~~~~~~~~~ State 2 (qt) ~~~~~~~~~~~~
    { 
      if (isVowel(c)) // aeiouIE
        state = 1;
      else if (c == 's') // s
        state = 5;
      else // error state
        state = -1;
    }
    else if (state == 3) // ~~~~~~~~~~~~ State 3 (q0qy) ~~~~~~~~~~~~
    {
      if (isVowel(c)) // aeiouIE
        state = 1;
      else if (c == 't') // t
        state = 2;
      else if (c == 's') // s
        state = 4;
      else if (c == 'c') // c
        state = 7;
      else if (isConsonantStart(c)) // bghkmnpr
        state = 6;
      else if (isConsonantEnd(c)) // dwzj
        state = 5;
      else // error state
        state = -1;
    }
    else if (state == 4) // ~~~~~~~~~~~~ State 4 (qs) ~~~~~~~~~~~~
    {
      if (isVowel(c)) // aeiouIE
        state = 1;
      else if (c == 'h') // h
        state = 5;
      else // error state
        state = -1;
    }
    else if (state == 5) // ~~~~~~~~~~~~ State 5 (qsa) ~~~~~~~~~~~~
    {
      if (isVowel(c)) // aeiouIE
        state = 1;
      else // error state
        state = -1;
    }
    else if (state == 6) // ~~~~~~~~~~~~ State 6 (qy) ~~~~~~~~~~~~
    {
      if (isVowel(c)) // aeiouIE
        state = 1;
      else if (c == 'y') // y
        state = 5;
      else // error state
        state = -1;
    }
    else if (state == 7) // ~~~~~~~~~~~~ State 7 (qc) ~~~~~~~~~~~~
    {
      if (c == 'h') // h
        state = 5;
      else // error state
        state = -1;
    }
    else // ~~~~~~~~~~~~ Error State ~~~~~~~~~~~~
    {
      return false;
    }

    charpos++; // increment character position
  } // End of while

  // where did I end up????
  if (state == 0 || state == 1 || state == 3) // Check if ended in valid end state
    return true; // if so passes dfa
  else
    return false; // if not fails dfa
}

// PERIOD DFA
// Done by: Jamison Coombs
bool period(string s)
{
  int state = 0; // set state to 0
  int charpos = 0; // set char state to 0
  while (s[charpos] != '\0')
  {
    char c = s[charpos]; // set char variable to char at char position

    if (state == 0 && c == '.') // check if the current char is a '.'
      state = 1;
    else
      return false; // return false if not

    charpos++; // increment char pos
  } // End of loop

  if (state == 1) // check that string ends in a valid ending state
    return true; // if so passes dfa
  else
    return false; // if not fails dfa
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Jamison Coombs, Lilly Mcgraw, Tim Nguyen

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype
{
  WORD1,
  WORD2,
  PERIOD,
  ERROR,
  EOFM,
  VERB,
  VERBNEG,
  VERBPAST,
  VERBPASTNEG,
  IS,
  WAS,
  OBJECT,
  SUBJECT,
  DESTINATION,
  PRONOUN,
  CONNECTOR
};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {
  "WORD1",
  "WORD2",
  "PERIOD",
  "ERROR",
  "EOFM",
  "VERB",
  "VERBNEG",
  "VERBPAST",
  "VERBPASTNEG",
  "IS",
  "WAS",
  "OBJECT",
  "SUBJECT",
  "DESTINATION",
  "PRONOUN",
  "CONNECTOR"
};

// ** Need the reservedwords table to be set up here.
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.

/**
 * @brief Check to see if a valid word from the word DFA is a reserve word.
 *        If a reserve word match is found set the token type to the 
 *        corresponding token type as found in reservedwords.txt
 * 
 * @param tt - Reference to the current token type
 * @param w - Reference to the current word
 * @return true - Reserved word found
 * @return false - No reserve word found
 */
bool isReserveWord(tokentype &tt, string &w)
{
  if(w == "masu") // check for reserved word "masu"
    tt = VERB; // set token type to verb
  else if(w == "masen") // check for reserved word "masen"
    tt = VERBNEG; // set token type to verb negative
  else if(w == "mashita") // check for reserved word "mashita"
    tt = VERBPAST; // set token type to verb past
  else if(w == "masendeshita") // check for reserved word "masendeshita"
    tt = VERBPASTNEG; // set token type to verb past neg
  else if(w == "desu") // check for reserved word "desu"
    tt = IS; // set token type to is
  else if(w == "deshita") // check for reserved word "deshita"
    tt = WAS; // set token type to was
  else if(w == "o") // check for reserved word "o"
    tt = OBJECT; // set token type to object
  else if(w == "wa") // check for reserved word "wa"
    tt = SUBJECT; // set token type to subject
  else if(w == "ni") // check for reserved word "ni"
    tt = DESTINATION; // set token type to destination
  else if(w == "watashi") // check for reserved word "watashi"
    tt = PRONOUN; // set token type to pronoun
  else if(w == "anata") // check for reserved word "anata"
    tt = PRONOUN; // set token type to pronoun
  else if(w == "kare") // check for reserved word "kare"
    tt = PRONOUN; // set token type to pronoun
  else if(w == "kanojo") // check for reserved word "kanojo"
    tt = PRONOUN; // set token type to pronoun
  else if(w == "sore") // check for reserved word "sore"
    tt = PRONOUN; // set token type to pronoun
  else if(w == "mata") // check for reserved word "mata"
    tt = CONNECTOR; // set token type to connector
  else if(w == "soshite") // check for reserved word "soshite"
    tt = CONNECTOR; // set token type to connector
  else if(w == "shikashi") // check for reserved word "shikashi"
    tt = CONNECTOR; // set token type to connector
  else if(w == "dakara") // check for reserved word "dakara"
    tt = CONNECTOR; // set token type to connector
  else
    return false;

  return true;
}

// ------------ Scanner and Driver -----------------------

ifstream fin; // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Jamison Coombs, Lilly Mcgraw, Tim Nguyen
int scanner(tokentype &tt, string &w)
{

  /* Grab the next word from the file via fin
   1. If it is eofm, return right now.
  */

  fin >> w; // Grab the next word

  if (w == "eofm" || w == "EOFM") // Check for end of file
  {
    tt = EOFM; // Set tokentype to EOFM
    return 0;
  }

  /*
  2. Call the token functions (word and period)
     one after another (if-then-else).
     Generate a lexical error message if both DFAs failed.
     Let the tokentype be ERROR in that case.

  3. If it was a word,
     check against the reservedwords list.
     If not reserved, tokentype is WORD1 or WORD2
     decided based on the last character.

  4. Return the token type & string  (pass by reference)
  */
  else if(period(w)) // Check period DFA
  {
    tt = PERIOD; // if passes dfa set token type to period
    return 0;
  }
  else if(word(w)) // Check word DFA
  {
    // Check word against reserved words list
    if(isReserveWord(tt, w))
      return 0;
    

    char lastChar = w[w.size()-1];
    if(lastChar == 'I' || lastChar == 'E') // Check if the last char is a capital I or E
    {
      // If it is set tokentype to word2
      tt = WORD2;
      return 0;
    }
    else if(isVowel(lastChar) || (isVowel(w[w.size()-2]) && lastChar == 'n')) // Check if word ends in vowel or vowel -> 'n'
    {
      // If so set tokentype to word1
      tt = WORD1;
      return 0;
    }
    else // does not match WORD1 or WORD2
    {
      // Set token type to error
      tt = ERROR;
      return 1;
    }
  }
  else
  {
    tt = ERROR;
    return 1;
  }
} // the end of scanner

//=================================================
// File parser.cpp written by Group Number: **
//=================================================

// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.  

// Type of error: **
// Done by: ** 
void syntaxerror1(  ){    }
// Type of error: **
// Done by: ** 
void syntaxerror2(  ) {    }

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: **
// Done by: **
tokentype next_token(){}

// Purpose: **
// Done by: **
bool match(tokentype expected) {}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

// Grammar: **
// Done by: **

string filename;

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by:  **
int main()
{
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  //** closes the input file 

}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
