// -------------------------------------------------------------------------------------------
//					LTEQ.
// -------------------------------------------------------------------------------------------


    if lda(i) and										// lda K+3		; 0	CARDINAL <=
       cmp_im(i+1) and										// cmp #		; 1
       bne(i+2) and										// bne @+		; 2
       lda(i+3) and										// lda K+2		; 3
       cmp_im(i+4) and										// cmp #		; 4
       bne(i+5) and										// bne @+		; 5
       lda(i+6) and										// lda K+1		; 6
       cmp_im(i+7) and										// cmp #		; 7
       bne(i+8) and										// bne @+		; 8
       lda(i+9) and										// lda K		; 9
       cmp_im(i+10) and										// cmp #		; 10
       (listing[i+11] = '@') and								//@			; 11
       (listing[i+12] = #9'bcc @+') and								// bcc @+		; 12
       jne(i+13) and										// jne l_		; 13
       (listing[i+14] = '@') then 								//@			; 14
     begin
      c := GetWORD(i+10, i+7) + GetWORD(i+4, i+1) shl 16;

      if c = 0 then begin
       listing[i+1] := '';
       listing[i+2] := listing[i+13];

       listing[i+4] := '';
       listing[i+5] := listing[i+13];

       listing[i+7] := '';
       listing[i+8] := listing[i+13];

       listing[i+10] := '';
       listing[i+11] := '';
       listing[i+12] := '';

       listing[i+14] := '';

       Result:=false; Break;
      end else
      if c < $FFFFFFFF then begin

        inc(c);

	listing[i+1] := #9'cmp #$'+IntToHex(byte(c shr 24), 2);

	listing[i+4] := #9'cmp #$'+IntToHex(byte(c shr 16), 2);

	listing[i+7] := #9'cmp #$'+IntToHex(byte(c shr 8), 2);

	if c and $ff = 0 then begin
	 listing[i+8] := #9'sne';

	 listing[i+10] := '';
	 listing[i+11] := '';
	end else
	 listing[i+10] := #9'cmp #$'+IntToHex(c and $ff, 2);

	listing[i+12] := '';
	listing[i+13] := #9'jcs ' + copy(listing[i+13], 6, 256);
	listing[i+14] := '';

	Result:=false; Break;
      end else begin
	listing[i]    := '';
	listing[i+1] := '';
	listing[i+2] := '';
	listing[i+3] := '';
	listing[i+4] := '';
	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';
	listing[i+10] := '';
	listing[i+11] := '';
	listing[i+12] := '';
	listing[i+13] := '';
	listing[i+14] := '';

	Result:=false; Break;
      end;

     end;


    if (SKIP(i-1) = false) and
       lda(i) and										// lda K+1		; 0	WHILE WORD <=
       cmp_im(i+1) and										// cmp #		; 1
       bne(i+2) and										// bne @+		; 2
       lda(i+3) and										// lda K		; 3
       cmp_im(i+4) and										// cmp #		; 4
       (listing[i+5] = '@') and									//@			; 5
       jcc(i+6) and										// jcc l_		; 6
       jeq(i+7) then										// jeq l_		; 7
     begin
      p := GetWORD(i+4, i+1);

      if (p > 0) and (p < $FFFF) then begin
       inc(p);

       listing[i+1] := #9'cmp #$'+IntToHex(byte(p shr 8), 2);
       listing[i+2] := listing[i+6];

       listing[i+7] := '@';

       if p and $ff <> 0 then
        listing[i+5] := #9'cmp #$'+IntToHex(p and $ff, 2)
       else
        listing[i+5] := '';

       listing[i+4] := listing[i+3];
       listing[i+3] := #9'bne @+';

       Result:=false; Break;
      end;

     end;


    if (SKIP(i-1) = false) and
       lda(i) and										// lda K+1		; 0	WORD <=
       cmp_im(i+1) and										// cmp #		; 1
       bne(i+2) and										// bne @+		; 2
       lda(i+3) and										// lda K		; 3
       cmp_im(i+4) and										// cmp #		; 4
       (listing[i+5] = '@') and									//@			; 5
       (listing[i+6] = #9'bcc @+') and								// bcc @+		; 6
       jne(i+7) and										// jne l_		; 7
       (listing[i+8] = '@') then 								//@			; 8
     begin

      p := GetWORD(i+4, i+1);

      if p = 0 then begin
       listing[i+1] := '';
       listing[i+2] := listing[i+7];

       listing[i+4] := '';
       listing[i+5] := '';
       listing[i+6] := '';

       listing[i+8] := '';

       Result:=false; Break;
      end else
      if p < $FFFF then begin

        inc(p);

	listing[i+1] := #9'cmp #$'+IntToHex(byte(p shr 8), 2);

	if p and $ff = 0 then begin
	 listing[i+2] := #9'sne';

	 listing[i+4] := '';
	 listing[i+5] := '';
	end else
	 listing[i+4] := #9'cmp #$'+IntToHex(p and $ff, 2);

	listing[i+6] := '';
	listing[i+7] := #9'jcs ' + copy(listing[i+7], 6, 256);
	listing[i+8] := '';

	Result:=false; Break;
      end else begin
	listing[i]   := '';
	listing[i+1] := '';
	listing[i+2] := '';
	listing[i+3] := '';
	listing[i+4] := '';
	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';

	Result:=false; Break;
      end;

     end;


    if (SKIP(i-1) = false) and
       lda(i) and										// lda K+1		; 0	BOOL := WORD <=  ->  WORD >=
       cmp(i+1) and (cmp_im(i+1) = false) and							// cmp W+1		; 1
       bne(i+2) and										// bne @+		; 2
       lda(i+3) and										// lda K		; 3
       cmp(i+4) and (cmp_im(i+4) = false) and							// cmp W		; 4
       (listing[i+5] = '@') and									//@			; 5
       (listing[i+6] = #9'bcc @+') and								// bcc @+		; 6
       (listing[i+7] = #9'beq @+') and								// beq @+		; 7
       dey(i+8) and										// dey			; 8
       (listing[i+9] = '@') then 								//@			; 9
      begin

{ !!!
	if cmp_im(i+1) and cmp_im(i+4) then begin
         c := GetWORD(i+4, i+1);

	 if c < $FFFF then begin
	  listing[i+1] := #9'cmp #$' + IntToHex(byte((c+1) shr 8),2);

	  listing[i+4] := #9'cmp #$' + IntToHex(byte(c+1),2);

	  listing[i+7] := '';

     	  Result:=false; Break;
	 end;

	end else begin
}
 	 tmp := listing[i];
	 listing[i]   := #9'lda ' + copy(listing[i+1], 6, 256);
	 listing[i+1] := #9'cmp ' + copy(tmp, 6, 256);

	 tmp := listing[i+3];
	 listing[i+3] := #9'lda ' + copy(listing[i+4], 6, 256);
	 listing[i+4] := #9'cmp ' + copy(tmp, 6, 256);

	 listing[i+6] := #9'bcs @+';
	 listing[i+7] := '';

	 Result:=false; Break;

//	end;

      end;


    if (SKIP(i-1) = false) and
       lda(i) and										// lda K+1		; 0	BOOL := WORD <=  ->  WORD >=
       cmp_im(i+1) and										// cmp #		; 1
       bne(i+2) and										// bne @+		; 2
       lda(i+3) and										// lda K		; 3
       cmp_im(i+4) and										// cmp #		; 4
       (listing[i+5] = '@') and									//@			; 5
       (listing[i+6] = #9'bcc @+') and								// bcc @+		; 6
       (listing[i+7] = #9'beq @+') and								// beq @+		; 7
       dey(i+8) and										// dey			; 8
       (listing[i+9] = '@') and 								//@			; 9
       tya(i+10) and										// tya			; 10
       sta(i+11) then										// sta			; 11
      begin

        c := GetWORD(i+4, i+1);

	if c = 0 then begin
	  listing[i+1] := '';
	  listing[i+2] := '';
	  listing[i+3] := #9'ora ' + copy(listing[i+3], 6, 256);
	  listing[i+4] := '';

	  listing[i+6] := '';

     	  Result:=false; Break;
	end else

	if c < $FFFF then begin
	  listing[i+1] := #9'cmp #$' + IntToHex(byte((c+1) shr 8),2);

	  listing[i+4] := #9'cmp #$' + IntToHex(byte(c+1),2);

	  listing[i+7] := '';

     	  Result:=false; Break;
	end;

      end;


    if (SKIP(i-1) = false) and
       lda(i) and										// lda K		; 0	BYTE <=
       cmp_im(i+1) and										// cmp #		; 1
       (listing[i+2] = #9'bcc @+') and								// bcc @+		; 2
       jne(i+3) and										// jne l_		; 3
       (listing[i+4] = '@') then								//@			; 4
     begin
      p := GetBYTE(i+1);

      if p = 0 then begin
       listing[i+2] := '';

       listing[i+4] := '';

       Result:=false; Break;
      end else
      if p < $FF then begin
	listing[i+1] := #9'cmp #$'+IntToHex((p+1) and $ff, 2);

	listing[i+2] := '';
	listing[i+3] := #9'jcs ' + copy(listing[i+3], 6, 256);
	listing[i+4] := '';

	Result:=false; Break;
      end else begin
	listing[i]   := '';
	listing[i+1] := '';
	listing[i+2] := '';
	listing[i+3] := '';
	listing[i+4] := '';

	Result:=false; Break;
      end;

     end;


    if (SKIP(i-1) = false) and									//				BYTE <=  ->  BYTE >=
       lda(i) and										// lda			; 0
       cmp(i+1) and (cmp_im(i+1) = false) and							// cmp			; 1
       jcc(i+2) and										// jcc			; 2
       jeq(i+3) then										// jeq			; 3
      begin
       listing[i+2] := #9'cmp ' + copy(listing[i], 6, 256);
       listing[i+1] := #9'lda ' + copy(listing[i+1], 6, 256);
       listing[i]   := '';

       listing[i+3] := #9'jcs ' + copy(listing[i+3], 6, 256);

       Result:=false; Break;
      end;


    if (SKIP(i-1) = false) and									//				BOOL := BYTE <=  ->  BYTE >=
       lda(i) and										// lda			; 0
       cmp(i+1) and 										// cmp			; 1
       bcc(i+2) and										// bcc @+		; 2
       beq(i+3) and										// beq @+		; 3
       dey(i+4) and										// dey			; 4
       (listing[i+5] = '@') and									//@			; 5
       tya(i+6) then										// tya			; 6
      begin
       listing[i+2] := #9'cmp ' + copy(listing[i], 6, 256);
       listing[i+1] := #9'lda ' + copy(listing[i+1], 6, 256);
       listing[i]   := '';

       listing[i+3] := #9'bcs @+';

       Result:=false; Break;
      end;


    if (listing[i+8] = #9'.ENDL') and								// .ENDL		; 8
       bmi(i+9) and										// bmi			; 9
       jne(i+10) and										// jne			; 10
       (listing[i+11] = '@') and								//@			; 11
       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	SHORTINT <= xx
       lda(i+1) and										// lda E		; 1
       sub_im(i+2) and										// sub #		; 2
       (listing[i+3] = #9'beq L5') and								// beq L5		; 3
       (listing[i+4] = #9'bvc L5') and								// bvc L5		; 4
       (listing[i+5] = #9'eor #$FF') and							// eor #$FF		; 5
       (listing[i+6] = #9'ora #$01') and							// ora #$01		; 6
       (listing[i+7] = 'L5') then								//L5			; 7
      begin
        p := GetBYTE(i+2) + 1;

	if p = $80 then begin
	 listing[i]   := '';
	 listing[i+1] := '';
	 listing[i+2] := '';
	 listing[i+3] := '';
	 listing[i+4] := '';
	end else begin
	 listing[i] := listing[i+1];
	 listing[i+1] := #9'sub #$' + IntToHex(p and $ff, 2);
	 listing[i+2] := #9'svc';
	 listing[i+3] := #9'eor #$80';
	 listing[i+4] := #9'jpl ' + copy(listing[i+10], 6, 256);
	end;

	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';
	listing[i+10] := '';
	listing[i+11] := '';

	Result:=false; Break;
      end;


    if (listing[i+8] = #9'.ENDL') and								// .ENDL		; 8
       bmi(i+9) and										// bmi			; 9
       beq(i+10) and										// beq			; 10
       dey(i+11) and										// dey			; 11
       (listing[i+12] = '@') and								//@			; 12

       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	SHORTINT_0 <= SHORTINT_1  ->  SHORTINT_1 >= SHORTINT_0
       lda(i+1) and										// lda A		; 1
       sub(i+2) and (sub_im(i+2) = false) and							// sub B		; 2
       (listing[i+3] = #9'beq L5') and								// beq L5		; 3
       (listing[i+4] = #9'bvc L5') and								// bvc L5		; 4
       (listing[i+5] = #9'eor #$FF') and							// eor #$FF		; 5
       (listing[i+6] = #9'ora #$01') and							// ora #$01		; 6
       (listing[i+7] = 'L5') then								//L5			; 7

      begin
	tmp:=listing[i+1];
	listing[i+1] := #9'lda ' + copy(listing[i+2], 6, 256);
	listing[i+2] := #9'sub ' + copy(tmp, 6, 256);

	listing[i+9]  := #9'bpl @+';
	listing[i+10] := '';

	Result:=false; Break;
      end;

{
    if (listing[i+8] = #9'.ENDL') and								// .ENDL		; 8
       bmi(i+9) and										// bmi			; 9
       jne(i+10) and										// jne			; 10
       (listing[i+11] = '@') and								//@			; 11
       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	SHORTINT_0 <= SHORTINT_1 -> SHORTINT_1 >= SHORTINT_0
       lda(i+1) and										// lda 			; 1
       sub(i+2) and (sub_im(i+2) = false) and							// sub			; 2
       (listing[i+3] = #9'beq L5') and								// beq L5		; 3
       (listing[i+4] = #9'bvc L5') and								// bvc L5		; 4
       (listing[i+5] = #9'eor #$FF') and							// eor #$FF		; 5
       (listing[i+6] = #9'ora #$01') and							// ora #$01		; 6
       (listing[i+7] = 'L5') then								//L5			; 7
      begin
	listing[i]   := #9'lda ' + copy(listing[i+2], 6, 256);
	listing[i+1] := #9'sub ' + copy(listing[i+1], 6, 256);
	listing[i+2] := #9'svc';
	listing[i+3] := #9'eor #$80';
	listing[i+4] := #9'jmi ' + copy(listing[i+10], 6, 256);
	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';
	listing[i+10] := '';
	listing[i+11] := '';

	Result:=false; Break;
      end;
}


    if (listing[i+15] = #9'.ENDL') and								// .ENDL		; 15
       (listing[i+16] = #9'bmi @+') and								// bmi @+		; 16
       jne(i+17) and										// jne l_		; 17
       (listing[i+18] = '@') and								//@			; 18
       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	SMALLINT <= xxyy
       lda(i+1) and										// lda E+1		; 1
       sub_im(i+2) and										// sub #		; 2
       (listing[i+3] = #9'bne L4') and								// bne L4		; 3
       lda(i+4) and										// lda E		; 4
       cmp_im(i+5) and										// cmp #		; 5
       (listing[i+6] = #9'beq L5') and								// beq L5		; 6
       lda_im_0(i+7) and									// lda #$00		; 7
       (listing[i+8] = #9'adc #$FF') and							// adc #$FF		; 8
       (listing[i+9] = #9'ora #$01') and							// ora #$01		; 9
       (listing[i+10] = #9'bne L5') and								// bne L5		; 10
       (listing[i+11] = 'L4'#9'bvc L5') and							//L4 bvc L5		; 11
       (listing[i+12] = #9'eor #$FF') and							// eor #$FF		; 12
       (listing[i+13] = #9'ora #$01') and							// ora #$01		; 13
       (listing[i+14] = 'L5') then								//L5			; 14
      begin
       p := GetWORD(i+5, i+2);

       if p = 0 then begin
	listing[i+10]   := listing[i+1];
	listing[i+11] := #9'bmi @+';
	listing[i+12] := #9'ora ' + copy(listing[i+4], 6, 256);
	listing[i+13] := #9'jne ' + copy(listing[i+17], 6, 256);
	listing[i+14] := '@';
	listing[i+15] := '';
	listing[i+16] := '';
       end else begin
        inc(p);

	if p = $8000 then begin
	 listing[i+10] := '';
	 listing[i+11] := '';
	 listing[i+12] := '';
	 listing[i+13] := '';
	 listing[i+14] := '';
	 listing[i+15] := '';
	 listing[i+16] := '';
	end else begin
	 listing[i+10] := listing[i+4];
	 listing[i+11] := #9'cmp #$' + IntToHex(p and $ff, 2);
	 listing[i+12] := listing[i+1];
	 listing[i+13] := #9'sbc #$' + IntToHex(byte(p shr 8), 2);
	 listing[i+14] := #9'svc';
	 listing[i+15] := #9'eor #$80';
	 listing[i+16] := #9'jpl ' + copy(listing[i+17], 6, 256);
	end;

       end;

	listing[i] := '';
	listing[i+1] := '';
	listing[i+2] := '';
	listing[i+3] := '';
	listing[i+4] := '';
	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';

 	listing[i+17] := '';
	listing[i+18] := '';

	Result:=false; Break;
      end;

{
    if (listing[i+15] = #9'.ENDL') and								// .ENDL		; 15
       (listing[i+16] = #9'bmi @+') and								// bmi @+		; 16
       jne(i+17) and										// jne l_		; 17
       (listing[i+18] = '@') and								//@			; 18
       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	SMALLINT_0 <= SMALLINT_1 -> SMALLINT_1 >= SMALLINT_0
       lda(i+1) and										// lda E+1		; 1
       sub(i+2) and (sub_im(i+2) = false) and							// sub 			; 2
       (listing[i+3] = #9'bne L4') and								// bne L4		; 3
       lda(i+4) and										// lda E		; 4
       cmp(i+5) and (cmp_im(i+5) = false) and							// cmp 			; 5
       (listing[i+6] = #9'beq L5') and								// beq L5		; 6
       lda_im_0(i+7) and									// lda #$00		; 7
       (listing[i+8] = #9'adc #$FF') and							// adc #$FF		; 8
       (listing[i+9] = #9'ora #$01') and							// ora #$01		; 9
       (listing[i+10] = #9'bne L5') and								// bne L5		; 10
       (listing[i+11] = 'L4'#9'bvc L5') and							//L4 bvc L5		; 11
       (listing[i+12] = #9'eor #$FF') and							// eor #$FF		; 12
       (listing[i+13] = #9'ora #$01') and							// ora #$01		; 13
       (listing[i+14] = 'L5') then								//L5			; 14
      begin
	listing[i+10] := #9'lda ' + copy(listing[i+5], 6, 256);
	listing[i+11] := #9'cmp ' + copy(listing[i+4], 6, 256);
	listing[i+12] := #9'lda ' + copy(listing[i+2], 6, 256);
	listing[i+13] := #9'sbc ' + copy(listing[i+1], 6, 256);
	listing[i+14] := #9'svc';
	listing[i+15] := #9'eor #$80';
	listing[i+16] := #9'jmi ' + copy(listing[i+17], 6, 256);

	listing[i] := '';
	listing[i+1] := '';
	listing[i+2] := '';
	listing[i+3] := '';
	listing[i+4] := '';
	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';

 	listing[i+17] := '';
	listing[i+18] := '';

	Result:=false; Break;
      end;
     }


    if (listing[i+15] = #9'.ENDL') and								// .ENDL		; 15
       (listing[i+16] = #9'bmi @+') and								// bmi @+		; 16
       (listing[i+17] = #9'beq @+') and								// beq @+		; 17
       dey(i+18) and										// dey			; 18
       (listing[i+19] = '@') and								//@			; 19

       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	SMALLINT_0 <= SMALLINT_1 -> SMALLINT_1 >= SMALLINT_0
       lda(i+1) and										// lda E+1		; 1
       sub(i+2) and (sub_im(i+2) = false) and							// sub 			; 2
       (listing[i+3] = #9'bne L4') and								// bne L4		; 3
       lda(i+4) and										// lda E		; 4
       cmp(i+5) and (cmp_im(i+5) = false) and							// cmp 			; 5
       (listing[i+6] = #9'beq L5') and								// beq L5		; 6
       lda_im_0(i+7) and									// lda #$00		; 7
       (listing[i+8] = #9'adc #$FF') and							// adc #$FF		; 8
       (listing[i+9] = #9'ora #$01') and							// ora #$01		; 9
       (listing[i+10] = #9'bne L5') and								// bne L5		; 10
       (listing[i+11] = 'L4'#9'bvc L5') and							//L4 bvc L5		; 11
       (listing[i+12] = #9'eor #$FF') and							// eor #$FF		; 12
       (listing[i+13] = #9'ora #$01') and							// ora #$01		; 13
       (listing[i+14] = 'L5') then								//L5			; 14
      begin
        tmp:=listing[i+1];
	listing[i+1] := #9'lda ' + copy(listing[i+2], 6, 256);
	listing[i+2] := #9'sub ' + copy(tmp, 6, 256);

        tmp:=listing[i+4];
	listing[i+4] := #9'lda ' + copy(listing[i+5], 6, 256);
	listing[i+5] := #9'cmp ' + copy(tmp, 6, 256);

	listing[i+16] := '';
	listing[i+17] := #9'bpl @+';

	Result:=false; Break;
      end;


    if (listing[i+22] = #9'.ENDL') and								// .ENDL		; 22
       bmi(i+23) and										// bmi @+		; 23
       jne(i+24) and										// jne l_		; 24
       (listing[i+25] = '@') and								//@			; 25
       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	INTEGER <= 0
       lda(i+1) and										// lda E+3		; 1
       sub_im_0(i+2) and									// sub #$00		; 2
       (listing[i+3] = #9'bne L4') and								// bne L4		; 3
       lda(i+4) and										// lda E+2		; 4
       cmp_im_0(i+5) and									// cmp #$00		; 5
       (listing[i+6] = #9'bne L1') and								// bne L1		; 6
       lda(i+7) and										// lda E+1		; 7
       cmp_im_0(i+8) and									// cmp #$00		; 8
       (listing[i+9] = #9'bne L1') and								// bne L1		; 9
       lda(i+10) and										// lda E		; 10
       cmp_im_0(i+11) and									// cmp #$00		; 11
       (listing[i+12] = 'L1'#9'beq L5') and							//L1 beq L5		; 12
       (listing[i+13] = #9'bcs L3') and								// bcs L3		; 13
       (listing[i+14] = #9'lda #$FF') and							// lda #$FF		; 14
       (listing[i+15] = #9'bne L5') and								// bne L5		; 15
       (listing[i+16] = 'L3'#9'lda #$01') and							//L3 lda #$01		; 16
       (listing[i+17] = #9'bne L5') and								// bne L5		; 17
       (listing[i+18] = 'L4'#9'bvc L5') and							//L4 bvc L5		; 18
       (listing[i+19] = #9'eor #$FF') and							// eor #$FF		; 19
       (listing[i+20] = #9'ora #$01') and							// ora #$01		; 20
       (listing[i+21] = 'L5') then								//L5			; 21
      begin
	listing[i]   := listing[i+1];
	listing[i+1] := #9'bmi @+';
	listing[i+2] := #9'ora ' + copy(listing[i+4], 6, 256);
	listing[i+3] := #9'ora ' + copy(listing[i+7], 6, 256);
	listing[i+4] := #9'ora ' + copy(listing[i+10], 6, 256);
	listing[i+5] := #9'jne ' + copy(listing[i+24], 6, 256);
	listing[i+6] := '@';

	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';
	listing[i+10] := '';
	listing[i+11] := '';
	listing[i+12] := '';
	listing[i+13] := '';
	listing[i+14] := '';
	listing[i+15] := '';
	listing[i+16] := '';
	listing[i+17] := '';
	listing[i+18] := '';
	listing[i+19] := '';
	listing[i+20] := '';
	listing[i+21] := '';
	listing[i+22] := '';
	listing[i+23] := '';
	listing[i+24] := '';
	listing[i+25] := '';

	Result:=false; Break;
      end;


    if (listing[i+22] = #9'.ENDL') and								// .ENDL		; 22
       bmi(i+23) and										// bmi @+		; 23
       jne(i+24) and										// jne l_		; 24
       (listing[i+25] = '@') and								//@			; 25
       (listing[i] = #9'.LOCAL') and								// .LOCAL		; 0	INTEGER <= HIGH(INTEGER)
       lda(i+1) and										// lda E+3		; 1
       (listing[i+2] = #9'sub #$7F') and							// sub #$7F		; 2
       (listing[i+3] = #9'bne L4') and								// bne L4		; 3
       lda(i+4) and										// lda E+2		; 4
       (listing[i+5] = #9'cmp #$FF') and							// cmp #$FF		; 5
       (listing[i+6] = #9'bne L1') and								// bne L1		; 6
       lda(i+7) and										// lda E+1		; 7
       (listing[i+8] = #9'cmp #$FF') and							// cmp #$FF		; 8
       (listing[i+9] = #9'bne L1') and								// bne L1		; 9
       lda(i+10) and										// lda E		; 10
       (listing[i+11] = #9'cmp #$FF') and							// cmp #$FF		; 11
       (listing[i+12] = 'L1'#9'beq L5') and							//L1 beq L5		; 12
       (listing[i+13] = #9'bcs L3') and								// bcs L3		; 13
       (listing[i+14] = #9'lda #$FF') and							// lda #$FF		; 14
       (listing[i+15] = #9'bne L5') and								// bne L5		; 15
       (listing[i+16] = 'L3'#9'lda #$01') and							//L3 lda #$01		; 16
       (listing[i+17] = #9'bne L5') and								// bne L5		; 17
       (listing[i+18] = 'L4'#9'bvc L5') and							//L4 bvc L5		; 18
       (listing[i+19] = #9'eor #$FF') and							// eor #$FF		; 19
       (listing[i+20] = #9'ora #$01') and							// ora #$01		; 20
       (listing[i+21] = 'L5') then								//L5			; 21
      begin
	listing[i]   := '';
	listing[i+1] := '';
	listing[i+2] := '';
	listing[i+3] := '';
	listing[i+4] := '';
	listing[i+5] := '';
	listing[i+6] := '';
	listing[i+7] := '';
	listing[i+8] := '';
	listing[i+9] := '';
	listing[i+10] := '';
	listing[i+11] := '';
	listing[i+12] := '';
	listing[i+13] := '';
	listing[i+14] := '';
	listing[i+15] := '';
	listing[i+16] := '';
	listing[i+17] := '';
	listing[i+18] := '';
	listing[i+19] := '';
	listing[i+20] := '';
	listing[i+21] := '';
	listing[i+22] := '';
	listing[i+23] := '';
	listing[i+24] := '';
	listing[i+25] := '';

	Result:=false; Break;
      end;
