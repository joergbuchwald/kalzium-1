from lxml import etree as ET
import pandas as pd

data= ET.parse(r'isotopes_work.xml')

root = ET.XML(ET.tostring(data.getroot()))
elements=[]
dataframes=[]
for child in root.getchildren():
    elements.append(child.attrib.values()[0])
    isotopes=[]
    numbers=[]
    masses=[]
    halflifes=[]
    atomicnumbers=[]
    for grandchild in child.getchildren():
        isotopes.append(grandchild.attrib.values()[0])
        numbers.append(grandchild.attrib.values()[1])
        mass=""
        halflife=""
        atomicnumber=""
        for grandgrandchild in grandchild.getchildren():
            if grandgrandchild.attrib.values()[0]=="bo:exactMass":
                mass=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:halfLife":
                halflife=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:atomicNumber":
                atomicnumber=grandgrandchild.text
        masses.append(mass)
        halflifes.append(halflife)
        atomicnumbers.append(atomicnumber)

    d={'isotop': isotopes, 'number': numbers, 'mass': masses, 'halflife': halflifes, 'atomicnumber': atomicnumbers}
    dataframes.append(pd.DataFrame(data=d))
for i,element in enumerate(elements):
    print(dataframes[i])
