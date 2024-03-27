import xml.etree.ElementTree as ET 

map_source = 'town'

# create element tree object
tree = ET.parse(map_source + '.tmx')

# get root element
root = tree.getroot()

child = root.find('layer')

map_width = int(root.attrib['width'])
map_height = int(root.attrib['height'])

map_size = map_height * map_width

tilemap = list(child)[0].text.replace('\n', '').split(',')
with open('cpp_output.txt', 'w') as file:    
    file.write('const uint8_t PROGMEM ' + map_source + '_data [] = {\n	')
    
    for n in range(map_size):
        file.write(str(int(tilemap[n]) - 1) + ', ')
        if n != 0 and n % 10 == 0:
            file.write('\n	')
        
    file.write('\n};')
    
    print('\a')
    
