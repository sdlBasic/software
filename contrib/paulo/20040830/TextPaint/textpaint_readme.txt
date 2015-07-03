
'-- I'm having difficulties coding this example
'-- The idea is you beign able to edit a text document like a picture
'--   (like those very known .txt pictures ), being able to use 
'--   both keyboard and mouse for it (mouse would draw the last typed chr)
'-- The reason is there are no rare situations we need to draw also some
'--   sketches on a text document, and this example would try helping on it
'-- I'm having difficulties to use the memory bank commands as well
'-- Some code portions i were about using on it below 

'-----

xprop=8:yprop=8
xched=80:yched=60
xcur=4:ycur=4
dim mat$ [xched,yched]

reservebank(1,yched*(xched+2))

'-----

for i=0 to (yched*(xched+2)):poke 1,i,0:next

for i=0 to (yched*(xched+2)) step xched+2 
  poke 1,i+xched,0x0D: poke 1,i+xched+1,0x0A:next

lastchr$=inkey$

xnmou=xmouse:ynmou=ymouse
xomou=xnmou:yomou=ynmou

'------

xnmou=xmouse:ynmou=ymouse
if  (xomou<>xnmou) or (yomou<>ynmou) then
   xcur=xnmou\xprop
   ycur=ynmou\yprop
 end if

'------

if mbutton=1 then
   locate xcur,ycur
   print lastchr$
   mat$[xcur,ycur]=lastchr$

 end if

'------

if xcur>(xched-1) then
   ycur=ycur+1:xcur=0:end if

if ycur>(yched-1) then
   ycur=0:xcur=0:end if
