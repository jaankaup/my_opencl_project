const char *gradient_Src = "\n"
"    int index = 0;\n"
"    float gradientPos = NOISE_VALUE;\n"
"    for(; index < GRAD_POINT_COUNT; ++index)\n"
"    {\n"
"	if (gradientPos < GRAD_POINT[index])break;\n"
"    }\n"
"\n"
"    int index0 = clamp(index - 1, 0, GRAD_POINT_COUNT - 1);\n"
"    int index1 = clamp(index, 0, GRAD_POINT_COUNT - 1);\n"
"    \n"
"    float4 color0 = (float4)(GRAD_POINT_COLOR_R[index0], GRAD_POINT_COLOR_G[index0], GRAD_POINT_COLOR_B[index0], GRAD_POINT_COLOR_A[index0]);\n"
"    float4 color1 = (float4)(GRAD_POINT_COLOR_R[index1], GRAD_POINT_COLOR_G[index1], GRAD_POINT_COLOR_B[index1], GRAD_POINT_COLOR_A[index1]);\n"
"\n"
"    float point0 = GRAD_POINT[index0];\n"
"    float point1 = GRAD_POINT[index1];\n"
"    \n"
"    float alpha = clamp((gradientPos - point0) / (point1 - point0), 0.0f, 1.0f);\n"
"\n"
"    uint4 color;\n"
"    color.x = (color1.x * alpha + (color0.x * (1.0f - alpha))) * 255.0f;\n"
"    color.y = (color1.y * alpha + (color0.y * (1.0f - alpha))) * 255.0f;\n"
"    color.z = (color1.z * alpha + (color0.z * (1.0f - alpha))) * 255.0f;\n"
"    color.w = (color1.w * alpha + (color0.w * (1.0f - alpha))) * 255.0f;\n"
"\n"
"    return color;\n"
"\n"
;