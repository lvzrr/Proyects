list1 = []
list2 = []
with open("input_1.txt") as f:
    for line in f.readlines():
        list1.append(int(line.split()[0]))
        list2.append(int(line.split()[1]))

out = 0
for n in list1:
    rep = list2.count(n)
    out += n * rep

print(out)
