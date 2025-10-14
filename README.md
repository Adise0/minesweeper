# Minesweeper

Welcome to the minesweeper repo.<br>
This is an exercise for M03 of CFGS DAM-VIOD at Enti.

## Running the app

I recommend the use of the pre-made launch.bat script, which launches the minesweeper in a conhost owned console instead of windows terminal. Usually this means worse display, but I think it's more legible in that console.

## Compiling

You should be able to open a `developer command prompt`, navigate to this folder and execute:

```
build_nonode.bat
```

<br>
If that fails, you will have to set up the cl compiler manually.

In order to use the cl.exe compiler we need to inject some enviroment variables into the terminal process. Open the `setupEnv.bat` script and replace the file path of `vcvarsall.bat` with the one correct for your computer. Check out my path to have a reference on where to find it.

If you have node installed, you can use the following command:

```
setupEnv.bat && build.bat
```

else you will need to run:

```
  setupEnv.bat && build_nonode.bat
```

The main diferences is that I have some js code in charge of optimizing the compilation by computing and storing some cache data. It makes it so only changed files get re-compiled, however since this is a 1-file program it does not really add anything.

## Nuances

- I did not use a 2D array for this exercise, instead, i used a flattened 1D array directly (it compiles to the same thing).
- I added the safeguard of the first cell ALLWAYS being empty (no neighbouring bombs).
- Because of the point above the min board size is 4x4, since in a 3x3 board, the center cell could not be empty while the board having a bomb, same for lower numbers.

## Code comments

Normally I'm `VERY` much againt's writing comments.

```
Good code should be self explanatory!
```

With the exception, of course, of variable and function definition summaries.
<br>
<br>
I however felt the need to add some comments here and there to justify my choices in this codebase, like, for example, adding technically unecessary bool variables for the sake of code readability.

## Opinion on flow break elements

My take on flow breaking elements: <br>
<br>
Using flow breaking elements is `NOT` a bad practice! They are a very powerfull tool to avoid extra if statements and bool flag checks.<br>
<br>
I don't know from where you guys got the idea that flow break elements are bad practices. I agree that goto is the exception, using goto is not a good approach.<br>
<br>
For example, continue here keeps the code lean and clean, since it avoids an extra check for the code bellow. I'd compare this to:

```c++
if(condition){return;}
else {/*Do stuff*/}
```

`else` here is unecessary. `continue;` provides the same value as return does in the code above.
<br>
<br>
Tremenda chapa<br>
