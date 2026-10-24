# Object table previews

Each object in the README table has a 512×288 animated preview on a plain black canvas
(shown at 256×144), black and white only, without a decorative frame.

| Object | Source |
|---|---|
| Match | demo samples 236–254: the last throw and the CAT WINS screen (4×) |
| Cat, Dog | firmware `bitmap_cat` / `bitmap_dog` and `bitmap_star`, drawn the way `cd_game_player_display()` does it: idle, then 12 hit ticks with a 1 px shake, the blink rule `(hit_ticks / 2) % 2` and the star (9×) |
| Border | the 6×26 wall drawn like `cd_game_border_display()` (outline and a brick line every 4 px), with `bitmap_dust_I` / `bitmap_dust_II` alternating for the 10 dust ticks (9×) |
| Wind | the wind arrow and strength cropped from each turn of the demo capture (12×) |
| Power | the cat and its power bar cropped from demo samples 2–12 while charging (9×) |
| Missile | demo samples 11–58: the fish flight and the bone thrown back (4×) |
| Link | the 2 Players sequence from `cd_game_screens_uart.json`: waiting, match start, throw, LINK LOST (4×) |
| CPU | demo samples 31–60: the computer aims, thinks and throws (4×) |

Captured previews keep the recorded frames and durations (long static holds shortened to
1.5 s). The Cat, Dog and Border previews are generated from the firmware bitmaps and drawing
rules, not recorded, and use the game tick (50 ms) for each effect step.
