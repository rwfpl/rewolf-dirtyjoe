# coding=utf-8
import sys
import os
from html.parser import HTMLParser

class JVMParser(HTMLParser):
    
    data = ""
    
    operbuf = ""
    operrec = 0
    
    stackbuf = ""
    stackrec = 0
    stackform = 0
    
    morebuf = ""
    morerec = 0
    
    tag_ul_cnt = 0
    skip_enter = False
    
    
    def __init__(self, inbuf):
        HTMLParser.__init__(self)
        self.feed(inbuf)
    
    
    def handle_starttag(self, tag, attrs):
        if tag == "div":
            a = next((a for a in attrs if  a[0] == "class" and a[1] == "section-execution"), None)
            if a != None:
                a2 = a = next((a for a in attrs if  a[0] == "title"), None)
                if a2 != None:
                    if self.morerec != 0:
                        print("jvm_full:%s" % self.data.strip())
                        self.morerec = 0            
                    print("jvm_opcode:%s" % a2[1])
            
            a = next((a for a in attrs if  a[0] == "class" and a[1] == "section"), None)
            if a != None:
                a2 = a = next((a for a in attrs if  a[0] == "title" and a[1] == "Operation"), None)
                if a2 != None:
                    self.operrec = 1
                    
                a2 = a = next((a for a in attrs if  a[0] == "title" and a[1] == "Operand Stack"), None)
                if a2 != None:
                    self.stackrec = 1
                    self.stackform = 0
                    
                a2 = a = next((a for a in attrs if  a[0] == "title" and a[1] == "Description"), None)
                if a2 != None:
                    print("jvm_stack:%s" % self.data.strip())
                    self.stackrec = 0                
                    self.morerec = 1
                    
                if self.morerec == 2:
                    self.data += "\n\n"
        
        if tag == "p" and self.operrec == 1:
            a = next((a for a in attrs if  a[0] == "class" and a[1] == "norm"), None)
            if a != None:
                self.operrec = 2
                self.data = ""

        if tag == "p" and self.stackrec == 1:
            a = next((a for a in attrs if  a[0] == "class" and a[1] == "norm"), None)
            if a != None:
                self.stackrec = 2
                if self.stackform == 0:
                    self.data = ""
                
        if tag == "p" and self.stackrec == 3:
            a = next((a for a in attrs if  a[0] == "class" and a[1] == "norm"), None)
            if a != None:
                self.stackrec = 4             
                
                
        if tag == "p" and self.morerec == 1:
            a = next((a for a in attrs if  a[0] == "class" and a[1] == "norm"), None)
            if a != None:
                self.morerec = 2
                self.data = ""
                
        if tag == "p" and self.morerec == 2 and self.tag_ul_cnt == 0:
            self.data += "\n\n"
                            
        if tag == "ul":
            self.tag_ul_cnt += 1
            if self.tag_ul_cnt > 1:
                self.data += "\n"
        
        if tag == "li":
            for x in range(self.tag_ul_cnt):
                self.data += "  "
            self.data += "-"


    def handle_endtag(self, tag):

        if tag == "ul":
            self.tag_ul_cnt -= 1
            if self.tag_ul_cnt > 0:
                self.data += "\n"            

        if tag == "p" and self.operrec == 2:
            print("jvm_short:%s" % self.data.strip())
            self.operrec = 0
        elif tag == "p" and self.stackrec == 2:
            if self.data.strip() == "No change":
                self.stackrec = 4
            elif self.data.strip() == "Same as modified instruction":
                self.stackrec = 4
            else:
                self.stackrec = 3
        elif tag == "p":
            if not self.skip_enter:
                self.data += "\n"
            else:
                self.skip_enter = False
        
        if tag == "html":
            if self.morerec != 0:
                print("jvm_full:%s" % self.data.strip())
                self.morerec = 0                    
    
    def handle_charref(self, name):
        try:
            if name == "8594":
                self.data += " -> "
                self.skip_enter = True
            else:
                self.data += chr(int(name))
        except:
            pass

            
    def handle_entityref(self, name):
        x = name
        if name == "lt":
            x = "<"
        elif name == "gt":
            x = ">"
        elif name == "sect":
            x = ""
        elif name == "nbsp":
            x = " "            
        self.data += x
        
        
    def handle_data(self, data):
        skipped = 0
        while self.data[-1:] == "\n":
            self.data = self.data[:-1]
            skipped += 1
        if skipped == 1:
            self.data += "\n"
        elif skipped > 1:
            self.data += "\n\n"
        
        temp = data.strip().split("\n")
        for s in temp:
            s = s.strip()
            if s != "":
                if self.data != "" and self.data[-1:] != "\n" and s[0] != "." and s[0] != "," and self.data[-1:] != "<" and s[0] != "'" and self.data[-1:] != "§":
                    self.data += " "
                
                s = s.strip()
                if s == "Form 1:":
                    s += "\n"
                
                self.data += s


if len(sys.argv) != 2:
	sys.exit()

f = open(sys.argv[1], "r")
inFile = f.read()
f.close()

JVMParser(inFile)
