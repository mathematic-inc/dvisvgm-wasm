/*************************************************************************
** psdefs.cpp                                                           **
**                                                                      **
** This file is part of dvisvgm -- a fast DVI to SVG converter          **
** Copyright (C) 2005-2020 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#include "PSInterpreter.hpp"

const char *PSInterpreter::PSDEFS =
"<</Install{matrix setmatrix}/HWResolution[72 72]/PageSize[10000 10000]/Imaging"
"BBox null>>setpagedevice/@dodraw true store/@patcnt 0 store/@GD globaldict def"
"/@SD systemdict def/@UD userdict def @GD/@nulldev false put true setglobal @SD"
"/:save @SD/save get put @SD/:restore @SD/restore get put @SD/:gsave @SD/gsave "
"get put @SD/:grestore @SD/grestore get put @SD/:grestoreall @SD/grestoreall ge"
"t put @SD/:newpath @SD/newpath get put @SD/:stroke @SD/stroke get put @SD/:fil"
"l @SD/fill get put @SD/:eofill @SD/eofill get put @SD/:clip @SD/clip get put @"
"SD/:eoclip @SD/eoclip get put @SD/:charpath @SD/charpath get put @SD/:show @SD"
"/show get put @SD/:stringwidth @SD/stringwidth get put @SD/:nulldevice @SD/nul"
"ldevice get put @SD/:image @SD/image get put @SD/:colorimage @SD/colorimage ge"
"t put @SD/.setopacityalpha known not{@SD/.setopacityalpha{pop}put}if @SD/.sets"
"hapealpha known not{@SD/.setshapealpha{pop}put}if @SD/.setblendmode known not{"
"@SD/.setblendmode{pop}put}if @SD/prseq{-1 1{-1 roll =only( )print}for(\\n)prin"
"t}put @SD/prcmd{( )exch(\\ndvi.)3{print}repeat prseq}put @SD/cvxall{{cvx}foral"
"l}put @SD/defpr{[exch[/copy @SD]cvxall 5 -1 roll dup 6 1 roll[/get/exec]cvxall"
" 6 -1 roll dup 7 1 roll 4 -1 roll dup 5 1 roll dup length string cvs/prcmd cvx"
"]cvx def}put @SD/querypos{{currentpoint}stopped{$error/newerror false put}{2(q"
"uerypos)prcmd}ifelse}put @SD/applyscalevals{1 0 dtransform exch dup mul exch d"
"up mul add sqrt 0 1 dtransform exch dup mul exch dup mul add sqrt 1 0 dtransfo"
"rm dup mul exch dup dup mul 3 -1 roll add dup 0 eq{pop}{sqrt div}ifelse 3(appl"
"yscalevals)prcmd}put @SD/prpath{{2(moveto)prcmd}{2(lineto)prcmd}{6(curveto)prc"
"md}{0(closepath)prcmd}pathforall}put @SD/nulldevice{@GD/@nulldev true put :nul"
"ldevice 1 1(setnulldevice)prcmd}put @SD/charpath{/@dodraw false store :charpat"
"h/@dodraw true store}put @SD/stringwidth{/@dodraw false store :stringwidth/@do"
"draw true store}put @SD/show{@dodraw @GD/@nulldev get not and{dup :gsave curre"
"ntpoint 2{50 mul exch}repeat :newpath moveto 50 50/scale sysexec true charpath"
" fill :grestore/@dodraw false store :show/@dodraw true store}{:show}ifelse}put"
" @SD/varxyshow{exch dup type/arraytype eq{<</arr 3 -1 roll/prc 5 -1 roll/chr 1"
" string/idx 0>>begin{chr 0 3 -1 roll put :gsave chr show :grestore currentpoin"
"t prc moveto/idx idx 1 add store}forall end}{pop show}ifelse}put @SD/xyshow{{e"
"xch arr idx 2 mul get add exch arr idx 2 mul 1 add get add}varxyshow}put @SD/x"
"show{{exch arr idx get add exch}varxyshow}put @SD/yshow{{arr idx get add}varxy"
"show}put @SD/awidthshow{{1 string dup 0 5 index put :gsave show :grestore pop "
"0 rmoveto 3 index eq{4 index 4 index rmoveto}if 1 index 1 index rmoveto}exch c"
"show 5{pop}repeat}put @SD/widthshow{0 0 3 -1 roll awidthshow}put @SD/ashow{0 0"
" 0 6 3 roll awidthshow}put @SD/newpath{:newpath 1 1(newpath)prcmd}put @SD/stro"
"ke{@dodraw @GD/@nulldev get not and{prcolor 0 1(newpath)prcmd prpath 0(stroke)"
"prcmd :newpath}{:stroke}ifelse}put @SD/fill{@dodraw @GD/@nulldev get not and{p"
"rcolor 0 1(newpath)prcmd prpath 0(fill)prcmd :newpath}{:fill}ifelse}put @SD/eo"
"fill{@dodraw @GD/@nulldev get not and{prcolor 0 1(newpath)prcmd prpath 0(eofil"
"l)prcmd :newpath}{:eofill}ifelse}put @SD/clip{:clip @GD/@nulldev get not{0 1(n"
"ewpath)prcmd prpath 0(clip)prcmd}if}put @SD/eoclip{:eoclip @GD/@nulldev get no"
"t{0 1(newpath)prcmd prpath 0(eoclip)prcmd}}put @SD/shfill{begin currentdict/Sh"
"adingType known currentdict/ColorSpace known and currentdict/DataSource known "
"and currentdict/Function known not and ShadingType 4 ge{DataSource type/arrayt"
"ype eq{<</DeviceGray 1/DeviceRGB 3/DeviceCMYK 4/bgknown currentdict/Background"
" known/bbknown currentdict/BBox known>>begin currentdict ColorSpace known{Shad"
"ingType ColorSpace load bgknown{1 Background aload pop}{0}ifelse bbknown{1 BBo"
"x aload pop}{0}ifelse ShadingType 5 eq{VerticesPerRow}if DataSource aload leng"
"th 4 add bgknown{ColorSpace load add}if bbknown{4 add}if ShadingType 5 eq{1 ad"
"d}if(shfill)prcmd}if end}if}if end}put @SD/image{dup type/dicttype eq{dup}{<</"
"Width 6 index/Height 7 index/colorimg false>>}ifelse @execimg}put @SD/colorima"
"ge{<<2 index{/Width 2 index 8 add index/Height 4 index 9 add index}{/Width 8 i"
"ndex/Height 9 index}ifelse/colorimg true>>@execimg}put/@execimg{@UD/@imgcnt 2 "
"copy .knownget{1 add}{1}ifelse put begin<</pngdev null/imgid @imgcnt/colorimg "
"currentdict/colorimg .knownget dup{pop}if/colordev 1 index currentcolorspace d"
"up length 1 ne exch 0 get/DeviceGray ne or or>>begin colordev{/png16m}{/pnggra"
"y}ifelse dup devicedict exch known{:gsave/pngdev exch finddevice def mark/Outp"
"utFile @imgbase imgid 20 string cvs strconcat(.png)strconcat/PageSize[Width He"
"ight]/UseFastColor true pngdev putdeviceprops setdevice[Width 0 0 Height neg 0"
" Height]/setmatrix sysexec colorimg{:colorimage}{:image}ifelse/copypage sysexe"
"c mark/OutputFile()pngdev putdeviceprops pop :grestore imgid Width Height 3(im"
"age)prcmd}{-1 0 0 3(image)prcmd}ifelse end end}def/@rect{4 -2 roll moveto exch"
" dup 0 rlineto exch 0 exch rlineto neg 0 rlineto closepath}bind def/@rectcc{4 "
"-2 roll moveto 2 copy 0 lt exch 0 lt xor{dup 0 exch rlineto exch 0 rlineto neg"
" 0 exch rlineto}{exch dup 0 rlineto exch 0 exch rlineto neg 0 rlineto}ifelse c"
"losepath}bind def @SD/rectclip{:newpath dup type/arraytype eq{aload length 4 i"
"div{@rectcc}repeat}{@rectcc}ifelse clip :newpath}put @SD/rectfill{gsave :newpa"
"th dup type/arraytype eq{aload length 4 idiv{@rectcc}repeat}{@rectcc}ifelse fi"
"ll grestore}put @SD/rectstroke{gsave :newpath dup type/arraytype eq{aload leng"
"th 4 idiv{@rect}repeat}{@rect}ifelse stroke grestore}put false setglobal @SD r"
"eadonly pop/initclip 0 defpr/clippath 0 defpr/sysexec{@SD exch get exec}def/ad"
"ddot{dup length 1 add string dup 0 46 put dup 3 -1 roll 1 exch putinterval}def"
"/setlinewidth{dup/setlinewidth sysexec 1(setlinewidth)prcmd}def/setlinecap 1 d"
"efpr/setlinejoin 1 defpr/setmiterlimit 1 defpr/setdash{mark 3 1 roll 2 copy/se"
"tdash sysexec exch aload length 1 add -1 roll counttomark(setdash)prcmd pop}de"
"f/@setpagedevice{pop<<>>/setpagedevice sysexec matrix setmatrix newpath 0(setp"
"agedevice)prcmd}def/@checknulldev{@GD/@nulldev get{currentpagedevice maxlength"
" 0 ne{@GD/@nulldev false put 0 1(setnulldevice)prcmd}if}if}def/prcolor{current"
"rgbcolor 3(setrgbcolor)prcmd}def/printgstate{@dodraw @GD/@nulldev get not and{"
"matrix currentmatrix aload pop 6(setmatrix)prcmd applyscalevals currentlinewid"
"th 1(setlinewidth)prcmd currentlinecap 1(setlinecap)prcmd currentlinejoin 1(se"
"tlinejoin)prcmd currentmiterlimit 1(setmiterlimit)prcmd currentrgbcolor 3(setr"
"gbcolor)prcmd currentdash mark 3 1 roll exch aload length 1 add -1 roll countt"
"omark(setdash)prcmd pop}if}def/strconcat{exch dup length 2 index length add st"
"ring dup dup 4 2 roll copy length 4 -1 roll putinterval}def/setgstate{/setgsta"
"te sysexec printgstate}def/save{@UD begin/@saveID vmstatus pop pop def end :sa"
"ve @saveID 1(save)prcmd}def/restore{:restore @checknulldev printgstate @UD/@sa"
"veID known{@UD begin @saveID end}{0}ifelse 1(restore)prcmd}def/gsave 0 defpr/g"
"restore{:grestore @checknulldev printgstate 0(grestore)prcmd}def/grestoreall{:"
"grestoreall @checknulldev setstate 0(grestoreall)prcmd}def/rotate{dup type/arr"
"aytype ne @dodraw and{dup 1(rotate)prcmd}if/rotate sysexec applyscalevals}def/"
"scale{dup type/arraytype ne @dodraw and{2 copy 2(scale)prcmd}if/scale sysexec "
"applyscalevals}def/translate{dup type/arraytype ne @dodraw and{2 copy 2(transl"
"ate)prcmd}if/translate sysexec}def/setmatrix{dup/setmatrix sysexec @dodraw{alo"
"ad pop 6(setmatrix)prcmd applyscalevals}{pop}ifelse}def/initmatrix{matrix setm"
"atrix}def/concat{matrix currentmatrix matrix concatmatrix setmatrix}def/makepa"
"ttern{gsave<</mx 3 -1 roll>>begin dup/XUID[1000000 @patcnt]put mx/makepattern "
"sysexec dup dup begin PatternType @patcnt BBox aload pop XStep YStep PaintType"
" mx aload pop 15(makepattern)prcmd :newpath matrix setmatrix PaintProc 0 1(mak"
"epattern)prcmd end/@patcnt @patcnt 1 add store end grestore}def/setpattern{beg"
"in PatternType 1 eq{PaintType 1 eq{XUID aload pop exch pop 1}{:gsave[currentco"
"lorspace aload length -1 roll pop]setcolorspace/setcolor sysexec XUID aload po"
"p exch pop currentrgbcolor :grestore 4}ifelse(setpattern)prcmd}{/setpattern sy"
"sexec}ifelse end}def/setcolor{dup type/dicttype eq{setpattern}{/setcolor sysex"
"ec/currentrgbcolor sysexec setrgbcolor}ifelse}def/setgray 1 defpr/setcmykcolor"
" 4 defpr/sethsbcolor 3 defpr/setrgbcolor 3 defpr/.setopacityalpha{dup/.setopac"
"ityalpha sysexec 1(setopacityalpha)prcmd}def/.setshapealpha{dup/.setshapealpha"
" sysexec 1(setshapealpha)prcmd}def/.setblendmode{dup/.setblendmode sysexec<</N"
"ormal 0/Compatible 0/Multiply 1/Screen 2/Overlay 3/SoftLight 4/HardLight 5/Col"
"orDodge 6/ColorBurn 7/Darken 8/Lighten 9/Difference 10/Exclusion 11/Hue 12/Sat"
"uration 13/Color 14/Luminosity 15/CompatibleOverprint 16>>exch get 1(setblendm"
"ode)prcmd}def/@pdfpagecount{(r)file runpdfbegin pdfpagecount runpdfend}def/@pd"
"fpagebox{(r)file runpdfbegin dup dup 1 lt exch pdfpagecount gt or{pop}{pdfgetp"
"age/MediaBox pget pop aload pop}ifelse runpdfend}def DELAYBIND{.bindnow}if ";

