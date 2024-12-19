list1 = []
list2 = []
with open("input_1.txt") as f:
    for line in f.readlines():
        list1.append(int(line.split()[0]))
        list2.append(int(line.split()[1]))

out = 0
for i in range(len(list1)):
    if min(list1) > min(list2):
        out += min(list1) - min(list2)
    else:
        out += min(list2) - min(list1)
    list2.remove(min(list2))
    list1.remove(min(list1))

print(out)
