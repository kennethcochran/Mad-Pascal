// --------------------------------------------------
// Library: a8libgadg.pas
// Desc...: Atari 8 Bit Gadget Library
// Author.: Wade Ripkowski, amarok
// Date...: 2022.09
// License: GNU General Public License v3.0
// Note...: Requires: a8defines.pas
//          -Converted from C
// Require: a8libwin.pas
//          a8libstr.pas
//          a8libmisc.pas
// Revised:
// --------------------------------------------------

unit a8libgadg;

interface

// --------------------------------------------------
// Includes
// --------------------------------------------------
uses
    a8defines, a8defwin;


// --------------------------------------------------
// Function Prototypes
// --------------------------------------------------
procedure GAlert(pS: string);
procedure GProg(bN, x, y, bS: Byte);
function GButton(bN, x, y, bD, bS: Byte; pA: TStringArray): Byte;
function GCheck(bN, x, y, bI, bD: Byte): Byte;
function GRadio(bN, x, y, bD, bE, bI, bS: Byte; pS: TStringArray): Byte;
function GSpin(bN, x, y, bL, bM, bI, bE: Byte): Byte;
function GInput(bN, x, y, bT, bS: Byte; var pS: string): Byte;


implementation

uses
    a8libwin, a8libmisc, a8libstr;

// ------------------------------------------------------------
// Func...: GAlert(pS: string)
// Desc...: Displays centered alert on screen
// Param..: pS = Message string
// Notes..: 38 characters max
// ------------------------------------------------------------
procedure GAlert(pS: string);
var
    bW, bL, x: Byte;
begin
    // Find left window position
    bL := Length(pS);
    x := (38 - bL) div 2;

    // Show window
    bW := WOpen(x, 10, bL + 2, 5, WOFF);
    WOrn(bW, WPTOP, WPCNT, 'Alert!');
    WPrint(bW, WPCNT, 1, WOFF, pS);
    WPrint(bW, WPCNT, 3, WON, ' OK ');

    // Wait for key
    x := WaitKCX(WOFF);

    // Close window
    WClose(bW);
end;


// ------------------------------------------------------------
// Func...: GProg(bN, x, y, bS: Byte)
// Desc...: Displays centered alert on screen
// Param..: bN = Window handle number
//           x = Window column to display at
//           y = Window row to display at
//          bS = Bar size (percent complete, 0-100)
// Notes..: 38 characters max
// ------------------------------------------------------------
procedure GProg(bN, x, y, bS: Byte);
var
    iL: Word;
    cL: string[21];
begin
    // Set default and block line
    cL := Space(20);

    // Update bar contents
    // Div by 5 since bar is 1/5 of 100
    iL := bS div 5;
    FillChar(@cL[1], iL, CHINVSP);

    // Display new bar
    WPrint(bN, x, y, WOFF, cL);
end;


// ------------------------------------------------------------
// Func...: GButton(bN, x, y, bD, bS: Byte; pA: TStringArray): Byte
// Desc...: Displays buttons and get choice
// Param..: bN = Window handle number
//           x = Column of window to place buttons
//           y = Row of window to place buttons
//          bD = Initial selected button (0 to display and exit)
//          bS = Number of buttons
//          pA = Array of button strings
// Notes..: Button ornaments should be defined in strings.
//          Max length of all buttons is 38.
// ------------------------------------------------------------
function GButton(bN, x, y, bD, bS: Byte; pA: TStringArray): Byte;
var
    bF: Boolean;
    bL, bK, bP: Byte;
begin
    bF := false;

    // Set default return
    Result := bD;

    // Continue until exit
    while not bF do
    begin
        // Set drawing position offset
        bP := 0;

        // Display buttons
        for bL := 0 to bS - 1 do
        begin
            // Display button (inverse if the selected one)
            if Result = bL + 1 then
            begin
                WPrint(bN, x + bP, y, WON, pA[bL]);
            end
            else begin
                WPrint(bN, x + bP, y, WOFF, pA[bL]);
            end;

            // Increase drawing position by button length
            Inc(bP, Length(pA[bL]));
        end;

        // If display item is 0, exit
        if bD = GDISP then
        begin
            bF := true;
        end
        else begin
            // Get a keystroke
            bK := WaitKCX(WOFF);

            // Process keystroke
            if (bK = KLEFT) or (bK = KPLUS) or (bK = KUP) or (bK = KMINUS) then
            begin
                // Decrement and check for underrun
                Dec(Result);
                if Result < 1 then
                begin
                    Result := bS;
                end;
            end
            else if (bK = KRIGHT) or (bK = KASTER) or (bK = KDOWN) or (bK = KEQUAL) then
            begin
                // Increment and check for overrun
                Inc(Result);
                if Result > bS then
                begin
                    Result := 1;
                end;
            end
            else if bK = KESC then
            begin
                Result := XESC;
                bF := true;
            end
            else if bK = KTAB then
            begin
                Result := XTAB;
                bF := true;
            end
            else if bK = KENTER then
            begin
                bF := true;
            end;
        end;
    end;
