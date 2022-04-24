/* Copyright (c) 2022 UT Longhorn Racing Solar */

const char *utsvt_logo = "                                                ,,qp@@@@@pw,\n\r\
                                           ,p0R*\"```   ``\"*R0E@w\n\r\
                                       ,=\"`                   `*0b\n\r\
                                    -^                            \"N\n\r\
                                                                    `\\\n\r\
       ,,wp@@BB0EE&&&&&&&&&&&&&&&&&&&&EEEbbBBBBBBBBBBB0BB@@@ppppwwwww,,,,\n\r\
    ;0&&&&&&&&&&&ERRM*\"^```                          ,,,wwwwww,,,,\n\r\
    T&&&&&&&B*`   y  ,=ywpw ,pp p  ,=wwpw    ,=*\"``             ``\"*0Bpw\n\r\
      TE&&&P      E  AL ]P  Rp, Tb/P  0    `                           \"0EW,\n\r\
        `*0p      TWm^  A  +m#\"  R^   R      ,~                          T&&N\n\r\
            `  %                         ,m*`                           ,0&&&\n\r\
                Bp                  ,q@R^       `%p,,               ,w@B&&&E\"\n\r\
                 TE@p,,    ,,,wp@B0M\"              \"T0&E0BBBBBBB0E&&&&&&&R^\n\r\
                   \"0E&&&&&&0RM\"`                      `\"*PRRB0bbBRRM\"`\n\r\
";

