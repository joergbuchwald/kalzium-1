from html.parser import HTMLParser
from html.entities import name2codepoint

from html.parser import HTMLParser

class MyHTMLParser(HTMLParser):
    def __init__(self):
        self.element=""
        self.isotope=""
        self.readinelement=False
        self.readinisotope=False
        self.decayenergy=""
        self.columncount=0
        self.spin=""
        self.kind=""
        self.halflife=""
        self.readindata=False
        self.readinunit=False
        HTMLParser.__init__(self)
    def handle_starttag(self, tag, attrs):
        if tag=="tr":
            self.columncount=0
        if tag=="td":
            self.columncount+=1
            self.readindata=True
            for i,attr in enumerate(attrs):
                if i==0:
                    if attr[0]=="align" and attr[1]=="right":
                        self.readinelement=True
        if tag=="sup":
            self.readinisotope=True

    def handle_endtag(self, tag):
        if tag=="td":        
            self.readinelement=False
            self.readindata=False
            self.readinunit=False
        if tag=="sup":
            self.readinisotope=False
    def handle_data(self, data):
        data=data.strip()
        if self.readinisotope==True:
            if data!=self.isotope:
                self.isotope=data.strip()
        if self.readinisotope==False and self.readinelement==True:
            if self.element!=data:
                self.element=data
                string="<isotopeList id=\""+ self.element+ "\">"
                print(string)
            string="<isotope id=\""+ self.element+ self.isotope+ "\" number=\""+  self.isotope + "\" elementType=\"" + self.element + "\">"
            print(string)
        if self.readindata==True and self.columncount==2:
            if not "<" in data: 
                if not ">" in data:
                    if not "stabil" in data:
                        if "·10" in data:
                            self.halflife=data.replace("·10","")
                        elif self.readinisotope==True:
                            self.readinunit=True
                            self.halflife=str(float(self.halflife.replace(",","."))*10**float(data))
                        elif self.readinunit==True:
                            if data.strip()=="s":
                                pass
                            if data.strip()=="ms":
                                self.halflife=str(float(self.halflife)/1000.0)
                            if data.strip()=="µs":
                                self.halflife=str(float(self.halflife)/1000.0e3)
                            if data.strip()=="ns":
                                self.halflife=str(float(self.halflife)/1000.0e6)
                            if data.strip()=="ps":
                                self.halflife=str(float(self.halflife)/1000.0e9)
                            if data.strip()=="min":
                                self.halflife=str(float(self.halflife)*60)
                            if data.strip()=="h":
                                self.halflife=str(float(self.halflife)*3600)
                            if data.strip()=="d":
                                self.halflife=str(float(self.halflife)*3600*24)
                            if data.strip()=="a":
                                self.halflife=str(float(self.halflife)*3600*24*365)
                            string="<scalar dictRef=\"bo:halfLife\" units=\"siUnits:s\">" + self.halflife + "</scalar>"
                            print(string)
                        else:
                            halflife=data.split()
                            halflife[0]=halflife[0].replace(",",".")
                            if len(halflife)==2:
                                if halflife[1].strip()=="s":
                                    pass
                                if halflife[1].strip()=="ms":
                                    halflife[0]=str(float(halflife[0])/1000.0)
                                if halflife[1].strip()=="µs":
                                    halflife[0]=str(float(halflife[0])/1000.0e3)
                                if halflife[1].strip()=="ns":
                                    halflife[0]=str(float(halflife[0])/1000.0e6)
                                if halflife[1].strip()=="ps":
                                    halflife[0]=str(float(halflife[0])/1000.0e9)
                                if halflife[1].strip()=="min":
                                    halflife[0]=str(float(halflife[0])*60)
                                if halflife[1].strip()=="h":
                                    halflife[0]=str(float(halflife[0])*3600)
                                if halflife[1].strip()=="d":
                                    halflife[0]=str(float(halflife[0])*3600*24)
                                if halflife[1].strip()=="a":
                                    halflife[0]=str(float(halflife[0])*3600*24*365)
                                string="<scalar dictRef=\"bo:halfLife\" units=\"siUnits:s\">" + halflife[0] + "</scalar>"
                                print(string)
                    else:
                        self.readindata=False
                else:
                    self.readindata=False
            else:
                self.readindata=False
        if self.readindata==True and self.columncount==3:
            self.decayenergy=data
        if self.readindata==True and self.columncount==4:
            self.spin=data
            string="<scalar dictRef=\"bo:spin\">" + data +"</scalar>"
            print(string)
        if self.readindata==True and self.columncount==5:
            self.kind=data

if __name__=='__main__':
    file="4_Periode.html"
    datei=open(file,"r")
    html=datei.read()
    parser = MyHTMLParser()
    parser.feed(html)

