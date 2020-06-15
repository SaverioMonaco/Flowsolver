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

First of all, all pairs (distinguished by colors) must be numerically ordered. Then the program will find all paths for the first pair, at the very moment it finds the first path, the program will try to find all path for the second pair with the presence of the first path as an obstacle and so on. If no paths are found, the program will search for the next paths for the previous pair.

In the example before the puzzle is solved by the program in this order:

<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/step0.png"
  width="128" height="128">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
    width="32" height="32">
<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/step1.png"
  width="128" height="128">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
    width="32" height="32">
<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/step2.png"
  width="128" height="128">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
    width="32" height="32">

  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
    width="32" height="32">
<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/step3.png"
  width="128" height="128">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
    width="32" height="32">
<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/step4.png"
  width="128" height="128">

Of course the program didn't find the solution on the first try, for example a valid path for the yellow pair could have been:

<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/stepfailed.png"
  width="128" height="128">

But the blue pair won't find a solution, so the program will find other solutions for the yellow one.

The biggest problem to address now is how can one find all possible paths that connects a pair in a grid with the presence of obstacles (paths of other pairs)

<h2> Finding all paths for one pair </h2>
Deciding from which dot of the pair to start, the maximum possible movement from there are 4: up, down, left, right (that can not be valid if on the place is occupied by a path, a dot of a pair, or if the movement leads the path out of the grid), from there, the possible movement are 3 because going back is not possible.

One can imagine to deal with a Tree, so the best approach is the technique is probably Backtracking.

Setting an arbitrary hierarchy of direction (like, up>left>down>right), the backtracking would be:
<ul>
  <li>If avaible, always go up</li>
  <li>If up is not avaible, go left</li>
  <li>If neither up or left are avaible, go down</li>
  <li>If up, left and down are not possible, go right</li>
  <li>If no movement is possible, go back and try the next direction</li>
</ul>

This way of visualising all possible way to find a path connecting two dots in a grid shows how easy it is to deal with an incredible high number of possibilities. For example, the program calculated the number of paths connecting two dots placed on the end of one diagonal in a squared grid for different size of the grid:

<table style="width:100%">
  <tr>
    <th>Side</th>
    <th>Number of paths</th>
  </tr>
  <tr>
   <td>2</td>
   <td>2</td>
  </tr>
  <tr>
   <td>3</td>
   <td>12</td>
  </tr>
  <tr>
   <td>4</td>
   <td>184</td>
  </tr>
  <tr>
   <td>5</td>
   <td>8512</td>
  </tr>
  <tr>
   <td>6</td>
   <td>1262816</td>
  </tr>
  <tr>
   <td>7</td>
   <td>575780564</td>
  </tr>
  <img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/npaths.png"
    width="128" height="128">


Given the amount of paths the program has to deal with, it is essential to find ways to trim some Tree branches.

<h2> Trimming the tree </h2>
Consider the following case:

<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/trim0.png"
  width="128" height="128">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/12/Right_arrow.svg/434px-Right_arrow.svg.png"
    width="32" height="32">
<img src="https://gitlab.com/saveriomonaco97/flowsolver/-/raw/master/readmeimgs/trim1.png"
  width="128" height="128">

Although the movement is possible, it is clear that will not lead to a solution because the other two pairs will not have enough space to connect (<em>there must be at least 2 spaces to connect those 2 pairs</em>)
