    __constant int4 x; 
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

    kernel void simple_add(global const int* A, global const int* B, global int* C){       
       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                 
    }                                                                               
