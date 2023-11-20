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
 263;
 3.85328;748.71178;49.00887;,
 -0.20454;839.14572;-0.63557;,
 -1.18574;839.14572;-0.84413;,
 -25.04266;724.75729;42.65024;,
 -1.99726;839.14572;-1.43376;,
 -32.85790;735.10943;21.82236;,
 -2.49884;839.14572;-2.30246;,
 -43.56527;724.99947;12.81935;,
 -2.60366;839.14572;-3.30003;,
 -54.87933;719.45377;-9.77730;,
 -2.29371;839.14572;-4.25405;,
 -46.24137;724.37965;-33.37827;,
 -1.62248;839.14572;-4.99953;,
 -28.30470;740.39361;-38.43643;,
 -0.70612;839.14572;-5.40751;,
 -11.64675;722.48942;-52.37474;,
 0.29698;839.14572;-5.40751;,
 9.90006;732.05856;-52.95087;,
 1.21335;839.14572;-4.99953;,
 34.30370;731.03971;-48.47540;,
 1.88457;839.14572;-4.25405;,
 48.59115;721.61783;-28.38684;,
 2.19453;839.14572;-3.30003;,
 55.94424;719.46022;-3.99332;,
 2.08970;839.14572;-2.30246;,
 49.70829;701.42604;8.58440;,
 1.58813;839.14572;-1.43376;,
 41.88419;724.55592;28.28985;,
 2.19453;839.14572;-3.30003;,
 1.88457;839.14572;-4.25405;,
 2.08970;839.14572;-2.30246;,
 1.58813;839.14572;-1.43376;,
 1.21335;839.14572;-4.99953;,
 0.77660;839.14572;-0.84413;,
 0.29698;839.14572;-5.40751;,
 -0.20454;839.14572;-0.63557;,
 -0.70612;839.14572;-5.40751;,
 -1.18574;839.14572;-0.84413;,
 -1.62248;839.14572;-4.99953;,
 -1.99726;839.14572;-1.43376;,
 -2.29371;839.14572;-4.25405;,
 -2.49884;839.14572;-2.30246;,
 -2.60366;839.14572;-3.30003;,
 0.77660;839.14572;-0.84413;,
 20.69674;743.97692;44.89286;,
 -0.20454;839.14572;-0.63557;,
 3.85328;748.71178;49.00887;,
 111.71557;186.69973;-14.76727;,
 107.79345;190.24698;31.92473;,
 97.98163;186.14312;-59.75953;,
 82.47162;189.45174;73.47556;,
 62.32162;187.76353;-95.17617;,
 43.76075;180.85193;101.55831;,
 23.96683;182.57014;-115.86349;,
 -1.66071;199.56861;113.50955;,
 -21.53882;187.48511;-115.13268;,
 -47.04510;177.98418;102.71973;,
 -67.80848;189.52683;-93.67151;,
 -82.85649;190.89519;70.71838;,
 -97.75101;183.56057;-59.38528;,
 -109.35184;188.15425;28.39926;,
 -112.18653;185.80108;-13.01730;,
 35.12114;305.51016;80.42950;,
 63.45551;280.07100;131.09784;,
 8.20040;318.73899;152.76404;,
 0.15397;283.00909;96.96302;,
 26.23154;404.30172;55.90373;,
 47.33044;403.01241;115.19257;,
 -3.47169;371.83875;122.21724;,
 -3.30526;393.02979;65.26287;,
 24.65986;522.20357;55.90540;,
 41.33058;502.27398;84.75109;,
 -3.98112;495.29556;96.73124;,
 -1.55511;517.58472;58.73010;,
 18.84157;637.64059;39.24034;,
 31.34473;614.99371;63.32875;,
 -1.09802;627.79811;69.72620;,
 0.80280;636.16686;40.50054;,
 11.14350;755.12320;20.55573;,
 -2.90262;758.15631;22.69462;,
 67.76153;295.83887;59.80093;,
 112.18246;274.01656;106.17118;,
 49.70576;418.35626;47.95522;,
 94.07013;391.76939;81.24407;,
 43.17718;525.12779;35.89443;,
 74.36665;497.92672;62.79767;,
 36.04474;651.79664;31.42143;,
 53.74208;630.96764;44.00867;,
 22.68676;747.10826;14.93472;,
 94.68618;278.77935;26.98588;,
 148.61059;228.82119;41.75428;,
 62.45716;425.36964;17.02095;,
 119.54116;385.75308;35.84802;,
 54.66490;520.51812;12.97146;,
 92.86260;510.53189;29.03631;,
 45.15509;629.68082;8.43331;,
 70.55125;628.07730;18.51514;,
 25.98106;751.02643;5.81000;,
 93.75157;308.11160;-10.78269;,
 161.76701;241.48648;-11.51950;,
 68.44308;403.38008;-8.63222;,
 121.62617;383.84327;-13.10356;,
 57.70593;515.88453;-10.40851;,
 98.02255;493.84010;-12.73533;,
 43.22516;631.90611;-12.47039;,
 70.20748;628.56383;-9.51228;,
 26.78905;751.33795;-7.29165;,
 81.65523;302.09251;-50.23293;,
 143.89850;263.12535;-84.85125;,
 61.06979;408.24727;-37.31235;,
 105.09158;396.07296;-63.29520;,
 49.05044;523.40181;-30.28654;,
 85.75101;496.08541;-53.30521;,
 36.04786;652.00729;-26.51914;,
 63.09213;617.88219;-46.32792;,
 22.35603;754.00826;-15.91283;,
 55.25436;302.03245;-79.20074;,
 92.76533;269.58594;-131.00781;,
 37.48635;405.75704;-56.30236;,
 72.05135;397.89068;-106.47772;,
 34.80002;523.18297;-51.93423;,
 58.85092;485.56075;-89.00279;,
 27.43786;638.77008;-37.69306;,
 44.29508;608.80435;-62.53325;,
 20.85412;752.27447;-25.63083;,
 18.77973;292.77254;-96.59877;,
 28.94055;278.33096;-151.18233;,
 14.33847;408.73160;-69.68024;,
 27.31506;394.70518;-125.72538;,
 10.55778;511.85328;-56.84226;,
 16.80849;507.81676;-104.71687;,
 11.34391;629.28189;-50.10658;,
 8.54408;637.06832;-73.91733;,
 8.62769;751.00065;-27.58506;,
 -20.32666;300.52420;-97.19484;,
 -32.74483;266.37586;-153.43286;,
 -10.99958;398.79937;-67.85659;,
 -25.24722;403.40333;-127.39158;,
 -16.60262;513.86803;-65.48678;,
 -19.63056;500.91910;-100.02667;,
 -9.96784;637.09293;-47.70699;,
 -11.89638;612.88619;-83.58279;,
 -13.10823;752.69352;-27.25600;,
 -56.96484;334.38531;-82.84836;,
 -94.70179;307.34217;-121.11641;,
 -40.93603;429.16246;-52.69322;,
 -73.33689;389.86692;-102.49710;,
 -36.43271;539.77750;-50.54402;,
 -61.12839;519.67701;-82.17499;,
 -27.81306;636.88551;-39.74534;,
 -38.30690;597.41451;-62.48727;,
 -15.06643;757.37916;-23.63361;,
 -86.16725;302.51299;-50.72545;,
 -136.63292;257.44981;-78.20754;,
 -59.38077;420.30362;-36.96760;,
 -110.90847;404.08091;-68.00336;,
 -58.46378;529.80572;-32.38285;,
 -89.38029;493.55601;-56.69183;,
 -42.07981;634.16656;-26.75582;,
 -61.08158;639.33014;-44.72278;,
 -26.89062;750.46412;-15.91744;,
 -90.55210;291.54659;-15.38885;,
 -161.48112;263.98511;-20.20169;,
 -67.28397;407.58575;-7.83013;,
 -126.48591;402.33629;-14.57467;,
 -60.50547;506.32613;-11.72212;,
 -98.00977;503.63189;-16.55318;,
 -48.63857;649.73668;-11.85482;,
 -76.08126;607.01666;-12.22081;,
 -27.42782;752.52867;-7.98941;,
 -92.80417;283.04908;29.12258;,
 -129.25824;280.16890;37.62437;,
 -62.44623;410.64341;15.28139;,
 -115.22374;383.71690;35.51964;,
 -57.17897;523.97525;15.50910;,
 -95.21698;500.88004;27.30144;,
 -42.85535;632.10230;10.02175;,
 -65.17174;608.46266;24.26543;,
 -24.26872;751.53971;3.68502;,
 -70.51431;301.25713;60.36349;,
 -110.92083;273.98499;93.40772;,
 -48.17194;404.54615;41.69241;,
 -94.58664;381.24886;81.29157;,
 -42.39382;518.15953;42.70405;,
 -71.91084;492.23653;65.26757;,
 -31.26077;634.19635;29.50750;,
 -59.22244;629.38199;46.65193;,
 -20.81329;746.70621;18.62745;,
 -37.00200;285.97666;78.06517;,
 -70.66443;288.36084;128.48953;,
 -28.21799;409.30992;58.00176;,
 -51.33415;376.15023;116.30609;,
 -20.01683;514.17428;51.30691;,
 -46.71964;499.63004;88.73491;,
 -19.06296;632.16275;38.80635;,
 -31.14410;619.32691;62.23315;,
 -10.32192;750.14479;22.80289;,
 0.15397;283.00909;96.96302;,
 8.20040;318.73899;152.76404;,
 -3.30526;393.02979;65.26287;,
 -3.47169;371.83875;122.21724;,
 -1.55511;517.58472;58.73010;,
 -3.98112;495.29556;96.73124;,
 0.80280;636.16686;40.50054;,
 -1.09802;627.79811;69.72620;,
 -2.90262;758.15631;22.69462;,
 7.47069;216.89473;181.17996;,
 -81.74184;149.72857;160.27243;,
 -125.74168;177.30039;106.43595;,
 -160.14026;165.72879;43.57630;,
 -184.98360;142.82122;-21.72264;,
 -160.12204;140.54859;-92.23069;,
 -112.36369;181.02422;-140.70252;,
 -34.85775;159.35569;-178.22733;,
 36.93175;166.10977;-179.15830;,
 103.08024;162.16690;-152.66282;,
 162.47388;151.52205;-93.97306;,
 185.05044;146.98501;-13.00110;,
 178.04064;120.78811;50.65841;,
 129.64269;168.05884;118.21552;,
 70.18070;174.67527;160.55331;,
 7.47069;216.89473;181.17996;,
 7.47069;216.89473;181.17996;,
 -81.74184;149.72857;160.27243;,
 -125.74168;177.30039;106.43595;,
 -160.14026;165.72879;43.57630;,
 -184.98360;142.82122;-21.72264;,
 -160.12204;140.54859;-92.23069;,
 -112.36369;181.02422;-140.70252;,
 -34.85775;159.35569;-178.22733;,
 36.93175;166.10977;-179.15830;,
 103.08024;162.16690;-152.66282;,
 162.47388;151.52205;-93.97306;,
 185.05044;146.98501;-13.00110;,
 178.04064;120.78811;50.65841;,
 129.64269;168.05884;118.21552;,
 70.18070;174.67527;160.55331;,
 -0.20454;-1.63115;28.78058;,
 -26.75971;179.62903;5.58027;,
 -30.47540;-1.63115;6.78757;,
 -18.91299;-1.63115;-28.79796;,
 18.50383;-1.63115;-28.79796;,
 -0.20454;-1.63115;-28.79796;,
 9.04430;745.02399;-15.77785;,
 -9.45340;745.02399;-15.77785;,
 14.76039;745.02399;1.81449;,
 9.04430;745.02399;-15.77785;,
 -0.20454;745.02399;12.68716;,
 -9.45340;745.02399;-15.77785;,
 -15.16953;745.02399;1.81449;,
 14.76039;745.02399;1.81449;,
 30.06627;-1.63115;6.78757;,
 -0.20454;745.02399;12.68716;,
 -0.20454;-1.63115;28.78058;,
 -0.20454;-1.63115;28.78058;,
 -30.47540;-1.63115;6.78757;,
 -0.20454;-1.63115;-5.76654;,
 -0.20454;-1.63115;-28.79796;,
 -18.91299;-1.63115;-28.79796;,
 -0.20454;745.02399;12.68716;,
 -15.16953;745.02399;1.81449;,
 30.06627;-1.63115;6.78757;,
 18.50383;-1.63115;-28.79796;;
 
 224;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 3;28,29,30;,
 3;30,29,31;,
 3;29,32,31;,
 3;31,32,33;,
 3;32,34,33;,
 3;33,34,35;,
 3;34,36,35;,
 3;35,36,37;,
 3;36,38,37;,
 3;37,38,39;,
 3;38,40,39;,
 3;39,40,41;,
 3;40,42,41;,
 4;27,26,43,44;,
 4;44,43,45,46;,
 3;47,48,49;,
 3;48,50,49;,
 3;49,50,51;,
 3;50,52,51;,
 3;51,52,53;,
 3;52,54,53;,
 3;53,54,55;,
 3;54,56,55;,
 3;55,56,57;,
 3;56,58,57;,
 3;57,58,59;,
 3;58,60,59;,
 3;60,61,59;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;78,44,46,79;,
 4;80,81,63,62;,
 4;82,83,67,66;,
 4;84,85,71,70;,
 4;86,87,75,74;,
 4;88,27,44,78;,
 4;89,90,81,80;,
 4;91,92,83,82;,
 4;93,94,85,84;,
 4;95,96,87,86;,
 4;97,25,27,88;,
 4;98,99,90,89;,
 4;100,101,92,91;,
 4;102,103,94,93;,
 4;104,105,96,95;,
 4;106,23,25,97;,
 4;107,108,99,98;,
 4;109,110,101,100;,
 4;111,112,103,102;,
 4;113,114,105,104;,
 4;115,21,23,106;,
 4;116,117,108,107;,
 4;118,119,110,109;,
 4;120,121,112,111;,
 4;122,123,114,113;,
 4;124,19,21,115;,
 4;125,126,117,116;,
 4;127,128,119,118;,
 4;129,130,121,120;,
 4;131,132,123,122;,
 4;133,17,19,124;,
 4;134,135,126,125;,
 4;136,137,128,127;,
 4;138,139,130,129;,
 4;140,141,132,131;,
 4;142,15,17,133;,
 4;143,144,135,134;,
 4;145,146,137,136;,
 4;147,148,139,138;,
 4;149,150,141,140;,
 4;151,13,15,142;,
 4;152,153,144,143;,
 4;154,155,146,145;,
 4;156,157,148,147;,
 4;158,159,150,149;,
 4;160,11,13,151;,
 4;161,162,153,152;,
 4;163,164,155,154;,
 4;165,166,157,156;,
 4;167,168,159,158;,
 4;169,9,11,160;,
 4;170,171,162,161;,
 4;172,173,164,163;,
 4;174,175,166,165;,
 4;176,177,168,167;,
 4;178,7,9,169;,
 4;179,180,171,170;,
 4;181,182,173,172;,
 4;183,184,175,174;,
 4;185,186,177,176;,
 4;187,5,7,178;,
 4;188,189,180,179;,
 4;190,191,182,181;,
 4;192,193,184,183;,
 4;194,195,186,185;,
 4;196,3,5,187;,
 4;197,198,189,188;,
 4;199,200,191,190;,
 4;201,202,193,192;,
 4;203,204,195,194;,
 4;205,0,3,196;,
 4;206,197,188,207;,
 4;207,188,179,208;,
 4;208,179,170,209;,
 4;209,170,161,210;,
 4;210,161,152,211;,
 4;211,152,143,212;,
 4;212,143,134,213;,
 4;213,134,125,214;,
 4;214,125,116,215;,
 4;215,116,107,216;,
 4;216,107,98,217;,
 4;217,98,89,218;,
 4;218,89,80,219;,
 4;219,80,62,220;,
 4;220,62,65,221;,
 4;198,199,190,189;,
 4;189,190,181,180;,
 4;180,181,172,171;,
 4;171,172,163,162;,
 4;162,163,154,153;,
 4;153,154,145,144;,
 4;144,145,136,135;,
 4;135,136,127,126;,
 4;126,127,118,117;,
 4;117,118,109,108;,
 4;108,109,100,99;,
 4;99,100,91,90;,
 4;90,91,82,81;,
 4;81,82,66,63;,
 4;63,66,69,64;,
 4;200,201,192,191;,
 4;191,192,183,182;,
 4;182,183,174,173;,
 4;173,174,165,164;,
 4;164,165,156,155;,
 4;155,156,147,146;,
 4;146,147,138,137;,
 4;137,138,129,128;,
 4;128,129,120,119;,
 4;119,120,111,110;,
 4;110,111,102,101;,
 4;101,102,93,92;,
 4;92,93,84,83;,
 4;83,84,70,67;,
 4;67,70,73,68;,
 4;202,203,194,193;,
 4;193,194,185,184;,
 4;184,185,176,175;,
 4;175,176,167,166;,
 4;166,167,158,157;,
 4;157,158,149,148;,
 4;148,149,140,139;,
 4;139,140,131,130;,
 4;130,131,122,121;,
 4;121,122,113,112;,
 4;112,113,104,103;,
 4;103,104,95,94;,
 4;94,95,86,85;,
 4;85,86,74,71;,
 4;71,74,77,72;,
 4;204,205,196,195;,
 4;195,196,187,186;,
 4;186,187,178,177;,
 4;177,178,169,168;,
 4;168,169,160,159;,
 4;159,160,151,150;,
 4;150,151,142,141;,
 4;141,142,133,132;,
 4;132,133,124,123;,
 4;123,124,115,114;,
 4;114,115,106,105;,
 4;105,106,97,96;,
 4;96,97,88,87;,
 4;87,88,78,75;,
 4;75,78,79,76;,
 4;56,54,222,223;,
 4;58,56,223,224;,
 4;60,58,224,225;,
 4;61,60,225,226;,
 4;59,61,226,227;,
 4;57,59,227,228;,
 4;55,57,228,229;,
 4;53,55,229,230;,
 4;51,53,230,231;,
 4;49,51,231,232;,
 4;47,49,232,233;,
 4;48,47,233,234;,
 4;50,48,234,235;,
 4;52,50,235,236;,
 4;54,52,236,222;,
 3;237,238,239;,
 3;239,238,240;,
 3;241,242,243;,
 3;243,242,244;,
 3;242,240,244;,
 3;245,246,247;,
 3;246,248,247;,
 3;248,249,247;,
 4;241,243,250,251;,
 4;251,250,252,253;,
 3;254,255,256;,
 3;257,256,258;,
 3;256,255,258;,
 4;237,259,260,238;,
 4;240,238,260,244;,
 3;261,254,262;,
 3;262,254,256;,
 3;256,257,262;;
 
 MeshMaterialList {
  2;
  224;
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
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  0;;
  Material {
   0.059608;0.470588;0.059608;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.257255;0.160000;0.160000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  211;
  0.005913;-0.997063;0.076363;,
  -0.046467;0.478007;0.877126;,
  0.113344;-0.990013;-0.083826;,
  -0.530910;0.401785;0.746125;,
  0.046942;-0.995234;-0.085475;,
  -0.745959;0.349144;0.567137;,
  0.206054;-0.978115;0.028866;,
  -0.859917;0.356203;0.365598;,
  0.328155;-0.938295;0.109163;,
  -0.920838;0.389443;-0.019807;,
  0.235819;-0.969992;0.059211;,
  -0.774561;0.409931;-0.481677;,
  0.051410;-0.997311;0.052225;,
  -0.492485;0.395783;-0.775123;,
  0.053885;-0.987780;0.146241;,
  -0.224083;0.392772;-0.891917;,
  -0.090954;-0.985167;0.145508;,
  0.091888;0.410938;-0.907021;,
  -0.028773;-0.994391;0.101775;,
  0.539150;0.440962;-0.717544;,
  -0.183844;-0.978752;0.090812;,
  0.822765;0.420042;-0.382913;,
  -0.396173;-0.918176;0.000471;,
  0.907902;0.408707;0.093123;,
  -0.294324;-0.954690;-0.044045;,
  0.866120;0.371545;0.334350;,
  -0.076917;-0.995494;-0.055457;,
  0.796723;0.415089;0.439243;,
  -0.114647;-0.993360;-0.009633;,
  0.000000;1.000000;0.000000;,
  0.000257;-0.990933;0.134355;,
  0.107780;-0.925897;-0.362074;,
  0.099192;-0.865213;-0.491495;,
  0.048246;-0.933112;-0.356335;,
  0.153720;-0.922632;-0.353724;,
  0.205313;-0.899976;-0.384565;,
  0.050672;-0.920975;-0.386313;,
  0.058914;-0.997380;0.041980;,
  0.298299;-0.945300;-0.132003;,
  0.320836;-0.869383;-0.375816;,
  0.298172;-0.912286;-0.280764;,
  0.203293;-0.972519;-0.113484;,
  0.642800;-0.758796;-0.105059;,
  0.384500;-0.915285;-0.120059;,
  0.305785;-0.923767;-0.230543;,
  0.267464;-0.962199;-0.051352;,
  0.692512;-0.705008;-0.152941;,
  0.437973;-0.897919;0.043830;,
  0.253431;-0.951811;-0.172707;,
  0.377073;-0.926182;-0.001808;,
  0.283914;-0.922609;0.261124;,
  0.573999;-0.818692;0.016393;,
  0.272145;-0.957851;0.091966;,
  0.615701;-0.787143;-0.036309;,
  0.217770;-0.813899;0.538651;,
  -0.302122;-0.792106;0.530368;,
  0.137063;-0.928395;0.345393;,
  -0.271711;-0.718805;0.639916;,
  0.365826;-0.850204;0.378582;,
  -0.124782;-0.916591;0.379856;,
  0.022835;-0.982717;0.183701;,
  -0.138369;-0.829458;0.541160;,
  -0.140187;-0.966332;0.215753;,
  -0.010507;-0.966987;0.254608;,
  0.001649;-0.996422;0.084496;,
  -0.057756;-0.915945;0.397127;,
  -0.318952;-0.777612;0.541838;,
  -0.255350;-0.895394;0.364782;,
  -0.101047;-0.985457;0.136616;,
  -0.239878;-0.908820;0.341328;,
  -0.417825;-0.808338;0.414743;,
  -0.492005;-0.840760;0.225951;,
  -0.222154;-0.974856;0.017399;,
  -0.503974;-0.838165;0.208540;,
  -0.334582;-0.942098;-0.022516;,
  -0.465980;-0.875989;0.124522;,
  -0.420127;-0.904659;0.071317;,
  -0.642067;-0.751471;-0.151792;,
  -0.297011;-0.953646;0.048412;,
  -0.349835;-0.936810;-0.001248;,
  -0.484185;-0.874936;-0.007209;,
  -0.621187;-0.766553;-0.162862;,
  -0.014313;-0.629810;-0.776617;,
  -0.330269;-0.889488;-0.315805;,
  -0.206148;-0.947806;-0.243241;,
  -0.467153;-0.881271;-0.071614;,
  0.467100;0.465546;0.751721;,
  -0.098970;-0.786691;-0.609362;,
  -0.161609;-0.855676;-0.491632;,
  0.223879;-0.948299;-0.224961;,
  -0.185186;-0.976071;-0.113979;,
  0.213209;-0.898413;-0.383922;,
  0.572040;-0.672357;-0.469794;,
  0.588517;-0.689891;-0.421543;,
  0.714573;-0.647575;-0.264636;,
  0.764392;-0.643939;-0.032352;,
  0.701185;-0.675891;0.226961;,
  0.330200;-0.743956;0.580946;,
  0.032704;-0.680053;0.732434;,
  0.020351;-0.756987;0.653112;,
  -0.429499;-0.784803;0.446783;,
  -0.724752;-0.662112;0.190635;,
  -0.762310;-0.631207;-0.143038;,
  -0.803656;-0.529519;-0.271565;,
  -0.773158;-0.624857;-0.108531;,
  -0.486535;-0.849138;-0.205546;,
  -0.067090;0.672585;0.736972;,
  -0.421070;0.553811;0.718327;,
  -0.621442;0.524572;0.581922;,
  -0.836440;0.505926;0.210730;,
  -0.866801;0.497995;-0.025615;,
  -0.848240;0.463304;-0.256590;,
  -0.531836;0.487636;-0.692360;,
  -0.061430;0.477527;-0.876467;,
  0.163182;0.529066;-0.832743;,
  0.479514;0.535240;-0.695403;,
  0.747517;0.511069;-0.424296;,
  0.864635;0.501760;0.025363;,
  0.814678;0.489475;0.310989;,
  0.688473;0.538352;0.485986;,
  0.446280;0.591117;0.671875;,
  -0.587695;0.017435;0.808895;,
  -0.587695;0.017435;0.808895;,
  -0.950912;0.017435;-0.308970;,
  0.000000;0.017435;-0.999848;,
  0.000000;0.017435;-0.999848;,
  0.950912;0.017435;-0.308970;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.492488;-0.857315;0.149885;,
  0.680827;-0.720456;-0.131974;,
  0.620038;-0.774647;0.124398;,
  0.088087;0.635215;0.767296;,
  -0.427192;0.585774;0.688749;,
  -0.703759;0.516302;0.488011;,
  -0.811829;0.490886;0.316173;,
  -0.864467;0.502682;-0.002796;,
  -0.806308;0.508946;-0.301398;,
  -0.435884;0.577920;-0.689937;,
  -0.091047;0.520302;-0.849115;,
  0.123084;0.549559;-0.826338;,
  0.473980;0.543022;-0.693160;,
  0.734899;0.559407;-0.383389;,
  0.878326;0.478053;0.002928;,
  0.875299;0.438392;0.204117;,
  0.602611;0.503309;0.619306;,
  0.313968;0.559191;0.767287;,
  -0.146757;0.419007;0.896044;,
  -0.326038;0.412975;0.850383;,
  -0.624578;0.425324;0.654982;,
  -0.854237;0.431677;0.289713;,
  -0.895602;0.439087;0.071411;,
  -0.814821;0.454726;-0.359570;,
  -0.488877;0.415297;-0.767156;,
  -0.080879;0.469441;-0.879252;,
  0.181874;0.474313;-0.861364;,
  0.471439;0.485831;-0.736012;,
  0.816773;0.449521;-0.361680;,
  0.894329;0.438753;-0.087585;,
  0.872969;0.446380;0.196648;,
  0.725704;0.454073;0.516886;,
  0.297152;0.466971;0.832850;,
  -0.015111;0.360451;0.932656;,
  -0.391782;0.381241;0.837354;,
  -0.706878;0.365799;0.605404;,
  -0.852993;0.349578;0.387554;,
  -0.931294;0.350512;-0.099161;,
  -0.780698;0.358807;-0.511632;,
  -0.531322;0.404732;-0.744237;,
  -0.193412;0.385266;-0.902309;,
  0.195512;0.389598;-0.899994;,
  0.543064;0.352743;-0.762007;,
  0.819366;0.352343;-0.452210;,
  0.931195;0.352284;-0.093655;,
  0.919634;0.359685;0.157796;,
  0.678398;0.342539;0.649957;,
  0.256810;0.326322;0.909705;,
  -0.033078;0.336607;0.941064;,
  -0.362334;0.322058;0.874638;,
  -0.751673;0.354089;0.556424;,
  -0.930574;0.303967;0.204047;,
  -0.946505;0.315106;0.069547;,
  -0.759377;0.341714;-0.553695;,
  -0.554717;0.282607;-0.782574;,
  -0.360175;0.344259;-0.867041;,
  0.157941;0.361792;-0.918782;,
  0.570666;0.349714;-0.742994;,
  0.843671;0.332542;-0.421467;,
  0.930082;0.339311;-0.140768;,
  0.901738;0.348101;0.256307;,
  0.686931;0.339930;0.642319;,
  0.396964;0.328088;0.857192;,
  0.045596;-0.988833;0.141881;,
  0.262737;-0.948706;-0.175859;,
  0.156403;-0.957345;-0.242958;,
  0.386241;-0.922396;0.001755;,
  0.469139;-0.873184;0.132130;,
  0.446797;-0.891401;0.076003;,
  0.212600;-0.965099;0.152925;,
  0.003447;-0.939413;0.342771;,
  -0.053620;-0.945540;0.321058;,
  -0.215918;-0.941603;0.258386;,
  -0.383682;-0.908455;0.165826;,
  -0.505680;-0.862188;-0.030337;,
  -0.528142;-0.838724;-0.132694;,
  -0.309056;-0.938119;-0.156259;,
  -0.271784;-0.962168;-0.019117;,
  -0.950912;0.017435;-0.308969;,
  0.587696;0.017435;0.808894;,
  -0.587695;0.017435;0.808895;,
  -0.950912;0.017435;-0.308971;;
  224;
  4;1,1,3,3;,
  4;3,3,5,5;,
  4;5,5,7,7;,
  4;7,7,9,9;,
  4;9,9,11,11;,
  4;11,11,13,13;,
  4;13,13,15,15;,
  4;15,15,17,17;,
  4;17,17,19,19;,
  4;19,19,21,21;,
  4;21,21,23,23;,
  4;23,23,25,25;,
  4;25,25,27,27;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  4;27,27,86,86;,
  4;86,86,1,1;,
  3;22,24,20;,
  3;24,26,20;,
  3;20,26,18;,
  3;26,28,18;,
  3;18,28,16;,
  3;28,0,16;,
  3;16,0,14;,
  3;0,2,14;,
  3;14,2,12;,
  3;2,4,12;,
  3;12,4,10;,
  3;4,6,10;,
  3;6,8,10;,
  4;90,90,30,30;,
  4;89,89,31,31;,
  4;88,88,32,32;,
  4;87,87,33,33;,
  4;105,105,91,91;,
  4;85,85,90,90;,
  4;84,84,89,89;,
  4;83,83,88,88;,
  4;82,82,87,87;,
  4;104,104,105,105;,
  4;81,81,85,85;,
  4;80,80,84,84;,
  4;79,79,83,83;,
  4;78,78,129,129;,
  4;103,103,104,104;,
  4;77,77,81,81;,
  4;76,76,80,80;,
  4;75,75,79,79;,
  4;74,74,78,78;,
  4;102,102,103,103;,
  4;73,73,77,77;,
  4;72,72,76,76;,
  4;71,71,75,75;,
  4;70,70,74,74;,
  4;101,101,102,102;,
  4;69,69,73,73;,
  4;68,68,72,72;,
  4;67,67,71,71;,
  4;66,66,70,70;,
  4;100,100,101,101;,
  4;65,65,69,69;,
  4;64,64,68,68;,
  4;63,63,67,67;,
  4;62,62,66,66;,
  4;99,99,100,100;,
  4;61,61,65,65;,
  4;60,60,64,64;,
  4;59,59,63,63;,
  4;58,58,62,62;,
  4;98,98,99,99;,
  4;57,57,61,61;,
  4;56,56,60,60;,
  4;55,55,59,59;,
  4;54,54,58,58;,
  4;97,97,98,98;,
  4;53,53,130,130;,
  4;52,52,56,56;,
  4;51,51,131,131;,
  4;50,50,54,54;,
  4;96,96,97,97;,
  4;49,49,53,53;,
  4;48,48,52,52;,
  4;47,47,51,51;,
  4;46,46,50,50;,
  4;95,95,96,96;,
  4;45,45,49,49;,
  4;44,44,48,48;,
  4;43,43,47,47;,
  4;42,42,46,46;,
  4;94,94,95,95;,
  4;41,41,45,45;,
  4;40,40,44,44;,
  4;39,39,43,43;,
  4;38,38,42,42;,
  4;93,93,94,94;,
  4;37,37,41,41;,
  4;36,36,40,40;,
  4;35,35,39,39;,
  4;34,34,38,38;,
  4;92,92,93,93;,
  4;30,30,37,37;,
  4;31,31,36,36;,
  4;32,32,35,35;,
  4;33,33,34,34;,
  4;91,91,92,92;,
  4;106,106,107,107;,
  4;107,107,108,108;,
  4;108,108,109,109;,
  4;109,109,110,110;,
  4;110,110,111,111;,
  4;111,111,112,112;,
  4;112,112,113,113;,
  4;113,113,114,114;,
  4;114,114,115,115;,
  4;115,115,116,116;,
  4;116,116,117,117;,
  4;117,117,118,118;,
  4;118,118,119,119;,
  4;119,119,120,120;,
  4;120,120,106,106;,
  4;132,132,133,133;,
  4;133,133,134,134;,
  4;134,134,135,135;,
  4;135,135,136,136;,
  4;136,136,137,137;,
  4;137,137,138,138;,
  4;138,138,139,139;,
  4;139,139,140,140;,
  4;140,140,141,141;,
  4;141,141,142,142;,
  4;142,142,143,143;,
  4;143,143,144,144;,
  4;144,144,145,145;,
  4;145,145,146,146;,
  4;146,146,132,132;,
  4;147,147,148,148;,
  4;148,148,149,149;,
  4;149,149,150,150;,
  4;150,150,151,151;,
  4;151,151,152,152;,
  4;152,152,153,153;,
  4;153,153,154,154;,
  4;154,154,155,155;,
  4;155,155,156,156;,
  4;156,156,157,157;,
  4;157,157,158,158;,
  4;158,158,159,159;,
  4;159,159,160,160;,
  4;160,160,161,161;,
  4;161,161,147,147;,
  4;162,162,163,163;,
  4;163,163,164,164;,
  4;164,164,165,165;,
  4;165,165,166,166;,
  4;166,166,167,167;,
  4;167,167,168,168;,
  4;168,168,169,169;,
  4;169,169,170,170;,
  4;170,170,171,171;,
  4;171,171,172,172;,
  4;172,172,173,173;,
  4;173,173,174,174;,
  4;174,174,175,175;,
  4;175,175,176,176;,
  4;176,176,162,162;,
  4;177,177,178,178;,
  4;178,178,179,179;,
  4;179,179,180,180;,
  4;180,180,181,181;,
  4;181,181,182,182;,
  4;182,182,183,183;,
  4;183,183,184,184;,
  4;184,184,185,185;,
  4;185,185,186,186;,
  4;186,186,187,187;,
  4;187,187,188,188;,
  4;188,188,189,189;,
  4;189,189,190,190;,
  4;190,190,191,191;,
  4;191,191,177,177;,
  4;2,0,192,193;,
  4;4,2,193,194;,
  4;6,4,194,195;,
  4;8,6,195,196;,
  4;10,8,196,197;,
  4;12,10,197,198;,
  4;14,12,198,199;,
  4;16,14,199,200;,
  4;18,16,200,201;,
  4;20,18,201,202;,
  4;22,20,202,203;,
  4;24,22,203,204;,
  4;26,24,204,205;,
  4;28,26,205,206;,
  4;0,28,206,192;,
  3;121,121,122;,
  3;207,123,123;,
  3;125,124,124;,
  3;124,124,124;,
  3;124,125,124;,
  3;127,29,29;,
  3;29,29,29;,
  3;29,29,29;,
  4;126,126,126,126;,
  4;208,208,208,208;,
  3;128,128,128;,
  3;128,128,128;,
  3;128,128,128;,
  4;121,209,209,121;,
  4;123,123,210,210;,
  3;128,128,128;,
  3;128,128,128;,
  3;128,128,128;;
 }
 MeshTextureCoords {
  263;
  1.000000;0.083330;,
  1.000000;0.000000;,
  0.933330;0.000000;,
  0.933330;0.083330;,
  0.866670;0.000000;,
  0.866670;0.083330;,
  0.800000;0.000000;,
  0.800000;0.083330;,
  0.733330;0.000000;,
  0.733330;0.083330;,
  0.666670;0.000000;,
  0.666670;0.083330;,
  0.600000;0.000000;,
  0.600000;0.083330;,
  0.533330;0.000000;,
  0.533330;0.083330;,
  0.466670;0.000000;,
  0.466670;0.083330;,
  0.400000;0.000000;,
  0.400000;0.083330;,
  0.333330;0.000000;,
  0.333330;0.083330;,
  0.266670;0.000000;,
  0.266670;0.083330;,
  0.200000;0.000000;,
  0.200000;0.083330;,
  0.133330;0.000000;,
  0.133330;0.083330;,
  0.011320;0.775090;,
  0.042150;0.870000;,
  0.021750;0.675840;,
  0.071640;0.589410;,
  0.108930;0.944160;,
  0.152380;0.530750;,
  0.200100;0.984750;,
  0.250000;0.510000;,
  0.299900;0.984750;,
  0.347620;0.530750;,
  0.391070;0.944160;,
  0.428360;0.589410;,
  0.457850;0.870000;,
  0.478250;0.675840;,
  0.488690;0.775090;,
  0.066670;0.000000;,
  0.066670;0.083330;,
  0.000000;0.000000;,
  0.000000;0.083330;,
  0.547990;0.771070;,
  0.555690;0.687490;,
  0.572600;0.851250;,
  0.599240;0.613830;,
  0.633800;0.914610;,
  0.668430;0.564080;,
  0.707250;0.950080;,
  0.750340;0.544940;,
  0.790120;0.949730;,
  0.831780;0.562290;,
  0.870450;0.913800;,
  0.898800;0.614600;,
  0.925370;0.852200;,
  0.944650;0.689800;,
  0.951470;0.768780;,
  0.066670;0.416670;,
  0.066670;0.416670;,
  0.000000;0.416670;,
  0.000000;0.416670;,
  0.066670;0.333330;,
  0.066670;0.333330;,
  0.000000;0.333330;,
  0.000000;0.333330;,
  0.066670;0.250000;,
  0.066670;0.250000;,
  0.000000;0.250000;,
  0.000000;0.250000;,
  0.066670;0.166670;,
  0.066670;0.166670;,
  0.000000;0.166670;,
  0.000000;0.166670;,
  0.066670;0.083330;,
  0.000000;0.083330;,
  0.133330;0.416670;,
  0.133330;0.416670;,
  0.133330;0.333330;,
  0.133330;0.333330;,
  0.133330;0.250000;,
  0.133330;0.250000;,
  0.133330;0.166670;,
  0.133330;0.166670;,
  0.133330;0.083330;,
  0.200000;0.416670;,
  0.200000;0.416670;,
  0.200000;0.333330;,
  0.200000;0.333330;,
  0.200000;0.250000;,
  0.200000;0.250000;,
  0.200000;0.166670;,
  0.200000;0.166670;,
  0.200000;0.083330;,
  0.266670;0.416670;,
  0.266670;0.416670;,
  0.266670;0.333330;,
  0.266670;0.333330;,
  0.266670;0.250000;,
  0.266670;0.250000;,
  0.266670;0.166670;,
  0.266670;0.166670;,
  0.266670;0.083330;,
  0.333330;0.416670;,
  0.333330;0.416670;,
  0.333330;0.333330;,
  0.333330;0.333330;,
  0.333330;0.250000;,
  0.333330;0.250000;,
  0.333330;0.166670;,
  0.333330;0.166670;,
  0.333330;0.083330;,
  0.400000;0.416670;,
  0.400000;0.416670;,
  0.400000;0.333330;,
  0.400000;0.333330;,
  0.400000;0.250000;,
  0.400000;0.250000;,
  0.400000;0.166670;,
  0.400000;0.166670;,
  0.400000;0.083330;,
  0.466670;0.416670;,
  0.466670;0.416670;,
  0.466670;0.333330;,
  0.466670;0.333330;,
  0.466670;0.250000;,
  0.466670;0.250000;,
  0.466670;0.166670;,
  0.466670;0.166670;,
  0.466670;0.083330;,
  0.533330;0.416670;,
  0.533330;0.416670;,
  0.533330;0.333330;,
  0.533330;0.333330;,
  0.533330;0.250000;,
  0.533330;0.250000;,
  0.533330;0.166670;,
  0.533330;0.166670;,
  0.533330;0.083330;,
  0.600000;0.416670;,
  0.600000;0.416670;,
  0.600000;0.333330;,
  0.600000;0.333330;,
  0.600000;0.250000;,
  0.600000;0.250000;,
  0.600000;0.166670;,
  0.600000;0.166670;,
  0.600000;0.083330;,
  0.666670;0.416670;,
  0.666670;0.416670;,
  0.666670;0.333330;,
  0.666670;0.333330;,
  0.666670;0.250000;,
  0.666670;0.250000;,
  0.666670;0.166670;,
  0.666670;0.166670;,
  0.666670;0.083330;,
  0.733330;0.416670;,
  0.733330;0.416670;,
  0.733330;0.333330;,
  0.733330;0.333330;,
  0.733330;0.250000;,
  0.733330;0.250000;,
  0.733330;0.166670;,
  0.733330;0.166670;,
  0.733330;0.083330;,
  0.800000;0.416670;,
  0.800000;0.416670;,
  0.800000;0.333330;,
  0.800000;0.333330;,
  0.800000;0.250000;,
  0.800000;0.250000;,
  0.800000;0.166670;,
  0.800000;0.166670;,
  0.800000;0.083330;,
  0.866670;0.416670;,
  0.866670;0.416670;,
  0.866670;0.333330;,
  0.866670;0.333330;,
  0.866670;0.250000;,
  0.866670;0.250000;,
  0.866670;0.166670;,
  0.866670;0.166670;,
  0.866670;0.083330;,
  0.933330;0.416670;,
  0.933330;0.416670;,
  0.933330;0.333330;,
  0.933330;0.333330;,
  0.933330;0.250000;,
  0.933330;0.250000;,
  0.933330;0.166670;,
  0.933330;0.166670;,
  0.933330;0.083330;,
  1.000000;0.416670;,
  1.000000;0.416670;,
  1.000000;0.333330;,
  1.000000;0.333330;,
  1.000000;0.250000;,
  1.000000;0.250000;,
  1.000000;0.166670;,
  1.000000;0.166670;,
  1.000000;0.083330;,
  1.000000;0.500000;,
  0.933330;0.500000;,
  0.866670;0.500000;,
  0.800000;0.500000;,
  0.733330;0.500000;,
  0.666670;0.500000;,
  0.600000;0.500000;,
  0.533330;0.500000;,
  0.466670;0.500000;,
  0.400000;0.500000;,
  0.333330;0.500000;,
  0.266670;0.500000;,
  0.200000;0.500000;,
  0.133330;0.500000;,
  0.066670;0.500000;,
  0.000000;0.500000;,
  0.750000;0.510000;,
  0.847620;0.530750;,
  0.928350;0.589410;,
  0.978250;0.675840;,
  0.988690;0.775090;,
  0.957850;0.870000;,
  0.891070;0.944160;,
  0.799900;0.984750;,
  0.700100;0.984750;,
  0.608930;0.944160;,
  0.542150;0.870000;,
  0.511310;0.775090;,
  0.521750;0.675840;,
  0.571650;0.589410;,
  0.652380;0.530750;,
  1.000000;0.500000;,
  0.800000;0.378620;,
  0.800000;0.500000;,
  0.600000;0.500000;,
  0.400000;0.500000;,
  0.500000;0.500000;,
  0.400000;0.000000;,
  0.600000;0.000000;,
  0.021750;0.675840;,
  0.108930;0.944160;,
  0.250000;0.510000;,
  0.391070;0.944160;,
  0.478250;0.675840;,
  0.200000;0.000000;,
  0.200000;0.500000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.750000;0.510000;,
  0.978250;0.675840;,
  0.750000;0.770500;,
  0.750000;0.944160;,
  0.891070;0.944160;,
  1.000000;0.000000;,
  0.800000;0.000000;,
  0.521750;0.675840;,
  0.608930;0.944160;;
 }
}