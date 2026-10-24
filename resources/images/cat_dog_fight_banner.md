# Cat vs Dog 2:1 banners

- [Animated GIF](cat_dog_fight_banner.gif): the README banner.
- [Static PNG cover](cat_dog_fight_banner_2x1.png): 1280×640 (2:1), suitable for sharing
  or uploading as a repository social preview. Clicking the README banner opens it;
  adding this file does not automatically change GitHub repository settings.

The PNG is a pixel-identical RGB export of the GIF's first frame.

[`cat_dog_fight_banner.gif`](cat_dog_fight_banner.gif) is a generated documentation banner,
not a firmware screen or a camera recording. Its layout is a 2:1 canvas with a large pixel
title, a tagline, a sprite row, and five preview panels. The backyard theme follows the game:
a dusk alley glow with scanlines, a faint brick wall in the middle, and a dotted throw arc from
the cat's side to the dog's side.

- **Lettering:** the firmware's own 5×7 OLED font (`glcdfont.cpp`), enlarged with
  nearest-neighbour scaling, with two offset shadow layers on the title.
- **Sprite row:** the firmware bitmaps `bitmap_cat`, `bitmap_dog`, `bitmap_fish`, `bitmap_bone`
  and `bitmap_star` (`screens_bitmap.cpp`), enlarged 4× (the star 3×). A fish flies from the cat
  to the dog, then a bone flies back, each ending with the hit star. This animation is
  illustrative; it is not a recorded throw.
- **Panels:** MENU, GAMEPLAY and GAME OVER are framebuffers from the
  [kit capture](../captures/README.md); GAMEPLAY plays 58 consecutive captured samples of the
  cat's first turn in their recorded order and timing. 2 PLAYERS is the "Waiting for DOG"
  framebuffer from the [2 Players capture](../captures/cd_game_screens_uart.json). AK BASE KIT
  is the repository's kit photograph. OLED panels are enlarged with nearest-neighbour scaling,
  the photograph with Lanczos.

The GIF has 58 frames and loops in 3.1 seconds, timed by the gameplay samples. A fixed
128-colour palette keeps colours stable between frames, and only the changed area of each frame
is stored. The firmware sources and the capture are unchanged by the banner.
