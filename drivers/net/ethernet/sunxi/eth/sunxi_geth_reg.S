int sunxi_mac_reset(void *iobase, void (*mdelay)(int), int n);
00000000 <mac_reset_v1>:
       0:	e1a0c00d 	mov	ip, sp
       4:	e92dd830 	push	{r4, r5, fp, ip, lr, pc}
       8:	e24cb004 	sub	fp, ip, #4
       c:	e3a04a01 	mov	r4, #4096	; 0x1000
      10:	e1a05000 	mov	r5, r0
      14:	e7903004 	ldr	r3, [r0, r4]
      18:	e3833001 	orr	r3, r3, #1
      1c:	e7803004 	str	r3, [r0, r4]
      20:	e1a00002 	mov	r0, r2
      24:	e12fff31 	blx	r1
      28:	e7950004 	ldr	r0, [r5, r4]
      2c:	e2000001 	and	r0, r0, #1
      30:	e89da830 	ldm	sp, {r4, r5, fp, sp, pc}

int sunxi_mac_init(void *, int txmode, int rxmode);
00000034 <mac_init_v1>:
      34:	e1a0c00d 	mov	ip, sp
      38:	e92dd878 	push	{r3, r4, r5, r6, fp, ip, lr, pc}
      3c:	e24cb004 	sub	fp, ip, #4
      40:	e59f30bc 	ldr	r3, [pc, #188]	; 104 <mac_init_v1+0xd0>
      44:	e1a04000 	mov	r4, r0
      48:	e1a05001 	mov	r5, r1
      4c:	e1a06002 	mov	r6, r2
      50:	e5933008 	ldr	r3, [r3, #8]
      54:	e5933064 	ldr	r3, [r3, #100]	; 0x64 =dma_init
      58:	e12fff33 	blx	r3
      5c:	e5942000 	ldr	r2, [r4]
      60:	e3550001 	cmp	r5, #1
      64:	e59f309c 	ldr	r3, [pc, #156]	; 108 <mac_init_v1+0xd4>
      68:	e1823003 	orr	r3, r2, r3
      6c:	e3012018 	movw	r2, #4120	; 0x1018
      70:	e5843000 	str	r3, [r4]
      74:	e3003207 	movw	r3, #519	; 0x207
      78:	e584303c 	str	r3, [r4, #60]	; 0x3c
      7c:	e3a03010 	mov	r3, #16
      80:	e5843010 	str	r3, [r4, #16]
      84:	e7943002 	ldr	r3, [r4, r2]
      88:	03833602 	orreq	r3, r3, #2097152	; 0x200000
      8c:	03833004 	orreq	r3, r3, #4
      90:	0a00000b 	beq	c4 <mac_init_v1+0x90>
      94:	e3550020 	cmp	r5, #32
      98:	e3c33987 	bic	r3, r3, #2211840	; 0x21c000
      9c:	d3833905 	orrle	r3, r3, #81920	; 0x14000
      a0:	da000007 	ble	c4 <mac_init_v1+0x90>
      a4:	e3550040 	cmp	r5, #64	; 0x40
      a8:	da000005 	ble	c4 <mac_init_v1+0x90>
      ac:	e3550080 	cmp	r5, #128	; 0x80
      b0:	d3833901 	orrle	r3, r3, #16384	; 0x4000
      b4:	da000002 	ble	c4 <mac_init_v1+0x90>
      b8:	e35500c0 	cmp	r5, #192	; 0xc0
      bc:	d3833902 	orrle	r3, r3, #32768	; 0x8000
      c0:	c3833903 	orrgt	r3, r3, #49152	; 0xc000
      c4:	e3560001 	cmp	r6, #1
      c8:	03833402 	orreq	r3, r3, #33554432	; 0x2000000
      cc:	0a000009 	beq	f8 <mac_init_v1+0xc4>
      d0:	e3c33402 	bic	r3, r3, #33554432	; 0x2000000
      d4:	e3560020 	cmp	r6, #32
      d8:	e3c33018 	bic	r3, r3, #24
      dc:	d3833008 	orrle	r3, r3, #8
      e0:	da000004 	ble	f8 <mac_init_v1+0xc4>
      e4:	e3560040 	cmp	r6, #64	; 0x40
      e8:	da000002 	ble	f8 <mac_init_v1+0xc4>
      ec:	e3560060 	cmp	r6, #96	; 0x60
      f0:	d3833010 	orrle	r3, r3, #16
      f4:	c3833018 	orrgt	r3, r3, #24
      f8:	e7843002 	str	r3, [r4, r2]
      fc:	e3a00000 	mov	r0, #0
     100:	e89da878 	ldm	sp, {r3, r4, r5, r6, fp, sp, pc}
     104:	00000000
			104: R_ARM_ABS32	.bss
     108:	00708480

void sunxi_hash_filter(void *iobase, unsigned long low, unsigned long high);
0000010c <hash_filter_v1>:
     10c:	e1a0c00d 	mov	ip, sp
     110:	e92dd800 	push	{fp, ip, lr, pc}
     114:	e24cb004 	sub	fp, ip, #4
     118:	e5802008 	str	r2, [r0, #8]
     11c:	e580100c 	str	r1, [r0, #12]
     120:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_set_filter(void *iobase, unsigned long flags);
00000124 <set_filter_v1>:
     124:	e1a0c00d 	mov	ip, sp
     128:	e92dd800 	push	{fp, ip, lr, pc}
     12c:	e24cb004 	sub	fp, ip, #4
     130:	e5801004 	str	r1, [r0, #4]
     134:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_set_umac(void *, unsigned char *, int);
00000138 <set_umac_v1>:
     138:	e1a0c00d 	mov	ip, sp
     13c:	e92dd800 	push	{fp, ip, lr, pc}
     140:	e24cb004 	sub	fp, ip, #4
     144:	e5d1c005 	ldrb	ip, [r1, #5]
     148:	e1a02182 	lsl	r2, r2, #3
     14c:	e5d13004 	ldrb	r3, [r1, #4]
     150:	e183340c 	orr	r3, r3, ip, lsl #8
     154:	e282c040 	add	ip, r2, #64	; 0x40
     158:	e2822044 	add	r2, r2, #68	; 0x44
     15c:	e780300c 	str	r3, [r0, ip]
     160:	e5d13002 	ldrb	r3, [r1, #2]
     164:	e5d1c003 	ldrb	ip, [r1, #3]
     168:	e1a03803 	lsl	r3, r3, #16
     16c:	e1833c0c 	orr	r3, r3, ip, lsl #24
     170:	e5d1c000 	ldrb	ip, [r1]
     174:	e5d11001 	ldrb	r1, [r1, #1]
     178:	e183300c 	orr	r3, r3, ip
     17c:	e1833401 	orr	r3, r3, r1, lsl #8
     180:	e7803002 	str	r3, [r0, r2]
     184:	e89da800 	ldm	sp, {fp, sp, pc}

00000188 <get_umac_v1>:
     188:	e1a0c00d 	mov	ip, sp
     18c:	e92dd800 	push	{fp, ip, lr, pc}
     190:	e24cb004 	sub	fp, ip, #4
     194:	e1a02182 	lsl	r2, r2, #3
     198:	e2823040 	add	r3, r2, #64	; 0x40
     19c:	e2822044 	add	r2, r2, #68	; 0x44
     1a0:	e790c003 	ldr	ip, [r0, r3]
     1a4:	e7903002 	ldr	r3, [r0, r2]
     1a8:	e5c1c004 	strb	ip, [r1, #4]
     1ac:	e5c13000 	strb	r3, [r1]
     1b0:	e1a0c42c 	lsr	ip, ip, #8
     1b4:	e1a02423 	lsr	r2, r3, #8
     1b8:	e5c1c005 	strb	ip, [r1, #5]
     1bc:	e5c12001 	strb	r2, [r1, #1]
     1c0:	e1a02823 	lsr	r2, r3, #16
     1c4:	e1a03c23 	lsr	r3, r3, #24
     1c8:	e5c12002 	strb	r2, [r1, #2]
     1cc:	e5c13003 	strb	r3, [r1, #3]
     1d0:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mdio_read(void *,  int, int);
000001d4 <mdio_read_v1>:
     1d4:	e1a0c00d 	mov	ip, sp
     1d8:	e92dd800 	push	{fp, ip, lr, pc}
     1dc:	e24cb004 	sub	fp, ip, #4
     1e0:	e5903010 	ldr	r3, [r0, #16]
     1e4:	e1a01581 	lsl	r1, r1, #11
     1e8:	e1a02302 	lsl	r2, r2, #6
     1ec:	e3811001 	orr	r1, r1, #1
     1f0:	e203301c 	and	r3, r3, #28
     1f4:	e1813003 	orr	r3, r1, r3
     1f8:	e2022d1f 	and	r2, r2, #1984	; 0x7c0
     1fc:	e1833002 	orr	r3, r3, r2
     200:	e6ff3073 	uxth	r3, r3
     204:	e6ff3073 	uxth	r3, r3
     208:	e5902010 	ldr	r2, [r0, #16]
     20c:	e3120001 	tst	r2, #1
     210:	1afffffc 	bne	208 <mdio_read_v1+0x34>
     214:	e5803010 	str	r3, [r0, #16]
     218:	e5903010 	ldr	r3, [r0, #16]
     21c:	e3130001 	tst	r3, #1
     220:	1afffffc 	bne	218 <mdio_read_v1+0x44>
     224:	e5900014 	ldr	r0, [r0, #20]
     228:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mdio_write(void *, int, int, unsigned short);
0000022c <mdio_write_v1>:
     22c:	e1a0c00d 	mov	ip, sp
     230:	e92dd800 	push	{fp, ip, lr, pc}
     234:	e24cb004 	sub	fp, ip, #4
     238:	e1a0c000 	mov	ip, r0
     23c:	e1a01581 	lsl	r1, r1, #11
     240:	e1a02302 	lsl	r2, r2, #6
     244:	e5900010 	ldr	r0, [r0, #16]
     248:	e3811003 	orr	r1, r1, #3
     24c:	e2022d1f 	and	r2, r2, #1984	; 0x7c0
     250:	e200001c 	and	r0, r0, #28
     254:	e1810000 	orr	r0, r1, r0
     258:	e1800002 	orr	r0, r0, r2
     25c:	e6ff0070 	uxth	r0, r0
     260:	e6ff0070 	uxth	r0, r0
     264:	e59c2010 	ldr	r2, [ip, #16]
     268:	e3120001 	tst	r2, #1
     26c:	1afffffc 	bne	264 <mdio_write_v1+0x38>
     270:	e58c3014 	str	r3, [ip, #20]
     274:	e58c0010 	str	r0, [ip, #16]
     278:	e59c0010 	ldr	r0, [ip, #16]
     27c:	e2100001 	ands	r0, r0, #1
     280:	1afffffc 	bne	278 <mdio_write_v1+0x4c>
     284:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mdio_reset(void *);
00000288 <mdio_reset_v1>:
     288:	e1a0c00d 	mov	ip, sp
     28c:	e92dd800 	push	{fp, ip, lr, pc}
     290:	e24cb004 	sub	fp, ip, #4
     294:	e3a03010 	mov	r3, #16
     298:	e5803010 	str	r3, [r0, #16]
     29c:	e3a00000 	mov	r0, #0
     2a0:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_mac_loopback(void *iobase, int enable);
000002a4 <mac_loopback_v1>:
     2a4:	e1a0c00d 	mov	ip, sp
     2a8:	e92dd800 	push	{fp, ip, lr, pc}
     2ac:	e24cb004 	sub	fp, ip, #4
     2b0:	e5903000 	ldr	r3, [r0]
     2b4:	e3510000 	cmp	r1, #0
     2b8:	13833a01 	orrne	r3, r3, #4096	; 0x1000
     2bc:	03c33a01 	biceq	r3, r3, #4096	; 0x1000
     2c0:	e5803000 	str	r3, [r0]
     2c4:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_set_link_mode(void *iobase, int duplex, int speed);
000002c8 <set_link_mode_v1>:
     2c8:	e1a0c00d 	mov	ip, sp
     2cc:	e92dd800 	push	{fp, ip, lr, pc}
     2d0:	e24cb004 	sub	fp, ip, #4
     2d4:	e5903000 	ldr	r3, [r0]
     2d8:	e3510000 	cmp	r1, #0
     2dc:	03c33b02 	biceq	r3, r3, #2048	; 0x800
     2e0:	13833b02 	orrne	r3, r3, #2048	; 0x800
     2e4:	e3520ffa 	cmp	r2, #1000	; 0x3e8
     2e8:	03c33902 	biceq	r3, r3, #32768	; 0x8000
     2ec:	0a000003 	beq	300 <set_link_mode_v1+0x38>
     2f0:	e3831902 	orr	r1, r3, #32768	; 0x8000
     2f4:	e3520064 	cmp	r2, #100	; 0x64
     2f8:	03833903 	orreq	r3, r3, #49152	; 0xc000
     2fc:	13c13901 	bicne	r3, r1, #16384	; 0x4000
     300:	e5803000 	str	r3, [r0]
     304:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_flow_ctrl(void *iobase, int duplex, int fc, int pause);
00000308 <flow_ctrl_v1>:
     308:	e1a0c00d 	mov	ip, sp
     30c:	e92dd800 	push	{fp, ip, lr, pc}
     310:	e24cb004 	sub	fp, ip, #4
     314:	e212c001 	ands	ip, r2, #1
     318:	13a0c004 	movne	ip, #4
     31c:	e3120002 	tst	r2, #2
     320:	138cc002 	orrne	ip, ip, #2
     324:	e3510000 	cmp	r1, #0
     328:	118cc803 	orrne	ip, ip, r3, lsl #16
     32c:	e580c018 	str	ip, [r0, #24]
     330:	e89da800 	ldm	sp, {fp, sp, pc}

00000334 <rx_tx_en_v1>:
     334:	e1a0c00d 	mov	ip, sp
     338:	e92dd800 	push	{fp, ip, lr, pc}
     33c:	e24cb004 	sub	fp, ip, #4
     340:	e5903000 	ldr	r3, [r0]
     344:	e383300c 	orr	r3, r3, #12
     348:	e5803000 	str	r3, [r0]
     34c:	e89da800 	ldm	sp, {fp, sp, pc}

00000350 <rx_tx_dis_v1>:
     350:	e1a0c00d 	mov	ip, sp
     354:	e92dd800 	push	{fp, ip, lr, pc}
     358:	e24cb004 	sub	fp, ip, #4
     35c:	e5903000 	ldr	r3, [r0]
     360:	e3c3300c 	bic	r3, r3, #12
     364:	e5803000 	str	r3, [r0]
     368:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_buf_set(struct dma_desc *p, unsigned long paddr, int size);
0000036c <desc_buf_set_v1>:
     36c:	e1a0c00d 	mov	ip, sp
     370:	e92dd800 	push	{fp, ip, lr, pc}
     374:	e24cb004 	sub	fp, ip, #4
     378:	e5903004 	ldr	r3, [r0, #4]
     37c:	e1a02a82 	lsl	r2, r2, #21
     380:	e5801008 	str	r1, [r0, #8]
     384:	e3c33e7f 	bic	r3, r3, #2032	; 0x7f0
     388:	e3c3300f 	bic	r3, r3, #15
     38c:	e1833aa2 	orr	r3, r3, r2, lsr #21
     390:	e5803004 	str	r3, [r0, #4]
     394:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_buf_get_addr(struct dma_desc *desc);
00000398 <desc_buf_get_addr_v1>:
     398:	e1a0c00d 	mov	ip, sp
     39c:	e92dd800 	push	{fp, ip, lr, pc}
     3a0:	e24cb004 	sub	fp, ip, #4
     3a4:	e5900008 	ldr	r0, [r0, #8]
     3a8:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_buf_get_len(struct dma_desc *desc);
000003ac <desc_buf_get_len_v1>:
     3ac:	e1a0c00d 	mov	ip, sp
     3b0:	e92dd800 	push	{fp, ip, lr, pc}
     3b4:	e24cb004 	sub	fp, ip, #4
     3b8:	e5900004 	ldr	r0, [r0, #4]
     3bc:	e1a00a80 	lsl	r0, r0, #21
     3c0:	e1a00aa0 	lsr	r0, r0, #21
     3c4:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_set_own(struct dma_desc *p);
000003c8 <desc_set_own_v1>:
     3c8:	e1a0c00d 	mov	ip, sp
     3cc:	e92dd800 	push	{fp, ip, lr, pc}
     3d0:	e24cb004 	sub	fp, ip, #4
     3d4:	e5903000 	ldr	r3, [r0]
     3d8:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
     3dc:	e5803000 	str	r3, [r0]
     3e0:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_own(struct dma_desc *desc);
000003e4 <desc_get_own_v1>:
     3e4:	e1a0c00d 	mov	ip, sp
     3e8:	e92dd800 	push	{fp, ip, lr, pc}
     3ec:	e24cb004 	sub	fp, ip, #4
     3f0:	e5900000 	ldr	r0, [r0]
     3f4:	e2000102 	and	r0, r0, #-2147483648	; 0x80000000
     3f8:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_init(struct dma_desc *p);
000003fc <desc_init_v1>:
     3fc:	e1a0c00d 	mov	ip, sp
     400:	e92dd800 	push	{fp, ip, lr, pc}
     404:	e24cb004 	sub	fp, ip, #4
     408:	e3a03000 	mov	r3, #0
     40c:	e5c03008 	strb	r3, [r0, #8]
     410:	e5c03009 	strb	r3, [r0, #9]
     414:	e5c0300a 	strb	r3, [r0, #10]
     418:	e5c0300b 	strb	r3, [r0, #11]
     41c:	e5c03004 	strb	r3, [r0, #4]
     420:	e5c03005 	strb	r3, [r0, #5]
     424:	e5c03006 	strb	r3, [r0, #6]
     428:	e3a03001 	mov	r3, #1
     42c:	e5c03007 	strb	r3, [r0, #7]
     430:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_rx_frame_len(struct dma_desc *desc);
00000434 <desc_rx_frame_len_v1>:
     434:	e1a0c00d 	mov	ip, sp
     438:	e92dd800 	push	{fp, ip, lr, pc}
     43c:	e24cb004 	sub	fp, ip, #4
     440:	e5d03002 	ldrb	r3, [r0, #2]
     444:	e5d00003 	ldrb	r0, [r0, #3]
     448:	e200003f 	and	r0, r0, #63	; 0x3f
     44c:	e1830400 	orr	r0, r3, r0, lsl #8
     450:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_tx_status(struct dma_desc *desc, void *x);
00000454 <desc_get_tx_status_v1>:
     454:	e1a0c00d 	mov	ip, sp
     458:	e92dd800 	push	{fp, ip, lr, pc}
     45c:	e24cb004 	sub	fp, ip, #4
     460:	e1a03000 	mov	r3, r0
     464:	e5d00001 	ldrb	r0, [r0, #1]
     468:	e2000080 	and	r0, r0, #128	; 0x80
     46c:	e6ef0070 	uxtb	r0, r0
     470:	e3500000 	cmp	r0, #0
     474:	0a000015 	beq	4d0 <desc_get_tx_status_v1+0x7c>
     478:	e5d32000 	ldrb	r2, [r3]
     47c:	e3e00000 	mvn	r0, #0
     480:	e2022002 	and	r2, r2, #2
     484:	e6ef2072 	uxtb	r2, r2
     488:	e3520000 	cmp	r2, #0
     48c:	15912000 	ldrne	r2, [r1]
     490:	12822001 	addne	r2, r2, #1
     494:	15812000 	strne	r2, [r1]
     498:	e5d32001 	ldrb	r2, [r3, #1]
     49c:	e2022004 	and	r2, r2, #4
     4a0:	e6ef2072 	uxtb	r2, r2
     4a4:	e3520000 	cmp	r2, #0
     4a8:	15912004 	ldrne	r2, [r1, #4]
     4ac:	12822001 	addne	r2, r2, #1
     4b0:	15812004 	strne	r2, [r1, #4]
     4b4:	e5d32001 	ldrb	r2, [r3, #1]
     4b8:	e2022008 	and	r2, r2, #8
     4bc:	e6ef2072 	uxtb	r2, r2
     4c0:	e3520000 	cmp	r2, #0
     4c4:	15912008 	ldrne	r2, [r1, #8]
     4c8:	12822001 	addne	r2, r2, #1
     4cc:	15812008 	strne	r2, [r1, #8]
     4d0:	e5d32000 	ldrb	r2, [r3]
     4d4:	e2022080 	and	r2, r2, #128	; 0x80
     4d8:	e6ef2072 	uxtb	r2, r2
     4dc:	e3520000 	cmp	r2, #0
     4e0:	15912014 	ldrne	r2, [r1, #20]
     4e4:	12822001 	addne	r2, r2, #1
     4e8:	15812014 	strne	r2, [r1, #20]
     4ec:	e5d33000 	ldrb	r3, [r3]
     4f0:	e3130001 	tst	r3, #1
     4f4:	15913010 	ldrne	r3, [r1, #16]
     4f8:	12833001 	addne	r3, r3, #1
     4fc:	15813010 	strne	r3, [r1, #16]
     500:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_tx_close(struct dma_desc *first, struct dma_desc *end, int csum_insert);
00000504 <desc_tx_close_v1>:
     504:	e1a0c00d 	mov	ip, sp
     508:	e92dd800 	push	{fp, ip, lr, pc}
     50c:	e24cb004 	sub	fp, ip, #4
     510:	e5d03007 	ldrb	r3, [r0, #7]
     514:	e3520000 	cmp	r2, #0
     518:	e3833020 	orr	r3, r3, #32
     51c:	e5c03007 	strb	r3, [r0, #7]
     520:	e5d13007 	ldrb	r3, [r1, #7]
     524:	e38330c0 	orr	r3, r3, #192	; 0xc0
     528:	e5c13007 	strb	r3, [r1, #7]
     52c:	089da800 	ldmeq	sp, {fp, sp, pc}
     530:	e5d03007 	ldrb	r3, [r0, #7]
     534:	e3833018 	orr	r3, r3, #24
     538:	e5c03007 	strb	r3, [r0, #7]
     53c:	e2800010 	add	r0, r0, #16
     540:	e1500001 	cmp	r0, r1
     544:	9afffff9 	bls	530 <desc_tx_close_v1+0x2c>
     548:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_tx_ls(struct dma_desc *desc);
0000054c <desc_get_tx_ls_v1>:
     54c:	e1a0c00d 	mov	ip, sp
     550:	e92dd800 	push	{fp, ip, lr, pc}
     554:	e24cb004 	sub	fp, ip, #4
     558:	e5d00007 	ldrb	r0, [r0, #7]
     55c:	e7e00350 	ubfx	r0, r0, #6, #1
     560:	e89da800 	ldm	sp, {fp, sp, pc}

00000564 <dma_init_v1>:
     564:	e1a0c00d 	mov	ip, sp
     568:	e92dd800 	push	{fp, ip, lr, pc}
     56c:	e24cb004 	sub	fp, ip, #4
     570:	e59f2018 	ldr	r2, [pc, #24]	; 590 <dma_init_v1+0x2c>
     574:	e3a03a01 	mov	r3, #4096	; 0x1000
     578:	e7802003 	str	r2, [r0, r3]
     57c:	e283301c 	add	r3, r3, #28
     580:	e59f200c 	ldr	r2, [pc, #12]	; 594 <dma_init_v1+0x30>
     584:	e7802003 	str	r2, [r0, r3]
     588:	e3a00000 	mov	r0, #0
     58c:	e89da800 	ldm	sp, {fp, sp, pc}
     590:	01050200 	mrseq	r0, SP_usr
     594:	0001a061 	andeq	sl, r1, r1, rrx

00000598 <dma_int_dis_v1>:
     598:	e1a0c00d 	mov	ip, sp
     59c:	e92dd800 	push	{fp, ip, lr, pc}
     5a0:	e24cb004 	sub	fp, ip, #4
     5a4:	e3a02000 	mov	r2, #0
     5a8:	e301301c 	movw	r3, #4124	; 0x101c
     5ac:	e7802003 	str	r2, [r0, r3]
     5b0:	e89da800 	ldm	sp, {fp, sp, pc}

000005b4 <dma_int_en_v1>:
     5b4:	e1a0c00d 	mov	ip, sp
     5b8:	e92dd800 	push	{fp, ip, lr, pc}
     5bc:	e24cb004 	sub	fp, ip, #4
     5c0:	e59f2008 	ldr	r2, [pc, #8]	; 5d0 <dma_int_en_v1+0x1c>
     5c4:	e301301c 	movw	r3, #4124	; 0x101c
     5c8:	e7802003 	str	r2, [r0, r3]
     5cc:	e89da800 	ldm	sp, {fp, sp, pc}
     5d0:	0001a061 	andeq	sl, r1, r1, rrx

int sunxi_int_status(void *, void *x);
000005d4 <int_status_v1>:
     5d4:	e1a0c00d 	mov	ip, sp
     5d8:	e92dd800 	push	{fp, ip, lr, pc}
     5dc:	e24cb004 	sub	fp, ip, #4
     5e0:	e59020d8 	ldr	r2, [r0, #216]	; 0xd8
     5e4:	e1a03000 	mov	r3, r0
     5e8:	e3120001 	tst	r2, #1
     5ec:	159020d8 	ldrne	r2, [r0, #216]	; 0xd8
     5f0:	e3012014 	movw	r2, #4116	; 0x1014
     5f4:	e7902002 	ldr	r2, [r0, r2]
     5f8:	e2120902 	ands	r0, r2, #32768	; 0x8000
     5fc:	0a000026 	beq	69c <int_status_v1+0xc8>
     600:	e2120020 	ands	r0, r2, #32
     604:	1591006c 	ldrne	r0, [r1, #108]	; 0x6c
     608:	12800001 	addne	r0, r0, #1
     60c:	1581006c 	strne	r0, [r1, #108]	; 0x6c
     610:	13a00002 	movne	r0, #2
     614:	e3120008 	tst	r2, #8
     618:	1591c074 	ldrne	ip, [r1, #116]	; 0x74
     61c:	128cc001 	addne	ip, ip, #1
     620:	1581c074 	strne	ip, [r1, #116]	; 0x74
     624:	e3120010 	tst	r2, #16
     628:	1591c078 	ldrne	ip, [r1, #120]	; 0x78
     62c:	128cc001 	addne	ip, ip, #1
     630:	1581c078 	strne	ip, [r1, #120]	; 0x78
     634:	e3120080 	tst	r2, #128	; 0x80
     638:	1591c07c 	ldrne	ip, [r1, #124]	; 0x7c
     63c:	128cc001 	addne	ip, ip, #1
     640:	1581c07c 	strne	ip, [r1, #124]	; 0x7c
     644:	e3120c01 	tst	r2, #256	; 0x100
     648:	1591c080 	ldrne	ip, [r1, #128]	; 0x80
     64c:	128cc001 	addne	ip, ip, #1
     650:	1581c080 	strne	ip, [r1, #128]	; 0x80
     654:	e3120c02 	tst	r2, #512	; 0x200
     658:	1591c084 	ldrne	ip, [r1, #132]	; 0x84
     65c:	128cc001 	addne	ip, ip, #1
     660:	1581c084 	strne	ip, [r1, #132]	; 0x84
     664:	e3120b01 	tst	r2, #1024	; 0x400
     668:	1591c088 	ldrne	ip, [r1, #136]	; 0x88
     66c:	128cc001 	addne	ip, ip, #1
     670:	1581c088 	strne	ip, [r1, #136]	; 0x88
     674:	e3120002 	tst	r2, #2
     678:	15910070 	ldrne	r0, [r1, #112]	; 0x70
     67c:	12800001 	addne	r0, r0, #1
     680:	15810070 	strne	r0, [r1, #112]	; 0x70
     684:	13a00001 	movne	r0, #1
     688:	e3120a02 	tst	r2, #8192	; 0x2000
     68c:	1591008c 	ldrne	r0, [r1, #140]	; 0x8c
     690:	12800001 	addne	r0, r0, #1
     694:	1581008c 	strne	r0, [r1, #140]	; 0x8c
     698:	13a00001 	movne	r0, #1
     69c:	e3120801 	tst	r2, #65536	; 0x10000
     6a0:	0a000004 	beq	6b8 <int_status_v1+0xe4>
     6a4:	e591c0a4 	ldr	ip, [r1, #164]	; 0xa4
     6a8:	e3120041 	tst	r2, #65	; 0x41
     6ac:	13a00003 	movne	r0, #3
     6b0:	e28cc001 	add	ip, ip, #1
     6b4:	e581c0a4 	str	ip, [r1, #164]	; 0xa4
     6b8:	e3c224ff 	bic	r2, r2, #-16777216	; 0xff000000
     6bc:	e3011014 	movw	r1, #4116	; 0x1014
     6c0:	e3c228fe 	bic	r2, r2, #16646144	; 0xfe0000
     6c4:	e7832001 	str	r2, [r3, r1]
     6c8:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_start_tx(void *, unsigned long);
000006cc <start_tx_v1>:
     6cc:	e1a0c00d 	mov	ip, sp
     6d0:	e92dd800 	push	{fp, ip, lr, pc}
     6d4:	e24cb004 	sub	fp, ip, #4
     6d8:	e3013010 	movw	r3, #4112	; 0x1010
     6dc:	e7801003 	str	r1, [r0, r3]
     6e0:	e2833008 	add	r3, r3, #8
     6e4:	e7902003 	ldr	r2, [r0, r3]
     6e8:	e3822a02 	orr	r2, r2, #8192	; 0x2000
     6ec:	e7802003 	str	r2, [r0, r3]
     6f0:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_stop_tx(void *);
000006f4 <stop_tx_v1>:
     6f4:	e1a0c00d 	mov	ip, sp
     6f8:	e92dd800 	push	{fp, ip, lr, pc}
     6fc:	e24cb004 	sub	fp, ip, #4
     700:	e3013018 	movw	r3, #4120	; 0x1018
     704:	e7902003 	ldr	r2, [r0, r3]
     708:	e3c22a02 	bic	r2, r2, #8192	; 0x2000
     70c:	e7802003 	str	r2, [r0, r3]
     710:	e89da800 	ldm	sp, {fp, sp, pc}

00000714 <poll_tx_v1>:
     714:	e1a0c00d 	mov	ip, sp
     718:	e92dd800 	push	{fp, ip, lr, pc}
     71c:	e24cb004 	sub	fp, ip, #4
     720:	e3a02001 	mov	r2, #1
     724:	e3013004 	movw	r3, #4100	; 0x1004
     728:	e7802003 	str	r2, [r0, r3]
     72c:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_start_rx(void *, unsigned long);
00000730 <start_rx_v1>:
     730:	e1a0c00d 	mov	ip, sp
     734:	e92dd800 	push	{fp, ip, lr, pc}
     738:	e24cb004 	sub	fp, ip, #4
     73c:	e301300c 	movw	r3, #4108	; 0x100c
     740:	e7801003 	str	r1, [r0, r3]
     744:	e283300c 	add	r3, r3, #12
     748:	e7902003 	ldr	r2, [r0, r3]
     74c:	e3822002 	orr	r2, r2, #2
     750:	e7802003 	str	r2, [r0, r3]
     754:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_stop_rx(void *);
00000758 <stop_rx_v1>:
     758:	e1a0c00d 	mov	ip, sp
     75c:	e92dd800 	push	{fp, ip, lr, pc}
     760:	e24cb004 	sub	fp, ip, #4
     764:	e3013018 	movw	r3, #4120	; 0x1018
     768:	e7902003 	ldr	r2, [r0, r3]
     76c:	e3c22002 	bic	r2, r2, #2
     770:	e7802003 	str	r2, [r0, r3]
     774:	e89da800 	ldm	sp, {fp, sp, pc}

00000778 <poll_rx_v1>:
     778:	e1a0c00d 	mov	ip, sp
     77c:	e92dd800 	push	{fp, ip, lr, pc}
     780:	e24cb004 	sub	fp, ip, #4
     784:	e3a02001 	mov	r2, #1
     788:	e3013008 	movw	r3, #4104	; 0x1008
     78c:	e7802003 	str	r2, [r0, r3]
     790:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mac_reset(void *iobase, void (*mdelay)(int), int n);
00000794 <mac_reset_v2>:
     794:	e1a0c00d 	mov	ip, sp
     798:	e92dd818 	push	{r3, r4, fp, ip, lr, pc}
     79c:	e24cb004 	sub	fp, ip, #4
     7a0:	e5903004 	ldr	r3, [r0, #4]
     7a4:	e1a04000 	mov	r4, r0
     7a8:	e3833001 	orr	r3, r3, #1
     7ac:	e5803004 	str	r3, [r0, #4]
     7b0:	e1a00002 	mov	r0, r2
     7b4:	e12fff31 	blx	r1
     7b8:	e5940004 	ldr	r0, [r4, #4]
     7bc:	e2000001 	and	r0, r0, #1
     7c0:	e89da818 	ldm	sp, {r3, r4, fp, sp, pc}

int sunxi_mac_init(void *, int txmode, int rxmode);
000007c4 <mac_init_v2>:
     7c4:	e1a0c00d 	mov	ip, sp
     7c8:	e92dd878 	push	{r3, r4, r5, r6, fp, ip, lr, pc}
     7cc:	e24cb004 	sub	fp, ip, #4
     7d0:	e59f30b0 	ldr	r3, [pc, #176]	; 888 <mac_init_v2+0xc4>
     7d4:	e1a04000 	mov	r4, r0
     7d8:	e1a05001 	mov	r5, r1
     7dc:	e1a06002 	mov	r6, r2
     7e0:	e5933008 	ldr	r3, [r3, #8]
     7e4:	e5933064 	ldr	r3, [r3, #100]	; 0x64 =dma_init
     7e8:	e12fff33 	blx	r3
     7ec:	e5943010 	ldr	r3, [r4, #16]
     7f0:	e3550001 	cmp	r5, #1
     7f4:	e3833101 	orr	r3, r3, #1073741824	; 0x40000000
     7f8:	e5843010 	str	r3, [r4, #16]
     7fc:	e5943024 	ldr	r3, [r4, #36]	; 0x24
     800:	e383330e 	orr	r3, r3, #939524096	; 0x38000000
     804:	e5843024 	str	r3, [r4, #36]	; 0x24
     808:	e3a03501 	mov	r3, #4194304	; 0x400000
     80c:	e5843010 	str	r3, [r4, #16]
     810:	e5943014 	ldr	r3, [r4, #20]
     814:	03833006 	orreq	r3, r3, #6
     818:	0a000009 	beq	844 <mac_init_v2+0x80>
     81c:	e3550040 	cmp	r5, #64	; 0x40
     820:	e3c33c07 	bic	r3, r3, #1792	; 0x700
     824:	e3c33002 	bic	r3, r3, #2
     828:	da000005 	ble	844 <mac_init_v2+0x80>
     82c:	e3550080 	cmp	r5, #128	; 0x80
     830:	d3833c01 	orrle	r3, r3, #256	; 0x100
     834:	da000002 	ble	844 <mac_init_v2+0x80>
     838:	e35500c0 	cmp	r5, #192	; 0xc0
     83c:	d3833c02 	orrle	r3, r3, #512	; 0x200
     840:	c3833c03 	orrgt	r3, r3, #768	; 0x300
     844:	e5843014 	str	r3, [r4, #20]
     848:	e3560001 	cmp	r6, #1
     84c:	e5943028 	ldr	r3, [r4, #40]	; 0x28
     850:	03833002 	orreq	r3, r3, #2
     854:	0a000008 	beq	87c <mac_init_v2+0xb8>
     858:	e3560020 	cmp	r6, #32
     85c:	e3c33032 	bic	r3, r3, #50	; 0x32
     860:	d3833010 	orrle	r3, r3, #16
     864:	da000004 	ble	87c <mac_init_v2+0xb8>
     868:	e3560040 	cmp	r6, #64	; 0x40
     86c:	da000002 	ble	87c <mac_init_v2+0xb8>
     870:	e3560060 	cmp	r6, #96	; 0x60
     874:	d3833020 	orrle	r3, r3, #32
     878:	c3833030 	orrgt	r3, r3, #48	; 0x30
     87c:	e5843028 	str	r3, [r4, #40]	; 0x28
     880:	e3a00000 	mov	r0, #0
     884:	e89da878 	ldm	sp, {r3, r4, r5, r6, fp, sp, pc}
     888:	00000000
			888: R_ARM_ABS32	.bss

void sunxi_hash_filter(void *iobase, unsigned long low, unsigned long high);
0000088c <hash_filter_v2>:
     88c:	e1a0c00d 	mov	ip, sp
     890:	e92dd800 	push	{fp, ip, lr, pc}
     894:	e24cb004 	sub	fp, ip, #4
     898:	e5802040 	str	r2, [r0, #64]	; 0x40
     89c:	e5801044 	str	r1, [r0, #68]	; 0x44
     8a0:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_set_filter(void *iobase, unsigned long flags);
000008a4 <set_filter_v2>:
     8a4:	e1a0c00d 	mov	ip, sp
     8a8:	e92dd800 	push	{fp, ip, lr, pc}
     8ac:	e24cb004 	sub	fp, ip, #4
     8b0:	e1a03f81 	lsl	r3, r1, #31
     8b4:	e5902038 	ldr	r2, [r0, #56]	; 0x38
     8b8:	e1833fa1 	orr	r3, r3, r1, lsr #31
     8bc:	e1833002 	orr	r3, r3, r2
     8c0:	e1a024a1 	lsr	r2, r1, #9
     8c4:	e2022002 	and	r2, r2, #2
     8c8:	e1833002 	orr	r3, r3, r2
     8cc:	e1a02081 	lsl	r2, r1, #1
     8d0:	e2022010 	and	r2, r2, #16
     8d4:	e1833002 	orr	r3, r3, r2
     8d8:	e1a021a1 	lsr	r2, r1, #3
     8dc:	e2022060 	and	r2, r2, #96	; 0x60
     8e0:	e1833002 	orr	r3, r3, r2
     8e4:	e1a02381 	lsl	r2, r1, #7
     8e8:	e2022c03 	and	r2, r2, #768	; 0x300
     8ec:	e1833002 	orr	r3, r3, r2
     8f0:	e1a02301 	lsl	r2, r1, #6
     8f4:	e1a01601 	lsl	r1, r1, #12
     8f8:	e2022a03 	and	r2, r2, #12288	; 0x3000
     8fc:	e1833002 	orr	r3, r3, r2
     900:	e2011803 	and	r1, r1, #196608	; 0x30000
     904:	e1831001 	orr	r1, r3, r1
     908:	e5801038 	str	r1, [r0, #56]	; 0x38
     90c:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_set_umac(void *, unsigned char *, int);
00000910 <set_umac_v2>:
     910:	e1a0c00d 	mov	ip, sp
     914:	e92dd800 	push	{fp, ip, lr, pc}
     918:	e24cb004 	sub	fp, ip, #4
     91c:	e5d1c005 	ldrb	ip, [r1, #5]
     920:	e1a02182 	lsl	r2, r2, #3
     924:	e5d13004 	ldrb	r3, [r1, #4]
     928:	e183340c 	orr	r3, r3, ip, lsl #8
     92c:	e282c050 	add	ip, r2, #80	; 0x50
     930:	e2822054 	add	r2, r2, #84	; 0x54
     934:	e780300c 	str	r3, [r0, ip]
     938:	e5d13002 	ldrb	r3, [r1, #2]
     93c:	e5d1c003 	ldrb	ip, [r1, #3]
     940:	e1a03803 	lsl	r3, r3, #16
     944:	e1833c0c 	orr	r3, r3, ip, lsl #24
     948:	e5d1c000 	ldrb	ip, [r1]
     94c:	e5d11001 	ldrb	r1, [r1, #1]
     950:	e183300c 	orr	r3, r3, ip
     954:	e1833401 	orr	r3, r3, r1, lsl #8
     958:	e7803002 	str	r3, [r0, r2]
     95c:	e89da800 	ldm	sp, {fp, sp, pc}

00000960 <get_umac_v2>:
     960:	e1a0c00d 	mov	ip, sp
     964:	e92dd800 	push	{fp, ip, lr, pc}
     968:	e24cb004 	sub	fp, ip, #4
     96c:	e1a02182 	lsl	r2, r2, #3
     970:	e2823050 	add	r3, r2, #80	; 0x50
     974:	e2822054 	add	r2, r2, #84	; 0x54
     978:	e790c003 	ldr	ip, [r0, r3]
     97c:	e7903002 	ldr	r3, [r0, r2]
     980:	e5c1c004 	strb	ip, [r1, #4]
     984:	e5c13000 	strb	r3, [r1]
     988:	e1a0c42c 	lsr	ip, ip, #8
     98c:	e1a02423 	lsr	r2, r3, #8
     990:	e5c1c005 	strb	ip, [r1, #5]
     994:	e5c12001 	strb	r2, [r1, #1]
     998:	e1a02823 	lsr	r2, r3, #16
     99c:	e1a03c23 	lsr	r3, r3, #24
     9a0:	e5c12002 	strb	r2, [r1, #2]
     9a4:	e5c13003 	strb	r3, [r1, #3]
     9a8:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mdio_read(void *,  int, int);
000009ac <mdio_read_v2>:
     9ac:	e1a0c00d 	mov	ip, sp
     9b0:	e92dd800 	push	{fp, ip, lr, pc}
     9b4:	e24cb004 	sub	fp, ip, #4
     9b8:	e1a01601 	lsl	r1, r1, #12
     9bc:	e1a02202 	lsl	r2, r2, #4
     9c0:	e3811001 	orr	r1, r1, #1
     9c4:	e5903048 	ldr	r3, [r0, #72]	; 0x48
     9c8:	e2022e7f 	and	r2, r2, #2032	; 0x7f0
     9cc:	e1811002 	orr	r1, r1, r2
     9d0:	e6ff1071 	uxth	r1, r1
     9d4:	e5903048 	ldr	r3, [r0, #72]	; 0x48
     9d8:	e3130001 	tst	r3, #1
     9dc:	1afffffc 	bne	9d4 <mdio_read_v2+0x28>
     9e0:	e5801048 	str	r1, [r0, #72]	; 0x48
     9e4:	e5903048 	ldr	r3, [r0, #72]	; 0x48
     9e8:	e3130001 	tst	r3, #1
     9ec:	1afffffc 	bne	9e4 <mdio_read_v2+0x38>
     9f0:	e590004c 	ldr	r0, [r0, #76]	; 0x4c
     9f4:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mdio_write(void *, int, int, unsigned short);
000009f8 <mdio_write_v2>:
     9f8:	e1a0c00d 	mov	ip, sp
     9fc:	e92dd800 	push	{fp, ip, lr, pc}
     a00:	e24cb004 	sub	fp, ip, #4
     a04:	e1a01601 	lsl	r1, r1, #12
     a08:	e1a02202 	lsl	r2, r2, #4
     a0c:	e1a0c000 	mov	ip, r0
     a10:	e3811003 	orr	r1, r1, #3
     a14:	e5900048 	ldr	r0, [r0, #72]	; 0x48
     a18:	e2022e7f 	and	r2, r2, #2032	; 0x7f0
     a1c:	e1811002 	orr	r1, r1, r2
     a20:	e6ff1071 	uxth	r1, r1
     a24:	e59c2048 	ldr	r2, [ip, #72]	; 0x48
     a28:	e3120001 	tst	r2, #1
     a2c:	1afffffc 	bne	a24 <mdio_write_v2+0x2c>
     a30:	e58c304c 	str	r3, [ip, #76]	; 0x4c
     a34:	e58c1048 	str	r1, [ip, #72]	; 0x48
     a38:	e59c0048 	ldr	r0, [ip, #72]	; 0x48
     a3c:	e2100001 	ands	r0, r0, #1
     a40:	1afffffc 	bne	a38 <mdio_write_v2+0x40>
     a44:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_mdio_reset(void *);
00000a48 <mdio_reset_v2>:
     a48:	e1a0c00d 	mov	ip, sp
     a4c:	e92dd800 	push	{fp, ip, lr, pc}
     a50:	e24cb004 	sub	fp, ip, #4
     a54:	e3a03010 	mov	r3, #16
     a58:	e5803048 	str	r3, [r0, #72]	; 0x48
     a5c:	e3a00000 	mov	r0, #0
     a60:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_mac_loopback(void *iobase, int enable);
00000a64 <mac_loopback_v2>:
     a64:	e1a0c00d 	mov	ip, sp
     a68:	e92dd800 	push	{fp, ip, lr, pc}
     a6c:	e24cb004 	sub	fp, ip, #4
     a70:	e5903000 	ldr	r3, [r0]
     a74:	e3510000 	cmp	r1, #0
     a78:	13833002 	orrne	r3, r3, #2
     a7c:	03c33002 	biceq	r3, r3, #2
     a80:	e5803000 	str	r3, [r0]
     a84:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_set_link_mode(void *iobase, int duplex, int speed);
00000a88 <set_link_mode_v2>:
     a88:	e1a0c00d 	mov	ip, sp
     a8c:	e92dd800 	push	{fp, ip, lr, pc}
     a90:	e24cb004 	sub	fp, ip, #4
     a94:	e5903000 	ldr	r3, [r0]
     a98:	e3510000 	cmp	r1, #0
     a9c:	03c33001 	biceq	r3, r3, #1
     aa0:	13833001 	orrne	r3, r3, #1
     aa4:	e3520ffa 	cmp	r2, #1000	; 0x3e8
     aa8:	03c3300c 	biceq	r3, r3, #12
     aac:	0a000003 	beq	ac0 <set_link_mode_v2+0x38>
     ab0:	e3831008 	orr	r1, r3, #8
     ab4:	e3520064 	cmp	r2, #100	; 0x64
     ab8:	0383300c 	orreq	r3, r3, #12
     abc:	13c13004 	bicne	r3, r1, #4
     ac0:	e5803000 	str	r3, [r0]
     ac4:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_flow_ctrl(void *iobase, int duplex, int fc, int pause);
00000ac8 <flow_ctrl_v2>:
     ac8:	e1a0c00d 	mov	ip, sp
     acc:	e92dd800 	push	{fp, ip, lr, pc}
     ad0:	e24cb004 	sub	fp, ip, #4
     ad4:	e3120001 	tst	r2, #1
     ad8:	1590c024 	ldrne	ip, [r0, #36]	; 0x24
     adc:	138cc801 	orrne	ip, ip, #65536	; 0x10000
     ae0:	1580c024 	strne	ip, [r0, #36]	; 0x24
     ae4:	e3120002 	tst	r2, #2
     ae8:	1590201c 	ldrne	r2, [r0, #28]
     aec:	13822001 	orrne	r2, r2, #1
     af0:	1580201c 	strne	r2, [r0, #28]
     af4:	e3510000 	cmp	r1, #0
     af8:	1590201c 	ldrne	r2, [r0, #28]
     afc:	11823203 	orrne	r3, r2, r3, lsl #4
     b00:	1580301c 	strne	r3, [r0, #28]
     b04:	e89da800 	ldm	sp, {fp, sp, pc}

00000b08 <rx_tx_en_v2>:
     b08:	e1a0c00d 	mov	ip, sp
     b0c:	e92dd800 	push	{fp, ip, lr, pc}
     b10:	e24cb004 	sub	fp, ip, #4
     b14:	e5903010 	ldr	r3, [r0, #16]
     b18:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
     b1c:	e5803010 	str	r3, [r0, #16]
     b20:	e5903024 	ldr	r3, [r0, #36]	; 0x24
     b24:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
     b28:	e5803024 	str	r3, [r0, #36]	; 0x24
     b2c:	e89da800 	ldm	sp, {fp, sp, pc}

00000b30 <rx_tx_dis_v2>:
     b30:	e1a0c00d 	mov	ip, sp
     b34:	e92dd800 	push	{fp, ip, lr, pc}
     b38:	e24cb004 	sub	fp, ip, #4
     b3c:	e5903010 	ldr	r3, [r0, #16]
     b40:	e3c33102 	bic	r3, r3, #-2147483648	; 0x80000000
     b44:	e5803010 	str	r3, [r0, #16]
     b48:	e5903024 	ldr	r3, [r0, #36]	; 0x24
     b4c:	e3c33102 	bic	r3, r3, #-2147483648	; 0x80000000
     b50:	e5803024 	str	r3, [r0, #36]	; 0x24
     b54:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_buf_set(struct dma_desc *p, unsigned long paddr, int size);
00000b58 <desc_buf_set_v2>:
     b58:	e1a0c00d 	mov	ip, sp
     b5c:	e92dd800 	push	{fp, ip, lr, pc}
     b60:	e24cb004 	sub	fp, ip, #4
     b64:	e5903004 	ldr	r3, [r0, #4]
     b68:	e1a02a82 	lsl	r2, r2, #21
     b6c:	e5801008 	str	r1, [r0, #8]
     b70:	e3c33e7f 	bic	r3, r3, #2032	; 0x7f0
     b74:	e3c3300f 	bic	r3, r3, #15
     b78:	e1833aa2 	orr	r3, r3, r2, lsr #21
     b7c:	e5803004 	str	r3, [r0, #4]
     b80:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_buf_get_addr(struct dma_desc *desc);
00000b84 <desc_buf_get_addr_v2>:
     b84:	e1a0c00d 	mov	ip, sp
     b88:	e92dd800 	push	{fp, ip, lr, pc}
     b8c:	e24cb004 	sub	fp, ip, #4
     b90:	e5900008 	ldr	r0, [r0, #8]
     b94:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_buf_get_len(struct dma_desc *desc);
00000b98 <desc_buf_get_len_v2>:
     b98:	e1a0c00d 	mov	ip, sp
     b9c:	e92dd800 	push	{fp, ip, lr, pc}
     ba0:	e24cb004 	sub	fp, ip, #4
     ba4:	e5900004 	ldr	r0, [r0, #4]
     ba8:	e1a00a80 	lsl	r0, r0, #21
     bac:	e1a00aa0 	lsr	r0, r0, #21
     bb0:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_set_own(struct dma_desc *p);
00000bb4 <desc_set_own_v2>:
     bb4:	e1a0c00d 	mov	ip, sp
     bb8:	e92dd800 	push	{fp, ip, lr, pc}
     bbc:	e24cb004 	sub	fp, ip, #4
     bc0:	e5903000 	ldr	r3, [r0]
     bc4:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
     bc8:	e5803000 	str	r3, [r0]
     bcc:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_own(struct dma_desc *desc);
00000bd0 <desc_get_own_v2>:
     bd0:	e1a0c00d 	mov	ip, sp
     bd4:	e92dd800 	push	{fp, ip, lr, pc}
     bd8:	e24cb004 	sub	fp, ip, #4
     bdc:	e5900000 	ldr	r0, [r0]
     be0:	e2000102 	and	r0, r0, #-2147483648	; 0x80000000
     be4:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_init(struct dma_desc *p);
00000be8 <desc_init_v2>:
     be8:	e1a0c00d 	mov	ip, sp
     bec:	e92dd800 	push	{fp, ip, lr, pc}
     bf0:	e24cb004 	sub	fp, ip, #4
     bf4:	e3a03000 	mov	r3, #0
     bf8:	e5c03008 	strb	r3, [r0, #8]
     bfc:	e5c03009 	strb	r3, [r0, #9]
     c00:	e5c0300a 	strb	r3, [r0, #10]
     c04:	e5c0300b 	strb	r3, [r0, #11]
     c08:	e5c03004 	strb	r3, [r0, #4]
     c0c:	e5c03005 	strb	r3, [r0, #5]
     c10:	e5c03006 	strb	r3, [r0, #6]
     c14:	e3a03001 	mov	r3, #1
     c18:	e5c03007 	strb	r3, [r0, #7]
     c1c:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_rx_frame_len(struct dma_desc *desc);
00000c20 <desc_rx_frame_len_v2>:
     c20:	e1a0c00d 	mov	ip, sp
     c24:	e92dd800 	push	{fp, ip, lr, pc}
     c28:	e24cb004 	sub	fp, ip, #4
     c2c:	e5d03002 	ldrb	r3, [r0, #2]
     c30:	e5d00003 	ldrb	r0, [r0, #3]
     c34:	e200003f 	and	r0, r0, #63	; 0x3f
     c38:	e1830400 	orr	r0, r3, r0, lsl #8
     c3c:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_tx_status(struct dma_desc *desc, void *x);
00000c40 <desc_get_tx_status_v2>:
     c40:	e1a0c00d 	mov	ip, sp
     c44:	e92dd800 	push	{fp, ip, lr, pc}
     c48:	e24cb004 	sub	fp, ip, #4
     c4c:	e1a03000 	mov	r3, r0
     c50:	e5d00000 	ldrb	r0, [r0]
     c54:	e2000002 	and	r0, r0, #2
     c58:	e6ef0070 	uxtb	r0, r0
     c5c:	e3500000 	cmp	r0, #0
     c60:	15912000 	ldrne	r2, [r1]
     c64:	13e00000 	mvnne	r0, #0
     c68:	12822001 	addne	r2, r2, #1
     c6c:	15812000 	strne	r2, [r1]
     c70:	e5d32001 	ldrb	r2, [r3, #1]
     c74:	e2022004 	and	r2, r2, #4
     c78:	e6ef2072 	uxtb	r2, r2
     c7c:	e3520000 	cmp	r2, #0
     c80:	15912004 	ldrne	r2, [r1, #4]
     c84:	13e00000 	mvnne	r0, #0
     c88:	12822001 	addne	r2, r2, #1
     c8c:	15812004 	strne	r2, [r1, #4]
     c90:	e5d32001 	ldrb	r2, [r3, #1]
     c94:	e2022008 	and	r2, r2, #8
     c98:	e6ef2072 	uxtb	r2, r2
     c9c:	e3520000 	cmp	r2, #0
     ca0:	15912008 	ldrne	r2, [r1, #8]
     ca4:	13e00000 	mvnne	r0, #0
     ca8:	12822001 	addne	r2, r2, #1
     cac:	15812008 	strne	r2, [r1, #8]
     cb0:	e5d33000 	ldrb	r3, [r3]
     cb4:	e3130001 	tst	r3, #1
     cb8:	15913010 	ldrne	r3, [r1, #16]
     cbc:	12833001 	addne	r3, r3, #1
     cc0:	15813010 	strne	r3, [r1, #16]
     cc4:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_tx_close(struct dma_desc *first, struct dma_desc *end, int csum_insert);
00000cc8 <desc_tx_close_v2>:
     cc8:	e1a0c00d 	mov	ip, sp
     ccc:	e92dd800 	push	{fp, ip, lr, pc}
     cd0:	e24cb004 	sub	fp, ip, #4
     cd4:	e5d03007 	ldrb	r3, [r0, #7]
     cd8:	e3520000 	cmp	r2, #0
     cdc:	e3833020 	orr	r3, r3, #32
     ce0:	e5c03007 	strb	r3, [r0, #7]
     ce4:	e5d13007 	ldrb	r3, [r1, #7]
     ce8:	e38330c0 	orr	r3, r3, #192	; 0xc0
     cec:	e5c13007 	strb	r3, [r1, #7]
     cf0:	089da800 	ldmeq	sp, {fp, sp, pc}
     cf4:	e5d03007 	ldrb	r3, [r0, #7]
     cf8:	e3833018 	orr	r3, r3, #24
     cfc:	e5c03007 	strb	r3, [r0, #7]
     d00:	e2800010 	add	r0, r0, #16
     d04:	e1500001 	cmp	r0, r1
     d08:	9afffff9 	bls	cf4 <desc_tx_close_v2+0x2c>
     d0c:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_tx_ls(struct dma_desc *desc);
00000d10 <desc_get_tx_ls_v2>:
     d10:	e1a0c00d 	mov	ip, sp
     d14:	e92dd800 	push	{fp, ip, lr, pc}
     d18:	e24cb004 	sub	fp, ip, #4
     d1c:	e5d00007 	ldrb	r0, [r0, #7]
     d20:	e7e00350 	ubfx	r0, r0, #6, #1
     d24:	e89da800 	ldm	sp, {fp, sp, pc}

00000d28 <dma_init_v2>:
     d28:	e1a0c00d 	mov	ip, sp
     d2c:	e92dd800 	push	{fp, ip, lr, pc}
     d30:	e24cb004 	sub	fp, ip, #4
     d34:	e3a03302 	mov	r3, #134217728	; 0x8000000
     d38:	e5803004 	str	r3, [r0, #4]
     d3c:	e3003111 	movw	r3, #273	; 0x111
     d40:	e580300c 	str	r3, [r0, #12]
     d44:	e3a00000 	mov	r0, #0
     d48:	e89da800 	ldm	sp, {fp, sp, pc}

00000d4c <dma_int_dis_v2>:
     d4c:	e1a0c00d 	mov	ip, sp
     d50:	e92dd800 	push	{fp, ip, lr, pc}
     d54:	e24cb004 	sub	fp, ip, #4
     d58:	e3a03000 	mov	r3, #0
     d5c:	e580300c 	str	r3, [r0, #12]
     d60:	e89da800 	ldm	sp, {fp, sp, pc}

00000d64 <dma_int_en_v2>:
     d64:	e1a0c00d 	mov	ip, sp
     d68:	e92dd800 	push	{fp, ip, lr, pc}
     d6c:	e24cb004 	sub	fp, ip, #4
     d70:	e3003111 	movw	r3, #273	; 0x111
     d74:	e580300c 	str	r3, [r0, #12]
     d78:	e89da800 	ldm	sp, {fp, sp, pc}

int sunxi_int_status(void *, void *x);
00000d7c <int_status_v2>:
     d7c:	e1a0c00d 	mov	ip, sp
     d80:	e92dd800 	push	{fp, ip, lr, pc}
     d84:	e24cb004 	sub	fp, ip, #4
     d88:	e59020d0 	ldr	r2, [r0, #208]	; 0xd0
     d8c:	e1a03000 	mov	r3, r0
     d90:	e3120001 	tst	r2, #1
     d94:	159020d0 	ldrne	r2, [r0, #208]	; 0xd0
     d98:	e5902008 	ldr	r2, [r0, #8]
     d9c:	e2120010 	ands	r0, r2, #16
     da0:	1591006c 	ldrne	r0, [r1, #108]	; 0x6c
     da4:	12800001 	addne	r0, r0, #1
     da8:	1581006c 	strne	r0, [r1, #108]	; 0x6c
     dac:	13a00002 	movne	r0, #2
     db0:	e3120008 	tst	r2, #8
     db4:	1591c074 	ldrne	ip, [r1, #116]	; 0x74
     db8:	128cc001 	addne	ip, ip, #1
     dbc:	1581c074 	strne	ip, [r1, #116]	; 0x74
     dc0:	e3120a01 	tst	r2, #4096	; 0x1000
     dc4:	1591c078 	ldrne	ip, [r1, #120]	; 0x78
     dc8:	128cc001 	addne	ip, ip, #1
     dcc:	1581c078 	strne	ip, [r1, #120]	; 0x78
     dd0:	e3120c02 	tst	r2, #512	; 0x200
     dd4:	1591c07c 	ldrne	ip, [r1, #124]	; 0x7c
     dd8:	128cc001 	addne	ip, ip, #1
     ddc:	1581c07c 	strne	ip, [r1, #124]	; 0x7c
     de0:	e3120b01 	tst	r2, #1024	; 0x400
     de4:	1591c080 	ldrne	ip, [r1, #128]	; 0x80
     de8:	128cc001 	addne	ip, ip, #1
     dec:	1581c080 	strne	ip, [r1, #128]	; 0x80
     df0:	e3120b02 	tst	r2, #2048	; 0x800
     df4:	1591c084 	ldrne	ip, [r1, #132]	; 0x84
     df8:	128cc001 	addne	ip, ip, #1
     dfc:	1581c084 	strne	ip, [r1, #132]	; 0x84
     e00:	e3120020 	tst	r2, #32
     e04:	1591c088 	ldrne	ip, [r1, #136]	; 0x88
     e08:	128cc001 	addne	ip, ip, #1
     e0c:	1581c088 	strne	ip, [r1, #136]	; 0x88
     e10:	e3120002 	tst	r2, #2
     e14:	15910070 	ldrne	r0, [r1, #112]	; 0x70
     e18:	e302c105 	movw	ip, #8453	; 0x2105
     e1c:	e002c00c 	and	ip, r2, ip
     e20:	12800001 	addne	r0, r0, #1
     e24:	15810070 	strne	r0, [r1, #112]	; 0x70
     e28:	13a00001 	movne	r0, #1
     e2c:	e35c0000 	cmp	ip, #0
     e30:	0a000006 	beq	e50 <int_status_v2+0xd4>
     e34:	e591c0a4 	ldr	ip, [r1, #164]	; 0xa4
     e38:	e28cc001 	add	ip, ip, #1
     e3c:	e581c0a4 	str	ip, [r1, #164]	; 0xa4
     e40:	e3001101 	movw	r1, #257	; 0x101
     e44:	e0021001 	and	r1, r2, r1
     e48:	e3510000 	cmp	r1, #0
     e4c:	13a00003 	movne	r0, #3
     e50:	e1a02902 	lsl	r2, r2, #18
     e54:	e1a02922 	lsr	r2, r2, #18
     e58:	e5832008 	str	r2, [r3, #8]
     e5c:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_start_tx(void *, unsigned long);
00000e60 <start_tx_v2>:
     e60:	e1a0c00d 	mov	ip, sp
     e64:	e92dd800 	push	{fp, ip, lr, pc}
     e68:	e24cb004 	sub	fp, ip, #4
     e6c:	e5801020 	str	r1, [r0, #32]
     e70:	e5903014 	ldr	r3, [r0, #20]
     e74:	e3833101 	orr	r3, r3, #1073741824	; 0x40000000
     e78:	e5803014 	str	r3, [r0, #20]
     e7c:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_stop_tx(void *);
00000e80 <stop_tx_v2>:
     e80:	e1a0c00d 	mov	ip, sp
     e84:	e92dd800 	push	{fp, ip, lr, pc}
     e88:	e24cb004 	sub	fp, ip, #4
     e8c:	e5903014 	ldr	r3, [r0, #20]
     e90:	e3c33101 	bic	r3, r3, #1073741824	; 0x40000000
     e94:	e5803014 	str	r3, [r0, #20]
     e98:	e89da800 	ldm	sp, {fp, sp, pc}

00000e9c <poll_tx_v2>:
     e9c:	e1a0c00d 	mov	ip, sp
     ea0:	e92dd800 	push	{fp, ip, lr, pc}
     ea4:	e24cb004 	sub	fp, ip, #4
     ea8:	e5903014 	ldr	r3, [r0, #20]
     eac:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
     eb0:	e5803014 	str	r3, [r0, #20]
     eb4:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_start_rx(void *, unsigned long);
00000eb8 <start_rx_v2>:
     eb8:	e1a0c00d 	mov	ip, sp
     ebc:	e92dd800 	push	{fp, ip, lr, pc}
     ec0:	e24cb004 	sub	fp, ip, #4
     ec4:	e5801034 	str	r1, [r0, #52]	; 0x34
     ec8:	e5903028 	ldr	r3, [r0, #40]	; 0x28
     ecc:	e3833101 	orr	r3, r3, #1073741824	; 0x40000000
     ed0:	e5803028 	str	r3, [r0, #40]	; 0x28
     ed4:	e89da800 	ldm	sp, {fp, sp, pc}

void sunxi_stop_rx(void *);
00000ed8 <stop_rx_v2>:
     ed8:	e1a0c00d 	mov	ip, sp
     edc:	e92dd800 	push	{fp, ip, lr, pc}
     ee0:	e24cb004 	sub	fp, ip, #4
     ee4:	e5903028 	ldr	r3, [r0, #40]	; 0x28
     ee8:	e3c33101 	bic	r3, r3, #1073741824	; 0x40000000
     eec:	e5803028 	str	r3, [r0, #40]	; 0x28
     ef0:	e89da800 	ldm	sp, {fp, sp, pc}

00000ef4 <poll_rx_v2>:
     ef4:	e1a0c00d 	mov	ip, sp
     ef8:	e92dd800 	push	{fp, ip, lr, pc}
     efc:	e24cb004 	sub	fp, ip, #4
     f00:	e5903028 	ldr	r3, [r0, #40]	; 0x28
     f04:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
     f08:	e5803028 	str	r3, [r0, #40]	; 0x28
     f0c:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_rx_status(struct dma_desc *desc, void *x);
00000f10 <desc_get_rx_status_v1>:
     f10:	e1a0c00d 	mov	ip, sp
     f14:	e92dd800 	push	{fp, ip, lr, pc}
     f18:	e24cb004 	sub	fp, ip, #4
     f1c:	e5d02001 	ldrb	r2, [r0, #1]
     f20:	e1a03000 	mov	r3, r0
     f24:	e3120001 	tst	r2, #1
     f28:	0a000038 	beq	1010 <desc_get_rx_status_v1+0x100>
     f2c:	e2020080 	and	r0, r2, #128	; 0x80
     f30:	e6ef0070 	uxtb	r0, r0
     f34:	e3500000 	cmp	r0, #0
     f38:	0a000029 	beq	fe4 <desc_get_rx_status_v1+0xd4>
     f3c:	e2022040 	and	r2, r2, #64	; 0x40
     f40:	e3a00001 	mov	r0, #1
     f44:	e6ef2072 	uxtb	r2, r2
     f48:	e3520000 	cmp	r2, #0
     f4c:	15912028 	ldrne	r2, [r1, #40]	; 0x28
     f50:	12822001 	addne	r2, r2, #1
     f54:	15812028 	strne	r2, [r1, #40]	; 0x28
     f58:	e5d32001 	ldrb	r2, [r3, #1]
     f5c:	e2022020 	and	r2, r2, #32
     f60:	e6ef2072 	uxtb	r2, r2
     f64:	e3520000 	cmp	r2, #0
     f68:	1591202c 	ldrne	r2, [r1, #44]	; 0x2c
     f6c:	12822001 	addne	r2, r2, #1
     f70:	1581202c 	strne	r2, [r1, #44]	; 0x2c
     f74:	e5d32001 	ldrb	r2, [r3, #1]
     f78:	e2022008 	and	r2, r2, #8
     f7c:	e6ef2072 	uxtb	r2, r2
     f80:	e3520000 	cmp	r2, #0
     f84:	15912030 	ldrne	r2, [r1, #48]	; 0x30
     f88:	12822001 	addne	r2, r2, #1
     f8c:	15812030 	strne	r2, [r1, #48]	; 0x30
     f90:	e5d32000 	ldrb	r2, [r3]
     f94:	e2022080 	and	r2, r2, #128	; 0x80
     f98:	e6ef2072 	uxtb	r2, r2
     f9c:	e3520000 	cmp	r2, #0
     fa0:	15912034 	ldrne	r2, [r1, #52]	; 0x34
     fa4:	12822001 	addne	r2, r2, #1
     fa8:	15812034 	strne	r2, [r1, #52]	; 0x34
     fac:	e5d32000 	ldrb	r2, [r3]
     fb0:	e2022040 	and	r2, r2, #64	; 0x40
     fb4:	e6ef2072 	uxtb	r2, r2
     fb8:	e3520000 	cmp	r2, #0
     fbc:	15912038 	ldrne	r2, [r1, #56]	; 0x38
     fc0:	12822001 	addne	r2, r2, #1
     fc4:	15812038 	strne	r2, [r1, #56]	; 0x38
     fc8:	e5d32000 	ldrb	r2, [r3]
     fcc:	e2022002 	and	r2, r2, #2
     fd0:	e6ef2072 	uxtb	r2, r2
     fd4:	e3520000 	cmp	r2, #0
     fd8:	1591203c 	ldrne	r2, [r1, #60]	; 0x3c
     fdc:	12822001 	addne	r2, r2, #1
     fe0:	1581203c 	strne	r2, [r1, #60]	; 0x3c
     fe4:	e5d32001 	ldrb	r2, [r3, #1]
     fe8:	e5d33000 	ldrb	r3, [r3]
     fec:	e2022010 	and	r2, r2, #16
     ff0:	e6ef2072 	uxtb	r2, r2
     ff4:	e2033008 	and	r3, r3, #8
     ff8:	e6ef3073 	uxtb	r3, r3
     ffc:	e3520000 	cmp	r2, #0
    1000:	13a00001 	movne	r0, #1
    1004:	e3530000 	cmp	r3, #0
    1008:	13a00001 	movne	r0, #1
    100c:	e89da800 	ldm	sp, {fp, sp, pc}
    1010:	e3a00001 	mov	r0, #1
    1014:	e89da800 	ldm	sp, {fp, sp, pc}

int desc_get_rx_status(struct dma_desc *desc, void *x);
00001018 <desc_get_rx_status_v2>:
    1018:	e1a0c00d 	mov	ip, sp
    101c:	e92dd800 	push	{fp, ip, lr, pc}
    1020:	e24cb004 	sub	fp, ip, #4
    1024:	e5d02001 	ldrb	r2, [r0, #1]
    1028:	e1a03000 	mov	r3, r0
    102c:	e3120001 	tst	r2, #1
    1030:	0a000038 	beq	1118 <desc_get_rx_status_v2+0x100>
    1034:	e2020080 	and	r0, r2, #128	; 0x80
    1038:	e6ef0070 	uxtb	r0, r0
    103c:	e3500000 	cmp	r0, #0
    1040:	0a000029 	beq	10ec <desc_get_rx_status_v2+0xd4>
    1044:	e2022040 	and	r2, r2, #64	; 0x40
    1048:	e3a00001 	mov	r0, #1
    104c:	e6ef2072 	uxtb	r2, r2
    1050:	e3520000 	cmp	r2, #0
    1054:	15912028 	ldrne	r2, [r1, #40]	; 0x28
    1058:	12822001 	addne	r2, r2, #1
    105c:	15812028 	strne	r2, [r1, #40]	; 0x28
    1060:	e5d32001 	ldrb	r2, [r3, #1]
    1064:	e2022020 	and	r2, r2, #32
    1068:	e6ef2072 	uxtb	r2, r2
    106c:	e3520000 	cmp	r2, #0
    1070:	1591202c 	ldrne	r2, [r1, #44]	; 0x2c
    1074:	12822001 	addne	r2, r2, #1
    1078:	1581202c 	strne	r2, [r1, #44]	; 0x2c
    107c:	e5d32001 	ldrb	r2, [r3, #1]
    1080:	e2022008 	and	r2, r2, #8
    1084:	e6ef2072 	uxtb	r2, r2
    1088:	e3520000 	cmp	r2, #0
    108c:	15912030 	ldrne	r2, [r1, #48]	; 0x30
    1090:	12822001 	addne	r2, r2, #1
    1094:	15812030 	strne	r2, [r1, #48]	; 0x30
    1098:	e5d32000 	ldrb	r2, [r3]
    109c:	e2022080 	and	r2, r2, #128	; 0x80
    10a0:	e6ef2072 	uxtb	r2, r2
    10a4:	e3520000 	cmp	r2, #0
    10a8:	15912034 	ldrne	r2, [r1, #52]	; 0x34
    10ac:	12822001 	addne	r2, r2, #1
    10b0:	15812034 	strne	r2, [r1, #52]	; 0x34
    10b4:	e5d32000 	ldrb	r2, [r3]
    10b8:	e2022040 	and	r2, r2, #64	; 0x40
    10bc:	e6ef2072 	uxtb	r2, r2
    10c0:	e3520000 	cmp	r2, #0
    10c4:	15912038 	ldrne	r2, [r1, #56]	; 0x38
    10c8:	12822001 	addne	r2, r2, #1
    10cc:	15812038 	strne	r2, [r1, #56]	; 0x38
    10d0:	e5d32000 	ldrb	r2, [r3]
    10d4:	e2022002 	and	r2, r2, #2
    10d8:	e6ef2072 	uxtb	r2, r2
    10dc:	e3520000 	cmp	r2, #0
    10e0:	1591203c 	ldrne	r2, [r1, #60]	; 0x3c
    10e4:	12822001 	addne	r2, r2, #1
    10e8:	1581203c 	strne	r2, [r1, #60]	; 0x3c
    10ec:	e5d32001 	ldrb	r2, [r3, #1]
    10f0:	e5d33000 	ldrb	r3, [r3]
    10f4:	e2022010 	and	r2, r2, #16
    10f8:	e6ef2072 	uxtb	r2, r2
    10fc:	e2033008 	and	r3, r3, #8
    1100:	e6ef3073 	uxtb	r3, r3
    1104:	e3520000 	cmp	r2, #0
    1108:	13a00001 	movne	r0, #1
    110c:	e3530000 	cmp	r3, #0
    1110:	13a00001 	movne	r0, #1
    1114:	e89da800 	ldm	sp, {fp, sp, pc}
    1118:	e3a00001 	mov	r0, #1
    111c:	e89da800 	ldm	sp, {fp, sp, pc}

void desc_init_chain(struct dma_desc *p, unsigned long paddr,  int size);
00001120 <desc_init_chain>:
    1120:	e1a0c00d 	mov	ip, sp
    1124:	e92dd870 	push	{r4, r5, r6, fp, ip, lr, pc}
    1128:	e24cb004 	sub	fp, ip, #4
    112c:	e3a0c000 	mov	ip, #0
    1130:	e1a04001 	mov	r4, r1
    1134:	e1a03000 	mov	r3, r0
    1138:	e2425001 	sub	r5, r2, #1
    113c:	ea000006 	b	115c <desc_init_chain+0x3c>
    1140:	e5936004 	ldr	r6, [r3, #4]
    1144:	e2844010 	add	r4, r4, #16
    1148:	e583400c 	str	r4, [r3, #12]
    114c:	e28cc001 	add	ip, ip, #1
    1150:	e3866401 	orr	r6, r6, #16777216	; 0x1000000
    1154:	e5836004 	str	r6, [r3, #4]
    1158:	e2833010 	add	r3, r3, #16
    115c:	e15c0005 	cmp	ip, r5
    1160:	3afffff6 	bcc	1140 <desc_init_chain+0x20>
    1164:	e242221f 	sub	r2, r2, #-268435455	; 0xf0000001
    1168:	e0800202 	add	r0, r0, r2, lsl #4
    116c:	e5903004 	ldr	r3, [r0, #4]
    1170:	e580100c 	str	r1, [r0, #12]
    1174:	e3833401 	orr	r3, r3, #16777216	; 0x1000000
    1178:	e5803004 	str	r3, [r0, #4]
    117c:	e89da870 	ldm	sp, {r4, r5, r6, fp, sp, pc}

int sunxi_mdio_read(void *,  int, int);
00001180 <sunxi_mdio_read>:
    1180:	e1a0c00d 	mov	ip, sp
    1184:	e92dd800 	push	{fp, ip, lr, pc}
    1188:	e24cb004 	sub	fp, ip, #4
    118c:	e59f300c 	ldr	r3, [pc, #12]	; 11a0 <sunxi_mdio_read+0x20>
    1190:	e5933008 	ldr	r3, [r3, #8]
    1194:	e5933028 	ldr	r3, [r3, #40]	; 0x28 =mdio_read
    1198:	e12fff33 	blx	r3
    119c:	e89da800 	ldm	sp, {fp, sp, pc}
    11a0:	00000000
			11a0: R_ARM_ABS32	.bss

int sunxi_mdio_write(void *, int, int, unsigned short);
000011a4 <sunxi_mdio_write>:
    11a4:	e1a0c00d 	mov	ip, sp
    11a8:	e92dd800 	push	{fp, ip, lr, pc}
    11ac:	e24cb004 	sub	fp, ip, #4
    11b0:	e59fc00c 	ldr	ip, [pc, #12]	; 11c4 <sunxi_mdio_write+0x20>
    11b4:	e59cc008 	ldr	ip, [ip, #8]
    11b8:	e59cc02c 	ldr	ip, [ip, #44]	; 0x2c =mdio_write
    11bc:	e12fff3c 	blx	ip
    11c0:	e89da800 	ldm	sp, {fp, sp, pc}
    11c4:	00000000
			11c4: R_ARM_ABS32	.bss

int sunxi_mdio_reset(void *);
000011c8 <sunxi_mdio_reset>:
    11c8:	e1a0c00d 	mov	ip, sp
    11cc:	e92dd800 	push	{fp, ip, lr, pc}
    11d0:	e24cb004 	sub	fp, ip, #4
    11d4:	e59f300c 	ldr	r3, [pc, #12]	; 11e8 <sunxi_mdio_reset+0x20>
    11d8:	e5933008 	ldr	r3, [r3, #8]
    11dc:	e5933030 	ldr	r3, [r3, #48]	; 0x30 =mdio_reset
    11e0:	e12fff33 	blx	r3
    11e4:	e89da800 	ldm	sp, {fp, sp, pc}
    11e8:	00000000
			11e8: R_ARM_ABS32	.bss

void sunxi_set_link_mode(void *iobase, int duplex, int speed);
000011ec <sunxi_set_link_mode>:
    11ec:	e1a0c00d 	mov	ip, sp
    11f0:	e92dd800 	push	{fp, ip, lr, pc}
    11f4:	e24cb004 	sub	fp, ip, #4
    11f8:	e59f300c 	ldr	r3, [pc, #12]	; 120c <sunxi_set_link_mode+0x20>
    11fc:	e5933008 	ldr	r3, [r3, #8]
    1200:	e5933018 	ldr	r3, [r3, #24]	; 0x18 =set_link_mode
    1204:	e12fff33 	blx	r3
    1208:	e89da800 	ldm	sp, {fp, sp, pc}
    120c:	00000000
			120c: R_ARM_ABS32	.bss

void sunxi_mac_loopback(void *iobase, int enable);
00001210 <sunxi_mac_loopback>:
    1210:	e1a0c00d 	mov	ip, sp
    1214:	e92dd800 	push	{fp, ip, lr, pc}
    1218:	e24cb004 	sub	fp, ip, #4
    121c:	e59f300c 	ldr	r3, [pc, #12]	; 1230 <sunxi_mac_loopback+0x20>
    1220:	e5933008 	ldr	r3, [r3, #8]
    1224:	e5933034 	ldr	r3, [r3, #52]	; 0x34 =mac_loopback
    1228:	e12fff33 	blx	r3
    122c:	e89da800 	ldm	sp, {fp, sp, pc}
    1230:	00000000
			1230: R_ARM_ABS32	.bss

void sunxi_flow_ctrl(void *iobase, int duplex, int fc, int pause);
00001234 <sunxi_flow_ctrl>:
    1234:	e1a0c00d 	mov	ip, sp
    1238:	e92dd800 	push	{fp, ip, lr, pc}
    123c:	e24cb004 	sub	fp, ip, #4
    1240:	e59fc00c 	ldr	ip, [pc, #12]	; 1254 <sunxi_flow_ctrl+0x20>
    1244:	e59cc008 	ldr	ip, [ip, #8]
    1248:	e59cc01c 	ldr	ip, [ip, #28]	; 0x1c =flow_ctrl
    124c:	e12fff3c 	blx	ip
    1250:	e89da800 	ldm	sp, {fp, sp, pc}
    1254:	00000000
			1254: R_ARM_ABS32	.bss

int sunxi_int_status(void *, void *x);
00001258 <sunxi_int_status>:
    1258:	e1a0c00d 	mov	ip, sp
    125c:	e92dd800 	push	{fp, ip, lr, pc}
    1260:	e24cb004 	sub	fp, ip, #4
    1264:	e59f300c 	ldr	r3, [pc, #12]	; 1278 <sunxi_int_status+0x20>
    1268:	e5933008 	ldr	r3, [r3, #8]
    126c:	e5933070 	ldr	r3, [r3, #112]	; 0x70 =int_status
    1270:	e12fff33 	blx	r3
    1274:	e89da800 	ldm	sp, {fp, sp, pc}
    1278:	00000000
			1278: R_ARM_ABS32	.bss

void sunxi_start_rx(void *, unsigned long);
0000127c <sunxi_start_rx>:
    127c:	e1a0c00d 	mov	ip, sp
    1280:	e92dd800 	push	{fp, ip, lr, pc}
    1284:	e24cb004 	sub	fp, ip, #4
    1288:	e59f300c 	ldr	r3, [pc, #12]	; 129c <sunxi_start_rx+0x20>
    128c:	e5933008 	ldr	r3, [r3, #8]
    1290:	e5933080 	ldr	r3, [r3, #128]	; 0x80 =start_rx
    1294:	e12fff33 	blx	r3
    1298:	e89da800 	ldm	sp, {fp, sp, pc}
    129c:	00000000
			129c: R_ARM_ABS32	.bss

void sunxi_stop_rx(void *);
000012a0 <sunxi_stop_rx>:
    12a0:	e1a0c00d 	mov	ip, sp
    12a4:	e92dd800 	push	{fp, ip, lr, pc}
    12a8:	e24cb004 	sub	fp, ip, #4
    12ac:	e59f300c 	ldr	r3, [pc, #12]	; 12c0 <sunxi_stop_rx+0x20>
    12b0:	e5933008 	ldr	r3, [r3, #8]
    12b4:	e5933084 	ldr	r3, [r3, #132]	; 0x84 =stop_rx
    12b8:	e12fff33 	blx	r3
    12bc:	e89da800 	ldm	sp, {fp, sp, pc}
    12c0:	00000000
			12c0: R_ARM_ABS32	.bss

void sunxi_start_tx(void *, unsigned long);
000012c4 <sunxi_start_tx>:
    12c4:	e1a0c00d 	mov	ip, sp
    12c8:	e92dd800 	push	{fp, ip, lr, pc}
    12cc:	e24cb004 	sub	fp, ip, #4
    12d0:	e59f300c 	ldr	r3, [pc, #12]	; 12e4 <sunxi_start_tx+0x20>
    12d4:	e5933008 	ldr	r3, [r3, #8]
    12d8:	e5933074 	ldr	r3, [r3, #116]	; 0x74 =start_tx
    12dc:	e12fff33 	blx	r3
    12e0:	e89da800 	ldm	sp, {fp, sp, pc}
    12e4:	00000000
			12e4: R_ARM_ABS32	.bss

void sunxi_stop_tx(void *);
000012e8 <sunxi_stop_tx>:
    12e8:	e1a0c00d 	mov	ip, sp
    12ec:	e92dd800 	push	{fp, ip, lr, pc}
    12f0:	e24cb004 	sub	fp, ip, #4
    12f4:	e59f300c 	ldr	r3, [pc, #12]	; 1308 <sunxi_stop_tx+0x20>
    12f8:	e5933008 	ldr	r3, [r3, #8]
    12fc:	e5933078 	ldr	r3, [r3, #120]	; 0x78 =stop_tx
    1300:	e12fff33 	blx	r3
    1304:	e89da800 	ldm	sp, {fp, sp, pc}
    1308:	00000000
			1308: R_ARM_ABS32	.bss

int sunxi_mac_init(void *, int txmode, int rxmode);
0000130c <sunxi_mac_init>:
    130c:	e1a0c00d 	mov	ip, sp
    1310:	e92dd800 	push	{fp, ip, lr, pc}
    1314:	e24cb004 	sub	fp, ip, #4
    1318:	e59f300c 	ldr	r3, [pc, #12]	; 132c <sunxi_mac_init+0x20>
    131c:	e5933008 	ldr	r3, [r3, #8]
    1320:	e5933004 	ldr	r3, [r3, #4]	; 0x4 =mac_init
    1324:	e12fff33 	blx	r3
    1328:	e89da800 	ldm	sp, {fp, sp, pc}
    132c:	00000000
			132c: R_ARM_ABS32	.bss

void sunxi_hash_filter(void *iobase, unsigned long low, unsigned long high);
00001330 <sunxi_hash_filter>:
    1330:	e1a0c00d 	mov	ip, sp
    1334:	e92dd800 	push	{fp, ip, lr, pc}
    1338:	e24cb004 	sub	fp, ip, #4
    133c:	e59f300c 	ldr	r3, [pc, #12]	; 1350 <sunxi_hash_filter+0x20>
    1340:	e5933008 	ldr	r3, [r3, #8]
    1344:	e593300c 	ldr	r3, [r3, #12]	; 0xc =hash_filter
    1348:	e12fff33 	blx	r3
    134c:	e89da800 	ldm	sp, {fp, sp, pc}
    1350:	00000000
			1350: R_ARM_ABS32	.bss

void sunxi_set_filter(void *iobase, unsigned long flags);
00001354 <sunxi_set_filter>:
    1354:	e1a0c00d 	mov	ip, sp
    1358:	e92dd800 	push	{fp, ip, lr, pc}
    135c:	e24cb004 	sub	fp, ip, #4
    1360:	e59f300c 	ldr	r3, [pc, #12]	; 1374 <sunxi_set_filter+0x20>
    1364:	e5933008 	ldr	r3, [r3, #8]
    1368:	e5933010 	ldr	r3, [r3, #16]	; 0x10 =set_filter
    136c:	e12fff33 	blx	r3
    1370:	e89da800 	ldm	sp, {fp, sp, pc}
    1374:	00000000
			1374: R_ARM_ABS32	.bss

void sunxi_set_umac(void *, unsigned char *, int);
00001378 <sunxi_set_umac>:
    1378:	e1a0c00d 	mov	ip, sp
    137c:	e92dd800 	push	{fp, ip, lr, pc}
    1380:	e24cb004 	sub	fp, ip, #4
    1384:	e59f300c 	ldr	r3, [pc, #12]	; 1398 <sunxi_set_umac+0x20>
    1388:	e5933008 	ldr	r3, [r3, #8]
    138c:	e5933008 	ldr	r3, [r3, #8]	; 0x8 =set_umac
    1390:	e12fff33 	blx	r3
    1394:	e89da800 	ldm	sp, {fp, sp, pc}
    1398:	00000000
			1398: R_ARM_ABS32	.bss

void sunxi_mac_enable(void *);
0000139c <sunxi_mac_enable>:
    139c:	e1a0c00d 	mov	ip, sp
    13a0:	e92dd800 	push	{fp, ip, lr, pc}
    13a4:	e24cb004 	sub	fp, ip, #4
    13a8:	e59f300c 	ldr	r3, [pc, #12]	; 13bc <sunxi_mac_enable+0x20>
    13ac:	e5933008 	ldr	r3, [r3, #8]
    13b0:	e5933020 	ldr	r3, [r3, #32]	; 0x20 =rx_tx_en
    13b4:	e12fff33 	blx	r3
    13b8:	e89da800 	ldm	sp, {fp, sp, pc}
    13bc:	00000000
			13bc: R_ARM_ABS32	.bss

void sunxi_mac_disable(void *);
000013c0 <sunxi_mac_disable>:
    13c0:	e1a0c00d 	mov	ip, sp
    13c4:	e92dd800 	push	{fp, ip, lr, pc}
    13c8:	e24cb004 	sub	fp, ip, #4
    13cc:	e59f300c 	ldr	r3, [pc, #12]	; 13e0 <sunxi_mac_disable+0x20>
    13d0:	e5933008 	ldr	r3, [r3, #8]
    13d4:	e5933024 	ldr	r3, [r3, #36]	; 0x24 =rx_tx_dis
    13d8:	e12fff33 	blx	r3
    13dc:	e89da800 	ldm	sp, {fp, sp, pc}
    13e0:	00000000
			13e0: R_ARM_ABS32	.bss

void sunxi_tx_poll(void *);
000013e4 <sunxi_tx_poll>:
    13e4:	e1a0c00d 	mov	ip, sp
    13e8:	e92dd800 	push	{fp, ip, lr, pc}
    13ec:	e24cb004 	sub	fp, ip, #4
    13f0:	e59f300c 	ldr	r3, [pc, #12]	; 1404 <sunxi_tx_poll+0x20>
    13f4:	e5933008 	ldr	r3, [r3, #8]
    13f8:	e593307c 	ldr	r3, [r3, #124]	; 0x7c =poll_tx
    13fc:	e12fff33 	blx	r3
    1400:	e89da800 	ldm	sp, {fp, sp, pc}
    1404:	00000000
			1404: R_ARM_ABS32	.bss

00001408 <sunxi_rx_poll>:
    1408:	e1a0c00d 	mov	ip, sp
    140c:	e92dd800 	push	{fp, ip, lr, pc}
    1410:	e24cb004 	sub	fp, ip, #4
    1414:	e59f300c 	ldr	r3, [pc, #12]	; 1428 <sunxi_rx_poll+0x20>
    1418:	e5933008 	ldr	r3, [r3, #8]
    141c:	e5933088 	ldr	r3, [r3, #136]	; 0x88 =poll_rx
    1420:	e12fff33 	blx	r3
    1424:	e89da800 	ldm	sp, {fp, sp, pc}
    1428:	00000000
			1428: R_ARM_ABS32	.bss

void sunxi_int_enable(void *);
0000142c <sunxi_int_enable>:
    142c:	e1a0c00d 	mov	ip, sp
    1430:	e92dd800 	push	{fp, ip, lr, pc}
    1434:	e24cb004 	sub	fp, ip, #4
    1438:	e59f300c 	ldr	r3, [pc, #12]	; 144c <sunxi_int_enable+0x20>
    143c:	e5933008 	ldr	r3, [r3, #8]
    1440:	e593306c 	ldr	r3, [r3, #108]	; 0x6c =dma_int_en
    1444:	e12fff33 	blx	r3
    1448:	e89da800 	ldm	sp, {fp, sp, pc}
    144c:	00000000
			144c: R_ARM_ABS32	.bss

void sunxi_int_disable(void *);
00001450 <sunxi_int_disable>:
    1450:	e1a0c00d 	mov	ip, sp
    1454:	e92dd800 	push	{fp, ip, lr, pc}
    1458:	e24cb004 	sub	fp, ip, #4
    145c:	e59f300c 	ldr	r3, [pc, #12]	; 1470 <sunxi_int_disable+0x20>
    1460:	e5933008 	ldr	r3, [r3, #8]
    1464:	e5933068 	ldr	r3, [r3, #104]	; 0x68 =dma_int_dis
    1468:	e12fff33 	blx	r3
    146c:	e89da800 	ldm	sp, {fp, sp, pc}
    1470:	00000000
			1470: R_ARM_ABS32	.bss

void desc_buf_set(struct dma_desc *p, unsigned long paddr, int size);
00001474 <desc_buf_set>:
    1474:	e1a0c00d 	mov	ip, sp
    1478:	e92dd800 	push	{fp, ip, lr, pc}
    147c:	e24cb004 	sub	fp, ip, #4
    1480:	e59f300c 	ldr	r3, [pc, #12]	; 1494 <desc_buf_set+0x20>
    1484:	e5933008 	ldr	r3, [r3, #8]
    1488:	e5933038 	ldr	r3, [r3, #56]	; 0x38 =desc_buf_set
    148c:	e12fff33 	blx	r3
    1490:	e89da800 	ldm	sp, {fp, sp, pc}
    1494:	00000000
			1494: R_ARM_ABS32	.bss

void desc_set_own(struct dma_desc *p);
00001498 <desc_set_own>:
    1498:	e1a0c00d 	mov	ip, sp
    149c:	e92dd800 	push	{fp, ip, lr, pc}
    14a0:	e24cb004 	sub	fp, ip, #4
    14a4:	e59f300c 	ldr	r3, [pc, #12]	; 14b8 <desc_set_own+0x20>
    14a8:	e5933008 	ldr	r3, [r3, #8]
    14ac:	e5933044 	ldr	r3, [r3, #68]	; 0x44 =desc_set_own
    14b0:	e12fff33 	blx	r3
    14b4:	e89da800 	ldm	sp, {fp, sp, pc}
    14b8:	00000000
			14b8: R_ARM_ABS32	.bss

void desc_tx_close(struct dma_desc *first, struct dma_desc *end, int csum_insert);
000014bc <desc_tx_close>:
    14bc:	e1a0c00d 	mov	ip, sp
    14c0:	e92dd800 	push	{fp, ip, lr, pc}
    14c4:	e24cb004 	sub	fp, ip, #4
    14c8:	e59f300c 	ldr	r3, [pc, #12]	; 14dc <desc_tx_close+0x20>
    14cc:	e5933008 	ldr	r3, [r3, #8]
    14d0:	e593305c 	ldr	r3, [r3, #92]	; 0x5c =desc_tx_close
    14d4:	e12fff33 	blx	r3
    14d8:	e89da800 	ldm	sp, {fp, sp, pc}
    14dc:	00000000
			14dc: R_ARM_ABS32	.bss

void desc_init(struct dma_desc *p);
000014e0 <desc_init>:
    14e0:	e1a0c00d 	mov	ip, sp
    14e4:	e92dd800 	push	{fp, ip, lr, pc}
    14e8:	e24cb004 	sub	fp, ip, #4
    14ec:	e59f300c 	ldr	r3, [pc, #12]	; 1500 <desc_init+0x20>
    14f0:	e5933008 	ldr	r3, [r3, #8]
    14f4:	e593304c 	ldr	r3, [r3, #76]	; 0x4c =desc_init
    14f8:	e12fff33 	blx	r3
    14fc:	e89da800 	ldm	sp, {fp, sp, pc}
    1500:	00000000
			1500: R_ARM_ABS32	.bss

int desc_get_tx_status(struct dma_desc *desc, void *x);
00001504 <desc_get_tx_status>:
    1504:	e1a0c00d 	mov	ip, sp
    1508:	e92dd800 	push	{fp, ip, lr, pc}
    150c:	e24cb004 	sub	fp, ip, #4
    1510:	e59f300c 	ldr	r3, [pc, #12]	; 1524 <desc_get_tx_status+0x20>
    1514:	e5933008 	ldr	r3, [r3, #8]
    1518:	e5933058 	ldr	r3, [r3, #88]	; 0x58 =desc_get_tx_status
    151c:	e12fff33 	blx	r3
    1520:	e89da800 	ldm	sp, {fp, sp, pc}
    1524:	00000000
			1524: R_ARM_ABS32	.bss

int desc_buf_get_len(struct dma_desc *desc);
00001528 <desc_buf_get_len>:
    1528:	e1a0c00d 	mov	ip, sp
    152c:	e92dd800 	push	{fp, ip, lr, pc}
    1530:	e24cb004 	sub	fp, ip, #4
    1534:	e59f300c 	ldr	r3, [pc, #12]	; 1548 <desc_buf_get_len+0x20>
    1538:	e5933008 	ldr	r3, [r3, #8]
    153c:	e5933040 	ldr	r3, [r3, #64]	; 0x40 =desc_buf_get_len
    1540:	e12fff33 	blx	r3
    1544:	e89da800 	ldm	sp, {fp, sp, pc}
    1548:	00000000
			1548: R_ARM_ABS32	.bss

int desc_buf_get_addr(struct dma_desc *desc);
0000154c <desc_buf_get_addr>:
    154c:	e1a0c00d 	mov	ip, sp
    1550:	e92dd800 	push	{fp, ip, lr, pc}
    1554:	e24cb004 	sub	fp, ip, #4
    1558:	e59f300c 	ldr	r3, [pc, #12]	; 156c <desc_buf_get_addr+0x20>
    155c:	e5933008 	ldr	r3, [r3, #8]
    1560:	e593303c 	ldr	r3, [r3, #60]	; 0x3c =desc_buf_get_addr
    1564:	e12fff33 	blx	r3
    1568:	e89da800 	ldm	sp, {fp, sp, pc}
    156c:	00000000
			156c: R_ARM_ABS32	.bss

int desc_rx_frame_len(struct dma_desc *desc);
00001570 <desc_rx_frame_len>:
    1570:	e1a0c00d 	mov	ip, sp
    1574:	e92dd800 	push	{fp, ip, lr, pc}
    1578:	e24cb004 	sub	fp, ip, #4
    157c:	e59f300c 	ldr	r3, [pc, #12]	; 1590 <desc_rx_frame_len+0x20>
    1580:	e5933008 	ldr	r3, [r3, #8]
    1584:	e5933050 	ldr	r3, [r3, #80]	; 0x50 =desc_rx_frame_len
    1588:	e12fff33 	blx	r3
    158c:	e89da800 	ldm	sp, {fp, sp, pc}
    1590:	00000000
			1590: R_ARM_ABS32	.bss

int desc_get_rx_status(struct dma_desc *desc, void *x);
00001594 <desc_get_rx_status>:
    1594:	e1a0c00d 	mov	ip, sp
    1598:	e92dd800 	push	{fp, ip, lr, pc}
    159c:	e24cb004 	sub	fp, ip, #4
    15a0:	e59f300c 	ldr	r3, [pc, #12]	; 15b4 <desc_get_rx_status+0x20>
    15a4:	e5933008 	ldr	r3, [r3, #8]
    15a8:	e5933054 	ldr	r3, [r3, #84]	; 0x54 =desc_get_rx_status
    15ac:	e12fff33 	blx	r3
    15b0:	e89da800 	ldm	sp, {fp, sp, pc}
    15b4:	00000000
			15b4: R_ARM_ABS32	.bss

int desc_get_own(struct dma_desc *desc);
000015b8 <desc_get_own>:
    15b8:	e1a0c00d 	mov	ip, sp
    15bc:	e92dd800 	push	{fp, ip, lr, pc}
    15c0:	e24cb004 	sub	fp, ip, #4
    15c4:	e59f300c 	ldr	r3, [pc, #12]	; 15d8 <desc_get_own+0x20>
    15c8:	e5933008 	ldr	r3, [r3, #8]
    15cc:	e5933048 	ldr	r3, [r3, #72]	; 0x48 =desc_get_own
    15d0:	e12fff33 	blx	r3
    15d4:	e89da800 	ldm	sp, {fp, sp, pc}
    15d8:	00000000
			15d8: R_ARM_ABS32	.bss

int desc_get_tx_ls(struct dma_desc *desc);
000015dc <desc_get_tx_ls>:
    15dc:	e1a0c00d 	mov	ip, sp
    15e0:	e92dd800 	push	{fp, ip, lr, pc}
    15e4:	e24cb004 	sub	fp, ip, #4
    15e8:	e59f300c 	ldr	r3, [pc, #12]	; 15fc <desc_get_tx_ls+0x20>
    15ec:	e5933008 	ldr	r3, [r3, #8]
    15f0:	e5933060 	ldr	r3, [r3, #96]	; 0x60 =desc_get_tx_ls
    15f4:	e12fff33 	blx	r3
    15f8:	e89da800 	ldm	sp, {fp, sp, pc}
    15fc:	00000000
			15fc: R_ARM_ABS32	.bss

void sunxi_geth_register(void *iobase, int version);
00001600 <sunxi_geth_register>:
    1600:	e1a0c00d 	mov	ip, sp
    1604:	e92dd800 	push	{fp, ip, lr, pc}
    1608:	e24cb004 	sub	fp, ip, #4
    160c:	e3510000 	cmp	r1, #0
    1610:	e59f3010 	ldr	r3, [pc, #16]	; 1628 <sunxi_geth_register+0x28>
    1614:	059f2010 	ldreq	r2, [pc, #16]	; 162c <sunxi_geth_register+0x2c>
    1618:	159f2010 	ldrne	r2, [pc, #16]	; 1630 <sunxi_geth_register+0x30>
    161c:	e8830007 	stm	r3, {r0, r1, r2}
    1620:	e3a00000 	mov	r0, #0
    1624:	e89da800 	ldm	sp, {fp, sp, pc}
    1628:	00000000
			1628: R_ARM_ABS32	.bss
    162c:	=function_table_v1
			162c: R_ARM_ABS32	.data
    1630:	=function_table_v2
			1630: R_ARM_ABS32	.data

int sunxi_mac_reset(void *iobase, void (*mdelay)(int), int n);
00001634 <sunxi_mac_reset>:
    1634:	e1a0c00d 	mov	ip, sp
    1638:	e92dd800 	push	{fp, ip, lr, pc}
    163c:	e24cb004 	sub	fp, ip, #4
    1640:	e59f300c 	ldr	r3, [pc, #12]	; 1654 <sunxi_mac_reset+0x20>
    1644:	e5933008 	ldr	r3, [r3, #8]
    1648:	e5933000 	ldr	r3, [r3]	; 0x0 =mac_reset
    164c:	e12fff33 	blx	r3
    1650:	e89da800 	ldm	sp, {fp, sp, pc}
    1654:	00000000
			1654: R_ARM_ABS32	.bss

Disassembly of section .data:

00000000 <.data>:
function_table_v1:
			0: R_ARM_ABS32	mac_reset_v1
			4: R_ARM_ABS32	mac_init_v1
			8: R_ARM_ABS32	set_umac_v1
			c: R_ARM_ABS32	hash_filter_v1
			10: R_ARM_ABS32	set_filter_v1
			14: R_ARM_ABS32	get_umac_v1
			18: R_ARM_ABS32	set_link_mode_v1
			1c: R_ARM_ABS32	flow_ctrl_v1
			20: R_ARM_ABS32	rx_tx_en_v1
			24: R_ARM_ABS32	rx_tx_dis_v1
			28: R_ARM_ABS32	mdio_read_v1
			2c: R_ARM_ABS32	mdio_write_v1
			30: R_ARM_ABS32	mdio_reset_v1
			34: R_ARM_ABS32	mac_loopback_v1
			38: R_ARM_ABS32	desc_buf_set_v1
			3c: R_ARM_ABS32	desc_buf_get_addr_v1
			40: R_ARM_ABS32	desc_buf_get_len_v1
			44: R_ARM_ABS32	desc_set_own_v1
			48: R_ARM_ABS32	desc_get_own_v1
			4c: R_ARM_ABS32	desc_init_v1
			50: R_ARM_ABS32	desc_rx_frame_len_v1
			54: R_ARM_ABS32	desc_get_rx_status_v1
			58: R_ARM_ABS32	desc_get_tx_status_v1
			5c: R_ARM_ABS32	desc_tx_close_v1
			60: R_ARM_ABS32	desc_get_tx_ls_v1
			64: R_ARM_ABS32	dma_init_v1
			68: R_ARM_ABS32	dma_int_dis_v1
			6c: R_ARM_ABS32	dma_int_en_v1
			70: R_ARM_ABS32	int_status_v1
			74: R_ARM_ABS32	start_tx_v1
			78: R_ARM_ABS32	stop_tx_v1
			7c: R_ARM_ABS32	poll_tx_v1
			80: R_ARM_ABS32	start_rx_v1
			84: R_ARM_ABS32	stop_rx_v1
			88: R_ARM_ABS32	poll_rx_v1
			8c: R_ARM_ABS32	mac_reset_v2
			90: R_ARM_ABS32	mac_init_v2
			94: R_ARM_ABS32	set_umac_v2
function_table_v2:
			98: R_ARM_ABS32	hash_filter_v2
			9c: R_ARM_ABS32	set_filter_v2
			a0: R_ARM_ABS32	get_umac_v2
			a4: R_ARM_ABS32	set_link_mode_v2
			a8: R_ARM_ABS32	flow_ctrl_v2
			ac: R_ARM_ABS32	rx_tx_en_v2
			b0: R_ARM_ABS32	rx_tx_dis_v2
			b4: R_ARM_ABS32	mdio_read_v2
			b8: R_ARM_ABS32	mdio_write_v2
			bc: R_ARM_ABS32	mdio_reset_v2
			c0: R_ARM_ABS32	mac_loopback_v2
			c4: R_ARM_ABS32	desc_buf_set_v2
			c8: R_ARM_ABS32	desc_buf_get_addr_v2
			cc: R_ARM_ABS32	desc_buf_get_len_v2
			d0: R_ARM_ABS32	desc_set_own_v2
			d4: R_ARM_ABS32	desc_get_own_v2
			d8: R_ARM_ABS32	desc_init_v2
			dc: R_ARM_ABS32	desc_rx_frame_len_v2
			e0: R_ARM_ABS32	desc_get_rx_status_v2
			e4: R_ARM_ABS32	desc_get_tx_status_v2
			e8: R_ARM_ABS32	desc_tx_close_v2
			ec: R_ARM_ABS32	desc_get_tx_ls_v2
			f0: R_ARM_ABS32	dma_init_v2
			f4: R_ARM_ABS32	dma_int_dis_v2
			f8: R_ARM_ABS32	dma_int_en_v2
			fc: R_ARM_ABS32	int_status_v2
			100: R_ARM_ABS32	start_tx_v2
			104: R_ARM_ABS32	stop_tx_v2
			108: R_ARM_ABS32	poll_tx_v2
			10c: R_ARM_ABS32	start_rx_v2
			110: R_ARM_ABS32	stop_rx_v2
			114: R_ARM_ABS32	poll_rx_v2

00000000 <.bss>:
	0:	iobase			// Unused
	4:	version			// Unused
	8:	function_table
