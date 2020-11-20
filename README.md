# SpaceShooter
 An attempt at writing a game from scratch at free time.
 On Windows need to compile with -lwinmm key to play sounds.
 
  ![Image alt](https://github.com/v43d3rm4k4r/SpaceShooter/raw/master/images/image1.PNG)
  
# Current problems/bugs/imperfections:  
- no sounds at Linux;
- could be some compile problems in Qt Creator on Linux, related to pthread library. In this case need to compile with terminal using -lpthread key;
- sounds on Windows disabled by default because of low FPS caused by PlaySound() WinAPI function. To enable sounds on Windows start program with -s key;

