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
    relativeabundances=[]
    spins=[]
    magneticmoments=[]
    for grandchild in child.getchildren():
        isotopes.append(grandchild.attrib.values()[0])
        numbers.append(grandchild.attrib.values()[1])
        mass=""
        halflife=""
        atomicnumber=""
        relativeabundance=""
        spin=""
        magneticmoment=""
        for grandgrandchild in grandchild.getchildren():
            if grandgrandchild.attrib.values()[0]=="bo:exactMass":
                mass=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:halfLife":
                halflife=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:atomicNumber":
                atomicnumber=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:relativeAbundance":
                relativeabundance=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:spin":
                spin=grandgrandchild.text
            if grandgrandchild.attrib.values()[0]=="bo:magneticMoment":
                magneticmoment=grandgrandchild.text
        masses.append(mass)
        halflifes.append(halflife)
        atomicnumbers.append(atomicnumber)
        relativeabundances.append(relativeabundance)
        spins.append(spin)
        magneticmoments.append(magneticmoment)

    d={'isotop': isotopes, 'number': numbers, 'mass': masses, 'spin': spins, 'magnetic_moment': magneticmoments, 'relative_abundance': relativeabundances, 'halflife': halflifes, 'atomicnumber': atomicnumbers}
    dataframes.append(pd.DataFrame(data=d))
for i,element in enumerate(elements):
    print(dataframes[i])
