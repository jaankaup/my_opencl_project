// The tritable.
__constant float triTable[3840] =
   {255.0, 255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,  
      0.0,   8.0,   3.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,     
      0.0,   1.0,   9.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,     
      1.0,   8.0,   3.0,    9.0,  8.0,    1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 
      1.0,   2.0,  10.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 
      0.0,   8.0,   3.0,    1.0,  2.0,   10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 
      9.0,   2.0,  10.0,    0.0,  2.0,    9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 
      2.0,   8.0,   3.0,    2.0, 10.0,    8.0,  10.0,   9.0,   8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 
      3.0,  11.0,   2.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 
      0.0,  11.0,   2.0,    8.0, 11.0,    0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,
      1.0,   9.0,   0.0,    2.0,  3.0,   11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,
      1.0,  11.0,   2.0,    1.0,  9.0,   11.0,   9.0,   8.0,  11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,
      3.0,  10.0,   1.0,   11.0, 10.0,    3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,
      0.0,  10.0,   1.0,    0.0,  8.0,   10.0,   8.0,  11.0,  10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0,
      3.0,   9.0,   0.0,    3.0, 11.0,    9.0,  11.0,  10.0,   9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   8.0,  10.0,   10.0,  8.0,   11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   7.0,   8.0,  255.0,255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   3.0,   0.0,    7.0,  3.0,    4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   1.0,   9.0,    8.0,  4.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   1.0,   9.0,    4.0,  7.0,    1.0,   7.0,   3.0,   1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,  10.0,    8.0,  4.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   4.0,   7.0,    3.0,  0.0,    4.0,   1.0,   2.0,  10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   2.0,  10.0,    9.0,  0.0,    2.0,   8.0,   4.0,   7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,  10.0,   9.0,    2.0,  9.0,    7.0,   2.0,   7.0,   3.0,   7.0,  9.0,  4.0, 255.0, 255.0, 255.0 ,
      8.0,   4.0,   7.0,    3.0, 11.0,    2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     11.0,   4.0,   7.0,   11.0,  2.0,    4.0,   2.0,   0.0,   4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   0.0,   1.0,    8.0,  4.0,    7.0,   2.0,   3.0,  11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   7.0,  11.0,    9.0,  4.0,   11.0,   9.0,  11.0,   2.0,   9.0,  2.0,  1.0, 255.0, 255.0, 255.0 ,
      3.0,  10.0,   1.0,    3.0, 11.0,   10.0,   7.0,   8.0,   4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,  11.0,  10.0,    1.0,  4.0,   11.0,   1.0,   0.0,   4.0,   7.0, 11.0,  4.0, 255.0, 255.0, 255.0 ,
      4.0,   7.0,   8.0,    9.0,  0.0,   11.0,   9.0,  11.0,  10.0,  11.0,  0.0,  3.0, 255.0, 255.0, 255.0 ,
      4.0,   7.0,  11.0,    4.0, 11.0,    9.0,   9.0,  11.0,  10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   4.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   4.0,    0.0,  8.0,    3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   5.0,   4.0,    1.0,  5.0,    0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   5.0,   4.0,    8.0,  3.0,    5.0,   3.0,   1.0,   5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,  10.0,    9.0,  5.0,    4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   0.0,   8.0,    1.0,  2.0,   10.0,   4.0,   9.0,   5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,   2.0,  10.0,    5.0,  4.0,    2.0,   4.0,   0.0,   2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,  10.0,   5.0,    3.0,  2.0,    5.0,   3.0,   5.0,   4.0,   3.0,  4.0,  8.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   4.0,    2.0,  3.0,   11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,  11.0,   2.0,    0.0,  8.0,   11.0,   4.0,   9.0,   5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   5.0,   4.0,    0.0,  1.0,    5.0,   2.0,   3.0,  11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   1.0,   5.0,    2.0,  5.0,    8.0,   2.0,   8.0,  11.0,   4.0,  8.0,  5.0, 255.0, 255.0, 255.0 ,
     10.0,   3.0,  11.0,   10.0,  1.0,    3.0,   9.0,   5.0,   4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   9.0,   5.0,    0.0,  8.0,    1.0,   8.0,  10.0,   1.0,   8.0, 11.0, 10.0,255.0, 255.0, 255.0 ,
      5.0,   4.0,   0.0,    5.0,  0.0,   11.0,   5.0,  11.0,  10.0,  11.0,  0.0,  3.0, 255.0, 255.0, 255.0 ,
      5.0,   4.0,   8.0,    5.0,  8.0,   10.0,  10.0,   8.0,  11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   7.0,   8.0,    5.0,  7.0,    9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   3.0,   0.0,    9.0,  5.0,    3.0,   5.0,   7.0,   3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   7.0,   8.0,    0.0,  1.0,    7.0,   1.0,   5.0,   7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   5.0,   3.0,    3.0,  5.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   7.0,   8.0,    9.0,  5.0,    7.0,  10.0,   1.0,   2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   1.0,   2.0,    9.0,  5.0,    0.0,   5.0,   3.0,   0.0,   5.0,  7.0,  3.0, 255.0, 255.0, 255.0 ,
      8.0,   0.0,   2.0,    8.0,  2.0,    5.0,   8.0,   5.0,   7.0,  10.0, 5.0,  2.0, 255.0, 255.0, 255.0 ,
      2.0,  10.0,   5.0,    2.0,  5.0,    3.0,   3.0,   5.0,   7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      7.0,   9.0,   5.0,    7.0,  8.0,    9.0,   3.0,  11.0,   2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   7.0,    9.0,  7.0,    2.0,   9.0,   2.0,   0.0,   2.0,  7.0, 11.0, 255.0, 255.0, 255.0 ,
      2.0,   3.0,  11.0,    0.0,  1.0,    8.0,   1.0,   7.0,   8.0,   1.0,  5.0,  7.0, 255.0, 255.0, 255.0 ,
     11.0,   2.0,   1.0,   11.0,  1.0,    7.0,   7.0,   1.0,   5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   8.0,    8.0,  5.0,    7.0,  10.0,   1.0,   3.0,  10.0, 3.0, 11.0, 255.0, 255.0, 255.0 ,
      5.0,   7.0,   0.0,    5.0,  0.0,    9.0,   7.0,  11.0,   0.0,   1.0,  0.0, 10.0,11.0, 10.0, 0 ,
     11.0,  10.0,   0.0,   11.0,  0.0,    3.0,  10.0,   5.0,   0.0,   8.0,  0.0,  7.0,  5.0,  7.0,  0 ,
     11.0,  10.0,   5.0,    7.0, 11.0,    5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   6.0,   5.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,   3.0,    5.0, 10.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   0.0,   1.0,    5.0, 10.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   8.0,   3.0,    1.0,  9.0,    8.0,   5.0,  10.0,   6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   6.0,   5.0,    2.0,  6.0,    1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   6.0,   5.0,    1.0,  2.0,    6.0,   3.0,   0.0,   8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   6.0,   5.0,    9.0,  0.0,    6.0,   0.0,   2.0,   6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,   9.0,   8.0,    5.0,  8.0,    2.0,   5.0,   2.0,   6.0,   3.0,  2.0,  8.0, 255.0, 255.0, 255.0 ,
      2.0,   3.0,  11.0,   10.0, 6.0,     5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     11.0,   0.0,   8.0,   11.0,  2.0,    0.0,  10.0,   6.0,   5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   1.0,   9.0,    2.0,  3.0,   11.0,   5.0,  10.0,   6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,  10.0,   6.0,    1.0,  9.0,    2.0,   9.0,  11.0,   2.0,   9.0,  8.0, 11.0, 255.0, 255.0, 255.0 ,
      6.0,   3.0,  11.0,    6.0,  5.0,    3.0,   5.0,   1.0,   3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,  11.0,    0.0, 11.0,    5.0,   0.0,   5.0,   1.0,   5.0, 11.0,  6.0, 255.0, 255.0, 255.0 ,
      3.0,  11.0,   6.0,    0.0,  3.0,    6.0,   0.0,   6.0,   5.0,   0.0,  5.0,  9.0, 255.0, 255.0, 255.0 ,
      6.0,   5.0,   9.0,    6.0,  9.0,   11.0,  11.0,   9.0,   8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,  10.0,   6.0,    4.0,  7.0,    8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   3.0,   0.0,    4.0,  7.0,    3.0,   6.0,   5.0,  10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   9.0,   0.0,    5.0, 10.0,    6.0,   8.0,   4.0,   7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   6.0,   5.0,    1.0,  9.0,    7.0,   1.0,   7.0,   3.0,   7.0,  9.0,  4.0, 255.0, 255.0, 255.0 ,
      6.0,   1.0,   2.0,    6.0,  5.0,    1.0,   4.0,   7.0,   8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,   5.0,    5.0,  2.0,    6.0,   3.0,   0.0,   4.0,   3.0,  4.0,  7.0, 255.0, 255.0, 255.0 ,
      8.0,   4.0,   7.0,    9.0,  0.0,    5.0,   0.0,   6.0,   5.0,   0.0,  2.0,  6.0, 255.0, 255.0, 255.0 ,
      7.0,   3.0,   9.0,    7.0,  9.0,    4.0,   3.0,   2.0,   9.0,   5.0,  9.0,  6.0,  2.0,  6.0,  9 ,
      3.0,  11.0,   2.0,    7.0,  8.0,    4.0,  10.0,   6.0,   5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,  10.0,   6.0,    4.0,  7.0,    2.0,   4.0,   2.0,   0.0,   2.0,  7.0, 11.0, 255.0, 255.0, 255.0 ,
      0.0,   1.0,   9.0,    4.0,  7.0,    8.0,   2.0,   3.0,  11.0,   5.0, 10.0, 6.0, 255.0, 255.0, 255.0 ,
      9.0,   2.0,   1.0,    9.0, 11.0,    2.0,   9.0,   4.0,  11.0,   7.0, 11.0,  4.0,  5.0, 10.0, 6 ,
      8.0,   4.0,   7.0,    3.0, 11.0,    5.0,   3.0,   5.0,   1.0,   5.0, 11.0,  6.0, 255.0, 255.0, 255.0 ,
      5.0,   1.0,  11.0,    5.0, 11.0,    6.0,   1.0,   0.0,  11.0,   7.0, 11.0,  4.0,  0.0,  4.0, 11 ,
      0.0,   5.0,   9.0,    0.0,  6.0,    5.0,   0.0,   3.0,   6.0,  11.0,  6.0,  3.0,  8.0,  4.0,  7 ,
      6.0,   5.0,   9.0,    6.0,  9.0,   11.0,   4.0,   7.0,   9.0,   7.0, 11.0,  9.0, 255.0, 255.0, 255.0 ,
     10.0,   4.0,   9.0,    6.0,  4.0,   10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,  10.0,   6.0,    4.0,  9.0,   10.0,   0.0,   8.0,   3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   0.0,   1.0,   10.0,  6.0,    0.0,   6.0,   4.0,   0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   3.0,   1.0,    8.0,  1.0,    6.0,   8.0,   6.0,   4.0,   6.0,  1.0, 10.0,255.0, 255.0, 255.0 ,
      1.0,   4.0,   9.0,    1.0,  2.0,    4.0,   2.0,   6.0,   4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   0.0,   8.0,    1.0,  2.0,    9.0,   2.0,   4.0,   9.0,   2.0,  6.0,  4.0, 255.0, 255.0, 255.0 ,
      0.0,   2.0,   4.0,    4.0,  2.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   3.0,   2.0,    8.0,  2.0,    4.0,   4.0,   2.0,   6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   4.0,   9.0,   10.0,  6.0,    4.0,  11.0,   2.0,   3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,   2.0,    2.0,  8.0,   11.0,   4.0,   9.0,  10.0,   4.0, 10.0, 6.0, 255.0, 255.0, 255.0 ,
      3.0,  11.0,   2.0,    0.0,  1.0,    6.0,   0.0,   6.0,   4.0,   6.0,  1.0, 10.0,255.0, 255.0, 255.0 ,
      6.0,   4.0,   1.0,    6.0,  1.0,   10.0,   4.0,   8.0,   1.0,   2.0,  1.0, 11.0,  8.0, 11.0,  1 ,
      9.0,   6.0,   4.0,    9.0,  3.0,    6.0,   9.0,   1.0,   3.0,  11.0,  6.0,  3.0, 255.0, 255.0, 255.0 ,
      8.0,  11.0,   1.0,    8.0,  1.0,    0.0,  11.0,   6.0,   1.0,   9.0,  1.0,  4.0,  6.0,  4.0,  1 ,
      3.0,  11.0,   6.0,    3.0,  6.0,    0.0,   0.0,   6.0,   4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      6.0,   4.0,   8.0,   11.0,  6.0,    8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      7.0,  10.0,   6.0,    7.0,  8.0,   10.0,  8.0,    9.0,  10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   7.0,   3.0,    0.0, 10.0,    7.0,   0.0,   9.0,  10.0,   6.0,  7.0, 10.0,255.0, 255.0, 255.0 ,
     10.0,   6.0,   7.0,    1.0, 10.0,    7.0,   1.0,   7.0,   8.0,   1.0,  8.0,  0.0, 255.0, 255.0, 255.0 ,
     10.0,   6.0,   7.0,   10.0,  7.0,    1.0,   1.0,   7.0,   3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,   6.0,    1.0,  6.0,    8.0,   1.0,   8.0,   9.0,   8.0,  6.0,  7.0, 255.0, 255.0, 255.0 ,
      2.0,   6.0,   9.0,    2.0,  9.0,    1.0,   6.0,   7.0,   9.0,   0.0,  9.0,  3.0,  7.0,  3.0,  9 ,
      7.0,   8.0,   0.0,    7.0,  0.0,    6.0,   6.0,   0.0,   2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      7.0,   3.0,   2.0,    6.0,  7.0,    2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   3.0,  11.0,   10.0,  6.0,    8.0,  10.0,   8.0,   9.0,   8.0,  6.0,  7.0, 255.0, 255.0, 255.0 ,
      2.0,   0.0,   7.0,    2.0,  7.0,   11.0,   0.0,   9.0,   7.0,   6.0,  7.0, 10.0, 9.0, 10.0, 7 ,
      1.0,   8.0,   0.0,    1.0,  7.0,    8.0,   1.0,  10.0,   7.0,   6.0,  7.0, 10.0, 2.0,  3.0, 11 ,
     11.0,   2.0,   1.0,   11.0,  1.0,    7.0,  10.0,   6.0,   1.0,   6.0,  7.0,  1.0, 255.0, 255.0, 255.0 ,
      8.0,   9.0,   6.0,    8.0,  6.0,    7.0,   9.0,   1.0,   6.0,  11.0,  6.0,  3.0,  1.0,  3.0,  6 ,
      0.0,   9.0,   1.0,   11.0,  6.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      7.0,   8.0,   0.0,    7.0,  0.0,    6.0,   3.0,  11.0,   0.0,  11.0,  6.0,  0.0, 255.0, 255.0, 255.0 ,
      7.0,  11.0,   6.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      7.0,   6.0,  11.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   0.0,   8.0,   11.0,  7.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   1.0,   9.0,   11.0,  7.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   1.0,   9.0,    8.0,  3.0,    1.0,  11.0,   7.0,  6.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   1.0,   2.0,    6.0, 11.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,  10.0,    3.0,  0.0,    8.0,   6.0,  11.0,  7.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   9.0,   0.0,    2.0, 10.0,    9.0,   6.0,  11.0,  7.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      6.0,  11.0,   7.0,    2.0, 10.0,    3.0,  10.0,   8.0,  3.0,   10.0, 9.0,  8.0, 255.0, 255.0, 255.0 ,
      7.0,   2.0,   3.0,    6.0,  2.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      7.0,   0.0,   8.0,    7.0,  6.0,    0.0,   6.0,   2.0,  0.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   7.0,   6.0,    2.0,  3.0,    7.0,   0.0,   1.0,  9.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   6.0,   2.0,    1.0,  8.0,    6.0,   1.0,   9.0,  8.0,    8.0,  7.0,  6.0, 255.0, 255.0, 255.0 ,
     10.0,   7.0,   6.0,   10.0,  1.0,    7.0,   1.0,  3.0,  7.0,   255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   7.0,   6.0,    1.0,  7.0,   10.0,   1.0,  8.0,  7.0,     1.0,  0.0,  8.0, 255.0, 255.0, 255.0 ,
      0.0,   3.0,   7.0,    0.0,  7.0,   10.0,   0.0, 10.0, 9.0,  6.0, 10.0, 7.0, 255.0, 255.0, 255.0 ,
      7.0,   6.0,  10.0,    7.0, 10.0,    8.0,   8.0, 10.0, 9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      6.0,   8.0,   4.0,   11.0,  8.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   6.0,  11.0,    3.0,  0.0,    6.0,   0.0,  4.0,  6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   6.0,  11.0,    8.0,  4.0,    6.0,   9.0,  0.0,  1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   4.0,   6.0,    9.0,  6.0,    3.0,   9.0,  3.0,  1.0, 11.0,  3.0,  6.0, 255.0, 255.0, 255.0 ,
      6.0,   8.0,   4.0,    6.0, 11.0,    8.0,   2.0, 10.0, 1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,  10.0,    3.0,  0.0,   11.0,   0.0,  6.0, 11.0,  0.0,  4.0,  6.0, 255.0, 255.0, 255.0 ,
      4.0,  11.0,   8.0,    4.0,  6.0,   11.0,   0.0,  2.0,  9.0,  2.0, 10.0, 9.0, 255.0, 255.0, 255.0 ,
     10.0,   9.0,   3.0,   10.0,  3.0,    2.0,   9.0,  4.0,  3.0, 11.0,  3.0,  6.0,  4.0,  6.0,  3 ,
      8.0,   2.0,   3.0,    8.0,  4.0,    2.0,   4.0,  6.0,  2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   4.0,   2.0,    4.0,  6.0,    2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   9.0,   0.0,    2.0,  3.0,    4.0,   2.0,  4.0,  6.0,  4.0,  3.0,  8.0, 255.0, 255.0, 255.0 ,
      1.0,   9.0,   4.0,    1.0,  4.0,    2.0,   2.0,  4.0,  6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   1.0,   3.0,    8.0,  6.0,    1.0,   8.0,  4.0,  6.0,  6.0, 10.0, 1.0, 255.0, 255.0, 255.0 ,
     10.0,   1.0,   0.0,   10.0,  0.0,    6.0,   6.0,  0.0,  4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   6.0,   3.0,    4.0,  3.0,    8.0,   6.0, 10.0, 3.0,  0.0,  3.0,  9.0, 10.0, 9.0,  3 ,
     10.0,   9.0,   4.0,    6.0, 10.0,    4.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   9.0,   5.0,    7.0,  6.0,   11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,   3.0,    4.0,  9.0,    5.0,  11.0,  7.0,  6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,   0.0,   1.0,    5.0,  4.0,    0.0,   7.0,  6.0, 11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     11.0,   7.0,   6.0,    8.0,  3.0,    4.0,   3.0,  5.0,  4.0,  3.0,  1.0,  5.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   4.0,   10.0,  1.0,    2.0,   7.0,  6.0, 11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      6.0,  11.0,   7.0,    1.0,  2.0,   10.0,   0.0,  8.0,  3.0,  4.0,  9.0,  5.0, 255.0, 255.0, 255.0 ,
      7.0,   6.0,  11.0,    5.0,  4.0,   10.0,   4.0,  2.0, 10.0, 4.0,  0.0,  2.0, 255.0, 255.0, 255.0 ,
      3.0,   4.0,   8.0,    3.0,  5.0,    4.0,   3.0,  2.0,  5.0, 10.0, 5.0,  2.0, 11.0,  7.0,  6 ,
      7.0,   2.0,   3.0,    7.0,  6.0,    2.0,   5.0,  4.0,  9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   4.0,    0.0,  8.0,    6.0,   0.0,  6.0,  2.0,  6.0,  8.0,  7.0, 255.0, 255.0, 255.0 ,
      3.0,   6.0,   2.0,    3.0,  7.0,    6.0,   1.0,  5.0,  0.0,  5.0,  4.0,  0.0, 255.0, 255.0, 255.0 ,
      6.0,   2.0,   8.0,    6.0,  8.0,    7.0,   2.0,  1.0,  8.0,  4.0,  8.0,  5.0,  1.0,  5.0,  8 ,
      9.0,   5.0,   4.0,   10.0,  1.0,    6.0,   1.0,  7.0,  6.0,  1.0,  3.0,  7.0, 255.0, 255.0, 255.0 ,
      1.0,   6.0,  10.0,    1.0,  7.0,    6.0,   1.0,  0.0,  7.0,  8.0,  7.0,  0.0,  9.0,  5.0,  4 ,
      4.0,   0.0,  10.0,    4.0, 10.0,    5.0,   0.0,  3.0, 10.0, 6.0, 10.0, 7.0,  3.0,  7.0, 10.0,
      7.0,   6.0,  10.0,    7.0, 10.0,    8.0,   5.0,  4.0, 10.0, 4.0,  8.0, 10.0,255.0, 255.0, 255.0 ,
      6.0,   9.0,   5.0,    6.0, 11.0,    9.0,  11.0,  8.0,  9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   6.0,  11.0,    0.0,  6.0,    3.0,   0.0,  5.0,  6.0,  0.0,  9.0,  5.0, 255.0, 255.0, 255.0 ,
      0.0,  11.0,   8.0,    0.0,  5.0,   11.0,   0.0,  1.0,  5.0,  5.0,  6.0, 11.0, 255.0, 255.0, 255.0 ,
      6.0,  11.0,   3.0,    6.0,  3.0,    5.0,   5.0,  3.0,  1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,  10.0,    9.0,  5.0,   11.0,   9.0, 11.0,  8.0, 11.0,  5.0,  6.0, 255.0, 255.0, 255.0 ,
      0.0,  11.0,   3.0,    0.0,  6.0,   11.0,   0.0,  9.0,  6.0,  5.0,  6.0,  9.0,  1.0,  2.0, 10.0,
     11.0,   8.0,   5.0,   11.0,  5.0,    6.0,   8.0,  0.0,  5.0, 10.0, 5.0,  2.0,  0.0,  2.0,  5 ,
      6.0,  11.0,   3.0,    6.0,  3.0,    5.0,   2.0, 10.0, 3.0, 10.0, 5.0,  3.0, 255.0, 255.0, 255.0 ,
      5.0,   8.0,   9.0,    5.0,  2.0,    8.0,   5.0,  6.0,  2.0,  3.0,  8.0,  2.0, 255.0, 255.0, 255.0 ,
      9.0,   5.0,   6.0,    9.0,  6.0,    0.0,   0.0,  6.0,  2.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   5.0,   8.0,    1.0,  8.0,    0.0,   5.0,  6.0,  8.0,  3.0,  8.0,  2.0,  6.0,  2.0,  8 ,
      1.0,   5.0,   6.0,    2.0,  1.0,    6.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   3.0,   6.0,    1.0,  6.0,   10.0,   3.0,  8.0,  6.0,  5.0,  6.0,  9.0,  8.0,  9.0,  6 ,
     10.0,   1.0,   0.0,   10.0,  0.0,    6.0,   9.0,  5.0,  0.0,  5.0,  6.0,  0.0, 255.0, 255.0, 255.0 ,
      0.0,   3.0,   8.0,    5.0,  6.0,   10.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   5.0,   6.0,  255.0,255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     11.0,   5.0,  10.0,    7.0,  5.0,   11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     11.0,   5.0,  10.0,   11.0,  7.0,    5.0,   8.0,  3.0,  0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,  11.0,   7.0,    5.0, 10.0,   11.0,   1.0,  9.0,  0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     10.0,   7.0,   5.0,   10.0, 11.0,    7.0,   9.0,  8.0,  1.0,  8.0,  3.0,  1.0, 255.0, 255.0, 255.0 ,
     11.0,   1.0,   2.0,   11.0,  7.0,    1.0,   7.0,  5.0,  1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,   3.0,    1.0,  2.0,    7.0,   1.0,  7.0,  5.0,  7.0,  2.0, 11.0, 255.0, 255.0, 255.0 ,
      9.0,   7.0,   5.0,    9.0,  2.0,    7.0,   9.0,  0.0,  2.0,  2.0, 11.0,  7.0, 255.0, 255.0, 255.0 ,
      7.0,   5.0,   2.0,    7.0,  2.0,   11.0,   5.0,  9.0,  2.0,  3.0,  2.0,  8.0,  9.0,  8.0,  2 ,
      2.0,   5.0,  10.0,    2.0,  3.0,    5.0,   3.0,  7.0,  5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   2.0,   0.0,    8.0,  5.0,    2.0,   8.0,  7.0,  5.0, 10.0, 2.0,  5.0, 255.0, 255.0, 255.0 ,
      9.0,   0.0,   1.0,    5.0, 10.0,    3.0,   5.0,  3.0,  7.0,  3.0, 10.0, 2.0, 255.0, 255.0, 255.0 ,
      9.0,   8.0,   2.0,    9.0,  2.0,    1.0,   8.0,  7.0,  2.0, 10.0, 2.0,  5.0,  7.0,  5.0,  2 ,
      1.0,   3.0,   5.0,    3.0,  7.0,    5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,   7.0,    0.0,  7.0,    1.0,   1.0,  7.0,  5.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   0.0,   3.0,    9.0,  3.0,    5.0,   5.0,  3.0,  7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,   8.0,   7.0,    5.0,  9.0,    7.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,   8.0,   4.0,    5.0, 10.0,    8.0,  10.0,11.0,  8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      5.0,   0.0,   4.0,    5.0, 11.0,    0.0,   5.0, 10.0,11.0, 11.0,  3.0,  0.0, 255.0, 255.0, 255.0 ,
      0.0,   1.0,   9.0,    8.0,  4.0,   10.0,   8.0, 10.0,11.0, 10.0, 4.0,  5.0, 255.0, 255.0, 255.0 ,
     10.0,  11.0,   4.0,   10.0,  4.0,    5.0,   11.0,  3.0,  4.0,  9.0,  4.0,  1.0,  3.0,  1.0,  4 ,
      2.0,   5.0,   1.0,    2.0,  8.0,    5.0,    2.0, 11.0,  8.0,  4.0,  5.0,  8.0, 255.0, 255.0, 255.0 ,
      0.0,   4.0,  11.0,    0.0, 11.0,    3.0,    4.0,  5.0, 11.0,  2.0, 11.0,  1.0,  5.0,  1.0, 11 ,
      0.0,   2.0,   5.0,    0.0,  5.0,    9.0,    2.0, 11.0,  5.0,  4.0,  5.0,  8.0, 11.0,  8.0,  5 ,
      9.0,   4.0,   5.0,    2.0, 11.0,    3.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   5.0,  10.0,    3.0,  5.0,    2.0,    3.0,  4.0,  5.0,  3.0,  8.0,  4.0, 255.0, 255.0, 255.0 ,
      5.0,  10.0,   2.0,    5.0,  2.0,    4.0,    4.0,  2.0,  0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,  10.0,   2.0,    3.0,  5.0,   10.0,    3.0,  8.0,  5.0,  4.0,  5.0,  8.0,  0.0,  1.0,  9 ,
      5.0,  10.0,   2.0,    5.0,  2.0,    4.0,    1.0,  9.0,  2.0,  9.0,  4.0,  2.0, 255.0, 255.0, 255.0 ,
      8.0,   4.0,   5.0,    8.0,  5.0,    3.0,    3.0,  5.0,  1.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   4.0,   5.0,    1.0,  0.0,    5.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      8.0,   4.0,   5.0,    8.0,  5.0,    3.0,    9.0,  0.0,  5.0,  0.0,  3.0,  5.0, 255.0, 255.0, 255.0 ,
      9.0,   4.0,   5.0,  255.0,255.0,  255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,  11.0,   7.0,    4.0,  9.0,   11.0,    9.0, 10.0,11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   8.0,   3.0,    4.0,  9.0,    7.0,    9.0, 11.0,  7.0,  9.0, 10.0,11.0, 255.0, 255.0, 255.0 ,
      1.0,  10.0,  11.0,    1.0, 11.0,    4.0,    1.0,  4.0,  0.0,  7.0,  4.0, 11.0, 255.0, 255.0, 255.0 ,
      3.0,   1.0,   4.0,    3.0,  4.0,    8.0,    1.0, 10.0, 4.0,  7.0,  4.0, 11.0, 10.0,11.0,  4 ,
      4.0,  11.0,   7.0,    9.0, 11.0,    4.0,    9.0,  2.0, 11.0,  9.0,  1.0,  2.0, 255.0, 255.0, 255.0 ,
      9.0,   7.0,   4.0,    9.0, 11.0,    7.0,    9.0,  1.0, 11.0,  2.0, 11.0,  1.0,  0.0,  8.0,  3 ,
     11.0,   7.0,   4.0,   11.0,  4.0,    2.0,    2.0,  4.0,  0.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
     11.0,   7.0,   4.0,   11.0,  4.0,    2.0,    8.0,  3.0,  4.0,  3.0,  2.0,  4.0, 255.0, 255.0, 255.0 ,
      2.0,   9.0,  10.0,    2.0,  7.0,    9.0,    2.0,  3.0,  7.0,  7.0,  4.0,  9.0, 255.0, 255.0, 255.0 ,
      9.0,  10.0,   7.0,    9.0,  7.0,    4.0,   10.0, 2.0,  7.0,  8.0,  7.0,  0.0,  2.0,  0.0,  7 ,
      3.0,   7.0,  10.0,    3.0, 10.0,    2.0,    7.0,  4.0, 10.0, 1.0, 10.0, 0.0,  4.0,  0.0, 10.0,
      1.0,  10.0,   2.0,    8.0,  7.0,    4.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   9.0,   1.0,    4.0,  1.0,    7.0,    7.0,  1.0,  3.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   9.0,   1.0,    4.0,  1.0,    7.0,    0.0,  8.0,  1.0,  8.0,  7.0,  1.0, 255.0, 255.0, 255.0 ,
      4.0,   0.0,   3.0,    7.0,  4.0,    3.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      4.0,   8.0,   7.0,  255.0,255.0,  255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,  10.0,   8.0,   10.0, 11.0,    8.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   0.0,   9.0,    3.0,  9.0,   11.0,   11.0,  9.0, 10.0,255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   1.0,  10.0,    0.0, 10.0,    8.0,    8.0, 10.0,11.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   1.0,  10.0,   11.0,  3.0,   10.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   2.0,  11.0,    1.0, 11.0,    9.0,    9.0, 11.0,  8.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   0.0,   9.0,    3.0,  9.0,   11.0,    1.0,  2.0,  9.0,  2.0, 11.0,  9.0, 255.0, 255.0, 255.0 ,
      0.0,   2.0,  11.0,    8.0,  0.0,   11.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      3.0,   2.0,  11.0,  255.0,255.0,  255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   3.0,   8.0,    2.0,  8.0,   10.0,   10.0, 8.0,  9.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      9.0,  10.0,   2.0,    0.0,  9.0,    2.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      2.0,   3.0,   8.0,    2.0,  8.0,   10.0,    0.0,  1.0,  8.0,  1.0, 10.0, 8.0, 255.0, 255.0, 255.0 ,
      1.0,  10.0,   2.0,  255.0,255.0,  255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      1.0,   3.0,   8.0,    9.0,  1.0,    8.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   9.0,   1.0,  255.0,255.0,  255.0,  255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0, 255.0 ,
      0.0,   3.0,   8.0,  255.0,255.0,  255.0,  255.0 ,255.0 ,255.0, 255.0 ,255.0 ,255.0, 255.0 ,255.0 ,255.0 ,
   255.0 , 255.0, 255.0,  255.0,255.0,  255.0,  255.0 ,255.0 ,255.0, 255.0 ,255.0 ,255.0, 255.0 ,255.0 ,255.0 };


// The kernel. 
__kernel void mc(__global __read_only float3* base_points,
	         __global float3* base_values,
		 __global float3* output,
		 __global int* counterArg,       
		 float isovalue,
		 int n)
{
  const int global_id = get_global_id(0);
  volatile __global int* counterPtr = counterArg;

  if(global_id < n) { 

     // Calculate the values for each f(x,y,z) = v.
     // Store the values in base_values.
     
     float3 f;
     f.x = global_id;
     f.y = counterPtr[0];
     f.z = 6.0;
     int index = atomic_inc(counterPtr);
     output[index] = base_points[index]; //base_points[global_id];                 
  }
}                                                                               