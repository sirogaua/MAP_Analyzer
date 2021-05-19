from os import name
from bs4 import BeautifulSoup

class Indication:
    def __init__(self, name, parameters, count):
        self.name = name
        self.parameters = parameters
        self.count = count
    
    def __eq__(self, other):
        if isinstance(other, Indication):
            return (self.name == other.name and
                        self.parameters == other.parameters and
                        self.count == other.count)
        return NotImplemented

    def Get_name(self):
        return self.name
    
    def Get_parameters(self):
        return self.parameters

    def Get_count(self):
        return self.count

def Parse_line(line):
    # print(len(line))
    # print(line)
    name_and_other = line.split(' [\'', 1)
    name = name_and_other[0]

    parameters_and_other = name_and_other[1].split('\', ')
    parameters = parameters_and_other[0]

    count = parameters_and_other[1].split(']', 1)[0]
    # print("name = {}, parameters = {}, count = {}".format(name, parameters, count))
    return Indication(name, parameters, count)
def Parse_add_date(line):
    name_and_description = line.split(' ', 1)
    name = name_and_description[0]
    description = name_and_description[1]
    return Indication(name, description, 1)

f = []
symptoms = {"...Link is down", "...Soc Version"}
add_date = {"...Dates:"}

for index in {-1, 0, 1, 2, 3, 4}:
    if index == -1:
        f = open("messages.html")
    else:
        f = open("messages.{}.html".format(index))
    
    content = f.read()
    soup = BeautifulSoup(content, 'lxml')
    sherlock_syms = soup.find_all('h4')

    if index == -1:
        f = open("messages.txt", "w")
    else:
        f = open("messages{}.txt".format(index), "w")

    for i in sherlock_syms:
        j = i.find("font")
        try:
            if j.text in symptoms:
                ll = Parse_line(i.text)
                f.write("{};{};{}\n".format(ll.Get_name(), ll.Get_parameters(), ll.Get_count()))
            elif j.text in add_date:
                ll = Parse_add_date(i.text)
                f.write("{};{};{}\n".format(ll.Get_name(), ll.Get_parameters(), ll.Get_count()))
        except:
            pass

    f.close()