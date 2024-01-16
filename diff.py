import sys

first_file = sys.argv[1]
second_file = sys.argv[2]

# extract PC:C7A8 SP:FF A:00 X:FF Y:00 P:26 Opcode:AD
PC = []
SP = []
A = []
X = []
Y = []
P = []
Opcode = []

with open(first_file) as f:
    for line in f:
        if line.startswith('PC'):
            PC.append(line[3:7])
            SP.append(line[11:13])
            A.append(line[16:18])
            X.append(line[21:23])
            Y.append(line[26:28])
            P.append(line[31:33])
            Opcode.append(line[41:43])

#extract C7A8  AD 02 20  LDA $2002 = FF                  A:10 X:FF Y:00 P:A4 SP:FF PPU:  0, 63 CYC:21
PC2 = []
SP2 = []
A2 = []
X2 = []
Y2 = []
P2 = []
Opcode2 = []

with open(second_file) as f:
    for line in f:
        PC2.append(line[0:4])
        SP2.append(line[71:73])
        A2.append(line[50:52])
        X2.append(line[55:57])
        Y2.append(line[60:62])
        P2.append(line[65:67])
        Opcode2.append(line[6:8])

# compare
i = 0
for i in range(len(PC)):
    if PC[i] != PC2[i]:
        print('PC: ' + PC[i] + ' != ' + PC2[i])
        print(i + 1)
        break
    if SP[i] != SP2[i]:
        print('SP: ' + SP[i] + ' != ' + SP2[i])
        print(i + 1)
        break
    if A[i] != A2[i]:
        print('A: ' + A[i] + ' != ' + A2[i])
        print(i + 1)
        break
    if X[i] != X2[i]:
        print('X: ' + X[i] + ' != ' + X2[i])
        print(i + 1)
        break
    if Y[i] != Y2[i]:
        print('Y: ' + Y[i] + ' != ' + Y2[i])
        print(i + 1)
        break
    if P[i] != P2[i]:
        print('P: ' + P[i] + ' != ' + P2[i])
        print(i + 1)
        break
    if Opcode[i] != Opcode2[i]:
        print('Opcode: ' + Opcode[i] + ' != ' + Opcode2[i])
        print(i + 1)
        break
    i += 1