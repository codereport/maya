[The FL Project: The Design of a Functional Language](http://theory.stanford.edu/~aiken/publications/trs/FLProject.pdf)

|FL|APL|
|:-:|:-:|
|`def SumAndProd ≡ [+,*]` |`SumAndProd ← +,×`|
|`def second ≡ s1 ∘ tl` | `tl ← 1∘↓` <br> `s1 ← ⊃` <br> `second ← s1 ∘ tl`|
|`second:⟨2,3,4⟩ ~> 3`|`second 2 3 4 ⍝ 3`|
|`def fact ≡ id = ~0 → ~1: id * (fact ∘ sub1)`|`fact ← !`|
|`def fact ≡ *∘intsto`|`fact ← ×/⍳`|
|`def length ≡ +∘⍺:~1`|`length ← +/1⍨¨`|
|`~"Hello " cat' id`|`'Hello '⍨,⊢` (note: same as `'Hello ',⊢`)|
|`def inc ≡ C: +: 1`|`inc ← 1∘+`|
|`def ave ≡ +/length where`<br> `{def length ≡ +∘⍺:~1}`|`ave ← +/÷≢`|
