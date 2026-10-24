# Banner-themed screen cards

The README shows the OLED screens inside 720×480 cards made from the banner's backyard
theme: the dusk glow, scanlines, faint brick wall and throw arc, the firmware's 5×7 font,
and the amber colours. Each card background is a different 720×480 crop of the shared
1280×640 banner background, with a title, a subtitle, a bezel and a footer added **outside**
the OLED viewport. The empty cards are saved in [`backgrounds/`](backgrounds/).

| Card | Raw OLED animation | Source frames |
|---|---|---|
| 01 Menu | [`scr_game_menu.gif`](../scr_game_menu.gif) | menu cycle in [`cd_game_screens_uart.json`](../../../captures/cd_game_screens_uart.json) |
| 02 Gameplay | [`scr_game_cat_dog.gif`](../scr_game_cat_dog.gif) | demo samples 2–60: the cat's first turn and the computer's answer |
| 03 2 Players | [`scr_game_wait.gif`](../scr_game_wait.gif) | 2 Players sequence in [`cd_game_screens_uart.json`](../../../captures/cd_game_screens_uart.json) |
| 04 Hit | [`scr_game_cat_dog_hit.gif`](../scr_game_cat_dog_hit.gif) | demo samples 24–40: the fish lands and the hit effect runs |
| 05 Game Over | [`scr_game_over.gif`](../scr_game_over.gif) | demo samples 236–254: the last throw and CAT WINS |

Demo samples come from [`cd_game_demo_uart.json`](../../../captures/cd_game_demo_uart.json).

The OLED viewport is exactly **512×256** at **(104, 128)**: the logical 128×64 pixels enlarged
4× with nearest-neighbour sampling. Nothing is drawn over the screen. GIF frame order and
per-frame durations follow the capture; static holds longer than 1.5 s are shortened to 1.5 s
(the game over screen keeps its 3.5 s hold). A fixed palette reserves pure black and white for
the OLED, and each frame is cleared before the next one, so no trails are left. The decorated
backgrounds are documentation art, not firmware graphics.