#ifdef PARTY_PARROT
const char *party_parrot_frames[10] = {
"                         .cccc;;cc;';c.          \r\n\
                      .,:dkdc:;;:c:,:d:.          \r\n\
                     .loc'.,cc::c:::,..;:.        \r\n\
                   .cl;....;dkdccc::,...c;        \r\n\
                  .c:,';:'..ckc',;::;....;c.      \r\n\
                .c:'.,dkkoc:ok:;llllc,,c,';:.     \r\n\
               .;c,';okkkkkkkk:;lllll,:kd;.;:,.   \r\n\
               co..:kkkkkkkkkk:;llllc':kkc..oNc   \r\n\
             .cl;.,oxkkkkkkkkkc,:cll;,okkc'.cO;   \r\n\
             ;k:..ckkkkkkkkkkkl..,;,.;xkko:',l'   \r\n\
            .,...';dkkkkkkkkkkd;.....ckkkl'.cO;   \r\n\
         .,,:,.;oo:ckkkkkkkkkkkdoc;;cdkkkc..cd,   \r\n\
      .cclo;,ccdkkl;llccdkkkkkkkkkkkkkkkd,.c;     \r\n\
     .lol:;;okkkkkxooc::coodkkkkkkkkkkkko'.oc     \r\n\
   .c:'..lkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkd,.oc     \r\n\
  .lo;,:cdkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkd,.c;     \r\n\
,dx:..;lllllllllllllllllllllllllllllllllc'...     \r\n\
cNO;........................................      \r\n",

"                .ckx;'........':c.                \r\n\
             .,:c:::::oxxocoo::::,',.             \r\n\
            .odc'..:lkkoolllllo;..;d,             \r\n\
            ;c..:o:..;:..',;'.......;.            \r\n\
           ,c..:0Xx::o:.,cllc:,'::,.,c.           \r\n\
           ;c;lkXKXXXXl.;lllll;lKXOo;':c.         \r\n\
         ,dc.oXXXXXXXXl.,lllll;lXXXXx,c0:         \r\n\
         ;Oc.oXXXXXXXXo.':ll:;'oXXXXO;,l'         \r\n\
         'l;;kXXXXXXXXd'.'::'..dXXXXO;,l'         \r\n\
         'l;:0XXXXXXXX0x:...,:o0XXXXx,:x,         \r\n\
         'l;;kXXXXXXXXXKkol;oXXXXXXXO;oNc         \r\n\
        ,c'..ckk0XXXXXXXXXX00XXXXXXX0:;o:.        \r\n\
      .':;..:do::ooookXXXXXXXXXXXXXXXo..c;        \r\n\
    .',',:co0XX0kkkxxOXXXXXXXXXXXXXXXOc..;l.      \r\n\
  .:;'..oXXXXXXXXXXXXXXXXXXXXXXXXXXXXXko;';:.     \r\n\
.ldc..:oOXKXXXXXXKXXKXXXXXXXXXXXXXXXXXXXo..oc     \r\n\
:0o...:dxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxo,.:,     \r\n\
cNo........................................;'     \r\n",

"            .cc;.  ...  .;c.                      \r\n\
         .,,cc:cc:lxxxl:ccc:;,.                   \r\n\
        .lo;...lKKklllookl..cO;                   \r\n\
      .cl;.,:'.okl;..''.;,..';:.                  \r\n\
     .:o;;dkd,.ll..,cc::,..,'.;:,.                \r\n\
     co..lKKKkokl.':lloo;''ol..;dl.               \r\n\
   .,c;.,xKKKKKKo.':llll;.'oOxl,.cl,.             \r\n\
   cNo..lKKKKKKKo'';llll;;okKKKl..oNc             \r\n\
   cNo..lKKKKKKKko;':c:,'lKKKKKo'.oNc             \r\n\
   cNo..lKKKKKKKKKl.....'dKKKKKxc,l0:             \r\n\
   .c:'.lKKKKKKKKKk;....lKKKKKKo'.oNc             \r\n\
     ,:.'oxOKKKKKKKOxxxxOKKKKKKxc,;ol:.           \r\n\
     ;c..'':oookKKKKKKKKKKKKKKKKKk:.'clc.         \r\n\
   ,xl'.,oxo;'';oxOKKKKKKKKKKKKKKKOxxl:::;,.      \r\n\
  .dOc..lKKKkoooookKKKKKKKKKKKKKKKKKKKxl,;ol.     \r\n\
  cx,';okKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKl..;lc.   \r\n\
  co..:dddddddddddddddddddddddddddddddddl::',::.  \r\n\
  co...........................................   \r\n",

"           .ccccccc.                              \r\n\
      .,,,;cooolccoo;;,,.                         \r\n\
     .dOx;..;lllll;..;xOd.                        \r\n\
   .cdo;',loOXXXXXkll;';odc.                      \r\n\
  ,ol:;c,':oko:cccccc,...ckl.                     \r\n\
  ;c.;kXo..::..;c::'.......oc                     \r\n\
,dc..oXX0kk0o.':lll;..cxxc.,ld,                   \r\n\
kNo.'oXXXXXXo',:lll;..oXXOo;cOd.                  \r\n\
KOc;oOXXXXXXo.':lol;..dXXXXl';xc                  \r\n\
Ol,:k0XXXXXX0c.,clc'.:0XXXXx,.oc                  \r\n\
KOc;dOXXXXXXXl..';'..lXXXXXo..oc                  \r\n\
dNo..oXXXXXXXOx:..'lxOXXXXXk,.:; ..               \r\n\
cNo..lXXXXXXXXXOolkXXXXXXXXXkl,..;:';.            \r\n\
.,;'.,dkkkkk0XXXXXXXXXXXXXXXXXOxxl;,;,;l:.        \r\n\
  ;c.;:''''':doOXXXXXXXXXXXXXXXXXXOdo;';clc.      \r\n\
  ;c.lOdood:'''oXXXXXXXXXXXXXXXXXXXXXk,..;ol.     \r\n\
  ';.:xxxxxocccoxxxxxxxxxxxxxxxxxxxxxxl::'.';;.   \r\n\
  ';........................................;l'   \r\n",

"                                                  \r\n\
        .;:;;,.,;;::,.                            \r\n\
     .;':;........'co:.                           \r\n\
   .clc;'':cllllc::,.':c.                         \r\n\
  .lo;;o:coxdllllllc;''::,,.                      \r\n\
.c:'.,cl,.'l:',,;;'......cO;                      \r\n\
do;';oxoc;:l;;llllc'.';;'.,;.                     \r\n\
c..ckkkkkkkd,;llllc'.:kkd;.':c.                   \r\n\
'.,okkkkkkkkc;lllll,.:kkkdl,cO;                   \r\n\
..;xkkkkkkkkc,ccll:,;okkkkk:,co,                  \r\n\
..,dkkkkkkkkc..,;,'ckkkkkkkc;ll.                  \r\n\
..'okkkkkkkko,....'okkkkkkkc,:c.                  \r\n\
c..ckkkkkkkkkdl;,:okkkkkkkkd,.',';.               \r\n\
d..':lxkkkkkkkkxxkkkkkkkkkkkdoc;,;'..'.,.         \r\n\
o...'';llllldkkkkkkkkkkkkkkkkkkdll;..'cdo.        \r\n\
o..,l;'''''';dkkkkkkkkkkkkkkkkkkkkdlc,..;lc.      \r\n\
o..;lc;;;;;;,,;clllllllllllllllllllllc'..,:c.     \r\n\
o..........................................;'     \r\n",

"                                                  \r\n\
           .,,,,,,,,,.                            \r\n\
         .ckKxodooxOOdcc.                         \r\n\
      .cclooc'....';;cool.                        \r\n\
     .loc;;;;clllllc;;;;;:;,.                     \r\n\
   .c:'.,okd;;cdo:::::cl,..oc                     \r\n\
  .:o;';okkx;';;,';::;'....,:,.                   \r\n\
  co..ckkkkkddkc,cclll;.,c:,:o:.                  \r\n\
  co..ckkkkkkkk:,cllll;.:kkd,.':c.                \r\n\
.,:;.,okkkkkkkk:,cclll;.ckkkdl;;o:.               \r\n\
cNo..ckkkkkkkkko,.;loc,.ckkkkkc..oc               \r\n\
,dd;.:kkkkkkkkkx;..;:,.'lkkkkko,.:,               \r\n\
  ;:.ckkkkkkkkkkc.....;ldkkkkkk:.,'               \r\n\
,dc..'okkkkkkkkkxoc;;cxkkkkkkkkc..,;,.            \r\n\
kNo..':lllllldkkkkkkkkkkkkkkkkkdcc,.;l.           \r\n\
KOc,c;''''''';lldkkkkkkkkkkkkkkkkkc..;lc.         \r\n\
xx:':;;;;,.,,...,;;cllllllllllllllc;'.;od,        \r\n\
cNo.....................................oc        \r\n",

"                                                  \r\n\
                                                  \r\n\
                   .ccccccc.                      \r\n\
               .ccckNKOOOOkdcc.                   \r\n\
            .;;cc:ccccccc:,:c::,,.                \r\n\
         .c;:;.,cccllxOOOxlllc,;ol.               \r\n\
        .lkc,coxo:;oOOxooooooo;..:,               \r\n\
      .cdc.,dOOOc..cOd,.',,;'....':l.             \r\n\
      cNx'.lOOOOxlldOc..;lll;.....cO;             \r\n\
     ,do;,:dOOOOOOOOOl'':lll;..:d:''c,            \r\n\
     co..lOOOOOOOOOOOl'':lll;.'lOd,.cd.           \r\n\
     co.'dOOOOOOOOOOOo,.;llc,.,dOOc..dc           \r\n\
     co..lOOOOOOOOOOOOc.';:,..cOOOl..oc           \r\n\
   .,:;.'::lxOOOOOOOOOo:'...,:oOOOc.'dc           \r\n\
   ;Oc..cl'':lldOOOOOOOOdcclxOOOOx,.cd.           \r\n\
  .:;';lxl''''':lldOOOOOOOOOOOOOOc..oc            \r\n\
,dl,.'cooc:::,....,::coooooooooooc'.c:            \r\n\
cNo.................................oc            \r\n",

"                                                  \r\n\
                                                  \r\n\
                                                  \r\n\
                        .cccccccc.                \r\n\
                  .,,,;;cc:cccccc:;;,.            \r\n\
                .cdxo;..,::cccc::,..;l.           \r\n\
               ,do:,,:c:coxxdllll:;,';:,.         \r\n\
             .cl;.,oxxc'.,cc,.';;;'...oNc         \r\n\
             ;Oc..cxxxc'.,c;..;lll;...cO;         \r\n\
           .;;',:ldxxxdoldxc..;lll:'...'c,        \r\n\
           ;c..cxxxxkxxkxxxc'.;lll:'','.cdc.      \r\n\
         .c;.;odxxxxxxxxxxxd;.,cll;.,l:.'dNc      \r\n\
        .:,''ccoxkxxkxxxxxxx:..,:;'.:xc..oNc      \r\n\
      .lc,.'lc':dxxxkxxxxxxxol,...',lx:..dNc      \r\n\
     .:,',coxoc;;ccccoxxxxxxxxo:::oxxo,.cdc.      \r\n\
  .;':;.'oxxxxxc''''';cccoxxxxxxxxxxxc..oc        \r\n\
,do:'..,:llllll:;;;;;;,..,;:lllllllll;..oc        \r\n\
cNo.....................................oc        \r\n",

"                                                  \r\n\
                                                  \r\n\
                              .ccccc.             \r\n\
                         .cc;'coooxkl;.           \r\n\
                     .:c:::c:,,,,,;c;;,.'.        \r\n\
                   .clc,',:,..:xxocc;'..c;        \r\n\
                  .c:,';:ox:..:c,,,,,,...cd,      \r\n\
                .c:'.,oxxxxl::l:.,loll;..;ol.     \r\n\
                ;Oc..:xxxxxxxxx:.,llll,....oc     \r\n\
             .,;,',:loxxxxxxxxx:.,llll;.,,.'ld,   \r\n\
            .lo;..:xxxxxxxxxxxx:.'cllc,.:l:'cO;   \r\n\
           .:;...'cxxxxxxxxxxxxoc;,::,..cdl;;l'   \r\n\
         .cl;':,'';oxxxxxxdxxxxxx:....,cooc,cO;   \r\n\
     .,,,::;,lxoc:,,:lxxxxxxxxxxxo:,,;lxxl;'oNc   \r\n\
   .cdxo;':lxxxxxxc'';cccccoxxxxxxxxxxxxo,.;lc.   \r\n\
  .loc'.'lxxxxxxxxocc;''''';ccoxxxxxxxxx:..oc     \r\n\
olc,..',:cccccccccccc:;;;;;;;;:ccccccccc,.'c,     \r\n\
Ol;......................................;l'      \r\n",

"                                                  \r\n\
                              ,ddoodd,            \r\n\
                         .cc' ,ooccoo,'cc.        \r\n\
                      .ccldo;...',,...;oxdc.      \r\n\
                   .,,:cc;.,'..;lol;;,'..lkl.     \r\n\
                  .dOc';:ccl;..;dl,.''.....oc     \r\n\
                .,lc',cdddddlccld;.,;c::'..,cc:.  \r\n\
                cNo..:ddddddddddd;':clll;,c,';xc  \r\n\
               .lo;,clddddddddddd;':clll;:kc..;'  \r\n\
             .,c;..:ddddddddddddd:';clll,;ll,..   \r\n\
             ;Oc..';:ldddddddddddl,.,c:;';dd;..   \r\n\
           .''',:c:,'cdddddddddddo:,''..'cdd;..   \r\n\
         .cdc';lddd:';lddddddddddddd;.';lddl,..   \r\n\
      .,;::;,cdddddol;;lllllodddddddlcldddd:.'l;  \r\n\
     .dOc..,lddddddddlcc:;'';cclddddddddddd;;ll.  \r\n\
   .coc,;::ldddddddddddddlcccc:ldddddddddl:,cO;   \r\n\
,xl::,..,cccccccccccccccccccccccccccccccc:;':xx,  \r\n\
cNd.........................................;lOc  \r\n"
};
#endif