end;


// ------------------------------------------------------------
// Func...: GCheck(bN, x, y, bI, bD: Byte): Byte
// Desc...: Displays check box and get choice
// Param..: bN = Window handle number
//           x = Column of window to place buttons
//           y = Row of window to place buttons
//          bI = Display Only indicator
//          bD = Default (initial value)
// Notes..: Associated text string should be drawn seperately.
// ------------------------------------------------------------
function GCheck(bN, x, y, bI, bD: Byte): Byte;
var
    bF: Boolean;
    bK, bC: Byte;
    tmpStr: string;
begin
    bF := false;

    // Set default return and current value to the passed default
    Result := bD;
    bC := bD;

    // Draw check frame
    WPrint(bN, x, y, WOFF, '[ ]');

    // Continue until finished
    while not bF do
    begin
        // If ON then add marker, else space, and inverse it
        if bc = GCON then
        begin
            tmpStr := 'X';
        end
        else begin
            tmpStr := ' ';
        end;
        WPrint(bN, x + 1, y, WON, tmpStr);

        // If display only, exit
        if bI = GDISP then
        begin
            bF := true;
        end
        else begin
            // Get keystroke
            bK := WaitKCX(WOFF);

            // Process keystroke
            if bK = KESC then
            begin
                // Set esc exit and exit flag
                Result := XESC;
                bF := true;
            end
            else if bK = KTAB then
            begin
                // Set tab exit and exit flag
                Result := XTAB;
                bF := true;
            end
            else if (bK = KSPACE) or (bK = KX) or (bK = KX_S) then
            begin
                // Toggle value
                if bC = GCON then
                begin
                    bC := GCOFF;
                end
                else begin
                    bC := GCON;
                end;
                Result := bC;
                bF := true;
            end;
        end;
    end;

    // If current and return are same
    if Result = bC then
    begin
        // Show exit value
        if Result = GCON then
        begin
            tmpStr := 'X';
        end
        else begin
            tmpStr := ' ';
        end;
        WPrint(bN, x + 1, y, WOFF, tmpStr);
    end
    else begin
        // Show default value (no change)
        if bD = GCON then
        begin
            tmpStr := 'X';
        end
        else begin
            tmpStr := ' ';
        end;
        WPrint(bN, x + 1, y, WOFF, tmpStr);
    end;
end;


// ------------------------------------------------------------
// Func...: GRadio(bN, x, y, bD, bE, bI, bS: Byte; pS: TStringArray): Byte;
// Desc...: Display radio buttons and get choice
// Param..: bN = Window handle number
//           x = Column of window to place buttons
//           y = Row of window to place buttons
//          bD = Direction of button placement
//          bE = Edit or display indicator (0 to display and exit)
//          bI = Initial selected button
//          bS = Number of buttons
//          pS = Pointer to array of radio button strings
// ------------------------------------------------------------
function GRadio(bN, x, y, bD, bE, bI, bS: Byte; pS: TStringArray): Byte;
var
    bF: Boolean;
    bL, bK, bC, xp, yp: Byte;
begin
    bF := false;

    // Set default return and current button tod default passed in
    Result := bI;
    bC := bI;

    // Loop until exit
    while not bF do
    begin
        // Set drawing position
        xp := 0;
        yp := 0;

        // Display buttons
        for bL := 0 to bS - 1 do
        begin
            // If current item then add pointer, else space
            WPos(bN, x + xp, y + yp);
            if ((bL + 1) = bC) and (bE <> GDISP) then
            begin
                WPut(bN, CHRGT_I);
            end
            else begin
                WPut(bN, CHSPACE);
            end;

            // If selected then add filled circle, else unfilled
            WPos(bN, x + xp + 1, y + yp);
            if bL + 1 = Result then
            begin
                WPut(bN, CHBALL);
            end
            else begin
                WPut(bN, CHO_L);
            end;

            // Display button label
            WPrint(bN, x + xp + 3, y + yp, WOFF, pS[bL]);

            // Compute next button location
            if bD = GHORZ then
            begin
                // Increase X position
                Inc(xp, Length(pS[bL]) + 4);
            end
            else begin
                // Increase Y position
                Inc(yp);
            end;
        end;

        // If initial item is display only, set exit flag
        if bE = GDISP then
        begin
            bF := true;
        end
        // Not display, edit, do it.
        else begin
            // Get keystroke
            bK := WaitKCX(WOFF);

            // Process keystrokes
            // Up or left
            if (bK = KLEFT) or (bK = KPLUS) or (bK = KUP) or (bK = KMINUS) then
            begin
                // Decrement and check for underrun
                Dec(bC);
                if bC < 1 then
                begin
                    bC := bS;
                end;
            end
            // Down or right
            else if (bK = KRIGHT) or (bK = KASTER) or (bK = KDOWN) or (bK = KEQUAL) then
            begin
                // Increment and check for overrun
                Inc(bC);
                if bC > bS then
                begin
                    bC := 1;
                end;
            end
            // ESC
            else if bK = KESC then
            begin
                Result := XESC;
                bF := true;
            end
            // Tab
            else if bK = KTAB then
            begin
                Result := XTAB;
                bF := true;
            end
            // Space
            else if bK = KSPACE then
            begin
                Result := bC;
            end
            // Enter
            else if bK = KENTER then
            begin
                Result := bC;
                bF := true;
            end;
        end;
    end;
