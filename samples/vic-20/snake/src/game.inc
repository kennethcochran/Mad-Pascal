const
  txt_level   = 'level 00'~;
  txt_points  = 'points 0000'~;
  txt_time    = 't:00'~;
  txt_hscore  = 'hi:0000'~;
  txt_fire    = 'press fire'~;
  txt_turbo   = 'hold fire for turbo'~;
  txt_info    = 'score points until t:99'~;

  SNAKE_SIZE  = 2;
  GAME_SPEED  = 14;
  LEVEL_THOLD = 10;
  SPEED_THOLD = 2;
  TIME_E_SET  = 4 * 50;
  TIME_THOLD  = $99;


  UP          = C_TAIL_UP;
  DOWN        = C_TAIL_DOWN;
  LEFT        = C_TAIL_LEFT;
  RIGHT       = C_TAIL_RIGHT;

  MOVE_UP     = -ROW_SIZE;
  MOVE_DOWN   =  ROW_SIZE;
  MOVE_LEFT   = -1;
  MOVE_RIGHT  =  1;

//o-------------------------------------------------------------o

type
  TSnake = record head, body: byte; m: shortint end;

//o-------------------------------------------------------------o

var
  t0b            : byte absolute $70;
  t1b            : byte absolute $71;
  t2b            : byte absolute $72;

  t0w            : word absolute $73;
  t1w            : word absolute $75;

//:-------------------------------------------------------------:

var
  snake_speed    : byte absolute $77;
  snake_spd_c    : byte absolute $78;
  tail_dir       : byte absolute $79;
  head_dir       : byte absolute $7a;
  level_up       : byte absolute $7b;
  time_entity    : byte absolute $7c;
  time_bcd       : byte absolute $7d;
  level_bcd      : byte absolute $7e;
  level          : byte absolute $7f;

  score_bcd      : word absolute $80;
  hi_score_bcd   : word absolute $82;
  head_pos       : word absolute $84;
  head_pos_col   : word absolute $86;
  tail_pos       : word absolute $88;

  fruit_on_board : boolean absolute $8a;

  snake          : TSnake absolute $8b;


  // system free ZP variables
  game_over      : boolean absolute $fb;
  sf0b           : byte absolute $fb;
  sf1b           : byte absolute $fc;


  hi_scr_score   : byte absolute SCREEN_ADR + 5;
  scr_score      : byte absolute SCREEN_ADR + SCREEN_SIZE - 4;
  scr_level      : byte absolute SCREEN_ADR + SCREEN_SIZE - ROW_SIZE + 4;
  scr_time       : byte absolute SCREEN_ADR + SCREEN_SIZE - ROW_SIZE + 9;

//o-------------------------------------------------------------o

procedure welcome_scr; inline;
begin
  clrcol(GAME_WHITE);

  t0b := ROW_SIZE shr 1; t2b := ROW_SIZE shr 1;

  t1b := length(txt_info) shr 1; dec(t0b, t1b);
  set_xy(t0b, 2);
  print(GAME_YELLOW, txt_info);

  t1b := length(txt_turbo) shr 1; dec(t2b, t1b);
  set_xy(t2b, COL_SIZE shr 1 + 10);
  print(GAME_YELLOW, txt_turbo);

  repeat until (JOY and JOY_FIRE) <> 0;
end;

//:-------------------------------------------------------------:

procedure draw_frame;
var
  i : byte absolute $70; // t0b
begin
  t0w := SCREEN_ADR + ROW_SIZE;
  for i := (COL_SIZE - 4) downto 0 do begin
    poke(t0w, C_WALL_VW);
    poke(t0w + (ROW_SIZE - 1), C_WALL_VE);
    inc(t0w, ROW_SIZE);
  end;

  for i := (ROW_SIZE - 1) downto 0 do begin
    poke(SCREEN_ADR + i, C_WALL_H);
    poke((SCREEN_ADR + SCREEN_SIZE - 2 * ROW_SIZE) + i, C_WALL_H);
  end;
end;

//:-------------------------------------------------------------:

procedure press_fire;
begin
  set_xy(ROW_SIZE - length(txt_fire) - 2, 0);
  print(GAME_WHITE, txt_fire);

  repeat until (JOY and JOY_FIRE) <> 0;
end;

//:-------------------------------------------------------------:

