xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 95;
 -23.00340;1.37555;-4.22155;,
 -23.00340;5.91073;-4.22155;,
 -9.43538;6.61087;-5.93189;,
 -9.43538;0.60120;-5.93189;,
 -0.11860;6.24999;0.00000;,
 -0.11860;6.24999;5.79964;,
 -0.08634;-0.00004;5.79964;,
 -0.08634;-0.00004;0.00000;,
 -9.43538;0.60120;5.93189;,
 -9.43538;6.61087;5.93189;,
 -23.00340;5.91073;4.22155;,
 -23.00340;1.37555;4.22155;,
 -23.65581;1.37555;3.87712;,
 -23.65581;6.47335;3.87712;,
 -23.65581;6.47335;0.00000;,
 -23.65581;1.37555;0.00000;,
 -23.00340;6.92623;0.00000;,
 -23.00340;6.92623;3.44922;,
 -9.43538;7.00231;5.15955;,
 -9.43538;7.00231;0.00000;,
 -23.00340;-4.17515;3.44922;,
 -23.00340;-4.17515;0.00000;,
 -9.43538;-6.42398;0.00000;,
 -9.43538;-6.42398;5.15955;,
 -23.65581;1.37555;-3.87712;,
 -23.65581;6.47335;-3.87712;,
 -8.40455;6.82843;-5.79964;,
 -8.40455;0.28918;-5.79964;,
 -8.40455;-6.25008;-5.79964;,
 -23.65581;-3.72227;-3.87712;,
 -23.00340;-3.15964;-4.22155;,
 -9.43538;-5.40847;-5.93189;,
 -8.40455;0.28918;5.79964;,
 -8.40455;6.82843;5.79964;,
 -23.65581;-3.72227;3.87712;,
 -8.40455;-6.25008;5.79964;,
 -9.43538;-5.40847;5.93189;,
 -23.00340;-3.15964;4.22155;,
 -23.65581;6.47335;0.00000;,
 -23.65581;6.47335;3.87712;,
 -8.40455;6.82843;5.79964;,
 -8.40455;6.82843;0.00000;,
 -23.00340;6.92623;-3.44922;,
 -9.43538;7.00231;-5.15955;,
 -23.65581;-3.72227;3.87712;,
 -23.65581;-3.72227;0.00000;,
 -9.43538;-6.42398;-5.15955;,
 -23.00340;-4.17515;-3.44922;,
 -8.40455;-6.25008;0.00000;,
 -8.40455;-6.25008;5.79964;,
 -0.11860;6.24999;-5.79964;,
 -0.08634;-0.00004;-5.79964;,
 -8.40455;6.82843;0.00000;,
 -8.40455;-6.25008;0.00000;,
 -0.05408;-6.25008;0.00000;,
 -0.05408;-6.25008;5.79964;,
 -0.05408;-6.25008;-5.79964;,
 -23.65581;6.47335;-3.87712;,
 -23.65581;1.37555;-3.87712;,
 -23.65581;-3.72227;0.00000;,
 -23.65581;-3.72227;-3.87712;,
 -15.80570;3.22387;-8.49710;,
 -19.65828;16.85198;-8.49710;,
 3.92203;2.06498;-7.34173;,
 -1.50610;-4.70216;-7.34173;,
 -1.19528;-5.50043;-7.17805;,
 4.90620;2.10619;-7.17805;,
 4.90620;2.10619;7.17805;,
 -1.19528;-5.50043;7.17805;,
 -1.50610;-4.70216;7.34173;,
 3.92203;2.06498;7.34173;,
 -19.65828;16.85198;8.49710;,
 -15.80570;3.22387;8.49710;,
 -7.54654;-1.47813;5.43881;,
 -8.98054;9.52258;5.43881;,
 -8.98054;9.52258;-5.43881;,
 -7.54654;-1.47813;-5.43881;,
 -18.99756;17.67567;-6.94255;,
 -18.99756;17.67567;6.94255;,
 4.32830;2.57145;6.38583;,
 4.32830;2.57145;-6.38583;,
 -16.83821;3.15711;-7.80383;,
 -19.95867;17.69793;-7.80383;,
 -19.95867;17.69793;7.80383;,
 -16.83821;3.15711;7.80383;,
 -19.95867;17.69793;7.80383;,
 4.90620;2.10619;7.17805;,
 -1.91234;-5.20864;-6.38583;,
 -16.46641;2.40015;-6.94255;,
 -1.19528;-5.50043;7.17805;,
 -1.91234;-5.20864;6.38583;,
 -16.83821;3.15711;7.80383;,
 -16.46641;2.40015;6.94255;,
 -19.95867;17.69793;-7.80383;,
 -16.83821;3.15711;-7.80383;;
 
 71;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,1,0;,
 4;25,26,2,1;,
 4;26,27,3,2;,
 4;28,29,30,31;,
 4;32,33,9,8;,
 4;33,13,10,9;,
 4;13,12,11,10;,
 4;34,35,36,37;,
 4;38,39,17,16;,
 4;39,40,18,17;,
 4;40,41,19,18;,
 4;26,25,42,43;,
 4;44,45,21,20;,
 4;29,28,46,47;,
 4;48,49,23,22;,
 4;49,44,20,23;,
 4;27,26,50,51;,
 4;52,33,5,4;,
 4;33,32,6,5;,
 4;35,53,54,55;,
 4;51,50,4,7;,
 4;53,28,56,54;,
 4;28,48,22,46;,
 4;21,47,46,22;,
 4;45,29,47,21;,
 4;14,57,58,15;,
 4;25,38,16,42;,
 4;42,16,19,43;,
 4;41,26,43,19;,
 4;26,52,4,50;,
 4;30,0,3,31;,
 4;27,28,31,3;,
 4;28,27,51,56;,
 4;56,51,7,54;,
 4;6,55,54,7;,
 4;32,35,55,6;,
 4;35,32,8,36;,
 4;36,8,11,37;,
 4;12,34,37,11;,
 4;34,12,15,59;,
 4;58,60,59,15;,
 4;29,24,0,30;,
 4;61,62,63,64;,
 4;65,66,67,68;,
 4;69,70,71,72;,
 4;73,74,75,76;,
 4;77,78,79,80;,
 4;81,82,62,61;,
 4;82,66,63,62;,
 4;66,65,64,63;,
 4;65,81,61,64;,
 4;68,67,70,69;,
 4;67,83,71,70;,
 4;83,84,72,71;,
 4;84,68,69,72;,
 4;82,85,78,77;,
 4;85,86,79,78;,
 4;86,66,80,79;,
 4;66,82,77,80;,
 4;81,65,87,88;,
 4;65,89,90,87;,
 4;89,91,92,90;,
 4;84,83,74,73;,
 4;83,93,75,74;,
 4;93,94,76,75;;
 
 MeshMaterialList {
  1;
  71;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  87;
  -0.417658;-0.587431;-0.693171;,
  -0.363350;0.675815;-0.641289;,
  -0.060880;-0.905321;-0.420342;,
  -0.019199;0.409062;-0.912304;,
  -0.061456;-0.530574;0.845408;,
  0.034971;0.724452;0.688437;,
  -0.306606;-0.424187;0.852090;,
  -0.886070;0.463551;0.000000;,
  -0.262746;-0.122800;-0.957019;,
  -0.252261;0.220549;-0.942190;,
  -0.040919;0.219289;-0.974801;,
  -0.059019;-0.122412;-0.990723;,
  -0.059019;-0.122412;0.990723;,
  -0.040919;0.219289;0.974801;,
  -0.252261;0.220549;0.942190;,
  -0.262746;-0.122800;0.957019;,
  -0.233596;0.954611;0.184801;,
  -0.233596;0.954611;-0.184801;,
  0.028535;0.983375;0.179330;,
  0.028535;0.983375;-0.179330;,
  -0.330577;-0.923763;0.193343;,
  -0.330577;-0.923763;-0.193343;,
  -0.070568;-0.978711;-0.192732;,
  -0.070568;-0.978711;0.192732;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.999987;0.005162;0.000000;,
  0.083465;-0.996511;-0.000000;,
  0.001437;-0.999999;-0.000000;,
  -0.376046;-0.926601;0.000000;,
  -0.886070;-0.463551;0.000000;,
  -0.301440;0.953485;0.000000;,
  0.080662;0.996741;0.000000;,
  0.118125;0.992999;0.000000;,
  -0.300837;0.000000;-0.953676;,
  0.001101;0.000000;-0.999999;,
  0.063756;0.000000;-0.997966;,
  0.999987;0.005162;0.000000;,
  0.063756;0.000000;0.997966;,
  0.001101;0.000000;0.999999;,
  -0.300837;0.000000;0.953676;,
  -1.000000;0.000000;0.000000;,
  -0.466867;0.000000;-0.884328;,
  -0.547477;-0.564997;-0.617290;,
  -0.780777;-0.549954;-0.296545;,
  0.780059;-0.625706;-0.000000;,
  -0.547477;-0.564997;0.617290;,
  0.438170;0.661835;0.608261;,
  -0.092992;-0.273199;-0.957452;,
  0.193526;0.229663;-0.953836;,
  0.187654;0.106475;-0.976447;,
  -0.003272;-0.227878;-0.973684;,
  -0.003272;-0.227878;0.973684;,
  0.187654;0.106475;0.976447;,
  0.193526;0.229663;0.953836;,
  -0.092992;-0.273199;0.957452;,
  0.377234;0.919492;0.110581;,
  0.377234;0.919492;-0.110581;,
  0.568999;0.815345;0.107021;,
  0.568999;0.815345;-0.107021;,
  -0.440252;-0.839473;0.318533;,
  -0.440252;-0.839473;-0.318533;,
  -0.414480;-0.895602;-0.161565;,
  -0.414480;-0.895602;0.161565;,
  -0.991611;-0.129262;0.000000;,
  -0.862725;-0.505673;0.000000;,
  -0.244669;0.543294;0.803100;,
  -0.466867;0.000000;0.884328;,
  0.166348;-0.986067;-0.000000;,
  0.069640;0.997572;0.000000;,
  0.000000;-0.000000;1.000000;,
  0.118125;0.992999;0.000000;,
  0.999987;0.005161;0.000000;,
  0.999987;0.005161;0.000000;,
  -0.819712;-0.196041;-0.538182;,
  0.438170;0.661835;-0.608261;,
  0.383227;0.429418;-0.817764;,
  -0.265502;-0.723013;-0.637779;,
  -0.192237;-0.561452;0.804871;,
  0.495162;0.578093;0.648555;,
  -0.780777;-0.549954;0.296545;,
  -0.819712;-0.196041;0.538182;,
  0.023154;0.999732;0.000000;,
  0.627108;0.778932;0.000000;,
  -0.376914;-0.926248;0.000000;,
  -0.255289;-0.043726;-0.965875;,
  -0.255289;-0.043726;0.965875;;
  71;
  4;34,9,10,35;,
  4;26,26,37,37;,
  4;39,13,14,40;,
  4;41,7,7,41;,
  4;31,16,18,32;,
  4;20,29,28,23;,
  4;42,1,9,34;,
  4;1,3,10,9;,
  4;3,36,35,10;,
  4;2,0,8,11;,
  4;38,5,13,39;,
  4;5,66,14,13;,
  4;66,67,40,14;,
  4;6,4,12,15;,
  4;7,7,16,31;,
  4;66,5,18,16;,
  4;5,33,32,18;,
  4;3,1,17,19;,
  4;30,30,29,20;,
  4;0,2,22,21;,
  4;27,68,23,28;,
  4;4,6,20,23;,
  4;36,3,25,25;,
  4;33,5,69,69;,
  4;5,38,70,70;,
  4;4,27,24,24;,
  4;37,26,26,37;,
  4;27,2,24,24;,
  4;2,27,28,22;,
  4;29,21,22,28;,
  4;30,0,21,29;,
  4;7,41,41,41;,
  4;1,7,31,17;,
  4;17,31,32,19;,
  4;33,71,19,32;,
  4;71,33,69,69;,
  4;8,34,35,11;,
  4;36,36,11,35;,
  4;36,36,25,25;,
  4;72,37,37,73;,
  4;37,72,73,37;,
  4;38,4,70,70;,
  4;4,38,39,12;,
  4;12,39,40,15;,
  4;67,6,15,40;,
  4;30,41,41,30;,
  4;41,41,30,41;,
  4;0,42,34,8;,
  4;48,49,50,51;,
  4;45,45,45,45;,
  4;52,53,54,55;,
  4;64,65,65,64;,
  4;57,56,58,59;,
  4;43,44,74,74;,
  4;75,76,50,49;,
  4;76,77,51,50;,
  4;77,43,48,51;,
  4;78,79,53,52;,
  4;79,47,54,53;,
  4;80,46,81,81;,
  4;46,78,52,55;,
  4;82,82,56,57;,
  4;47,79,58,56;,
  4;79,83,59,58;,
  4;76,75,57,59;,
  4;43,77,62,61;,
  4;77,84,63,62;,
  4;78,46,60,63;,
  4;85,85,85,85;,
  4;80,44,65,65;,
  4;86,86,86,86;;
 }
 MeshTextureCoords {
  95;
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.625000;,
  0.625000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.625000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.625000;0.250000;,
  0.500000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.625000;0.125000;,
  0.500000;0.125000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.750000;0.750000;,
  0.125000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.750000;,
  0.125000;0.750000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.375000;0.750000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.375000;0.250000;,
  0.375000;0.500000;,
  0.375000;0.500000;,
  0.375000;0.250000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.750000;0.750000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.125000;1.000000;,
  0.250000;1.000000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.250000;0.750000;,
  0.125000;0.750000;,
  0.625000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.625000;,
  0.375000;0.625000;,
  0.375000;0.625000;,
  0.375000;0.500000;,
  0.375000;0.750000;,
  0.625000;0.000000;,
  0.500000;0.000000;,
  0.375000;0.125000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.750000;,
  0.375000;0.750000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.625000;0.500000;,
  0.375000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.500000;,
  0.625000;0.250000;,
  0.375000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.625000;0.000000;,
  0.375000;0.000000;,
  0.625000;1.000000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.625000;0.750000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.625000;0.250000;,
  0.375000;0.250000;,
  0.875000;1.000000;,
  0.875000;0.750000;,
  0.375000;0.750000;,
  0.375000;1.000000;,
  0.125000;0.750000;,
  0.125000;0.750000;,
  0.125000;1.000000;,
  0.125000;1.000000;,
  0.625000;0.000000;,
  0.375000;0.000000;;
 }
}