end;


// ------------------------------------------------------------
// Func...: GSpin(bN, x, y, bL, bM, bI, bE: Byte): Byte
// Desc...: Display value and spin 0 to 100
// Param..: bN = Window handle number
//           x = Column of window to place buttons
//           y = Row of window to place buttons
//          bL = Lowest allowed value
//          bM = Max allowed value
//          bI = Initial value
//          bE = GDISP to display only, GEDIT to edit
// Notes..: Max is 250 (above are form control values)
// ------------------------------------------------------------
function GSpin(bN, x, y, bL, bM, bI, bE: Byte): Byte;
var
    bD, bK: Byte;
    bF: Boolean;
    cL: string[4];
begin
    bF := false;

    // Set working value to initial value
    bD := bI;

    // Ensure max is not greater than 250
    if bM > 250 then
    begin
        bM := 250;
    end;

    // If display only, just copy to string and set exit flag
    if bE = GDISP then
    begin
        cL := ByteToStr3(bD);
        bF := true;
    end;

    // Loop until exit
    while not bF do
    begin
        // Convert to string and display in inverse
        cL := ByteToStr3(bD);
        WPrint(bN, x, y, WON, cL);

        // Only adjust if in edit mode
        if bE = GEDIT then
        begin
            // Get key
            bK := WaitKCX(WOFF);

            // Process keystroke
            if (bK = KLEFT) or (bK = KPLUS) or (bK = KDOWN) or (bK = KEQUAL) then
            begin
                // Decrement only if not 0, then check for underrun
                if bD <> 0 then
                begin
                  Dec(bD);
                end;
                if bD < bL then
                begin
                    bD := bL;
                end;
            end
            else if (bK = KRIGHT) or (bK = KASTER) or (bK = KUP) or (bK = KMINUS) then
            begin
                // Increment and check for overrun
                Inc(bD);
                if bD > bM then
                begin
                    bD := bM;
                end;
            end
            else if bK = KESC then
            begin
                Result := XESC;
                bF := true;
                cL := ByteToStr3(bI);
            end
            else if bK = KTAB then
            begin
                Result := XTAB;
                bF := true;
                cL := ByteToStr3(bI);
            end
            else if bK = KENTER then
            begin
                Result := bD;
                bF := true;
                cL := ByteToStr3(bD);
            end;
        end;
    end;

    // Redisplay value post edit, or initially for display only
    WPrint(bN, x, y, WOFF, cL);
end;


// ------------------------------------------------------------
// Func...: GInput(bN, x, y, bT, bS: Byte; var pS: string): Byte
// Desc...: Gets string with type restrictions
// Param..: bN = Window handle number
//           x = Column of window to place buttons
//           y = Row of window to place buttons
//          bT = Allowed character type
//          bS = Display size for string (max 40)
//          pS = Pointer to string to edit (max 128)
// ------------------------------------------------------------
function GInput(bN, x, y, bT, bS: Byte; var pS: string): Byte;
var
    bF, bP: Boolean;
    bD, bE, bK, bC, bL, bZ, bI: Byte;
    cD: string[41];
    cE: string[128];
