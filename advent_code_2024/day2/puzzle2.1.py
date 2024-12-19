out = 0
data = []

def check_inc(row,x = 0,y = 1):
    if row[x] < row[y]:
        return True
    elif row[x] > row[y]:
        return False
    else:
        return check_inc(row,x+1,y+1)

def check_increments(row,increasing):
    for i in range(1,len(row)):
        if (increasing and row[i] - row[i-1] < 0) or ((not increasing) and row[i - 1] - row[i] < 0):
                return False    
    return True

# ALL DECREASING OR INCREASING // ALSO 1 <= diff <= 3
# data = [[x,x,x,x,x],...,[x,x,x,x,x]]

def check_diff(row,increasing):
    for i in range(1,len(row)):
        if (increasing and not(1 <= row[i] - row[i-1]<= 3)) or (not increasing and not(1 <= row[i-1] - row[i]<= 3)):
            return False
    return True

with open('input_2.1.txt') as f:
    lines = f.readlines()
    for line in lines :
        buf = []
        for char in line.split(" "):
            buf.append(int(char))
        data.append(buf)

# ALL DECREASING OR INCREASING // ALSO 1 <= diff <= 3
# data = [[x,x,x,x,x],...,[x,x,x,x,x]]

for row in data:
    increasing = check_inc(row)
    if check_diff(row,increasing) and check_increments(row,increasing):
        out+=1
print(out)
    
