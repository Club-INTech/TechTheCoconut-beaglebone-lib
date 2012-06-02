# -*- coding:utf-8 -*-

# Convention : Mutex à 1 <-> PAS POSSIBLE D'ECRIRE.

from threading import Lock

# Classe de mutex de file
class MutexFile :
    def __init__(self):
        # Dictionnaire type "nomFichier" = MutexOk/PasOk
        if not hasattr(MutexFile, "mutex") :
            MutexFile.mutex = {}
            
    def __str__(self) :
        return str(MutexFile.mutex)
        
    # Retourne True si le mutex pour ce fichier existait déjà, False sinon
    def addMutex(self, nom) :
        # Ajout du mutex s'il n'existe pas déjà.
        if not nom in MutexFile.mutex :
            lock = Lock()
            MutexFile.mutex.update({nom:lock})
            return False
        return True
    

    def acquire(self, nom) :
        MutexFile.mutex[nom].acquire()
    def release(self, nom) :
        MutexFile.mutex[nom].release()
    
class File :
    # L'argument deleteAll détermine si oui ou non on veut supprimer les données
    # contenues dans le fichier avant utilisation. (ne peut se faire que pour la
    # première instanciation de ce fichier).
    def __init__(self, nom, deleteAll = True) :
        if not hasattr(File, "mutex") :
            File.mutex = MutexFile()
            File.pattern = "\n"
            File.timeout = 1 #second
        
        self.nom = nom
        
        # Ouverture du fichier
        try :
            dejaOuvert = File.mutex.addMutex(self.nom)
            if (not dejaOuvert) and deleteAll: 
                self.fichier = open(self.nom, "w+")
                print "Opening in Delete Mode"
            else :
                self.fichier = open(self.nom, "a+")
            
        except :
            print "Impossible d'ouvrir le fichier " + nom + "."
            
        
    # Retourne True si le message a bien été écrit.
    def write(self, message, noRepetition = True) :
        
        File.mutex.acquire(self.nom)
        
        lines = self.readlines()
        
        if ((not message in lines) and noRepetition) or not noRepetition : 
        
            print "Ecriture du message..."
            # On va à la fin du fichier
            self.fichier.seek(0,2)
            # Pour écrire le message
            self.fichier.write(str(message) + File.pattern)
            # On force l'écriture
            self.fichier.flush()
        
        # On rend le mutex
        File.mutex.release(self.nom)

        
    # WARNING cette fonction ne checke pas le mutex. Elle doit donc être
    # placés à l'intérieur d'un bloc sous l'influence du mutex
    def readlines(self) :
        self.fichier.seek(0)
        lines = self.fichier.readlines()
        
        for i in range(len(lines)) :
            if lines[i][-1] == File.pattern :
                lines[i] = lines[i][0:len(lines[i])-1]
        return lines

    # WARNING cette fonction ne checke pas le mutex. Elle doit donc être
    # placés à l'intérieur d'un bloc sous l'influence du mutex
    def writelines(self, lines) :
        for i in range(len(lines)) :
            lines[i] = lines[i] + File.pattern
        
        self.fichier.seek(0)
        self.fichier.truncate(0)
        self.fichier.writelines(lines)
        self.fichier.flush()


        
    # Permet d'enlever le message "message" du fichier.
    # Retourne True si le message a bien été enlevé du fichier.
    def remove(self, message) :
        
        linesToDelete = []
        File.mutex.acquire(self.nom)
        
        lines = self.readlines()
        
        # On cherche les lignes où apparaît le message :
        for i in range(len(lines)) :
            if lines[i] == message :
                linesToDelete.append(i - len(linesToDelete))
                
        # on détruit ces lignes :
        for i in range(len(linesToDelete)) :
            del lines[linesToDelete[i]]
            
        self.writelines(lines)
        
        File.mutex.release(self.nom)

        
    # Proprifie le fichier (i.e. enlève les doublons et les lignes vides)
    def clean(self) :
        # Contient l'indices de lignes à détruire
        linesToDelete = []
        # Contient les lignes du fichier mais sans les doublons
        linesDejaVu   = []
        
        File.mutex.acquire(self.nom)

        lines = self.readlines()
        
        for i in range(len(lines)) :
            if (not lines[i] in linesDejaVu) and lines[i] != "" :
                linesDejaVu.append(lines[i])
                
        self.writelines(linesDejaVu)
        
        File.mutex.release(self.nom)

        
            
            
    def close(self) :
        self.fichier.close()
        