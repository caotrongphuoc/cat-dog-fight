# OLED framebuffer captures

## Demo GIF

[`cd_game_demo_uart.json`](cd_game_demo_uart.json) is the source of the README's autoplay
[demo GIF](../videos/cat_dog_fight_demo.gif). It records the real kit's OLED (version 2 kit,
SH1106, `BOARD=cat`) through the kit's USB UART at 115200 8N1 on **2026-10-24**: boot logo,
menu, one full **1 Player** match on **Easy**, the game over screen, then back to the menu.

The recording used a temporary capture build. Only `shell.cpp` was changed, and the change
was reverted after the capture:

- Console printing was enabled and the buzzer was silenced (`LOGIN_PRINT_EN`, `SYS_PRINT_EN`, `APP_SILENT_EN`).
- `lcd d` removed the game tick (`CD_GAME_TIME_TICK`) while the framebuffer was sent over UART,
  printed one state line (match state, turn, phase, power, missile, hit effects) before the dump,
  then posted `CD_GAME_ANIMATE` so the screen re-armed its own tick. The game therefore did not
  move while a frame was on the wire.
- `btn M` / `btn R` posted only `AC_DISPLAY_BUTTON_MODE_PRESSED` / `RELEASED`, so the capture
  script could release MODE when the power bar reached the value it wanted.

The capture script played the cat: it read the wind with `stt`, picked the middle of the hitting
power window at 45° with the same integer flight as the firmware, held MODE and released it at
that power. The computer played the dog with its normal Easy plan and random error. Game rules,
physics, sprites and the computer player were not changed.

Between two samples the script waited one game tick (50 ms), so consecutive samples are one or
two ticks apart. Each moving sample is held **50 ms** in the GIF. Samples where nothing moves
(aiming, the computer thinking) are held for the real time that passed, at most one second, and
identical neighbours are merged. The boot logo (1.5 s), the menu (1.8 s), the game over screen
(3.5 s) and the menu at the end (2 s) are held longer for reading. It is sampled playback,
not a real-time video.

The record has **256 samples** and a **23.2 s** playback. The cat hit five throws in a row and
won. The fatal log of the kit was still empty after the first recording attempt. After the
capture `shell.cpp` was restored from Git and the normal release image (`BOARD=dog`, 50,284 bytes)
was flashed back.

Each sample stores the 1,024-byte framebuffer in page layout: byte `(y // 8) * 128 + x`,
bit `y % 8`. The GIF enlarges every pixel 4× (512×256) with nearest-neighbour scaling, a fixed
black and white palette, and clears each frame before drawing the next one.

## Menu and 2 Players screens

[`cd_game_screens_uart.json`](cd_game_screens_uart.json) holds two more sequences recorded
on **2026-10-24** with the same temporary capture build, used by the menu and 2 Players cards
and by the Link preview:

- **`menu`:** the menu on each of its five items (1 Player, 2 Players, Level, Sound, Exit) and
  back to 1 Player, one sample per press of DOWN, held 0.9 s each.
- **`two_players`:** the cat kit selects 2 Players and shows "Waiting for DOG". The second kit
  runs the normal release image with `BOARD=dog`; its DOWN and MODE pins were pulled low over SWD
  to select 2 Players, so the two kits made a real RF24 handshake. The cat charged and threw,
  then the dog kit's CPU was halted over SWD for 12 s to stand in for a kit losing power. The cat
  kit showed LINK LOST after its 6 s timeout and went back to the menu.

The capture rules are the same as for the demo (game tick removed during each dump, 50 ms
between samples, identical neighbours merged). After the capture both kits were flashed back to
their normal release images (`BOARD=cat` on the ST-Link kit, `BOARD=dog` on the USB kit), and
`shell.cpp` was restored from Git.
