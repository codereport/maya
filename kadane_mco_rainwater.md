### BQN Solutions

```bqn
# This is single char in Dyalog APL
Partition ← (¬-˜⊢×·+`»⊸>)⊸⊔

Kadane    ← ⌈´(⊢⌈+)`
Mco       ← ⌈´·+´¨Partition˜
RainWater ← +´⊢-˜⌈`⌾⌽⌊⌈`

# Tests
Kadane    ¯3‿¯4‿5‿¯1‿2‿¯4‿6‿¯1 # 8 
Mco       1‿0‿0‿1‿1‿1‿0‿1‿1    # 3   
RainWater 1‿0‿2‿1‿3‿0‿2        # 4
```

![image](https://user-images.githubusercontent.com/36027403/156029531-1ab07bc2-e6d4-4f0c-a6cc-c92bdb33cfd1.png)
