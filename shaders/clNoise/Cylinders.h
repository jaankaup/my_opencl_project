const char *Cylinders_Src = "\n"
"  float x = pos.x * frequency;\n"
"  float z = pos.y * frequency;\n"
"\n"
"  float distFromCenter = sqrt (x * x + z * z);\n"
"  float distFromSmallerSphere = distFromCenter - floor (distFromCenter);\n"
"  float distFromLargerSphere = 1.0 - distFromSmallerSphere;\n"
"  float nearestDist = min (distFromSmallerSphere, distFromLargerSphere);\n"
"  return 1.0 - (nearestDist * 4.0); // Puts it in the -1.0 to +1.0 range.\n"
"\n"
;