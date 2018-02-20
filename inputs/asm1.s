addiu $a0, $zero, 0 	//set sum initially to 0
addiu $a1, $zero, 0xA 	// upper bound for i is 10
addiu $a2, $zero, 0 	// i is initially 0
addiu $a0, $a0, 0x1 	//update sum
addiu $a2, $a2, 0x1 	// increment i
bne $a1, $a2, -8 		// if branch taken, then jump to head of loop at 0x400000c
addiu $v0, $zero, 0xA 	//set $v0 to 10 to exit
syscall 