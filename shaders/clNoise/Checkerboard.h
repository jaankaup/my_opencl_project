const char *Checkerboard_Src = "\n"
"    int ix = (int)(floor (MakeInt32Range (pos.x)));\n"
"//    int iy = (int)(floor (MakeInt32Range (1.0)));\n"
"    int iz = (int)(floor (MakeInt32Range (pos.y)));\n"
"    return (ix & 1 /*^ iy & 1*/ ^ iz & 1)? -1.0: 1.0;\n"
"\n"
;