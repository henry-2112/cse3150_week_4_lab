#       CSE 3666 Lab 4
#	TAG: EEE007ACD8D766FC885C6393AF25

	.data
	.align	2	
word_array:     .word
        0,   10,   20,  30,  40,  50,  60,  70,  80,  90, 
        100, 110, 120, 130, 140, 150, 160, 170, 180, 190,
        200, 210, 220, 230, 240, 250, 260, 270, 280, 290,
        300, 310, 320, 330, 340, 350, 360, 370, 380, 390,
        400, 410, 420, 430, 440, 450, 460, 470, 480, 490,
        500, 510, 520, 530, 540, 550, 560, 570, 580, 590,
        600, 610, 620, 630, 640, 650, 660, 670, 680, 690,
        700, 710, 720, 730, 740, 750, 760, 770, 780, 790,
        800, 810, 820, 830, 840, 850, 860, 870, 880, 890,
        900, 910, 920, 930, 940, 950, 960, 970, 980, 990

        # code
        .text
main:   
        # s1, s2, and s3 are set later
	addi	s0, sp, 0
	addi	s4, x0, -4
	addi	s5, x0, -5
	addi	s6, x0, -6
	addi	s7, x0, -7
	addi	s8, x0, -8
	addi	s9, x0, -9
	addi	s10, x0, -20
	addi	s11, x0, -25

        lui     s1, 0x10010      # starting addr of word_array in standard memory config
        addi    s2, x0, 100      # 100 elements in the array

        # read an integer from the console
        addi    a7, x0, 5
        ecall

        addi    s3, a0, 0       # keep a copy of v in s3
        
        # call binary search
        addi	a0, s1, 0
        addi	a1, s2, 0
        addi	a2, s3, 0
        jal	ra, binary_search

exit:   addi    a7, x0, 10      
        ecall

#### Do not change lines above
binary_search:
	# TODO
	addi sp, sp, -12
	sw ra, 8(sp)
	sw s1, 4(sp)
	sw s0, 0(sp)
	
	addi s0, a0, 0 # preserve original base address
	
	addi t1, x0, 0 # int rv
	
	bne a1, x0, skip # if (n == 0)
	addi a0, x0, -1
	j f_exit

skip:
	srli t2, a1, 1 # int half = n / 2
	
	slli t3, t2, 2 # half * 4 for word offset
	add t3, s0, t3 # address of a[half] using preserved base
	lw t4, 0(t3) # t4 = a[half]
	
	bne t4, a2, else_if
	addi a0, t2, 0 # rv = half
	j f_exit

else_if:
	bge a2, t4, else
	
	# Search left half: a1 = half, a0 and a2 stay same
	addi a1, t2, 0
	jal ra, binary_search
	# result is already in a0
	j f_exit

else:
	addi s1, t2, 1 # int left = half + 1
	
	sub t5, a1, s1 # n - left
	
	slli t6, s1, 2 # left * 4
	add t0, s0, t6 # &a[left] using preserved base
	
	# Set up arguments for recursive call
	addi a0, t0, 0 # new base address
	addi a1, t5, 0 # new size
	# a2 (value) stays the same
	
	jal ra, binary_search # rv = binary_search(&a[left], n - left, v)
	
	blt a0, x0, f_exit # if result < 0, return it as is
	add a0, a0, s1 # add left offset to result

f_exit:
	lw ra, 8(sp)
	lw s1, 4(sp)
	lw s0, 0(sp)
	addi sp, sp, 12
	jalr x0, ra, 0
