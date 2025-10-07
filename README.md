# Minesweeper

Welcome to the minesweeper repo.<br>
This is an exercise for M03 of CFGS DAM-VIOD at Enti.

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
