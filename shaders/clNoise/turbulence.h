const char *turbulence_Src = "\n"
"    float x0 = pos.x + (12414.0 / 65536.0);\n"
"    float y0 = pos.y + (65124.0 / 65536.0);\n"
"    float x1 = pos.x + (26519.0 / 65536.0);\n"
"    float y1 = pos.y + (18128.0 / 65536.0);\n"
"\n"
"    float2 orig_pos = pos;\n"
"    pos = (float2)(x0, y0);\n"
"    float xDistort = orig_pos.x + ( XDISTORT * power);\n"
"    pos = (float2)(x1, y1);\n"
"    float yDistort = orig_pos.y + ( YDISTORT * power);\n"
"\n"
"    // Retrieve the output value at the offsetted input value instead of the\n"
"    // original input value.\n"
"    pos = (float2)(xDistort, yDistort);\n"
"    return INPUT;\n"
"\n"
;