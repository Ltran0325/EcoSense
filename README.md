# EcoSense

An open source device that collects and transmits ecosystem data.

## Hardware

### Board
Nucleo L476RG

### UART Peripheral Devices
1. Camera
3. Wi-Fi
4. GPS
5. LTE

#### Camera SC03MPA

This module retrieves image capture data. Image data is stored as hex data which can be read and converted to a jpeg file using an editor such as Notepad++.  Image data can be stored on an SD card for later use. Baud rate is set to 115200 bps. Refer to SC03MPA User Manual, Rev. C for more details. 
##### Camera Image Capture Example

![EcoSense Camera Response Image 8-15-21](https://user-images.githubusercontent.com/62213019/129496232-ca1dcfe2-618c-4a0b-bdc5-17be9190a87b.jpg)

##### Camera Test Response (CoolTerm)

************************************************

Beginning camera capture image sequence.

************************************************

************************************************
SEND: camera stop capture
************************************************
v.6..
************************************************
SEND: camera image resolution to 160x120
************************************************
v.T..
************************************************
SEND: camera image compression ratio to 99
************************************************
v.1..
************************************************
SEND: camera image get
************************************************
v.6..
************************************************
SEND: camera image length
************************************************

Success: image length 
v.4.....ü
************************************************
SEND: camera image data
************************************************
v.2..ÿØÿþ.$} ˜................x. ..2..Q.Q...ÿÛ.„. ..... ...$" &0O40,,0aEI:Oseywqeom·›‡­‰moŸØ¡­½ÂÌÎÌ{™àðÞÆî·ÈÌÄ."$$0*0]44]ÄƒoƒÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÿþ.....ÿÀ....x. ..!.......ÿÄ.¢...........................
............................
.................}........!1A..Qa."q.2‘¡.#B±Á.RÑð$3br‚.
.....%&'()*456789:CDEFGHIJSTUVWXYZcdefghijstuvwxyzƒ„…†‡ˆ‰Š’“”•–—˜™š¢£¤¥¦§¨©ª²³´µ¶·¸¹ºÂÃÄÅÆÇÈÉÊÒÓÔÕÖ×ØÙÚáâãäåæçèéêñòóôõö÷øùú................w.......!1..AQ.aq."2..B‘¡±Á.#3Rð.brÑ
.$4á%ñ....&'()*56789:CDEFGHIJSTUVWXYZcdefghijstuvwxyz‚ƒ„…†‡ˆ‰Š’“”•–—˜™š¢£¤¥¦§¨©ª²³´µ¶·¸¹ºÂÃÄÅÆÇÈÉÊÒÓÔÕÖ×ØÙÚâãäåæçèéêòóôõö÷øùúÿÝ...
ÿÚ..........?.ˆqÖ‚).PqNÇ.j.ŒÑj„ä.x!†*·Ô´..¦”óÈ9 .$±¥ÎÞ.¥%ub¢í¨.€Š.b¦;XrÞâ²†..)S@
¢ÂŠ.( .ÿÐ‚(.ô„.bœ¹©–ÅGqHõ4ÞE(¾…Iu.dÓ¹J².Žâ“¨Ç9¤.ç..ÜäÔ¨ÙÜ§+«...¯>”î.J."&Lr:S*F.”.( .ÿÑ„SúŠLBŽx4„bŠ
).ëY½.Í.¼¬'CÅ8.ÃÒ¯¥È{Ø@Jœ.”¤g‘Å..¼0:Ó@ç.l=Ç.?.2Iã­DuEÉYŽ.wÖ˜éÜP"*JC
(.ÿÒ†œ§..8óÈ¥.#.‘CHÅ(÷¤ö..Œô¦ò)Eô.—Qÿ.).ÐH¦Hâ9ÊÓIï@.SN#..5-ÙØ´®®'n¼ÓÔäsùÐÄ†¼yéÖ <.jJ.Š.ÿÓ„RÐHå8¥#¸¤0ûÔ”.Px Œô¨ÙÜ½Õ„..œH#ÔÕ b.)B.¹ .<Ž´.˜ÔKMKŽº.ØÝFsœ.)­Pžã•û.”I.aïëP2³.§.’ŸÿÔ€RŠ..œ.é@.Œt œýi.%.ãŠM\iØBsA.¡i =u.žŠ¦H.©Í)...•HÆ“N#.*[´¬ZWÆž1NWÇ.¥
.
è.#UJœ.’ÿÕ¯J)’:Š@9[ÖŽ‡4†) ýi´.åÅ+r+7ñ./„a.¢´NæMXz¶F
'+Ï84.§®V•NrMg5¥ËƒÖÂw'.R`€.cUº.ÌUl})Î¡Ö¤gÿÖ¯KLih.SÏ.“..ºiN.È¤.ià“S5ÔÒ. Œ{S(†ÂžáOßòàÕ.¨.à.R1‚¼ƒIê5 g÷t˜Æ2x¨Ž…ËPïŽµ*®.}i=..ÿ×‚Šd…-.-..ðwqI¦¤aŽ3IœP1Àü¼SO'Š…£¹£ÕXB1EhÌš±"6x=qH.RG¯.¤1H.ñÊš2.ûb³’kSH´Õ†.
HU-UÈZ;.ÿÐ‚Š.
)€´´.f‚s@.<Ó˜w.†4.S×€Mg5¡¤.¢}å$Ó*¡Ø™÷
y|¨.­Q"Ž.Òx<fü„úTµub“³¸.»šh8¨[.-ÏÿÑ‚Š.
)€´P.E..ål})..žGJ@qI«¢“³¸ìîàSHÇJÎ>ë±¤½åq(­ŒIw.\.1úÓG÷["¤¡„1IšMXiÜÿÒ‹.˜ ¢˜...Q@...ªØëÒ‚;ö¤1.Á§qŒ.õœ×Ràô°Ö.ÒUÅÝ.%f.àæžpã#¯Jl.±ãéÅ35,hÿÓŽÒ.”S.”S.Í-..P.S”ã­..9â..–®†˜¼cŽôŒ1Q.gfi%ut6€psZ™
äm.b£ÍC).ÿÔe.„4ŠCLBRS.¥ AE..P0É¤ ..šSÓëYKsXì4ŠmhÑ›V.ši4¤8ŸÿÕe.‚ÌCHiŠÃM%1Y…..˜Q@Y…..˜”P.
.`ÒjãM¡Ù.{
cv¬ã£±¤µC.¦.W".?ÿÿÿÙv.2..
************************************************
SEND: camera stop capture
************************************************
v.6..
************************************************

SUCCESS: Camera image captured successfully.

************************************************

## Software

### Scheduler

Round-Robin