begin
    // Defaults for return, start display and edit postions, and loop exit
    Result := Byte(false);
    bD := 0;
    bE := 0;
    bF := false;

    // Get string size
    bZ := Length(pS);

    // Copy original string to edit buffer
    SetLength(cE, Length(pS));
    Move(@pS[1], @cE[1], Length(pS));

    // Loop until exit (ESC or RETURN)
    while not bF do
    begin
        // Copy display string from edit buffer
        SetLength(cD, bS);
        Move(@cE[bD + 1], @cD[1], bs);

        // Inverse the cursor char & make sure it doesnt run past visible end
        bI := bE - bD;
        if bI > bS - 1 then
        begin
            bI := bS - 1;
        end;
        cD[bI + 1] := Char(Byte(cD[bI + 1]) xor 128);

        // Display editable portion of string in inverse
        WPrint(bN, x, y, WON, cD);

        // Wait for keystroke
        bK := WaitKCX(WOFF);

        // Get ATASCII version of keystroke
        bC := IKC2ATA(bK);

        // Is internal code RIGHT?
        if bK = KRIGHT then
        begin
            Inc(bE);
        end
        // Is internal code LEFT?
        else if bK = KLEFT then
        begin
            // Decrement only if not 0 already
            if bE <> 0 then
            begin
                Dec(bE);
            end;
        end
        // Is internal code Ctrl-Shft-S (start of string)?
        else if bK = KS_CS then
        begin
            bE := 0;
        end
        // Is internal code Ctrl-Shft-E (end of string)?
        else if bK = KE_CS then
        begin
            bE := bZ;
        end
        // Is internal code DEL?
        // Set char to space, move position
        else if bK = KDEL then
        begin
            // Move edit and display positions if > 1
            if bE > 0 then
            begin
                Dec(bE);
                cE[bE + 1] := CHSPACE;
            end;
        end
        // Is internal code Shift_Del?  (clear line)
        else if bK = KDEL_S then
        begin
            FillChar(@cE[1], bZ, CHSPACE);
        end
        // Is internal code Ctrl-Del? (right delete)
        else if bK = KDEL_C then
        begin
            // Copy each char to the char before
            SetLength(cE, bZ);
            for bL := bE + 1 to bZ - 1 do
            begin
                cE[bL] := cE[bL + 1];
            end;

            // Set last char to space
            cE[bZ] := Char(CHSPACE);
        end
        // Is internal code INS? (right insert)
        else if bK = KINS then
        begin
            // Only insert if not at end
            if bE < bZ then
            begin
                // Copy each char to the char after work from end
                bL := bZ;
                while bL < bE + 1 do
                begin
                    cE[bL + 1] := cE[bL];
                    Dec(bL);
                end;

                // Put space in current position
                cE[bE + 1] := CHSPACE;
            end;
        end
        // Is internal code ENTER?
        else if bK = KENTER then
        begin
            // Copy edit buffer to original string
            SetLength(pS, Length(cE));
            Move(@cE[1], @pS[1], Length(cE));
            Result := Byte(true);
            bF := true;
        end
        // Is internal code ESC?
        else if bK = KESC then
        begin
            // Restore original string
            SetLength(cE, Length(pS));
            Move(@pS[1], @cE[1], Length(cE));
            Result := XESC;
            bF := true;
        end
        // Is internal code TAB?
        else if bK = KTAB then
        begin
            // Restore original string
            SetLength(cE, Length(pS));
            Move(@pS[1], @cE[1], Length(cE));
            Result := XTAB;
            bF := true;
        end
        // Is ATASCII code a printing char?
        // 0 (heart) is not possible
        else if (bC >= 1) and (bC <= 191) then
        begin
            // Set add flag to false
            bP := false;

            // Apply type restrictions
            // For ANY, allow all but cursor keys
            if (bT = GANY) and ((bC <= 28) or (bC >= 32)) then
            begin
                bP := true;
            end
            // For ALNUM, allow _ 0-9 A-Z a-z
            else if bT = GALNUM then
            begin
                if (bC = 32) or ((bC >= 48) and (bC <= 57)) or
                                ((bC >= 65) and (bC <= 90)) or
                                ((bC >= 97) and (bC <= 122)) then
                begin
                    bP := true;
                end;
            end
            // For ALPHA, allow _ A-Z a-z
            else if bT = GALPHA then
            begin
                if (bC = 32) or ((bC >= 65) and (bC <= 90)) or
                                ((bC >= 97) and (bC <= 122)) then
                begin
                    bP := true;
                end;
            end
            // For NUMBER, allow . - 0-9
            else if bT = GNUMER then
            begin
                if (bC = 45) or (bC = 46) or
                   ((bC >= 48) and (bC <= 57)) then
                begin
                    bP := true;
                end;
            end;

            // Replace char in edit buffer at edit position if  allowed
            if bP then
            begin
                cE[bE + 1] := Char(bC);
                Inc(bE);
            end;
        end;

        // Check edit & display position extents

        // If edit > max len (-1=0 based), set equal (-1)
        if bE > bZ - 1 then
        begin
            bE := bZ - 1;
        end;

        // If edit >= display size, then display = edit - size + 1
        if bE >= bS then
        begin
            bD := bE - bS + 1;
        end
        // Else if edit < display size then display = 0
        else if bE < bS then
        begin
            bD := 0;
        end;
    end;

    // Get display string and print
    SetLength(cD, bS);
    Move(@cE[1], @cD[1], bS);
    WPrint(bN, x, y, WOFF, cD);
end;


end.