const char *RidgedMulti_Src = "\n"
"  float x = pos.x * frequency;\n"
"  float y = frequency;\n"
"  float z = pos.y * frequency;\n"
"\n"
"  float signal = 0.0f;\n"
"  float value  = 0.0f;\n"
"  float weight = 1.0f;\n"
"\n"
"  float m_pSpectralWeights[30];\n"
"  float f = 1.0;\n"
"  for (int i = 0; i < octaveCount; ++i)\n"
"  {\n"
"    m_pSpectralWeights[i] = pow(f, -spectralWeightExponent);\n"
"    f *= lacunarity;\n"
"  }\n"
"\n"
"  for (int curOctave = 0; curOctave < octaveCount; curOctave++) {\n"
"\n"
"    // Make sure that these floating-point values have the same range as a 32-\n"
"    // bit integer so that we can pass them to the coherent-noise functions.\n"
"    float nx, ny, nz;\n"
"    nx = MakeInt32Range (x);\n"
"    ny = MakeInt32Range (y);\n"
"    nz = MakeInt32Range (z);\n"
"\n"
"    // Get the coherent-noise value.\n"
"    int seed = (seed + curOctave) & 0x7fffffff;\n"
"    signal = GradientCoherentNoise3D (nx, ny, nz, seed);\n"
"\n"
"    // Make the ridges.\n"
"    signal = fabs (signal);\n"
"    signal = offset - signal;\n"
"\n"
"    // Square the signal to increase the sharpness of the ridges.\n"
"    signal *= signal;\n"
"\n"
"    // The weighting from the previous octave is applied to the signal.\n"
"    // Larger values have higher weights, producing sharp points along the\n"
"    // ridges.\n"
"    signal *= weight;\n"
"\n"
"    // Weight successive contributions by the previous signal.\n"
"    weight = signal * gain;\n"
"    weight = (weight > 1.0f)?1.0f:weight;\n"
"    weight = (weight < 0.0f)?0.0f:weight;\n"
"\n"
"    // Add the signal to the output value.\n"
"    value += (signal * m_pSpectralWeights[curOctave]);\n"
"\n"
"    // Go to the next octave.\n"
"    x *= lacunarity;\n"
"    y *= lacunarity;\n"
"    z *= lacunarity;\n"
"  }\n"
"\n"
"  return (value * 1.25) - 1.0;\n"
"\n"
;