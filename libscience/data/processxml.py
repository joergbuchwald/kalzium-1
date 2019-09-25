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
    for grandchild in child.getchildren():
        isotopes.append(grandchild.attrib.values()[0])
        numbers.append(grandchild.attrib.values()[1])
    d={'isotop': isotopes, 'number': numbers}
    dataframes.append(pd.DataFrame(data=d))
for i,element in enumerate(elements):
    print(dataframes[i])
