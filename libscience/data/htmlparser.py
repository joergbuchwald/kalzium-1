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
        self.readindata=False
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
                        string="<scalar dictRef=\"bo:halfLife\" units=\"siUnits:s\">" + data + "</scalar>"
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

