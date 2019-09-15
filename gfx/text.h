/* Generated with shader-compressor by NR4/Team210. */
#ifndef TEXT_H
#define TEXT_H
const char * text_frag =
"/* Endeavor by Team210 - 64k intro by Team210 at Revision 2k19\n"
" * Copyright (C) 2019  Alexander Kraus <nr4@z10.info>\n"
" *\n"
" * This program is free software: you can redistribute it and/or modify\n"
" * it under the terms of the GNU General Public License as published by\n"
" * the Free Software Foundation, either version 3 of the License, or\n"
" * (at your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
" * GNU General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU General Public License\n"
" * along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
" */\n"
" \n"
"#version 130\n"
"\n"
"uniform float iFontWidth, iTime;\n"
"uniform vec2 iResolution;\n"
"uniform sampler2D iChannel0, iFont;\n"
"\n"
"out vec4 gl_FragColor;\n"
"\n"
"// Global constants\n"
"const vec3 c = vec3(1.,0.,-1.);\n"
"const float pi = acos(-1.);\n"
"float a; // Aspect ratio\n"
"\n"
"void rand(in vec2 x, out float num);\n"
"void lfnoise(in vec2 t, out float num);\n"
"void rshort(in float off, out float val);\n"
"void rfloat(in float off, out float val);\n"
"void dbox(in vec2 x, in vec2 b, out float dst);\n"
"void dcircle(in vec2 x, out float d);\n"
"void dlinesegment(in vec2 x, in vec2 p1, in vec2 p2, out float d);\n"
"void drhomboid(in vec2 x, in vec2 b, in float tilt, out float dst);\n"
"void dcirclesegment(in vec2 x, in float r, in float p0, in float p1, out float d);\n"
"void stroke(in float d0, in float s, out float d);\n"
"void dglyph(in vec2 x, in float ordinal, in float size, out float dst);\n"
"void dstring(in vec2 x, in float ordinal, in float size, out float dst);\n"
"void dfloat(in vec2 x, in float num, in float size, out float dst);\n"
"void smoothmin(in float a, in float b, in float k, out float dst);\n"
"void dint(in vec2 x, in float num, in float size, in float ndigits, out float dst);\n"
"\n"
"// Fixme: remove vec4 technique in favor of separate distance\n"
"// void blendadd(in vec4 src1, in vec4 src2, in float tlo, in float thi, out vec4 dst)\n"
"// {\n"
"//     vec4 added;\n"
"//     add(src1, src2, added);\n"
"//     dst = mix(src1, added, smoothstep(tlo-.5,tlo+.5,iTime)*(1.-smoothstep(thi-.5,thi+.5,iTime)));\n"
"// }\n"
"\n"
"void window(in vec2 x, in vec2 size, in vec3 bg, in float title_index, out vec4 col);\n"
"void progressbar(in vec2 x, in float width, in float progress, out vec4 col);\n"
"\n"
"float sm(float d)\n"
"{\n"
"    return smoothstep(1.5/iResolution.y, -1.5/iResolution.y, d);\n"
"}\n"
"\n"
"void dvoronoi(in vec2 x, out float d, out vec2 z);\n"
"void colorize(in vec2 x, out vec3 col)\n"
"{\n"
"    vec3 c1;\n"
"    vec2 ind,\n"
"        xv,\n"
"        xi;\n"
"    float d,\n"
"        vs = 16.,\n"
"        n,\n"
"        size = .1,\n"
"        xix = mod(x.x, size)-.5*size,\n"
"        xixj = (x.x - xix),\n"
"        ri,\n"
"        rim1,\n"
"        rip1,\n"
"        lines = 8.,\n"
"        da,\n"
"        op,\n"
"        s;\n"
"    \n"
"    // Background blending\n"
"    s = smoothstep(0.,.5,.5-abs(x.y));\n"
"    col = mix(1.e-4*c.xxx, vec3(0.04,0.18,0.24), s);\n"
"    \n"
"    // Background circles\n"
"    dvoronoi(vs*x, d, ind);\n"
"    xv = ind/vs-x;\n"
"    lfnoise(vec2(3.,33.)*ind/vs-3.*iTime*c.xy,n);\n"
"    n = .5+.5*n;\n"
"    d = length(xv)-mix(.0,.35,n)/vs;\n"
"    col = mix(col, n*.5*vec3(1.00,0.40,0.39), sm(d));\n"
"    d = abs(d-.005) -.002;\n"
"    col = mix(col, (1.-n)*vec3(0.49,0.71,0.78), sm(d));\n"
"    \n"
"    for(float i = 1.; i < 9.; i += 1.)\n"
"    {\n"
"        rand((9.-i)*c.xx, op);\n"
"        op = .5+.5*round(16.*op)/16.;\n"
"        x += -.1+.2*op;\n"
"        \n"
"        xix = mod(x.x, size)-.5*size;\n"
"        xixj = (x.x - xix);\n"
"        \n"
"        // Edges\n"
"        lfnoise(2.e0*xixj*c.xx+14.*i, ri);\n"
"        lfnoise(2.e0*(xixj+size)*c.xx+14.*i, rip1);\n"
"        lfnoise(2.e0*(xixj-size)*c.xx+14.*i, rim1);\n"
"\n"
"        float h = .2;\n"
"        \n"
"        ri = h*round(lines*ri)/lines;\n"
"        rip1 = h*round(lines*rip1)/lines;\n"
"        rim1 = h*round(lines*rim1)/lines;\n"
"\n"
"        //if(ri < 0.)\n"
"        {\n"
"            dlinesegment(vec2(xix, x.y), vec2(-.5*size, mix(ri,rim1,.5)), vec2(-.25*size, ri), d);\n"
"            dlinesegment(vec2(xix, x.y), vec2(-.25*size, ri), vec2(.25*size, ri), da);\n"
"            d = min(d, da);\n"
"            dlinesegment(vec2(xix, x.y), vec2(.25*size, ri), vec2(.5*size, mix(ri,rip1,.5)), da);\n"
"            d = min(d, da);\n"
"            stroke(d, .002+.002*op, d);\n"
"            col = mix(col, op*(1.-n)*vec3(1.00,0.40,0.39), sm(d));\n"
"\n"
"            // Dots\n"
"            lfnoise(8.*xixj*c.xx-3.*iTime*c.xy+14.*i, n);\n"
"            n = .5+.5*n;\n"
"            d = length(vec2(xix, x.y-ri))-mix(.0,.35,n)/vs;\n"
"            c1 = mix(vec3(1.00,0.40,0.39), vec3(0.85,0.87,0.89), n);\n"
"            col = mix(col, op*(1.-n)*c1, sm(d));\n"
"            stroke(d - .009, (1.-n)*.005, d);\n"
"            c1 *= 2.4;\n"
"            col = mix(col, op*(1.-n)*c1, sm(d));\n"
"        }\n"
"        \n"
"        x -= -.1+.2*op;\n"
"    }\n"
"    \n"
"    //mix to blackish\n"
"    lfnoise(3.*x.xy-vec2(1.,.1)*iTime, n);\n"
"    stroke(n, .3, n);\n"
"    col = mix(col, 1.e-4*c.xxx, n);\n"
"    col = mix(col, .1*col, 1.-s);\n"
"    \n"
"    col = mix(col, mix(col, vec3(1.00,0.40,0.39), mix(.4,.8,.5+.5*x.y/.1)), sm(abs(x.y)-.1));\n"
"    col = mix(col, c.xxx, sm(abs(abs(x.y)-.11)-.001));\n"
"    \n"
"    col = mix(col, col*col, clamp(-x.y/.1,0.,1.));\n"
"    col *= col;\n"
"}\n"
"\n"
"void mainImage( out vec4 fragColor, in vec2 fragCoord )\n"
"{\n"
"    a = iResolution.x/iResolution.y;\n"
"    vec2 uv = fragCoord/iResolution.yy-0.5*vec2(a, 1.0);\n"
"    \n"
"    float d;\n"
"\n"
"    vec4 old = vec4(-1.,texture(iChannel0, fragCoord/iResolution.xy).rgb), \n"
"    new = old; // Scene\n"
"    \n"
"    // Add overlay\n"
"    colorize(2.*(c.xz*uv-.45*vec2(-a,1.)-12.*c.xy), new.gba);\n"
"    new.gba = mix(old.gba, mix(old.gba, new.gba,.4), step(5.e-2,length(new.gba)));\n"
"    \n"
"    // Add Static text\n"
"    dstring((uv-.45*vec2(-.85*a,1.)), 3., .02, d); // Team210\n"
"\n"
"    new.gba = mix(new.gba, mix(new.gba, c.xxx, .5), sm(d));\n"
"    \n"
"    stroke(d-.002, .001, d);\n"
"    new.gba = mix(new.gba, vec3(1.00,0.40,0.39), sm(d));\n"
"\n"
"    if(iTime < 6.)\n"
"    {\n"
"        vec2 dx = (.25*a+.3*c.xy)*c.xy;\n"
"        if(iTime < 3.)\n"
"        {\n"
"            float ind = mix(100000., 2., clamp(iTime/3.,0.,1));\n"
"            dint(uv+dx*c.xy, ind, .02, 6., d);\n"
"        }\n"
"        else if(iTime < 4.)\n"
"        {\n"
"            dint(uv+dx, 2., .02, 6., d);\n"
"        }\n"
"        else if(iTime < 5.)\n"
"        {\n"
"            dint(uv+dx, 1., .02, 6., d);\n"
"        }\n"
"        else if(iTime < 6.)\n"
"        {\n"
"            dint(uv+dx, 0., .02, 6., d);\n"
"        }\n"
"        \n"
"        float da;\n"
"        dstring(uv+dx-2.*9.*.02*c.xy, 4., .02, da);\n"
"        d = min(d, da);\n"
"            \n"
"        new.gba = mix(new.gba, mix(new.gba, vec3(1.00,0.87,0.57), .7), sm(d));\n"
"        stroke(d-.002, .001, d);\n"
"        new.gba = mix(new.gba, c.xxx, sm(d));\n"
"        \n"
"        \n"
"    }\n"
"    \n"
"//     \n"
"    // Display time\n"
"    /*\n"
"    vec4 b = vec4(1., vec3(0.99,0.64,0.02)), bd = vec4(1., .5*vec3(0.99,0.64,0.02));\n"
"    box(uv-vec2(-.48,.45)-.03*sin(iTime)*c.xy, vec2(.2,.02), b.x);    \n"
"    stroke(b.x, .001, bd.x);\n"
"    add(b, bd, b);\n"
"    box(uv-vec2(-.08,.45)-.03*sin(iTime)*c.xy, vec2(.2,.02), bd.x);\n"
"    bd.gba = vec3(0.44,0.07,0.66);\n"
"    add(b, bd, b);\n"
"    stroke(bd.x, .001, bd.x);\n"
"    add(b, bd, b);\n"
"    dfloat(uv-vec2(-.63,.45)-.03*sin(iTime)*c.xy, iTime, .018, bd.x);\n"
"    stroke(bd.x, .004, bd.x);\n"
"    add(b, bd, b);\n"
"//     dfloat(uv-vec2(-.23,.45)-.03*sin(iTime)*c.xy, iExecutableSize, .018, bd.x);\n"
"    dstring(uv-vec2(-.225,.45)-.03*sin(iTime)*c.xy, 4., .018, bd.x);\n"
"    stroke(bd.x, .004, bd.x);\n"
"    bd.gba = vec3(0.99,0.64,0.02);\n"
"    add(b, bd, b);\n"
"    b.gba = mix(old.gba, b.gba, .8);\n"
"    \n"
"    blendadd(old, b, 5., 999., old);\n"
"    \n"
"    vec4 bda;\n"
"    \n"
"    if(iTime < 49.655)\n"
"    {\n"
"        dstring(uv+.6*c.xy, 1., .05, d); //Meme210 present\n"
"        stroke(d, .01, d);\n"
"        new = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(old,new,2.,8.,new);\n"
"        \n"
"        dstring(uv+.6*c.xy+.1*c.yx, 2., .03, d); // no partycoding this time!\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,4.,8.,new);\n"
"        \n"
"        dstring(uv+.6*c.xy+.15*c.yx, 3., .02, d); // well, shit.\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,6.,8.,new);\n"
"        \n"
"        dstring(uv+.6*c.xy, 5., .03, d); // Anyway, we present\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,9.,15.,new);\n"
"    \n"
"        dstring(uv+.6*c.xy-.15*c.yx, 6., .07, d); // GROSS GLOSS\n"
"        stroke(d, .015, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,11.,15.,new);\n"
"\n"
"        dstring(uv+.6*c.xy-.35*c.yx, 7., .03, d); // QM\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,30.,37.,new);\n"
"\n"
"        dstring(uv+.3*c.xy-.35*c.yx, 8., .03, d); // NR4\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,31.,38.,new);\n"
"\n"
"        dstring(uv+.0*c.xy-.35*c.yx, 9., .03, d); // MIC\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,32.,39.,new);\n"
"\n"
"        dstring(uv-.3*c.xy-.35*c.yx, 10., .03, d); // grenzdevil\n"
"        stroke(d, .005, d);\n"
"        old = vec4(d, mix(old.gba, c.xxx, .6));\n"
"        blendadd(new,old,33.,40.,new);\n"
"\n"
"    }\n"
"    else \n"
"    {\n"
"        new = old;\n"
"    }\n"
"    */\n"
"    \n"
"    fragColor = vec4(new.gba, 1.);\n"
"}\n"
"\n"
"void main()\n"
"{\n"
"    mainImage(gl_FragColor, gl_FragCoord.xy);\n"
"}\n"
"\n"
;
#endif