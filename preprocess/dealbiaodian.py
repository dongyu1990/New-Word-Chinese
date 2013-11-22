fin = open("sampletianlong.txt",'r')
fout = open("outtianlong1.txt",'a')
for line in fin:
    linelenth = len(line)
    cur = 0
    beg = 0
    end = 0
    flag =0
    while(cur < linelenth):
        index = ord(line[cur])
        if index >=176: #and index <=  254:
            tempGet = ""
            if flag ==0:
                beg = cur
                flag = 1
            cur += 2

        else:
            if flag == 1:
                end = cur
                part = line[beg:end]
                #print part
                if len(part) > 0:
                    fout.write(part)
                    fout.write('\n')
                cur+=2
                flag = 0
            else:
                cur +=2

fin.close()
fout.close()
