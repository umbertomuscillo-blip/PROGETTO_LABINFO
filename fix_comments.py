import os
import re

files = [
    "Carta.h", "Carta.cpp", "Mazzo.h", "Mazzo.cpp", "Giocatore.h", "Giocatore.cpp", 
    "Partita.h", "Partita.cpp", "Database.h", "Database.cpp", "GestoreRete.h", 
    "GestoreRete.cpp", "main.cpp", "Main_grafico.cpp", "GameScene.cpp", "GameScene.h"
]

for f in files:
    if not os.path.exists(f): continue
    with open(f, "r", encoding="utf-8") as file:
        content = file.read()
    
    def replacer(match):
        block = match.group(0)
        lines = block.split('\n')
        new_lines = []
        for i, line in enumerate(lines):
            line = line.strip()
            if line.startswith('/**'):
                line = line[3:].strip()
            elif line.startswith('*/'):
                continue
            elif line.startswith('*'):
                line = line[1:].strip()
            
            if not line:
                continue
                
            line = line.replace('@brief ', '')
            line = line.replace('@param', 'Parametro:')
            line = line.replace('@return', 'Ritorna:')
            line = line.replace('@file ', '')
            line = line.replace('@class ', 'Classe ')
            line = line.replace('@enum ', 'Valori per ')
            line = line.replace('@struct ', '')
            
            line = line.strip()
            if line:
                new_lines.append('// ' + line)
                
        return '\n'.join(new_lines)
    
    new_content = re.sub(r'/\*\*.*?\*/', replacer, content, flags=re.DOTALL)
    
    with open(f, "w", encoding="utf-8") as file:
        file.write(new_content)
print("Done!")