procedure put_snake;
begin
  head_dir     := UP;
  tail_dir     := UP;
  head_pos     := SCREEN_ADR   + ROW_SIZE shr 1 + SCREEN_SIZE shr 1;
  head_pos_col := COLORMAP_ADR + ROW_SIZE shr 1 + SCREEN_SIZE shr 1;

  poke(head_pos, C_HEAD_UP); poke(head_pos_col, GAME_WHITE);

  t0w := head_pos; t1w := head_pos_col;
  for t0b := (SNAKE_SIZE - 2) downto 0 do begin
    inc(t0w, ROW_SIZE); inc(t1w, ROW_SIZE);
    poke(t0w, C_BODY_V); poke(t1w, GAME_WHITE);
  end;

  inc(t0w, ROW_SIZE); inc(t1w, ROW_SIZE); tail_pos := t0w;
  poke(tail_pos, C_TAIL_UP); poke(t1w, GAME_WHITE);
end;

//:-------------------------------------------------------------:

procedure draw_bottom_line;
begin
  set_xy(0, COL_SIZE - 1);
  print(GAME_WHITE, txt_level);

  set_xy(length(txt_level) + 1, COL_SIZE - 1);
  print(GAME_WHITE, txt_time);

  set_xy(ROW_SIZE - length(txt_points), COL_SIZE - 1);
  print(GAME_WHITE, txt_points);
end;

//:-------------------------------------------------------------:

procedure print_hi_score;
begin
  set_xy(2, 0);
  print(GAME_WHITE, txt_hscore);
  update_counter_4(0, @hi_score_bcd, @hi_scr_score);
end;

//:-------------------------------------------------------------:

procedure hall_of_fame;
begin
  if score_bcd > hi_score_bcd then hi_score_bcd := score_bcd;
  print_hi_score;
end;

//:-------------------------------------------------------------:

procedure move_tail;
begin
  poke(tail_pos, C_SPACE);

  case tail_dir of
    UP    : dec(tail_pos, ROW_SIZE);
    DOWN  : inc(tail_pos, ROW_SIZE);
    RIGHT : inc(tail_pos);
    LEFT  : dec(tail_pos);
  end;

  t0b := peek(tail_pos);

  case tail_dir of
    UP, DOWN : begin
      case t0b of
        C_BODY_NW, C_BODY_SW : tail_dir := LEFT;
        C_BODY_NE, C_BODY_SE : tail_dir := RIGHT;
      end;
    end;
    RIGHT, LEFT : begin
      case t0b of
        C_BODY_SW, C_BODY_SE : tail_dir := UP;
        C_BODY_NW, C_BODY_NE : tail_dir := DOWN;
      end;
    end;
  end;

  poke(tail_pos, tail_dir);
end;

//:-------------------------------------------------------------:

procedure update_snake(s: byte); register;
begin
  case (s and %00111111) of
    $01: snake.body := C_BODY_SE;
    $02: snake.body := C_BODY_SW;
    $04: snake.body := C_BODY_NE;
    $08: snake.body := C_BODY_NW;
    $10: snake.body := C_BODY_V;
    $20: snake.body := C_BODY_H;
  end;

  case (s and %11000000) of
    $00 : begin snake.head := C_HEAD_UP;    snake.m := MOVE_UP    end;
    $40 : begin snake.head := C_HEAD_DOWN;  snake.m := MOVE_DOWN  end;
    $80 : begin snake.head := C_HEAD_RIGHT; snake.m := MOVE_RIGHT end;
    $c0 : begin snake.head := C_HEAD_LEFT;  snake.m := MOVE_LEFT  end;
  end;

  t0w := head_pos + snake.m;
  t1w := peek(t0w);

  if (t1w = C_SPACE) or (t1w = C_FRUIT) then begin

    if t1w = C_FRUIT then begin
      fruit_on_board := false;

      update_counter_4($10, @score_bcd, @scr_score);

      dec(level_up);
      if level_up = 0 then begin
        inc(level);
        update_counter_2($1, @level_bcd, @scr_level);
        level_up := LEVEL_THOLD;
      end;

    end else
      move_tail;

    poke(head_pos, snake.body);

    head_pos_col := head_pos_col + snake.m;
    poke(t0w, snake.head);
    poke(head_pos_col, GAME_WHITE);
    head_pos := t0w;

  end else
    game_over := true;

end;

