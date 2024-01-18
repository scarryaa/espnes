import sys

first_file = sys.argv[1]
second_file = sys.argv[2]

# extract C004  78  D8  A2  A:00 X:00 Y:00 P:24 SP:FD CYC:007
PC = []
SP = []
A = []
X = []
Y = []
P = []
Opcode = []
CYC = []

with open(first_file) as f:
    for line in f:
        PC.append(line[0:4])
        SP.append(line[41:43])
        A.append(line[20:22])
        X.append(line[25:27])
        Y.append(line[30:32])
        P.append(line[35:37])
        Opcode.append(line[6:8])
        CYC.append(line[48:51])

#extract C7A8  AD 02 20  LDA $2002 = FF                  A:10 X:FF Y:00 P:A4 SP:FF CYC:021
PC2 = []
SP2 = []
A2 = []
X2 = []
Y2 = []
P2 = []
Opcode2 = []
CYC2 = []

with open(second_file) as f:
    for line in f:
        PC2.append(line[0:4])
        SP2.append(line[71:73])
        A2.append(line[50:52])
        X2.append(line[55:57])
        Y2.append(line[60:62])
        P2.append(line[65:67])
        Opcode2.append(line[6:8])
        CYC2.append(line[78:81])

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