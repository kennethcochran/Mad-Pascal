BYTE FUNC InCarpet(BYTE x,y)
  DO
    IF x MOD 3=1 AND y MOD 3=1 THEN
      RETURN (0)
    FI
    x==/3 y==/3
  UNTIL x=0 AND y=0
  OD
RETURN (1)

PROC DrawCarpet(INT x0 BYTE y0,depth)
  BYTE i,x,y,size

  size=1
  FOR i=1 TO depth
  DO size==*3 OD

  FOR y=0 TO size-1
  DO
    FOR x=0 TO size-1
    DO
      IF InCarpet(x,y) THEN
        Plot(x0+2*x,y0+2*y)
        Plot(x0+2*x+1,y0+2*y)
        Plot(x0+2*x+1,y0+2*y+1)
        Plot(x0+2*x,y0+2*y+1)
      FI
    OD
  OD
RETURN

PROC Main()
  BYTE CH=$02FC,COLOR1=$02C5,COLOR2=$02C6

  Graphics(8+16)
  Color=1
  COLOR1=$0C
  COLOR2=$02

  DrawCarpet(79,15,4)

  DO UNTIL CH#$FF OD
  CH=$FF
RETURN

