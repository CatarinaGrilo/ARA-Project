To compile the program: make

Execute the program: ./graphs [File.txt] -[simulation/algorithm][mode]


To run the various modes:

Simulation mode (widest-shortest): ./graphs example.txt -sl
Simulation mode (shortest-widest): ./graphs example.txt -sw

Interactive Simulation mode (widest-shortest): ./graphs example.txt -sil [Source] [Destiny]
Interactive Simulation mode (shortest-widest): ./graphs example.txt -siw [Source] [Destiny]

Algorithm mode (widest-shortest):  ./graphs example.txt -al
Algorithm mode (shortest-widest):  ./graphs example.txt -aw

Interactive Simulation mode (widest-shortest): ./graphs example.txt -ail [Source] [Destiny]
Interactive Simulation mode (shortest-widest): ./graphs example.txt -aiw [Source] [Destiny]

Corrected Algorithm mode for shortest-widest:  ./graphs example.txt -ao

nteractive Corrected Algorithm (shortest-widest): ./graphs example.txt -aio [Source] [Destiny]