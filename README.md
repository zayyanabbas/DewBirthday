 # O(1) Tic-Tac-Toe AI
  ###### with O(n^d) space complexity ;)
  
  This was mainly made for fun, because the idea of an O(1) unbeatable AI is really funny to me. Fortunately for me, C++ is Turing-complete at compile time, meaning that this was technically possible! It generates a lookup table at compile time, using a standard minimax algorithm, which can then be used in runtime to find the optimal move in any given gamestate.
  
  Notably, the program uses 2 integers to represent the game state. One represents all the changes made to the board, and one represents all of the moves of a given player (the AI). The binary values of the integers represent whether or not there is a mark on a particular position in the board. For example, if the moves being tracked were of X, the values would look like this:
  
	  TEXT	    BOARD       MOVES
	  X O -     1 1 0       1 0 0
	  O - X     1 0 1       0 0 1
	  - O -     0 1 0       0 0 0
	  
Or put in proper binary, 110101010 and 100001000 respectively. These values would give a unique ID for any possible game state in Tic-Tac-Toe! With this in mind, a 511x511 (the max binary number) sized table was generated to calculate the best move for any game state. 

Since all of the processing has basically been moved into compile time, this isn't really much more of an efficient algorithm. This program basically replaces time with storage. It is more resource intensive during compilation, but the processing during runtime is reduced to just an array lookup which is neat!

C++ compilers are typically not built to handle this level of stupidity, so when compiling you need to put a flag to allow for enough compile-time processing:

	g++ -fconstexpr-ops-limit=166393690 src.cpp -o o.out
