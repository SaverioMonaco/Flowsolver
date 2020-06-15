#

<h1>(EN) Automatic Flow Free puzzle solver written in C and terminal-based game</h1>
Project for Metodi informatici per la Fisica a.a. 2019/2020

<h2> What is Flow Free? </h2>
Flow Free is a popular puzzle game developed for Android and iOS.

The goal of this game is to connect all pair of dots (distinguished by colors) creating pipes in the given grid,
for example:

<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/example1.jpg"
  width="256" height="256">
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
  width="64" height="64">
<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/example1solved.jpg"
  width="256" height="256">

<h2> Goal of the program </h2>
Unlike the popular game, where there exists only one solution for each puzzle, this program can find all existing solutions and display them.

<h2> How the program works </h2>
To find all solutions, the algorithm must be less holistic than other solutions given online, a rule to enumerate all possible paths must be found.
