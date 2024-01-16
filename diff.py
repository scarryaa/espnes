import sys

first_file = sys.argv[1]
second_file = sys.argv[2]

# extract PC from first file (PC:0000)
PC_list = []
with open(first_file) as f:
    for line in f:
        if (line[0:4].isupper()):
            PC_list.append(line.split(":")[1].split(" ")[0])

# extract PC from second file (first 4)
PC_list2 = []
with open(second_file) as f:
    for line in f:
        if (line[0:4].isupper()):
            PC_list2.append(line.split("  ")[0])

# compare PC_list and PC_list2 and print the first difference
for i in range(len(PC_list)):
    if PC_list[i] != PC_list2[i]:
        print("PC_list: ", PC_list[i])
        print("PC_list2: ", PC_list2[i])
        break