procedure move_snake;
begin
  case head_dir of
    UP    : t0b := %00010000; // %00 01 xxxx => body: C_BODY_V
    DOWN  : t0b := %01010000; // %01 01 xxxx => body: C_BODY_V
    RIGHT : t0b := %10100000; // %10 10 xxxx => body: C_BODY_H
    LEFT  : t0b := %11100000; // %11 10 xxxx => body: C_BODY_H
  end;

  update_snake(t0b);
end;

//:-------------------------------------------------------------:

procedure joy_handler;
begin
  t0b := 0;

  if (JOY and JOY_UP <> 0) and (head_dir <> DOWN) then begin
    case head_dir of
      LEFT  : t0b := %00000001; // C_BODY_SE
      RIGHT : t0b := %00000010; // C_BODY_SW
    end;
    head_dir := UP;
  end
  else if (JOY and JOY_DOWN <> 0) and (head_dir <> UP) then begin
    case head_dir of
      LEFT  : t0b := %01000100; // C_BODY_NE
      RIGHT : t0b := %01001000; // C_BODY_NW
    end;
    head_dir := DOWN;
  end
  else if (JOY and JOY_RIGHT <> 0) and (head_dir <> LEFT) then begin
    case head_dir of
      UP    : t0b := %10000100; // C_BODY_NE
      DOWN  : t0b := %10000001; // C_BODY_SE
    end;
    head_dir := RIGHT;
  end
  else if (JOY and JOY_LEFT <> 0) and (head_dir <> RIGHT) then begin
    case head_dir of
      UP    : t0b := %11001000; // C_BODY_NW
      DOWN  : t0b := %11000010; // C_BODY_SW
    end;
    head_dir := LEFT;
  end;
  if (JOY and JOY_FIRE <> 0) then snake_speed := snake_speed shr 2;

  if t0b <> 0 then begin
    snake_spd_c := snake_speed;
    update_snake(t0b);
  end;
end;

//:-------------------------------------------------------------:

procedure put_fruit;
begin
  t0b := prnd(1, 24, ROW_MASK);
  t1b := prnd(1, 27, COL_MASK);

  t0w := ROW_SIZE * t1b; inc(t0w, SCREEN_ADR); inc(t0w, t0b);

  if peek(t0w) = C_SPACE then begin
    set_xy(t0b, t1b); put_char(GAME_GREEN, C_FRUIT);
    fruit_on_board := true;
  end;
end;

//:-------------------------------------------------------------:

procedure update_time; inline;
begin
  dec(time_entity);
  if time_entity = 0 then begin
    time_entity := TIME_E_SET;
    update_counter_2($1, @time_bcd, @scr_time);
  end;
end;

//:-------------------------------------------------------------:

procedure snake_step; inline;
begin
  snake_speed := GAME_SPEED - level;
  if snake_speed < SPEED_THOLD then snake_speed := SPEED_THOLD;

  joy_handler;

  if snake_spd_c = 0 then begin
    move_snake;
    snake_spd_c := snake_speed;
  end else
    dec(snake_spd_c);
end;

//:-------------------------------------------------------------:

procedure set_game;
begin
  clrscr(C_SPACE); clrcol(GAME_YELLOW);
  draw_frame; draw_bottom_line; put_snake; print_hi_score;

  time_entity    := TIME_E_SET;
  level_up       := LEVEL_THOLD;
  score_bcd      := $0;
  time_bcd       := $0;
  level_bcd      := $0;
  level          := 0;
  snake_spd_c    := 0;
  fruit_on_board := false;
  game_over      := false;

  wait(1);
end;

//:-------------------------------------------------------------:

procedure restart_game;
begin
  hall_of_fame;
  wait(25);
  press_fire;
  set_game;
end;

//:-------------------------------------------------------------:

procedure vbi; keep;
begin
  if not game_over then begin
    update_time;
    snake_step;
    if not fruit_on_board then put_fruit;
    if time_bcd = TIME_THOLD then game_over := true;
  end;
end;

//o-------------------------------------------------------------o

procedure prepare;
begin
  Move(pointer(CRT_CHARS_ADR), pointer(CHARSET_ADR), CHARSET_SIZE);
  Move(pointer(CRT_TITLE_ADR), pointer(SCREEN_ADR), SCREEN_SIZE);
end;

//o-------------------------------------------------------------o

begin
  game_over := true;

  prepare; sys_init; welcome_scr;

  hi_score_bcd := $0; set_game;

  repeat
    if game_over then restart_game;
  until false;
end.

//o-------------------------------------------------------------o
