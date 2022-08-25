# !/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys



def loadUserAgent():
    '''
    加载模拟浏览器需要的headers
    '''
    l=[]
    count= 0
    
    try:
        with open("user_agents.ini",'r') as f:
            line = f.readline()
            while line:
                l.append(line)
                count+=1
                line = f.readline()
    except FileNotFoundError as e:
        print(e)
        sys.exit()
    
    f.close()
    return count,l







def main():


if __name__=="__main__":
    main()