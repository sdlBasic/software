;	ScreenSaver - rnd8x8
;	Paulo Silva (GPL) - 200101211933 (HotSoupProcessor version)

;	a='s'
	s=cmdline:peek a,s,1	
	if a='s' : goto *_proc
	if a='p' : goto *_null	
	if a='c' : goto *_null	
	if a=0   : goto *_null	
	dialog "Unknown switch ["+cmdline+"]"
	end

*_proc
	randomize		
	ax=dispx:ay=dispy
	bgscr 2,ax,ay,1,0,0,ax,ay	
	cls 4				
	gmode 0,sx,sy

*_loop	
	 rnd ux,ax
	rnd uy,ay
	 rnd cr,255
	rnd cg,255
	rnd cb,255
	 color cr,cg,cb : boxf ux,uy,ux+7,uy+7
	goto *_loop
	
*_null
	stop