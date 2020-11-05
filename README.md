# CECS503_project
Project for CECS 503 - implement word autocomplete using a trie structure

This is old code I have from school, the assignment was:
"implement word autocomplete like you would find on Google using a Trie structure. Use the attached dictionary.txt as your word base"

I did just that, and implemented Levenshtein distance in cpp to determine the closest words. Is this the best solution? No, but it's very funny to imagine this being the implementation.

The code walks to the leaf of every trie and works backwards and inserts every possible similar word into an array, then calculates levenshtein distance for every one, sorts by lowest, and outputs the top 3.

There's probably an easier way, but a proper solution was never given
